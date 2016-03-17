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

#include "Purchase.h"
#include "ml/common.h"
#include "ml/ParamCollection.h"

#include "cocos2d.h"
#include "2d/CCFontAtlasCache.h"
//#include "consts.h"

USING_NS_CC;

static inapp::CallBackPurchase _callback_purchase;
static inapp::CallBackRequestDetails _callback_details;
static bool _locked = false;

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
	if( _callback_purchase )
		_callback_purchase( result );
	_locked = false;

}
