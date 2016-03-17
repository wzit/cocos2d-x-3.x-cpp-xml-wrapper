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
//  chartboost_ios.cpp
//  IslandDefense
//
//  Created by Владимир Толмачев on 03.12.14.
//
//

#include "AppController.h"
#include <string>

namespace chartboost
{
	void initialization( const std::string appID, const std::string& appSignature )
	{ }
	void activateTestMode(){}
	bool isInitialized(){return true;}
	bool interstitialIsAvailabled()
	{
		return [AppController chartboostIsAvailabled];
	}
	
	void showInterstitial()
	{
		[AppController chartboostShowInterstitial];
	}
	
	void showMoreApps()
	{
		//[AppController chartboostShowMoreApps];
	}
}
