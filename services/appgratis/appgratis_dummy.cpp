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

#include "appgratis.h"
#include <chrono>

namespace appgratis
{
	void setCallback( Callback callback )
	{
		static bool once( true );
		if( once )
		{
			once = false;

			//Offer offer;
			//offer.name = "ACTIVATE_PROMO";
			//offer.resources["GEM"] = 2500;
			//offer.resources["GOLD"] = 5000;
			//offer.params = ParamCollection( "path:ini/promo/promo_cg_nodisposable.xml,name:MINI_PROMO_13,minlevel:1,product:com.mixgamesapps.romanwars.promo1,duration:28800" );
			//if( callback ) callback( offer );


			//Offer offer;
			//offer.name = "PRO";
			//offer.resources["GEM"] = 1000;
			//if( callback ) callback( offer );
		}
	}
}
