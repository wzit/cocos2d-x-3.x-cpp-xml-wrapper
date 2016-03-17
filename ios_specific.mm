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

#include "common.h"

void openUrl( const std::string & url_ )
{
	NSString * string = [[NSString alloc]initWithUTF8String:url_.c_str()];
	NSURL * url = [NSURL URLWithString:string];
	
	[[UIApplication sharedApplication] openURL:url];
}

NS_CC_BEGIN
NS_CC_END;