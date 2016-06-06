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


#ifndef __Fyber_h__
#define __Fyber_h__
#include <functional>

namespace fyber
{
	void cacheInterstitial();
	void cacheVideo();
	void initialization( const std::string& id, const std::string& key );
	void showInterstitial();
	void showVideo();
	bool isVideoLoaded();
	bool isInterstitialLoaded();
	int getVideoRewardAmound();
}

#endif
