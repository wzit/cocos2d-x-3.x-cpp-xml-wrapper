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

#ifndef IslandDefense_Vundle_h
#define IslandDefense_Vundle_h
#include <functional>

namespace vungle {
	typedef std::function<void(bool)> CallBackOnFinished;

	void playAd(const CallBackOnFinished & callback );
	void consume();
}
#endif
