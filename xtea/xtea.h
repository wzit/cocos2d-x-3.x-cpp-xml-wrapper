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

#ifndef __XTEA_H__
#define __XTEA_H__

#include <stdint.h>
#include <memory>
#include <assert.h>
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>


namespace xtea
{
	typedef uint32_t Word[2];
	typedef uint32_t KeyBlock[4];

	void crypt( uint32_t * V, const KeyBlock & K );
	void decrypt( uint32_t * V, const KeyBlock & K );
	uint32_t char4ToUint32( const std::string& s, int pos );
	void unit32ToCahr4( uint32_t l, std::string&result, int InsertAt );
	std::string cryptStr( const std::string& Msg, const std::string& Pwd );
	std::string decryptStr( const std::string& Msg, const std::string& Pwd );

	void convertKeyToXtea( const std::string& key, KeyBlock & out );
	void convertKeyFromXtea( std::string& out, const KeyBlock & key );
}

#endif