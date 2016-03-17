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

#include "unityads.h"
#include "cocos2d.h"

#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include "ml/JniBind.h"

USING_NS_CC;

namespace unityads
{
	const std::string package("org.cocos2dx.cpp");
	const std::string classname("mlUnityAds");

	bool _videoIsAvailabled(false);

	void show()
	{
		JavaBind bind(package, classname, "show", "");
		bind.call();
	}

	bool videoIsAvailabled()
	{
		return _videoIsAvailabled;
	}

	int getVideoRewardAmound()
	{
		return 0;
	}
}

extern
"C"
{
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_mlUnityAds_videoFetch( JNIEnv*  env, jobject thiz, bool value  )
	{
		unityads::_videoIsAvailabled = value;
	}
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_mlUnityAds_videoStart( JNIEnv*  env, jobject thiz )
	{
		ServicePlugin::shared().onVideoStarted();
	}
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_mlUnityAds_videoFinish( JNIEnv*  env, jobject thiz )
	{
		ServicePlugin::shared().onVideoFinihed( ServicePlugin::Result::Ok, 0 );
	}
}
