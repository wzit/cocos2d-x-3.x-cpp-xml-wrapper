#include "facebook.h"
#include <assert.h>

namespace facebook
{
	void login()
	{
		_onLogin( "John", "Doy", "12312315235" );
	}
	void logout()
	{
		_onLogout();
	}

	void share( const std::string& ulr, const std::string& imageUrl, const std::string& description )
	{
		assert( ulr.empty() == false && imageUrl.empty() == false );
	}
}
