/*
 * This file is part of `et engine`
 * Copyright 2009-2012 by Sergey Reznik
 * Please, do not modify contents without approval.
 *
 */
#pragma once
#include <set>
#include <string>
#include "ml/Observer.h"


namespace et
{
	class PurchasesManager
	{
	public:
		typedef Observer<PurchasesManager, std::function<void(const std::string&)> > ObserverPurchaseRestored;
		typedef Observer<PurchasesManager, std::function<void(const std::string&)> > ObserverPurchaseFinished;
		typedef Observer<PurchasesManager, std::function<void(const std::string&)> > ObserverPurchaseCanceled;
		typedef Observer<PurchasesManager, std::function<void(const std::string&,const std::string&)> > ObserverPurchaseFailed;
	public:
		struct Product
		{
			std::string description;
			std::string title;
			std::string identifier;
			std::string currency;
			std::string currencySymbol;
			float price = 0.0f;
		};
		struct Transaction
		{
			std::string productId;
			std::string originalTransactionId;
			std::string transactionId;
		};
	public:
		typedef std::set<std::string> ProductsSet;
		static bool purchasesEnabled();
		static const std::string defaultVerificationServer;
		static const std::string defaultVerificationSandboxServer;
	public:
		void setShouldVerifyReceipts(bool);
		void checkAvailableProducts(const ProductsSet& products);
		void restorePurchases();
		bool purchaseProduct(const std::string& product);
		Product productForIdentifier(const std::string&);
		NSDictionary* receiptData() const;
	public:
		ObserverPurchaseRestored onPurchaseRestored;
		ObserverPurchaseFinished onPurchaseFinished;
		ObserverPurchaseCanceled onPurchaseCanceled;
		ObserverPurchaseFailed onPurchaseFailed;
//		ET_DECLARE_EVENT1(availableProductsChecked, StringList)
//		ET_DECLARE_EVENT0(failedToCheckAvailableProducts)
//		ET_DECLARE_EVENT1(productPurchased, Transaction)
//		ET_DECLARE_EVENT2(failedToPurchaseProduct, std::string, std::string)
//		ET_DECLARE_EVENT1(purchaseRestored, Transaction)
//		ET_DECLARE_EVENT0(restoringPurchasesFinished)
//		ET_DECLARE_EVENT0(failedToRestorePurchases)
	private:
		PurchasesManager();
		friend PurchasesManager& sharedPurchasesManager();
	};
	PurchasesManager& sharedPurchasesManager();
}