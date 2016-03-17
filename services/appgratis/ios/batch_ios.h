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

#ifndef __GreecoDefense__batch_ios__
#define __GreecoDefense__batch_ios__

#include <string>

namespace batch
{
	void OnRedeemResource( const std::string& name, const std::string& resource, int count );
	void OnRedeemOffer( const std::string& name, const std::string& params );
}

#endif
