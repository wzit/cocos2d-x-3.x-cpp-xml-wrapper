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

#ifndef __appgratis_h__
#define __appgratis_h__

#include <functional>
#include "ml/ParamCollection.h"
#include "ml/common.h"

namespace appgratis
{
	struct Offer
	{
		Offer():complited(false){}
		std::string string()const
		{
			std::stringstream ss;
			std::string space( " " );
			ss << name << space;
			ss << resources.size() << space;
			for( auto res : resources )
			{
				ss << res.first << ":" << res.second << space;
			}
			ss << params.string();

			std::string result = ss.str();
			return result;
		}

		void parce( const std::string& string )
		{
			std::stringstream ss( string );
			std::string space( " " );
			ss >> name;
			size_t count( 0 );
			ss >> count;
			for( size_t i = 0; i < count; ++i )
			{
				std::string pair;
				ss >> pair;
				int k = pair.find( ":" );
				std::string res = pair.substr( 0, k );
				std::string cnt = pair.substr( k + 1 );
				resources.emplace( res, strTo<int>( cnt ) );
			}
			std::string p;
			ss >> p;
			params = ParamCollection( p );
		}

		bool complited;
		std::string name;
		std::map<std::string, int> resources;
		ParamCollection params;
	};
	typedef std::function<void(Offer)> Callback;


	void push();
	void onRedeemOffer( Offer offer );
	void onRedeemResource( const std::string& name, const std::string& res, int count );
	void setCallback( Callback callback );
}

#endif
