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

#include <stdio.h>
#include <thread>
#include "adcolony.h"

namespace adcolony
{
	int reward = 50;
	bool isAvailabled = false;

	void startDownload()
	{
		auto dummy = []()
		{
			std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );
			isAvailabled = true;
			ServicePlugin::shared().onVideoAvailabledChanged( true );
		};
		std::thread thread( dummy );
		thread.detach();
	}

	void initialization(const std::string& id, const std::string& zoneId)
	{
	}

	void show()
	{
		auto result = []()
		{
			std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
			ServicePlugin::shared().onVideoFinihed( ServicePlugin::Result::Ok, reward );
			isAvailabled = false;
			startDownload();
			ServicePlugin::shared().onVideoAvailabledChanged( false );
		};
		std::thread thread( result );
		thread.detach();
		ServicePlugin::shared().onVideoStarted();
	}

	bool videoIsAvailabled()
	{
		if( !isAvailabled )
			startDownload();
		return isAvailabled;
	}

	int getVideoRewardAmound()
	{
		return reward;
	}
}
