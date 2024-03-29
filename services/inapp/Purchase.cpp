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

#include "Purchase.h"
#include "common.h"
#include "ParamCollection.h"
#include "IntrusivePtr.h"

#include "cocos2d.h"
#include "2d/CCFontAtlasCache.h"
//#include "consts.h"

USING_NS_CC;


static inapp::CallBackPurchase _callback_purchase;
static inapp::CallBackRequestDetails _callback_details;
static bool _locked = false;
static std::mutex _mutex;
static inapp::PurchaseResult _boughtResult;
static bool _boughtProduct;

namespace
{
	std::map<std::string, inapp::SkuDetails> _productDescriptions;
}

void inapp::details( const std::string & productID, const CallBackRequestDetails callback )
{
	_callback_details = callback;
	auto it = _productDescriptions.find(productID);
	if( it != _productDescriptions.end() )
	{
		if( _callback_details )
			_callback_details(it->second);
	}
	else
	{
		requestDetails( productID );
	}
}

void inapp::purchase( const std::string & product, const CallBackPurchase & callback )
{
	static bool update( true );
	if( update )
	{
		static IntrusivePtr<Node> dummy( Node::create() );
		auto scheduler = Director::getInstance()->getScheduler();
		auto callback = std::bind( inapp::mainThreadUpdate, std::placeholders::_1 );
		scheduler->schedule( callback, dummy.ptr(), 0, false, "inapps" );
		update = false;
	}
	if( _locked == false )
	{
		_locked = true;
		_callback_purchase = callback;

		requestPurchase( product );
	}
}

void inapp::requestResultsDetails( const SkuDetails& result )
{
	if( result.result == Result::Ok )
	{
		_productDescriptions.emplace( result.productId, result );
	}

	if( _callback_details )
		_callback_details( result );
}

void inapp::requestResultsPurchase( PurchaseResult result )
{
	_mutex.lock();
	_boughtProduct = true;
	_boughtResult = result;
	_mutex.unlock();
	_locked = false;
}

void inapp::mainThreadUpdate( float dt )
{
	_mutex.lock();
	if( _boughtProduct )
	{
		if( _callback_purchase )
			_callback_purchase( _boughtResult );
		_boughtProduct = false;
	}
	_mutex.unlock();
}
