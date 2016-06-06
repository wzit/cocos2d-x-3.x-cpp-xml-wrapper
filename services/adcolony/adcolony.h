/******************************************************************************/
/*
 * Copyright 2014-2015 Vladimir Tolmachev
 *
 * Author: Vladimir Tolmachev
 * Project: ml
 * e-mail: tolm_vl@hotmail.com
 * If you received the code is not the author, please contact me
 */
/******************************************************************************/


#ifndef __AdColony_h__
#define __AdColony_h__
#include <functional>
#include "services/plugins/ServicePlugin.h"

namespace adcolony
{
	void initialization(const std::string& id, const std::string& zoneId);
	void show();
	bool videoIsAvailabled();
	int getVideoRewardAmound();
}

#endif
