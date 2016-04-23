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
#include "facebook.h"
#include <assert.h>
#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import <FBSDKLoginKit/FBSDKLoginKit.h>
#import "AppController.h"

namespace facebook
{
	void applicationDidBecomeActive(UIApplication *application) {
		[FBSDKAppEvents activateApp];
	}
	
	void application(UIApplication *application, NSDictionary *launchOptions) {
		[[FBSDKApplicationDelegate sharedInstance] application:application
								 didFinishLaunchingWithOptions:launchOptions];
	}
	
	void login( bool friends )
	{
//		logout();
//		return;
		FBSDKLoginManager *login = [[FBSDKLoginManager alloc] init];
		[login logInWithReadPermissions: @[@"public_profile", @"user_friends"]
					 fromViewController:(UIViewController*)[AppController getInstance].viewController
								handler:^(FBSDKLoginManagerLoginResult *result, NSError *error) {
									if (error) {
										NSLog(@"Process error");
									} else if (result.isCancelled) {
										NSLog(@"Cancelled");
									} else {
										NSLog(@"Logged in");
									}
								}];
	}
	void logout()
	{
		FBSDKLoginManager *login = [[FBSDKLoginManager alloc] init];
		[login logOut];
	}
	
	bool isLogged()
	{
		FBSDKAccessToken* token = [FBSDKAccessToken currentAccessToken];
		return token != nil;
	}
	
	void share( const std::string& ulr, const std::string& imageUrl, const std::string& description )
	{
	}
	
	void shareLink( const std::string& ulr, const std::string& imageUrl, const std::string& description )
	{
	}
}