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
#include "purchase.h"
#include "ml/JniBind.h"
#include "ml/ParamCollection.h"
#include "ml/common.h"
#include "cocos2d.h"

USING_NS_CC;

static const char * Activity = "org.cocos2dx.cpp/InApps";

extern
"C"
{
	JNIEXPORT void JNICALL  Java_org_cocos2dx_cpp_InApps_nativeResultDetails(JNIEnv*  env, jobject thiz, jstring results)
	{
		std::string str = JniHelper::jstring2string(results);
		ParamCollection params( str );
		inapp::SkuDetails result;
		
		result.result = (params["result"] == "ok") ? inapp::Result::Ok : inapp::Result::Fail;
		if( result.result == inapp::Result::Ok )
		{
			result.productId = params["id"];
			result.title = params["title"];
			result.description = params["description"];
			result.priceText = params["pricetext"];
			result.currency = params["currency"];
			result.priceValue = strToFloat( params["pricevalue"] );
			
			if( result.currency != "RUB" && result.currency != "USD" )
			{
				result.priceText = toStr( result.priceValue ) + " " + result.currency;
			}
		}
		inapp::requestResultsDetails( result );
	}
	JNIEXPORT void JNICALL  Java_org_cocos2dx_cpp_InApps_nativeResultPurchase(JNIEnv*  env, jobject thiz, jstring results)
	{
		std::string str = JniHelper::jstring2string(results);

		ParamCollection params( str );

		/*
		log("---------------------------");
		log("---------------------------");
		log("[%s]", str.c_str());
		log("[%s]", params.string().c_str());
		log("[%s]", params["result"].c_str());
		log("[%s]", params["id"].c_str());
		log("[%s]", params["errorcode"].c_str());
		log("[%s]", params["errormsg"].c_str());
		log("---------------------------");
		log("---------------------------");
		*/

		inapp::PurchaseResult result;
		result.errorcode = 0;
		result.result = (params["result"] == "ok") ? inapp::Result::Ok : inapp::Result::Fail;
		result.productId = params["id"];
		result.errorcode = strToInt( params["errorcode"] );
		result.errormsg = params["errormsg"];
		
		inapp::requestResultsPurchase( result );
	}
}

void inapp::requestDetails( const std::string & productID )
{
	JavaBind bind("org.cocos2dx.cpp", "InApps", "requestDetails", "%s");
	bind.call( productID );
}

void inapp::requestPurchase( const std::string & productID )
{
	JavaBind bind("org.cocos2dx.cpp", "InApps", "purchase", "%s");
	bind.call( productID );
}

void inapp::confirm( const std::string & product )
{
	JavaBind bind("org.cocos2dx.cpp", "InApps", "consume", "%s");
	bind.call( product );
}

void inapp::restore()
{}

#endif
