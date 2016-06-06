/******************************************************************************/
/*
 * Copyright 2014-2015 Vladimir Tolmachev
 *
 * Author: Vladimir Tolmachev
 * Project: ml
 * e-mail: tolm_vl@hotmail.com
 * If you received the code is not the author, please contact me
 */
/******************************************************************************/

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || \
	(CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || \
	(CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
#include "purchase.h"
#include <chrono>
#include "cocos2d.h"

USING_NS_CC;

void dummy_details( const std::string & productID )
{
	auto sku = inapp::Emulator::getDetails( productID );
	inapp::requestResultsDetails( sku );
}

void dummy_purchase( const std::string & productID )
{
	std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
	inapp::PurchaseResult res;
	res.result = inapp::Result::Ok;
	res.productId = productID;
	inapp::requestResultsPurchase( res );
}

void inapp::requestDetails( const std::string & productID )
{
	dummy_details( productID );
}

void inapp::requestPurchase( const std::string & productID )
{
	std::thread thread( [productID](){dummy_purchase( productID ); } );
	thread.detach();
}

void inapp::confirm( const std::string & product )
{}

void inapp::restore()
{}

#endif
