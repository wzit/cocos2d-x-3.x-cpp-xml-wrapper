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

#include "tapjoy.h"
#include <thread>


namespace tapjoy
{
	std::function<void( int )> GetBalance;


	void showOffers()
	{}

	void getBalance()
	{
		auto result = []()
		{
			std::this_thread::sleep_for( std::chrono::milliseconds(1000) );
			if( tapjoy::GetBalance )
				tapjoy::GetBalance( 0 );
		};

		std::thread thread( result );
		thread.detach();
	}

};