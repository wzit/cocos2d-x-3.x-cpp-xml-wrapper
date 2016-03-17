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
#include "appgratis.h"
#include "cocos2d.h"
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <list>
#include <thread>

USING_NS_CC;

#define JAVA(method)JNIEXPORT void JNICALL  Java_org_cocos2dx_cpp_AppGratisBatch_##method

namespace {
	static const char * Activity = "org.cocos2dx.cpp/AppGratisBatch";
}

extern
"C"
{

	JAVA(nativeOnRedeemResource)(JNIEnv*  env, jobject thiz, jstring offername, jstring resource, int count ) {
		std::string res = JniHelper::jstring2string(resource);
		std::string name  = JniHelper::jstring2string(offername);

		appgratis::onRedeemResource( name, res, count );
	}
	JAVA(nativeOnRedeemOffer)(JNIEnv*  env, jobject thiz, jstring offername, jstring paramstring ) {
		std::string params = JniHelper::jstring2string(paramstring);
		std::string name  = JniHelper::jstring2string(offername);
	
		appgratis::Offer offer;
		offer.name = name;
		offer.params = ParamCollection( params );
		for( auto p : offer.params )
			log( "[%s = %s]", p.first.c_str(), p.second.c_str() );
		appgratis::onRedeemOffer( offer );
	}

}

namespace appgratis
{
}
#endif
