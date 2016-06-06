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
#include "appodeal_.h"

namespace appodeal
{
	bool isAvailabled = false;

	void showInterstitial();
	void showBanner();
	void hideBanner();
	void showVideo();
	bool isVideoLoaded();
	bool isInterstitialLoaded();
	int getVideoRewardAmound();

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

	void showInterstitial()
	{
		cocos2d::log( "appodeal::showInterstitial" );
	}
	void showBanner()
	{
		cocos2d::log( "appodeal::showBanner" );
	}
	void hideBanner()
	{
		cocos2d::log( "appodeal::hideBanner" );
	}

	void showVideo()
	{
		auto result = []()
		{
			std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
			ServicePlugin::shared().onVideoFinihed( ServicePlugin::Result::Ok, 0 );
			isAvailabled = false;
			startDownload();
			ServicePlugin::shared().onVideoAvailabledChanged( false );
		};
		std::thread thread( result );
		thread.detach();
		ServicePlugin::shared().onVideoStarted();
	}

	bool isVideoLoaded()
	{
		if( !isAvailabled )
			startDownload();
		return isAvailabled;
	}

	bool isInterstitialLoaded()
	{
		return true;
	}

	int getVideoRewardAmound()
	{
		return 50;
	}
}