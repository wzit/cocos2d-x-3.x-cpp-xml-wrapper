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
#include <thread>
#include "adcolony.h"
#include "AppController.h"

namespace adcolony
{
	void show()
	{
		[AppController AdColonyShowV4VC];
	}

	bool videoIsAvailabled()
	{
		return [AppController AdColonyIsVideoAvailabled];
	}
	
	int getVideoRewardAmound()
	{
		return [AppController AdColonyGetVideoRewardAmound];
	}
	
}