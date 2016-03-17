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

#include "tapjoy.h"
#include "cocos2d.h"
#include "ml/JniBind.h"

USING_NS_CC;
extern
"C"
{
	JNIEXPORT void JNICALL  Java_org_cocos2dx_cpp_mlTapjoy_nativeGetBalance( JNIEnv*  env, jobject thiz, bool success, int balance )
	{
		if( tapjoy::GetBalance )
			tapjoy::GetBalance( balance );
	}
}


namespace tapjoy
{
	std::function<void( int )> GetBalance;

	void showOffers()
	{
		JavaBind bind( "org.cocos2dx.cpp", "mlTapjoy", "showOffers", "" );
		bind.call();
	}

	void getBalance()
	{
		JavaBind bind( "org.cocos2dx.cpp", "mlTapjoy", "requestBalance", "" );
		bind.call();
	}
};
