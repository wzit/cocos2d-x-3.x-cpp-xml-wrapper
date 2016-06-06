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

#include "flurry_.h"

namespace flurry
{
	bool initialized( false );
	void initialization( const std::string& appId )
	{
		assert( !initialized );
		initialized = true;
	}

	void logEvent( const ParamCollection & params )
	{
		assert( params.size() < 10 );
	}
}
