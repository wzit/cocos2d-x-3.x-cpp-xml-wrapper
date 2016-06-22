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

#ifndef __Singlton_h__
#define __Singlton_h__

template <class T>
class Singlton
{
public:
	static T& shared()
	{
		static T* instance;
		static bool firstrun( true );
		if( firstrun )
		{
			firstrun = false;
			instance = new T();
			instance->onCreate();
		}
		return *instance;
	}
	virtual void onCreate() {}
};

#endif