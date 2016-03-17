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

#ifndef __std_extension_h__
#define __std_extension_h__
#include <algorithm>
#include <list>

namespace std
{

	template <class Conteiner, class Type>
	typename Conteiner::const_iterator find( Conteiner & conteiner, const Type & value )
	{
		typename Conteiner::const_iterator iterator = std::find( conteiner.begin(), conteiner.end(), value );
		return iterator;
	}

	template <class Type>
	Type sign( const Type & signedvalue )
	{
		return signedvalue < 0 ? -1 : (signedvalue>0 ? 1 : 0);
	}

};


#endif