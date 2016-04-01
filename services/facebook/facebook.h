#ifndef __FACEBOOK_H__
#define __FACEBOOK_H__
#include <string>
#include <functional>

namespace facebook
{
	typedef std::function<void( const std::string& firstname, const std::string& lastname, const std::string& id )> CallbackLogin;
	typedef std::function<void()> CallbackLogout;

	void setCallback( CallbackLogin callback );
	void setCallback( CallbackLogout callback );
	void login();
	void logout();
	void share( const std::string& ulr, const std::string& imageUrl, const std::string& description );

	std::string getFirstName();
	std::string getLastName();
	std::string getUserId();

	void _onLogin( const std::string& firstname, const std::string& lastname, const std::string& id );
	void _onLogout();
}

#endif