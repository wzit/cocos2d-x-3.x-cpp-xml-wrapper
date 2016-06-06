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

#ifndef __Intrusive_Ptr__
#define __Intrusive_Ptr__

#include "base/CCRef.h"
#include <memory>
#include <assert.h>

template <class TRef>
class IntrusivePtr
{
public:
	IntrusivePtr( TRef * ptr = nullptr ) : _ptr( nullptr )
	{
		reset( ptr );
	}

	IntrusivePtr( const IntrusivePtr & holder ) : _ptr( nullptr )
	{
		reset( holder._ptr );
	}

	template <class OtherPtr>
	IntrusivePtr( IntrusivePtr<OtherPtr> holder ) : _ptr( nullptr )
	{
		reset( holder.ptr() );
	}

	IntrusivePtr( IntrusivePtr && holder ) : _ptr( nullptr )
	{
		_ptr = holder._ptr;
		holder._ptr = nullptr;
	}

	IntrusivePtr& operator = (const IntrusivePtr& r)
	{
		reset( r._ptr );
		return *this;
	}

	template <typename R>
	IntrusivePtr<TRef>& operator = (IntrusivePtr<R> r)
	{
		reset( r.ptr() );
		return *this;
	}

	IntrusivePtr<TRef>& operator = (TRef * r)
	{
		reset( r );
		return *this;
	}

	virtual ~IntrusivePtr()
	{
		reset( nullptr );
	}

	TRef* ptr()
	{
		return _ptr;
	}

	const TRef* ptr()const
	{
		return _ptr;
	}

	TRef* operator -> ()
	{
		assert( _ptr );
		return _ptr;
	}
	const TRef* operator -> ()const
	{
		assert( _ptr );
		return _ptr;
	}

	TRef& operator * ()
	{
		assert( _ptr );
		return *_ptr;
	}
	const TRef& operator * ()const
	{
		assert( _ptr );
		return *_ptr;
	}

	operator TRef* ()
	{
		return _ptr;
	}
	operator const TRef* ()const
	{
		return _ptr;
	}

	void reset( TRef * ptr )
	{
		if( ptr != _ptr )
		{
			if( ptr ) ptr->retain();
			if( _ptr ) _ptr->release();
			_ptr = ptr;
		}
	}

	bool operator == (const IntrusivePtr & holder)const
	{
		return _ptr == holder._ptr;
	}

	bool operator == (const TRef * pointer)const
	{
		return _ptr == pointer;
	}

	template <class Other>
	bool operator == (const IntrusivePtr<Other> & holder)const
	{
		return _ptr == holder.ptr();
	}

	bool operator != (const TRef * pointer)const
	{
		return _ptr != pointer;
	}

	bool operator != (const IntrusivePtr & holder)const
	{
		return _ptr != holder._ptr;
	}

	template <class Other>
	bool operator != (const IntrusivePtr<Other> & holder)const
	{
		return _ptr != holder.ptr();
	}

	bool operator < (const IntrusivePtr & holder)const
	{
		return _ptr < holder._ptr;
	}

private:
	TRef * _ptr;
};

/*
template<class Type, class...TArgs>
inline IntrusivePtr<Type> make_intrusive( TArgs && ... _Args )
{
	IntrusivePtr<Type> holder;
	//try
	//{
		Type * ptr( nullptr );
		ptr = new Type( std::forward<TArgs>( _Args )... );
		if( ptr )
		{
			holder.reset( ptr );
			ptr->release();
		}
	//}
	//catch( std::exception e )
	//{
	//	cocos2d::log( "EXCCEPTION: [%s] in make_intrusive", e.what() );
	//}

	return holder;
}
*/

template<class Type>
inline IntrusivePtr<Type> make_intrusive()
{
	IntrusivePtr<Type> holder;
//	try
	{
		Type * ptr( nullptr );
		ptr = new Type();
		holder.reset( ptr );
		if( ptr ) ptr->release();
	}
//	catch( std::exception e )
//	{
//		cocos2d::log( "EXCCEPTION: [%s] in make_intrusive", e.what() );
//	}

	return holder;
}


template<class Type, class Arg0> IntrusivePtr<Type> make_intrusive(Arg0 arg0)
{ IntrusivePtr<Type> holder; Type * ptr(nullptr); ptr = new Type(arg0); holder.reset(ptr); if (ptr) ptr->release(); return holder; }

template<class Type, class A0, class A1> IntrusivePtr<Type> make_intrusive(A0 a0, A1 a1)
{ IntrusivePtr<Type> holder; Type * ptr(nullptr); ptr = new Type(a0, a1); holder.reset(ptr); if (ptr) ptr->release(); return holder; }

template<class Type, class A0, class A1, class A2> IntrusivePtr<Type> make_intrusive(A0 a0, A1 a1, A2 a2)
{ IntrusivePtr<Type> holder; Type * ptr(nullptr); ptr = new Type(a0, a1, a2); holder.reset(ptr); if (ptr) ptr->release(); return holder; }

template<class Type, class A0, class A1, class A2, class A3> IntrusivePtr<Type> make_intrusive(A0 a0, A1 a1, A2 a2, A3 a3)
{ IntrusivePtr<Type> holder; Type * ptr(nullptr); ptr = new Type(a0, a1, a2, a3); holder.reset(ptr); if (ptr) ptr->release(); return holder; }

template<class Type, class A0, class A1, class A2, class A3, class A4> IntrusivePtr<Type> make_intrusive(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4)
{ IntrusivePtr<Type> holder; Type * ptr(nullptr); ptr = new Type(a0, a1, a2, a3, a4); holder.reset(ptr); if (ptr) ptr->release(); return holder; }

template<class Type, class A0, class A1, class A2, class A3, class A4, class A5> IntrusivePtr<Type> make_intrusive(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
{ IntrusivePtr<Type> holder; Type * ptr(nullptr); ptr = new Type(a0, a1, a2, a3, a4, a5); holder.reset(ptr); if (ptr) ptr->release(); return holder; }



bool IntrusivePtr_test();

#endif
