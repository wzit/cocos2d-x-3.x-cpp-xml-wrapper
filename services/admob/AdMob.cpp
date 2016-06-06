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
#include "AdMob.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include "JniBind.h"
#endif


USING_NS_CC;

namespace AdMob
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	bool _initialized = false;
	void show()
	{
		JavaBind bind("org.cocos2dx.cpp", "AdMob", "showBanner", "");
		bind.call();
	}
	
	void hide()
	{
		JavaBind bind("org.cocos2dx.cpp", "AdMob", "hideBanner", "");
		bind.call();
	}
	void showInterstitial()
	{

		JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t,
										   "org.cocos2dx.cpp/AdMob",
										   "showInterstitial",
										   "()V")) {
			log("AdMob::showInterstitial exist");
			t.env->CallStaticIntMethod(t.classID, t.methodID);
			t.env->DeleteLocalRef(t.classID);
		}
		else
		{
			log("AdMob::showInterstitial notfound");
		}


		//JavaBind bind( "org.cocos2dx.cpp", "AdMob", "showInterstitial", "" );
		//bind.call();
	}

	void initialization( const std::string & appID )
	{
		JavaBind bind("org.cocos2dx.cpp", "AdMob", "initialization", "%s");
		bind.call(appID);
		_initialized = true;
	}

	void activateTestMode()
	{}

	bool isInitialized()
	{
		return _initialized;
	}

	bool interstitialIsAvailabled()
	{
		JavaBind bind("org.cocos2dx.cpp", "AdMob", "interstitialAvailabled", "");
		return bind.bool_call();
	}


#else
	void show()
	{
	}
	void hide()
	{
	}
	void showInterstitial()
	{
	}
	void initialization( const std::string & appID ){}
	void activateTestMode() {}
	bool isInitialized() { return false; }
	bool interstitialIsAvailabled() { return false; }

#endif
	
}

