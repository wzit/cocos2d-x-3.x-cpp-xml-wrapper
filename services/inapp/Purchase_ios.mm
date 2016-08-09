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

//
//  Purchase_ios.m
//  IslandDefense
//
//  Created by Владимир Толмачев on 07.12.14.
//
//
#include "ml/ParamCollection.h"
#include "Purchase.h"
#import "ios/iap_objc.h"
#include "ml/common.h"

using namespace et;

enum class TransactionResult
{
	ok,
	canceled,
	restored,
	failed,
};

void requestResult( const std::string & product, const std::string & message, TransactionResult result)
{
	ParamCollection params;
	if( result == TransactionResult::ok )
		params["result"] = "ok";
	if( result == TransactionResult::failed )
		params["result"] = "failed";
	if( result == TransactionResult::canceled )
		params["result"] = "canceled";
	params["id"] = product;
	params["errormsg"] = message;
	
	inapp::PurchaseResult res;
	res.result = result == TransactionResult::ok ? inapp::Result::Ok : inapp::Result::Fail;
	if( result == TransactionResult::canceled )
		res.result = inapp::Result::Fail;
	res.productId = product;
	res.errormsg = message;
	
	inapp::requestResultsPurchase( res );
}

void inapp::requestDetails( const std::string & productID )
{
	static bool first(true);
	if( first )
	{
		first = false;
		
		std::list<std::string> items;
		PurchasesManager::ProductsSet products;
		inapp::fetch(items);
		
		for( auto& item : items )
		{
			products.insert(item);
		}
		sharedPurchasesManager().checkAvailableProducts( products );
		
		auto c0 = std::bind( inapp::onRestored, std::placeholders::_1 );
		auto c1 = std::bind( ::requestResult, std::placeholders::_1, "", TransactionResult::ok );
		auto c2 = std::bind( ::requestResult, std::placeholders::_1, std::placeholders::_2, TransactionResult::failed );
		auto c3 = std::bind( ::requestResult, std::placeholders::_1, "", TransactionResult::canceled );
		sharedPurchasesManager().onPurchaseRestored.add(0x1, c0 );
		sharedPurchasesManager().onPurchaseFinished.add(0x1, c1 );
		sharedPurchasesManager().onPurchaseFailed.add(0x1, c2 );
		sharedPurchasesManager().onPurchaseCanceled.add(0x1, c3 );
	}
	
	auto product = sharedPurchasesManager().productForIdentifier(productID);
	if( product.identifier == productID )
	{
		inapp::SkuDetails details;

		details.priceValue = product.price;;
		details.priceText = product.currency + ":" + cocos2d::toStr(product.price);
		details.productId = product.identifier;
		details.description = product.description;
		details.title = product.title;
		details.currency = product.currency;
		details.result = Result::Ok;
		
		inapp::requestResultsDetails( details );
	}
}

void inapp::requestPurchase( const std::string & productID )
{
	sharedPurchasesManager().purchaseProduct(productID);
}

void inapp::confirm( const std::string & product )
{
	/*
	 How do I resolve the "You've already purchased this In-App Purchase but it hasn't been downloaded." error message?
	 
	 You are getting the "You've already purchased this In-App Purchase but it hasn't been downloaded." error message because you did not call SKPaymentQueue 's finishTransaction:method in your application. Calling finishTransaction: allows you to remove a transaction from the payment queue.
	 
	 -(void)completeTransaction:(SKPaymentTransaction *)transaction
	 
	 {
	 
	 NSData *receiptData = [transaction transactionReceipt];
	 
	 }
	 */
}

void inapp::restore()
{
	sharedPurchasesManager().restorePurchases();
}
