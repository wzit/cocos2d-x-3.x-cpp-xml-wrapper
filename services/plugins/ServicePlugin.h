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

#ifndef __ServicePlugin_h__
#define __ServicePlugin_h__

#include <map>
#include "Singlton.h"
#include "ParamCollection.h"
#include "services/adcolony/adcolony.h"
#include "Observer.h"

class ServicePlugin : public Singlton<ServicePlugin>
{
public:
	enum class Result
	{
		Ok,
		Fail,
		Canceled,
	};
	enum class Service
	{
		admob, 
		chartboost,
		flurry,
		adcolony,
		unityads,
		appodeal,
		fyber,
		none,
	};
	enum class Type
	{
		interstitialBanner,
		banner,
		rewardVideo,
		statistic,
	};
public:
	Observer < ServicePlugin, std::function< void()> > observerVideoStarted;
	Observer < ServicePlugin, std::function< void( bool, int )> > observerVideoResult;
	Observer < ServicePlugin, std::function< void( bool )> > observerVideoAvailabledChanged;

public:
	void use( Type type, Service servis );
	void cacheRewardedVideo();
	void cacheInterstitial();
	void showBanner();
	void hideBanner();
	void showInterstitialBanner();
	void showOffers();
	void showRewardVideo();

	void disableInterstitials();

	int getVideoRewardAmound();
	bool isVideoAvailabled();

	void sendStatistic( const ParamCollection& params );

	void onVideoStarted();
	void onVideoFinihed( Result result, int reward );
	void onVideoAvailabledChanged( bool availabled );
protected:
private:
	friend Singlton < ServicePlugin > ;
	ServicePlugin();
	virtual ~ServicePlugin();

	std::map<Type, std::list<Service>> _servises;
};


#endif
