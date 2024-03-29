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

#ifndef __AdMob_h__
#define __AdMob_h__


namespace AdMob
{
	void show();
	void hide();

	void initialization( const std::string & appID );
	void showInterstitial();
	void activateTestMode();
	bool isInitialized();
	bool interstitialIsAvailabled();

}

#endif
