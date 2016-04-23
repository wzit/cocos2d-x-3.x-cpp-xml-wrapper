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

#include <stdio.h>
#include <thread>
#include "fyber_.h"
#include "cocos2d.h"
#include "ml/services/plugins/ServicePlugin.h"
#include "AppController.h"

namespace fyber
{
	std::mutex _mutex;
	bool isAvailabled = false;
	bool isInitialized = false;
	
	void initialization( const std::string& id, const std::string& key )
	{
	}
	void cacheInterstitial()
	{
	}
	
	void cacheVideo()
	{
	}
	
	void showInterstitial()
	{
		[AppController fyberRequestInterstitial];
	}
	
	void showVideo()
	{
	}
	
	bool isVideoLoaded()
	{
		return false;
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