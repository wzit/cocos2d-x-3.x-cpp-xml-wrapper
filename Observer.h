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

#ifndef __Observer_h__
#define __Observer_h__
#include "IntrusivePtr.h"
#include <functional>
#include <vector>
#include <assert.h>
#include <memory>
#include <map>
#include <set>
#include <list>

template <class Handler, class Function>
class Observer
{
public:
	Observer()
		: _notification(false)
		, _lockEventsCounter(0)
		, _functions()
		, _functionsOnAdd()
		, _functionsOnRemove()
		, _holder(nullptr)
	{}

	Observer(Handler * holder)
	{}

	~Observer(void)
	{}

	void add(int tag, const Function & function)
	{
		assert(function);
		if (_notification)
			_functionsOnAdd[tag] = function;
		else
			_functions[tag] = function;
	}

	void remove(int tag)
	{
		auto iter = _functions.find(tag);
		if (iter != _functions.end())
		{
			if (_notification)
				_functionsOnRemove.insert(tag);
			else
				_functions.erase(iter);
		}
	}

	/*
	template<typename ...TArgs>
	void pushevent( TArgs && ... _Args )
	{
	_notification = true;
	if( _lockEventsCounter == 0 )
	for( auto& func : _functions )
	{
	if( _functionsOnRemove.find( func.first ) == _functionsOnRemove.end() )
	func.second( std::forward<TArgs>( _Args )... );
	}
	_notification = false;

	for( auto func : _functionsOnAdd )
	add( func.first, func.second );
	for( auto func : _functionsOnRemove )
	remove( func );
	_functionsOnAdd.clear();
	_functionsOnRemove.clear();
	}
	*/
	/*
	template< class TResult, class...TArgs>
	std::list<TResult> pushevent_result( TArgs && ... _Args )
	{
	std::list<TResult> result;
	_notification = true;
	if( _lockEventsCounter == 0 )
	{
	for( auto& func : _functions )
	{
	if( _functionsOnRemove.find( func.first ) == _functionsOnRemove.end() )
	{
	auto r = func.second( std::forward<TArgs>( _Args )... );
	result.push_back( r );
	}
	}
	}
	_notification = false;

	for( auto func : _functionsOnAdd )
	add( func.first, func.second );
	for( auto func : _functionsOnRemove )
	remove( func );
	_functionsOnAdd.clear();
	_functionsOnRemove.clear();

	return result;
	}
	*/

	void lockEvents()
	{
		++_lockEventsCounter;
		assert(_lockEventsCounter >= 0);
	}

	void unlockEvents()
	{
		--_lockEventsCounter;
		assert(_lockEventsCounter >= 0);
	};

	Handler * holder()
	{
		return _holder;
	}

	size_t getListenersSize()const
	{
		return
			_functions.size() +
			_functionsOnAdd.size() -
			_functionsOnRemove.size();
	}


	void pushevent()
	{
		_notification = true;
		if (_lockEventsCounter == 0) for (auto& func : _functions) call(func.first, func.second);
		_notification = false;
		prepare();
	}

	template<class A0>void pushevent(A0 a0)
	{
		_notification = true;
		if (_lockEventsCounter == 0) for (auto& func : _functions) call(func.first, func.second, a0);
		_notification = false;
		prepare();
	}

	template<class A0, class A1>void pushevent(A0 a0, A1 a1)
	{
		_notification = true;
		if (_lockEventsCounter == 0) for (auto& func : _functions) call(func.first, func.second, a0,a1);
		_notification = false;
		prepare();
	}

	template<class A0, class A1, class A2>void pushevent(A0 a0, A1 a1, A2 a2)
	{
		_notification = true;
		if (_lockEventsCounter == 0) for (auto& func : _functions) call(func.first, func.second, a0,a1,a2);
		_notification = false;
		prepare();
	}

	/*
	* push event with return value
	*/

	template< class TResult, class A0> std::list<TResult> pushevent_result(A0 a0)
	{
		std::list<TResult> result;
		_notification = true;
		if (_lockEventsCounter == 0)
		{
			for (auto& func : _functions)
			{
				if (_functionsOnRemove.find(func.first) == _functionsOnRemove.end())
				{
					auto r = func.second(a0);
					result.push_back(r);
				}
			}
		}
		_notification = false;
		prepare();
		return result;
	}

protected:
	template <class F> void call(int id, const F& f)
	{ if (_functionsOnRemove.find(id) == _functionsOnRemove.end()) f(); }

	template <class F, class A0> void call(int id, const F& f, A0 a0)
	{ if (_functionsOnRemove.find(id) == _functionsOnRemove.end()) f(a0); }

	template <class F, class A0, class A1> void call(int id, const F& f, A0 a0, A1 a1)
	{ if (_functionsOnRemove.find(id) == _functionsOnRemove.end()) f(a0, a1); }

	template <class F, class A0, class A1, class A2> void call( int id, const F& f, A0 a0, A1 a1, A2 a2 )
	{ if (_functionsOnRemove.find(id) == _functionsOnRemove.end()) f(a0, a1, a2); }

	void prepare()
	{
		for (auto func : _functionsOnAdd) add(func.first, func.second);
		for (auto func : _functionsOnRemove) remove(func);
		_functionsOnAdd.clear();
		_functionsOnRemove.clear();
	}
private:
	bool _notification;
	int _lockEventsCounter;
	std::map<int, Function > _functions;
	std::map<int, Function > _functionsOnAdd;
	std::set<int> _functionsOnRemove;
	Handler * _holder;
};



#endif
