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

//
//  batch_ios.cpp
//  GreecoDefense
//
//  Created by Владимир Толмачев on 26.07.15.
//
//

#include "batch_ios.h"
#include "../appgratis.h"


namespace batch
{
	void OnRedeemResource( const std::string& name, const std::string& resource, int count )
	{
		cocos2d::appgratis::onRedeemResource( name, resource, count );
	}
	
	void OnRedeemOffer( const std::string& name, const std::string& params )
	{
		cocos2d::appgratis::Offer offer;
		offer.name = name;
		offer.params = ParamCollection( params );
		cocos2d::appgratis::onRedeemOffer( offer );
	}
}
