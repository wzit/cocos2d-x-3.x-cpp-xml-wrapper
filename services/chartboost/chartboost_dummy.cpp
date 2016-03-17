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

#include "chartboost.h"

namespace chartboost
{
	void initialization( const std::string appID, const std::string& appSignature ) {}
	void showInterstitial() {}
	void initialization( const std::string & appID ) {}
	void activateTestMode() {}
	bool isInitialized() { return false; }
	bool interstitialIsAvailabled() { return false; }
}
