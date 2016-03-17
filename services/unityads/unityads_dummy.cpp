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

#include <stdio.h>
#include "unityads.h"

namespace unityads
{
	void show()
	{
		ServicePlugin::shared().onVideoStarted();
		ServicePlugin::shared().onVideoFinihed( ServicePlugin::Result::Ok, 0 );
	}

	bool videoIsAvailabled()
	{
		return true;
	}

	int getVideoRewardAmound()
	{
		return 0;
	}
}