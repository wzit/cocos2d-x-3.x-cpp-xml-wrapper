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

#ifndef __TAPJOY_h__
#define __TAPJOY_h__

#include <functional>

namespace tapjoy
{
	void showOffers();
	void getBalance();

	extern std::function<void( int )> GetBalance;
};

#endif