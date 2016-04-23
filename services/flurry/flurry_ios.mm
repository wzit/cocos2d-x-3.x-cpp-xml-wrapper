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

#import <Foundation/Foundation.h>
#import "AppController.h"
#include "ml/services/flurry/flurry_.h"

namespace flurry
{
	void initialization( const std::string& appId )
	{
		//NSString* string = [NSString stringWithUTF8String: appId.c_str()];
		//[Flurry startSession:string];
	}
	
	void logEvent( const ParamCollection & params )
	{
		/*
		NSMutableDictionary *dictionary = [[NSMutableDictionary alloc] init];
		for( auto& pair : params )
		{
			NSString * obj = [NSString stringWithUTF8String:pair.second.c_str()];
			NSString * key = [NSString stringWithUTF8String:pair.first.c_str()];
			[dictionary setObject:obj forKey:key];
		}
		
		NSString * event = [NSString stringWithUTF8String: params.at("event").c_str()];
		[Flurry logEvent:event withParameters:dictionary];
		 */
	}
}