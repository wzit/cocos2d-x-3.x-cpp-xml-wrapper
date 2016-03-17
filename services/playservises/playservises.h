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

#include <string>

namespace PlayServises
{
	void init();
	bool isConnected();
	void addCallbackOnConnected( std::function<void()> function );

	namespace Leaderboard
	{
		void record( const std::string& leaderBoardID, int value );
		void open( const std::string& leaderBoardID );
	}

	namespace Achievements
	{
		void unlock( const std::string& id);
		void increment( const std::string& id, int value );
		void open();
	}
}
