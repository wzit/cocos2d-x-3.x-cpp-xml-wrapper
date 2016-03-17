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

#ifndef __ml_chartboost_h__
#define __ml_chartboost_h__

#include <string>

namespace chartboost
{
	void initialization( const std::string appID, const std::string& appSignature );
	void showInterstitial();
	void activateTestMode();
	bool isInitialized();
	bool interstitialIsAvailabled();
}

#endif
