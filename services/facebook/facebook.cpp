#include "facebook.h"
#include "ml/mlUserData.h"

namespace facebook
{
	static CallbackLogin onLogin;
	static CallbackLogout onLogout;

	void setCallback( CallbackLogin callback )
	{
		onLogin = callback;
	}

	void setCallback( CallbackLogout callback )
	{
		onLogout = callback;
	}

	std::string getFirstName()
	{
		return cocos2d::mlUserData::getInstance()->get_str( "facebook__login__fn" );
	}

	std::string getLastName()
	{
		return cocos2d::mlUserData::getInstance()->get_str( "facebook__login__ln" );
	}

	std::string getUserId()
	{
		return cocos2d::mlUserData::getInstance()->get_str( "facebook__login__id" );
	}

	void _onLogin( const std::string& firstname, const std::string& lastname, const std::string& id )
	{
		if( cocos2d::mlUserData::getInstance() )
		{
			cocos2d::mlUserData::getInstance()->write( "facebook__login__st", true );
			cocos2d::mlUserData::getInstance()->write( "facebook__login__fn", firstname );
			cocos2d::mlUserData::getInstance()->write( "facebook__login__ln", lastname );
			cocos2d::mlUserData::getInstance()->write( "facebook__login__id", id );
		}
		if( onLogin )
			onLogin( firstname, lastname, id );
	}

	void _onLogout()
	{
		if( onLogout )
			onLogout();
	}
}