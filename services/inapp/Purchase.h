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

#ifndef __Purchase_h__
#define __Purchase_h__
#include <functional>
#include <list>
#include <string>

class inapp
{
public:
	enum class Result
	{
		Ok,
		Fail,
	};

	struct SkuDetails
	{
		SkuDetails():priceValue(-1), result(Result::Fail){}
		SkuDetails(
			float _priceValue,
			std::string _priceText,
			std::string _productId,
			std::string _description,
			std::string _title,
			std::string _currency,
			Result _result )
			: priceValue( _priceValue )
			, priceText( _priceText )
			, productId( _productId )
			, description( _description )
			, title( _title )
			, currency( _currency )
			, result( _result )
		{}

		float priceValue;
		std::string priceText;
		std::string productId;
		std::string description;
		std::string title;
		std::string currency;
		Result result;
	};
	
	struct PurchaseResult
	{
		PurchaseResult():result(Result::Fail), errorcode(0){}
		Result result;
		std::string productId;
		std::string errormsg;
		int errorcode;
	};

	struct Emulator
	{
		static inapp::SkuDetails getDetails( const std::string& product );
	};

	typedef std::function< void( PurchaseResult ) > CallBackPurchase;
	typedef std::function< void( SkuDetails ) > CallBackRequestDetails;
	
	static void onRestored( std::string item );
	static void fetch( std::list<std::string> & items );
	static void details(const std::string & productID, const CallBackRequestDetails callback);
	static void purchase( const std::string & product, const CallBackPurchase & callback);
	static void confirm( const std::string & product );
	static void restore();

	static void requestResultsDetails( const SkuDetails& details );
	static void requestResultsPurchase( PurchaseResult result );
private:
	static void requestPurchase( const std::string & productID );
	static void requestDetails( const std::string & productID );
	static void mainThreadUpdate( float dt );
private:
};


#endif
