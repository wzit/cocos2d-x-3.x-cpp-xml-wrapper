/******************************************************************************/
/*
 * Copyright 2014-2015 Vladimir Tolmachev
 *
 * Author: Vladimir Tolmachev
 * Project: Defense of Greece
 * e-mail: tolm_vl@hotmail.com
 * If you received the code is not the author, please contact me
 */
/******************************************************************************/

#ifndef __TRandom_h__
#define __TRandom_h__

typedef unsigned long RandomType;

class Random
{
public:
	//typedef unsigned int Type;
	Random( RandomType seed, RandomType a, RandomType b, RandomType m ) : _seed( seed ), _a( a ), _b( b ), _m( m ) {}
	Random( const Random& random ) : _seed( random._seed ), _a( random._a ), _b( random._b ), _m( random._m ) {}
public:
	void set_seed( RandomType value )
	{
		_seed = value;
	}

	/*
	 * return Type pseudo random value [0 .. +m]
	 */
	RandomType rand()
	{
		return _seed = (_a * _seed + _b) % _m;
	}

	/*
	 * return Type pseudo random value [0 .. +interval]
	 */
	RandomType rand( int interval )
	{
		return rand() % interval;
	}

	/*
	 * return float pseudo random value [0 .. +interval]
	 */
	float frand( float interval = 1.f )
	{
		return static_cast<float>(rand()) / static_cast<float>(_m - 1) * interval;
	}

	/*
	 * return float pseudo random value [-interval .. +interval]
	 */
	float frand_n( float interval = 1.f )
	{
		return (static_cast<float>(rand()) / static_cast<float>(_m - 1) - 0.5f) * 2.f * interval;
	}

public:
	static Random& _6075()
	{
		static Random Random_6075( 0, 106, 1283, 6075 );
		return Random_6075;
	}

	static Random& _1771875()
	{
		static Random Random_1771875( 0, 2416, 374441, 1771875 );
		return Random_1771875;
	}

	static Random& _small() { return _6075(); }
	static Random& _big() { return _1771875(); }
private:
	RandomType _seed;
	RandomType _a;
	RandomType _b;
	RandomType _m;
};

//typedef TRandom<unsigned long> Random;



#endif //__TRandom_h__
