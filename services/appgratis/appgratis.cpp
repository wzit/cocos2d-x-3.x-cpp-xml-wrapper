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
//  appgratis.cpp
//  GreecoDefense
//
//  Created by Владимир Толмачев on 26.07.15.
//
//

#include "appgratis.h"


namespace appgratis
{
	std::mutex _mutex;
	std::map< std::string, Offer > _queue;
	Callback _callback;
	
	
	void setCallback( Callback callback )
	{
		_callback = callback;
		push();
	}
	
	void push()
	{
		_mutex.lock();
		if(_callback )
		{
			for( auto iter = _queue.begin(); iter != _queue.end(); )
			{
				if( iter->second.complited )
				{
					_callback( iter->second );
					_queue.erase( iter++ );
				}
				else
				{
					++iter;
				}
			}
		}
		_mutex.unlock();
	}
	
	void onRedeemResource( const std::string& name, const std::string& res, int count )
	{
		_mutex.lock();
		Offer& offer = _queue[name];
		offer.name = name;
		offer.resources[res] = count;
		_mutex.unlock();
	}
	
	void onRedeemOffer( Offer offer )
	{
		{
			_mutex.lock();
			Offer& off = _queue[offer.name];
			off.name = offer.name;
			off.params = offer.params;
			off.complited = true;
			_mutex.unlock();
		}
		push();
	}

}