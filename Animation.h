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

#ifndef __Animation_h__
#define __Animation_h__
#include "cocos2d.h"
#include "stdio.h"

cocos2d::Animation* createAnimation(const std::string & path, int firstIndex, int lastIndex, const std::string & fileExt, float duration);
cocos2d::Animation* createAnimation(const std::string & path, std::vector<std::string> indexes, const std::string & fileExt, float duration);
cocos2d::Animation* createAnimation(std::vector<std::string> textures, float duration);

#endif