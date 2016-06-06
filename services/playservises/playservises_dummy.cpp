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

#include "cocos2d.h"
#include "playservises.h"
#include "mlUserData.h"

USING_NS_CC;

namespace PlayServises
{
	bool _isInit( false );
	std::list<std::function<void()>> _onConnected;

	void init()
	{
		auto connect = []()
		{
			std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
			_isInit = true;
			for( auto func : _onConnected )
			{
				func();
			}
		};
		std::thread th( connect );
		th.detach();
	}

	void addCallbackOnConnected( std::function<void()> function )
	{
		_onConnected.push_back( function );
	}

	bool isConnected()
	{
		return _isInit;
	}

	namespace Leaderboard
	{
		void record( const std::string& leaderBoardID, int value )
		{
			auto id = "PlayServises_Leaderboard_record" + leaderBoardID;
			int max = mlUserData::getInstance()->get<int>( id );
			max = std::max( max, value );
			mlUserData::getInstance()->write( id, max );
		}

		void open( const std::string& leaderBoardID )
		{
			auto id = "PlayServises_Leaderboard_record" + leaderBoardID;
			auto value = mlUserData::getInstance()->get_str( id, "0" );
			cocos2d::MessageBox( ("Current record: " + value).c_str(), leaderBoardID.c_str() );
		}
	}
}
