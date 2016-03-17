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

#include "adcolony.h"
#include "cocos2d.h"

#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include "ml/JniBind.h"

USING_NS_CC;

namespace adcolony
{
	int _rewardAmount = 0;
	bool _videoIsAvailabled( false );

	void initialization(const std::string& id, const std::string& zoneId)
	{
		JavaBind bind("org.cocos2dx.cpp", "Adcolony", "init", "%s%s");
		bind.call(id, zoneId);
	}

	void show()
	{
		JavaBind bind("org.cocos2dx.cpp", "Adcolony", "showVideoV4VC", "");
		bind.call();
	}

	bool videoIsAvailabled()
	{
		return _videoIsAvailabled;
	}

	int getVideoRewardAmound()
	{
		return _rewardAmount;
	}
}

extern
"C"
{
	JNIEXPORT void JNICALL  Java_org_cocos2dx_cpp_Adcolony_onAdColonyAdAvailability( JNIEnv*  env, jobject thiz, bool value, jstring zoneid, int reward )
	{
		adcolony::_videoIsAvailabled = value;
		adcolony::_rewardAmount = reward;
		ServicePlugin::shared().onVideoAvailabledChanged( value );
	}
	JNIEXPORT void JNICALL  Java_org_cocos2dx_cpp_Adcolony_onAdColonyAdV4VCReward( JNIEnv*  env, jobject thiz, jstring rewardtype, int reward )
	{
		ServicePlugin::shared().onVideoFinihed( ServicePlugin::Result::Ok, reward );
	}
	JNIEXPORT void JNICALL  Java_org_cocos2dx_cpp_Adcolony_onAdColonyAdStarted( JNIEnv*  env, jobject thiz )
	{
		ServicePlugin::shared().onVideoStarted();
	}
	JNIEXPORT void JNICALL  Java_org_cocos2dx_cpp_Adcolony_onAdColonyAdFailed( JNIEnv*  env, jobject thiz )
	{
		ServicePlugin::shared().onVideoFinihed( ServicePlugin::Result::Fail, 0 );
	}
}

