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

#include "cocos2d.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "flurry_.h"
#include "cocos2d.h"
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include "ml/JniBind.h"
USING_NS_CC;

namespace {
	static const char * Activity = "org.cocos2dx.cpp/Flurry";
}

namespace flurry
{
	void logEvent( const ParamCollection & params )
	{
		JavaBind bind("org.cocos2dx.cpp", "Flurry", "logEvent", "%s");
		bind.call( params.string() );
	}
	void initialization( const std::string& id)
	{
		JavaBind bind("org.cocos2dx.cpp", "Flurry", "initialization", "%s");
		bind.call( id );
	}
}
#endif
