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

#include "cocos2d.h"
#include "AdMob.h"
#include "AppController.h"

USING_NS_CC;

namespace AdMob
{
	void show()
	{
		//AdMob_show();
	}
	void hide()
	{
		//AdMob_hide();
	}
	void showInterstitial()
	{
		[AppController admobShowInterstitial];
	}
	void initialization( const std::string & appID ){}
	void activateTestMode() {}
	bool isInitialized() { return true; }
	bool interstitialIsAvailabled() { return true; }
}

