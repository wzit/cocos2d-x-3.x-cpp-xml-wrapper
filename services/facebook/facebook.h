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
#ifndef __FACEBOOK_H__
#define __FACEBOOK_H__
#include <string>
#include <functional>

namespace facebook
{
	typedef std::function<void( const std::string& firstname, const std::string& lastname, const std::string& id )> CallbackLogin;
	typedef std::function<void()> CallbackLogout;

	void setCallbackLogin( CallbackLogin callback );
	void setCallbackLogout( CallbackLogout callback );
	void login( bool friends );
	void logout();
	void share( const std::string& ulr, const std::string& imageUrl, const std::string& description );
	void shareLink( const std::string& ulr, const std::string& title, const std::string& description );

	bool isLogged();
	std::string getFirstName();
	std::string getLastName();
	std::string getUserId();

	void _onLogin( const std::string& firstname, const std::string& lastname, const std::string& id );
	void _onLogout();
}

#endif