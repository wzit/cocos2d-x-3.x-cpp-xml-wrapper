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

#import "AppController.h"
#import <Chartboost/Chartboost.h>
#include <string>
#include "cocos2d.h"


@interface ChartboostDelegateImpl : NSObject<ChartboostDelegate>
@end

@implementation ChartboostDelegateImpl
- (void)didInitialize:(BOOL)status
{
	cocos2d::log( "didInitialize");
}
@end

namespace chartboost
{
	void initialization( const std::string appID, const std::string& appSignature )
	{
		static ChartboostDelegateImpl* impl = [[ChartboostDelegateImpl alloc] init];
		
		NSString* appId = [NSString stringWithUTF8String:appID.c_str()];
		NSString* appKey = [NSString stringWithUTF8String:appSignature.c_str()];
		// Initialize the Chartboost library
		[Chartboost startWithAppId:appId
					  appSignature:appKey
						  delegate:impl];
	}
	
	void activateTestMode()
	{
	}
	bool isInitialized()
	{
		return true;
	}
	bool interstitialIsAvailabled()
	{
		return true;
	}
	
	void showInterstitial()
	{
		[Chartboost showInterstitial:CBLocationHomeScreen];
	}
	
	void showMoreApps()
	{
		//[AppController chartboostShowMoreApps];
	}
}
