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

#ifndef __ml_Generics__
#define __ml_Generics__

#include <string>
#include "cocos2d.h"

NS_CC_BEGIN;

template <typename T> T strTo(const std::string &value);
template <typename T> std::string toStr(T value);

NS_CC_END;

#endif