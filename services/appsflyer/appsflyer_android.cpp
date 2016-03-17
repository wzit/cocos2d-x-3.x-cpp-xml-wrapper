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
#include "appsflyer.h"
#include "cocos2d.h"
#include <jni.h>
#include "platform/android/jni/JniHelper.h"

USING_NS_CC;

namespace {
	static const char * Activity = "org.cocos2dx.cpp/Appsflyer";
}

namespace appsflyer
{
	void logEvent( const ParamCollection & params )
	{
		//JniMethodInfo t;
		//if( JniHelper::getStaticMethodInfo( t, Activity, "logEvent", "(Ljava/lang/String;)V" ) )
		//{
		//	jstring stringArg = t.env->NewStringUTF( params.string().c_str() );
		//	t.env->CallStaticIntMethod( t.classID, t.methodID, stringArg );
		//	t.env->DeleteLocalRef( stringArg );
		//	t.env->DeleteLocalRef( t.classID );
		//}
		//else
		//{
		//	log( "............................." );
		//	log( "appsflyer::logEvent::jni appsflyerLogEvent notfound" );
		//	log( "............................." );
		//}
	}
}
#endif