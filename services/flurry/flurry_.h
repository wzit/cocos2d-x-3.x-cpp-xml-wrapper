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

#ifndef __ml_flurry__
#define __ml_flurry__

#include "ParamCollection.h"

namespace flurry
{
	void initialization( const std::string& appId );
	void logEvent( const ParamCollection & params );
}

#endif
