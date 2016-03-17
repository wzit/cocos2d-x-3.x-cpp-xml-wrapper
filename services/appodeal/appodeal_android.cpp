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

#include "appodeal_.h"
#include "cocos2d.h"

#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include "ml/JniBind.h"

USING_NS_CC;

namespace appodeal
{
	int _rewardAmount = 0;
	bool _videoIsAvailabled( false );

	void showInterstitial()
	{
		JavaBind bind("org.cocos2dx.cpp", "mlAppodeal", "showInterstitial", "");
		bind.call();
	}
	void showBanner()
	{
		JavaBind bind("org.cocos2dx.cpp", "mlAppodeal", "showBanner", "");
		bind.call();
	}
	void hideBanner()
	{
		JavaBind bind("org.cocos2dx.cpp", "mlAppodeal", "hideBanner", "");
		bind.call();
	}
	void showVideo()
	{
		JavaBind bind("org.cocos2dx.cpp", "mlAppodeal", "showVideo", "");
		bind.call();
	}
	bool isVideoLoaded()
	{
		JavaBind bind("org.cocos2dx.cpp", "mlAppodeal", "isVideoLoaded", "");
		return bind.bool_call();
	}
	bool isInterstitialLoaded()
	{
		JavaBind bind("org.cocos2dx.cpp", "mlAppodeal", "isInterstitialLoaded", "");
		return bind.bool_call();
	}
	int getVideoRewardAmound()
	{
		return 50;
	}
}

extern
"C"
{
	JNIEXPORT void JNICALL  Java_org_cocos2dx_cpp_mlAppodeal_nativeVideoState( JNIEnv*  env, jobject thiz, bool value )
	{
		ServicePlugin::shared().onVideoAvailabledChanged( value );
	}
	JNIEXPORT void JNICALL  Java_org_cocos2dx_cpp_mlAppodeal_nativeFinishedVideo( JNIEnv*  env, jobject thiz, jstring type, int amount )
	{
		ServicePlugin::shared().onVideoFinihed( ServicePlugin::Result::Ok, amount );
	}
	JNIEXPORT void JNICALL  Java_org_cocos2dx_cpp_mlAppodeal_nativeStartVideo( JNIEnv*  env, jobject thiz, bool value )
	{
		if( value )
			ServicePlugin::shared().onVideoStarted();
	}
	JNIEXPORT void JNICALL  Java_org_cocos2dx_cpp_mlAppodeal_onAdColonyAdFailed( JNIEnv*  env, jobject thiz )
	{
		ServicePlugin::shared().onVideoFinihed( ServicePlugin::Result::Fail, 0 );
	}
}

