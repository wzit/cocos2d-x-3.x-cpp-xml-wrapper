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

#include <assert.h>
#include "ServicePlugin.h"
#include "ml/services/admob/AdMob.h"
#include "ml/services/flurry/flurry_.h"
#include "ml/services/tapjoy/tapjoy.h"
#include "ml/services/chartboost/chartboost.h"
#include "ml/services/unityads/unityads.h"
#include "ml/services/appodeal/appodeal_.h"
#include "ml/services/fyber/fyber_.h"
#include "ml/services/mlStatistic.h"
#include "ml/common.h"
#include "ml/mlUserData.h"

USING_NS_CC;

ServicePlugin::ServicePlugin()
{
}

ServicePlugin::~ServicePlugin()
{
}

void ServicePlugin::use( Type type, Service servis )
{
	_servises[type].push_back( servis );
}

void ServicePlugin::cacheRewardedVideo()
{
	if( _servises.find( Type::rewardVideo ) == _servises.end() )
		return;
	auto type = _servises.at( Type::rewardVideo ).front();
	switch( type )
	{
		case Service::fyber:
			fyber::cacheVideo();
			break;
		default:
			break;
	}
}

void ServicePlugin::cacheInterstitial()
{
	if( _servises.find( Type::interstitialBanner ) == _servises.end() )
		return;
	auto type = _servises.at( Type::interstitialBanner ).front();
	switch( type )
	{
		case Service::fyber:
			fyber::cacheInterstitial();
			break;
		default:
			break;

	}
}

void ServicePlugin::showBanner()
{
	if( _servises.find( Type::banner ) == _servises.end() )
		return;
	auto type = _servises.at( Type::banner ).front();
	switch( type )
	{
		case Service::admob:
			AdMob::show();
			break;
		case Service::appodeal:
			appodeal::showBanner();
			break;
		default:
			break;
	}
}

void ServicePlugin::hideBanner()
{
	if( _servises.find( Type::banner ) == _servises.end() )
		return;
	auto type = _servises.at( Type::banner ).front();
	switch( type )
	{
		case Service::admob: 
			AdMob::hide(); 
			break;
		case Service::appodeal:
			appodeal::hideBanner();
			break;
		default:
			assert( 0 );
	}
}

void ServicePlugin::showInterstitialBanner()
{
	bool disabled = mlUserData::getInstance()->get<bool>( "ServicePlugin_disableInterstitials" );

	if( disabled || _servises.find( Type::interstitialBanner ) == _servises.end() )
		return;
	const auto& types = _servises.at( Type::interstitialBanner );
	bool show(false);
	Service service;
	for( auto type : types )
	{
		switch( type )
		{
			case Service::admob:
				if( AdMob::isInitialized() && AdMob::interstitialIsAvailabled() )
				{
					AdMob::showInterstitial();
					show = true;
					service = Service::admob;
				}
				break;
			case Service::chartboost:
				if( chartboost::isInitialized() && chartboost::interstitialIsAvailabled() )
				{
					chartboost::showInterstitial();
					show = true;
					service = Service::chartboost;
				}
				break;
			case Service::appodeal:
				appodeal::showInterstitial();
				show = true;
				service = Service::appodeal;
				break;
			case Service::fyber:
				fyber::showInterstitial();
				show = true;
				service = Service::fyber;
				break;
			default:
				assert( 0 );
		}
		if( show )
			break;
	}

	ParamCollection pc;
	pc["event"] = "ad_interstitial";
	if( show )
	{
		if( service == Service::admob )pc["ad"] = "admob";
		if( service == Service::chartboost )pc["ad"] = "chartboost";
		if( service == Service::appodeal )pc["ad"] = "appodeal";
		if( service == Service::fyber )pc["ad"] = "fyber";
	}
	pc["successful"] = toStr(show);
	sendStatistic(pc);
}

void ServicePlugin::showOffers()
{
	tapjoy::showOffers();
}

void ServicePlugin::showRewardVideo()
{
	if( _servises.find( Type::rewardVideo ) == _servises.end() )
		return;
	auto type = _servises.at( Type::rewardVideo ).front();
	bool isShowed(false);
	ParamCollection pc;
	pc["ad"] = "";
	pc["started"] = "no";
	pc["finished"] = "no";

	switch( type )
	{
		case Service::adcolony:
		{
			if( adcolony::videoIsAvailabled() ) { adcolony::show(); isShowed = true; }
			else isShowed = false;
			pc["ad"] = "adcolony";
			break;
		}
		case Service::unityads:
		{
			if( unityads::videoIsAvailabled() ) { unityads::show(); isShowed = true; }
			else isShowed = false;
			pc["ad"] = "unityads";
			break;
		}
		case Service::appodeal:
		{
			if( appodeal::isVideoLoaded() ) { appodeal::showVideo(); isShowed = true; }
			else isShowed = false;
			pc["ad"] = "appodeal";
			break;
		}
		case Service::fyber:
		{
			if( fyber::isVideoLoaded() ) { fyber::showVideo(); isShowed = true; }
			else isShowed = false;
			pc["ad"] = "fyber";
			break;
		}
		default:
			break;
	}
	if( isShowed == false )
	{
		onVideoFinihed( Result::Fail, 0 );
		cocos2d::MessageBox( "No ads available. Please try again later.", "" );
	}

	mlStatistic::shared().accumulate( "ad_rewardervideo", pc, false );
}

void ServicePlugin::disableInterstitials()
{
	mlUserData::getInstance()->write( "ServicePlugin_disableInterstitials", true );
}

int ServicePlugin::getVideoRewardAmound()
{
	auto type = _servises.at( Type::rewardVideo ).front();
	switch( type )
	{
		case Service::adcolony:
			return adcolony::getVideoRewardAmound();
		case Service::unityads:
			return unityads::getVideoRewardAmound();
		case Service::appodeal:
			return appodeal::getVideoRewardAmound();
		case Service::fyber:
			return fyber::getVideoRewardAmound();
		default:
			break;
	}
	return 0;
}

bool ServicePlugin::isVideoAvailabled()
{
	auto type = _servises.at( Type::rewardVideo ).front();
	switch( type )
	{
		case Service::adcolony:
			return adcolony::videoIsAvailabled();
		case Service::unityads:
			return unityads::videoIsAvailabled();
		case Service::appodeal:
			return appodeal::isVideoLoaded();
		case Service::fyber:
			return fyber::isVideoLoaded();
		default:
			break;
	}
	return false;
}

void ServicePlugin::sendStatistic( const ParamCollection& params )
{
	if( _servises.find( Type::statistic ) == _servises.end() )
		return;
	auto type = _servises.at( Type::statistic ).front();
	switch( type )
	{
		case Service::flurry: 
			flurry::logEvent( params );
			break;
		default:
			assert(0);
	}
}

void ServicePlugin::onVideoStarted()
{
	ParamCollection pc;
	pc["started"] = "yes";
	mlStatistic::shared().accumulate( "ad_rewardervideo", pc, false );

	observerVideoStarted.pushevent();
}

void ServicePlugin::onVideoFinihed( Result result, int reward )
{
	ParamCollection pc;
	pc["finished"] = "yes";
	mlStatistic::shared().accumulate( "ad_rewardervideo", pc, true );

	observerVideoResult.pushevent( result == Result::Ok, reward );
}

void ServicePlugin::onVideoAvailabledChanged( bool availabled )
{
	observerVideoAvailabledChanged.pushevent( availabled );
	if( !availabled )
		cacheRewardedVideo();
}
