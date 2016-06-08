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

#ifndef __ml_Shaders_Cache__
#define __ml_Shaders_Cache__
#include "cocos2d.h"
#include "Singlton.h"
#include "IntrusivePtr.h"

NS_CC_BEGIN;

class CustomShadersCache : public Singlton < CustomShadersCache >
{
	friend class Singlton < CustomShadersCache > ;
	virtual void onCreate()override;
public:
	IntrusivePtr<GLProgram> program( const std::string& path );
private:
	void add( const std::string& path, GLProgram* program );
	void reload();
private:
	std::multimap<std::string, IntrusivePtr<GLProgram>> _programs;
};

NS_CC_END;
#endif