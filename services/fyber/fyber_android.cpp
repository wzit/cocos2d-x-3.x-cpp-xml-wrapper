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

#include "fyber_.h"
#include "cocos2d.h"

#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include "ml/JniBind.h"
#include "ml/services/plugins/ServicePlugin.h"

USING_NS_CC;

const std::string JavaPackage( "org.cocos2dx.cpp" );
const std::string JavaClass( "FyberDelegate" );

namespace fyber {
	int _rewardAmount = 0;

	void initialization(const std::string &id, const std::string &key) {
		JavaBind bind(JavaPackage, JavaClass, "initialization", "%s%s");
		bind.call(id, key);
	}

	void cacheInterstitial()
	{
		JavaBind bind(JavaPackage, JavaClass, "cacheInterstitial", "");
		bind.call();
	}

	void cacheVideo()
	{
		JavaBind bind(JavaPackage, JavaClass, "cacheRewardedVideo", "");
		bind.call();
	}

	void showInterstitial()
	{
		JavaBind bind(JavaPackage, JavaClass, "showInterstitial", "");
		bind.call();
	}
	void showVideo()
	{
		JavaBind bind(JavaPackage, JavaClass, "showRewardedVideo", "");
		bind.call();
	}
	bool isVideoLoaded()
	{
		JavaBind bind(JavaPackage, JavaClass, "isVideoLoaded", "");
		return bind.bool_call();
	}
	bool isInterstitialLoaded()
	{
		JavaBind bind(JavaPackage, JavaClass, "isInterstitialLoaded", "");
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
	JNIEXPORT void JNICALL  Java_org_cocos2dx_cpp_FyberDelegate_nativeVideoLoaded( JNIEnv*  env, jobject thiz, bool value )
	{
		ServicePlugin::shared().onVideoAvailabledChanged( value );
	}
	JNIEXPORT void JNICALL  Java_org_cocos2dx_cpp_FyberDelegate_nativeFinishedVideo( JNIEnv*  env, jobject thiz, bool success )
	{
		if( success )
			ServicePlugin::shared().onVideoFinihed( ServicePlugin::Result::Ok, fyber::getVideoRewardAmound() );
		else
			ServicePlugin::shared().onVideoFinihed( ServicePlugin::Result::Fail, 0 );
	}
	JNIEXPORT void JNICALL  Java_org_cocos2dx_cpp_FyberDelegate_nativeStartVideo( JNIEnv*  env, jobject thiz, bool value )
	{
		if( value )
			ServicePlugin::shared().onVideoStarted();
	}
	JNIEXPORT void JNICALL  Java_org_cocos2dx_cpp_FyberDelegate_nativeVideoFailed( JNIEnv*  env, jobject thiz )
	{
		ServicePlugin::shared().onVideoFinihed( ServicePlugin::Result::Fail, 0 );
	}
}

