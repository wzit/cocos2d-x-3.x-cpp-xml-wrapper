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

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "chartboost.h"
#include "cocos2d.h"
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include "ml/JniBind.h"

USING_NS_CC;

namespace chartboost
{
	bool _initialized = false;

	bool isInitialized()
	{
		return _initialized;
	}

	void initialization( const std::string appID, const std::string& appSignature )
	{
		log( " chartboost initialization" );
		_initialized = true;
		JavaBind bind("org.cocos2dx.cpp", "ChartBoost", "initialization", "%s%s");
		bind.call( appID, appSignature );
		log( " chartboost initialization end " );
	}

	bool interstitialIsAvailabled()
	{
		JavaBind bind( "org.cocos2dx.cpp", "ChartBoost", "interstitialAvailabled", "" );
		return bind.bool_call();
	}

	void showInterstitial()
	{
		JavaBind bind("org.cocos2dx.cpp", "ChartBoost", "showInterstitial", "");
		bind.call();
	}
}
#endif
