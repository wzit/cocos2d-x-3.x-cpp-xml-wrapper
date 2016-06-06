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


#ifndef __Appodeal_h__
#define __Appodeal_h__
#include <functional>
#include "services/plugins/ServicePlugin.h"

namespace appodeal
{
	void showInterstitial();
	void showBanner();
	void hideBanner();
	void showVideo();
	bool isVideoLoaded();
	bool isInterstitialLoaded();
	int getVideoRewardAmound();
}

#endif
