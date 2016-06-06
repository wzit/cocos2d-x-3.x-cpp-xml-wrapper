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
#include "fyber_.h"
#include "cocos2d.h"
#include "services/plugins/ServicePlugin.h"

namespace fyber
{
	std::mutex _mutex;
	bool isAvailabled = false;
	bool isInitialized = false;

	void initialization( const std::string& id, const std::string& key )
	{
		isInitialized = true;
	}

	void startDownload()
	{
		auto dummy = []()
		{
			std::this_thread::sleep_for( std::chrono::milliseconds( 2000 ) );
			_mutex.lock();
			isAvailabled = true;
			ServicePlugin::shared().onVideoAvailabledChanged( true );
			_mutex.unlock();
		};
		std::thread thread( dummy );
		thread.detach();
	}

	void cacheInterstitial()
	{
		assert( isInitialized );
	}

	void cacheVideo()
	{
		assert( isInitialized );
		startDownload();
	}

	void showInterstitial()
	{
		assert( isInitialized );
		cocos2d::log( "fyber::showInterstitial" );
	}

	void showVideo()
	{
		assert( isInitialized );
		auto result = []()
		{
			_mutex.lock();
			std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
			ServicePlugin::shared().onVideoFinihed( ServicePlugin::Result::Ok, 0 );
			isAvailabled = false;
			startDownload();
			ServicePlugin::shared().onVideoAvailabledChanged( false );
			_mutex.unlock();
		};
		std::thread thread( result );
		thread.detach();
		ServicePlugin::shared().onVideoStarted();
	}

	bool isVideoLoaded()
	{
		assert( isInitialized );
		_mutex.lock();
		if( !isAvailabled )
			startDownload();
		_mutex.unlock();
		return isAvailabled;
	}

	bool isInterstitialLoaded()
	{
		assert( isInitialized );
		return true;
	}

	int getVideoRewardAmound()
	{
		assert( isInitialized );
		return 50;
	}
}