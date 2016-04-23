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
#include "facebook.h"
#include <assert.h>
#include "ml/mlUserData.h"

namespace facebook
{
	void login( bool friends )
	{
		cocos2d::mlUserData::getInstance()->write( "facebook_login", true );
		_onLogin( "John", "Doy", "12312315235" );
	}
	void logout()
	{
		cocos2d::mlUserData::getInstance()->write( "facebook_login", false );
		_onLogout();
	}

	bool isLogged()
	{
		return cocos2d::mlUserData::getInstance()->get<bool>( "facebook_login" );
	}

	void share( const std::string& ulr, const std::string& imageUrl, const std::string& description )
	{
		assert( ulr.empty() == false && imageUrl.empty() == false );
	}

	void shareLink( const std::string& ulr, const std::string& imageUrl, const std::string& description )
	{
		assert( ulr.empty() == false );
	}
}
