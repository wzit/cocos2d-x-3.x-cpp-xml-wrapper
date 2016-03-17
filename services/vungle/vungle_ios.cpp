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

#include "vungle.h"
#include "cocos2d.h"

USING_NS_CC;

namespace vungle {
	void playAd( const CallBackOnFinished & callback )
	{
		if(callback )
			callback(true);
	}
	
	void consume()
	{
	}
}