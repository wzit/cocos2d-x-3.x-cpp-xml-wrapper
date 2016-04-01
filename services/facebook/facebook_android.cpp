#include "facebook.h"
#include <assert.h>


#include "ml/JniBind.h"
#include "cocos2d.h"

USING_NS_CC;

namespace
{
	const std::string& facebook_package( "com.mixgamesapps.facebook" );
	const std::string& facebook_javaClass( "Facebook" );
}

namespace facebook
{
	void login( bool friends )
	{
		JavaBind bind( facebook_package, facebook_javaClass, "login", "%b" );
		bind.call( friends );
	}
	void logout()
	{
		JavaBind bind( facebook_package, facebook_javaClass, "logout", "" );
		bind.call();
	}

	void share( const std::string& ulr, const std::string& imageUrl, const std::string& description )
	{
		JavaBind bind( facebook_package, facebook_javaClass, "share", "%s%s%s" );
		bind.call( ulr, imageUrl, description );
	}

	void shareLink( const std::string& ulr, const std::string& title, const std::string& description )
	{
		JavaBind bind( facebook_package, facebook_javaClass, "shareLink", "%s%s%s" );
		bind.call( ulr, title, description );
	}

	bool isLogged()
	{
		JavaBind bind( facebook_package, facebook_javaClass, "isLogged", "" );
		return bind.bool_call();
	}
}

#define METHOD(name)  Java_com_mixgamesapps_facebook_Facebook_##name
extern
"C"
{
	JNIEXPORT void JNICALL  METHOD( nativeOnLogin )(JNIEnv*  env, jobject thiz, jstring js1, jstring js2, jstring js3)
	{
		std::string s1 = JniHelper::jstring2string( js1 );
		std::string s2 = JniHelper::jstring2string( js2 );
		std::string s3 = JniHelper::jstring2string( js3 );
		facebook::_onLogin( s1, s2, s3 );
	}

	JNIEXPORT void JNICALL  METHOD( nativeOnLogout )(JNIEnv*  env, jobject thiz)
	{
		facebook::_onLogout();
	}
}