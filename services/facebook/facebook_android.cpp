#include "facebook.h"
#include <assert.h>


#include "ml/JniBind.h"
#include "cocos2d.h"

USING_NS_CC;

namespace
{
	const std::string& facebook_package( "org.cocos2dx.cpp" );
	const std::string& facebook_javaClass( "AppActivity" );
}

namespace facebook
{
	void login()
	{
		JavaBind bind( facebook_package, facebook_javaClass, "facebook_login", "" );
		bind.call();
	}
	void logout()
	{
		JavaBind bind( facebook_package, facebook_javaClass, "facebook_logout", "" );
		bind.call();
	}

	void share( const std::string& ulr, const std::string& imageUrl, const std::string& description )
	{
		JavaBind bind( facebook_package, facebook_javaClass, "facebook_share", "%s%s%s" );
		bind.call( ulr, imageUrl, description );
	}
}

#define METHOD(name)  Java_org_cocos2dx_cpp_AppActivity_##name
extern
"C"
{
	JNIEXPORT void JNICALL  METHOD( facebook_1connected )(JNIEnv*  env, jobject thiz, jstring js1, jstring js2, jstring js3)
	{
		std::string s1 = JniHelper::jstring2string( js1 );
		std::string s2 = JniHelper::jstring2string( js2 );
		std::string s3 = JniHelper::jstring2string( js3 );
		facebook::_onLogin( s1, s2, s3 );
	}

	JNIEXPORT void JNICALL  METHOD( facebook_1disconnected )(JNIEnv*  env, jobject thiz)
	{
		facebook::_onLogout();
	}
}