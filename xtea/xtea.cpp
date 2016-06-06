/******************************************************************************/
/*
 * Copyright 2014-2015 Vladimir Tolmachev
 *
 * Author: Vladimir Tolmachev
 * Project: ml
 * e-mail: tolm_vl@hotmail.com
 * If you received the code is not the author, please contact me
 */
/******************************************************************************/

#include "xtea.h"

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4307)
#endif

namespace xtea
{
	static const uint32_t DELTA = 0x9e3779b9;
	static const uint32_t num_rounds = 32;

	template <class T> T& inc( T& value, T range = 1 ) { return (value += range); }
	template <class T> T& dec( T& value, T range = 1 ) { return (value -= range); }

	void crypt( uint32_t * V, const KeyBlock & K )
	{
		uint32_t S( 0 );

		for( int I = 0; I < num_rounds; ++I )
		{
			inc<uint32_t>( V[0], (V[1] << 4 ^ V[1] >> 5) + (V[1] ^ S) + K[S & 3] );
			inc<uint32_t>( S, DELTA );
			inc<uint32_t>( V[1], (V[0] << 4 ^ V[0] >> 5) + (V[0] ^ S) + K[S >> 11 & 3] );
		}
	}

	void decrypt( uint32_t * V, const KeyBlock & K )
	{
		uint32_t S;

		//warning C4307 here is normal for xTea algorithm;
		S = DELTA*num_rounds;
		for( int I = 0; I < num_rounds; ++I )
		{
			dec<uint32_t>( V[1], (V[0] << 4 ^ V[0] >> 5) + (V[0] ^ S) + K[S >> 11 & 3] );
			dec<uint32_t>( S, DELTA );
			dec<uint32_t>( V[0], (V[1] << 4 ^ V[1] >> 5) + (V[1] ^ S) + K[S & 3] );
		}

	}

	uint32_t char4ToUint32( const std::string& s, int pos )
	{
		uint32_t result = 0;
		char * lc = (char*)&result;
		size_t i(0);
		if( pos + i < s.size() ) { lc[i] = s[pos + i]; } ++i;
		if( pos + i < s.size() ) { lc[i] = s[pos + i]; } ++i;
		if( pos + i < s.size() ) { lc[i] = s[pos + i]; } ++i;
		if( pos + i < s.size() ) { lc[i] = s[pos + i]; } ++i;

		return result;
	}

	void unit32ToCahr4( uint32_t l, std::string&result, int InsertAt )
	{
		char * lc = (char*)&l;
		size_t i( 0 );
		if( InsertAt + i < result.size() ) { result[InsertAt + i] = lc[i]; } ++i;
		if( InsertAt + i < result.size() ) { result[InsertAt + i] = lc[i]; } ++i;
		if( InsertAt + i < result.size() ) { result[InsertAt + i] = lc[i]; } ++i;
		if( InsertAt + i < result.size() ) { result[InsertAt + i] = lc[i]; } ++i;
	}

	std::string cryptStr( const std::string& Msg, const std::string& Pwd )
	{
		Word V;
		KeyBlock K;
		uint32_t N;
		uint32_t cnt;
		Word LastEncodedBlock;
		uint32_t p;
		std::string result;
		result.resize( Msg.size() );

		convertKeyToXtea( Pwd, K );

		cnt = Msg.size() / sizeof( V );
		if( cnt == 0 )
		{
			LastEncodedBlock[0] = K[0] ^ K[2] ^ DELTA;
			LastEncodedBlock[1] = K[1] ^ K[3] ^ DELTA;
		}

		N = 0;
		p = 0;
		for( uint32_t i = 0; i < cnt; ++i )
		{
			V[0] = char4ToUint32( Msg, N );
			N += sizeof( uint32_t );
			V[1] = char4ToUint32( Msg, N );
			N += sizeof( uint32_t );
			crypt( V, K );

			unit32ToCahr4( V[0], result, p );
			p += 4;
			unit32ToCahr4( V[1], result, p );
			p += 4;

			LastEncodedBlock[0] = V[0];
			LastEncodedBlock[1] = V[1];
		}

		if( N < Msg.size() )
		{
			V[0] = char4ToUint32( Msg, N );
			N += sizeof( uint32_t );
			V[1] = char4ToUint32( Msg, N );

			V[0] = V[0] ^ LastEncodedBlock[0];
			V[1] = V[1] ^ LastEncodedBlock[1];
			unit32ToCahr4( V[0], result, p ); p += 4;
			unit32ToCahr4( V[1], result, p );
		}
		return result;

	}

	std::string decryptStr( const std::string& Msg, const std::string& Pwd )
	{
		Word V;
		KeyBlock K;
		unsigned int N;
		unsigned int cnt;
		Word LastEncodedBlock;
		int p;
		std::string result;
		result.resize( Msg.size() );

		convertKeyToXtea( Pwd, K );

		cnt = Msg.size() / sizeof( V );
		if( cnt == 0 )
		{
			LastEncodedBlock[0] = K[0] ^ K[2] ^ DELTA;
			LastEncodedBlock[1] = K[1] ^ K[3] ^ DELTA;
		}

		N = 0;
		p = 0;
		for( uint32_t i = 0; i < cnt; ++i )
		{
			V[0] = char4ToUint32( Msg, N );
			N += sizeof( uint32_t );
			V[1] = char4ToUint32( Msg, N );
			N += sizeof( uint32_t );
			LastEncodedBlock[0] = V[0];
			LastEncodedBlock[1] = V[1];

			decrypt( V, K );

			unit32ToCahr4( V[0], result, p );
			p += 4;
			unit32ToCahr4( V[1], result, p );
			p += 4;
		}

		if( N < Msg.size() )
		{
			V[0] = char4ToUint32( Msg, N );
			N += sizeof( uint32_t );
			V[1] = char4ToUint32( Msg, N );

			V[0] = V[0] ^ LastEncodedBlock[0];
			V[1] = V[1] ^ LastEncodedBlock[1];
			unit32ToCahr4( V[0], result, p ); p += 4;
			unit32ToCahr4( V[1], result, p );
		}
		return result;

	}

	void convertKeyToXtea( const std::string& key, KeyBlock & out )
	{
		for( int i = 0; i < 4; ++i )
			out[i] = char4ToUint32( key, i * 4 );
	}
	void convertKeyFromXtea( std::string& out, const KeyBlock & key )
	{
		out.resize( 16 );
		for( int i = 0; i < 4; ++i )
			unit32ToCahr4( key[i], out, i * 4 );
	}

}

#ifdef _MSC_VER
#	pragma warning (pop)
#endif
