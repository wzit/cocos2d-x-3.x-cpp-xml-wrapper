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
#include "vungle.h"
#include "cocos2d.h"
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <chrono>
#include "ml/JniBind.h"
USING_NS_CC;

#define JAVA(method)JNIEXPORT void JNICALL  Java_org_cocos2dx_cpp_Vungle_##method(JNIEnv*  env, jobject thiz, jstring results)

static const char * Activity = "org.cocos2dx.cpp/Vungle";

namespace vungle
{
	void onStart( const std::string & args );
	void onFinished( const std::string & args );
	void AdUnavailable( const std::string & args );
	void onCashedAvailabled( const std::string & args );
}

extern
"C"
{
	JAVA(nativeVungleStart) {
		std::string str = JniHelper::jstring2string(results);  log( "Java native nativeVungleStart: \n\t\t%s", str.c_str());
		vungle::onStart( str );
	}
	JAVA(nativeVungleFinished) {
		std::string str = JniHelper::jstring2string(results);  log( "Java native nativeVungleFinished: \n\t\t%s", str.c_str());
		vungle::onFinished( str );
	}
	JAVA(nativeVungleAdUnavailable) {
		std::string str = JniHelper::jstring2string(results);  log( "Java native nativeVungleAdUnavailable: \n\t\t%s", str.c_str());
		vungle::AdUnavailable( str );
	}
	JAVA(nativeVungleCachedAdAvailable) {
		std::string str = JniHelper::jstring2string(results);  log( "Java native nativeVungleCachedAdAvailable: \n\t\t%s", str.c_str());
		vungle::onCashedAvailabled( str );
	}
}

namespace vungle {
	static CallBackOnFinished _callback;
	static bool succesful = false;
	
	void playAd(const CallBackOnFinished & callback)
	{
		_callback = callback;

		JavaBind bind("org.cocos2dx.cpp", "Vungle", "playAd", "%s");
		bind.call("");
		/*
		JniMethodInfo t;
		if( JniHelper::getStaticMethodInfo( t, Activity, "playAd", "(Ljava/lang/String;)V" ) )
		{
			log( "............................." );
			log( "vungle::playAd  java playAd founded" );
			log( "............................." );
			
			jstring stringArg = t.env->NewStringUTF( "" );
			t.env->CallStaticIntMethod( t.classID, t.methodID, stringArg );
			t.env->DeleteLocalRef( stringArg );
			t.env->DeleteLocalRef( t.classID );
		}
		else
		{
			log( "............................." );
			log( "vungle::playAd::jni playAd notfounded" );
			log( "............................." );

			if( _callback )
				_callback(false);
		}
		*/
	}

	void onStart( const std::string & args )
	{
		succesful = true;
	}
	void onFinished( const std::string & args )
	{
		if( _callback )
			_callback(succesful);
	}
	void AdUnavailable( const std::string & args )
	{
		succesful = false;
	}
	void onCashedAvailabled( const std::string & args )
	{
	}

	void consume()
	{
		_callback = nullptr;
		succesful = false;
	}
}

#undef JAVA
#endif
