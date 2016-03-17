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
	void logEvent( const ParamCollection & params )
	{
		[AppController flurryEvent:params];
	}
}