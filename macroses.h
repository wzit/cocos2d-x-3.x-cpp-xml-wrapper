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

#ifndef __ml_macroses_h__
#define __ml_macroses_h__

#include "types.h"

#pragma warning( disable : 4996 )


#include <memory>
#include "cocos2d.h"
#include "IntrusivePtr.h"

#define DECLARE_BUILDER( CLASS ) \
public:\
typedef IntrusivePtr<CLASS> Pointer; \
typedef IntrusivePtr<CLASS> CLASS##Pointer; \
CLASS(); \
virtual ~CLASS(); 


#define DECLARE_AUTOBUILDER( CLASS ) \
public:\
typedef IntrusivePtr<CLASS> Pointer; \
typedef IntrusivePtr<CLASS> CLASS##Pointer; \
CLASS(); \
virtual ~CLASS(); \
template <class ... Types> \
static CLASS::Pointer create(Types && ... _Args){	\
	Pointer pointer = make_intrusive<CLASS>(); \
	if( !pointer || !pointer->init( std::forward<Types>( _Args )... ) ) \
		pointer.reset( nullptr ); \
	return pointer; \
}

#define CREATE_0(CLASS) \
public: static CLASS::Pointer create(){ Pointer pointer = make_intrusive<CLASS>(); if( !pointer || !pointer->init() ) pointer.reset( nullptr ); return pointer; } \

#define CREATE_1(CLASS) \
public: template <class A0> static CLASS::Pointer create(A0 a0){ Pointer pointer = make_intrusive<CLASS>(); if( !pointer || !pointer->init(a0) ) pointer.reset( nullptr ); return pointer; } \

#define CREATE_2(CLASS) \
public: template <class A0, class A1> static CLASS::Pointer create(A0 a0, A1 a1) \
{ Pointer pointer = make_intrusive<CLASS>(); if( !pointer || !pointer->init(a0,a1) ) pointer.reset( nullptr ); return pointer; } \

#define CREATE_3(CLASS) \
public: template <class A0, class A1, class A2> static CLASS::Pointer create(A0 a0, A1 a1, A2 a2) \
{ Pointer pointer = make_intrusive<CLASS>(); if( !pointer || !pointer->init(a0,a1,a2) ) pointer.reset( nullptr ); return pointer; } \

#define CREATE_4(CLASS) \
public: template <class A0, class A1, class A2, class A3> static CLASS::Pointer create(A0 a0, A1 a1, A2 a2, A3 a3) \
{ Pointer pointer = make_intrusive<CLASS>(); if( !pointer || !pointer->init(a0,a1,a2,a3) ) pointer.reset( nullptr ); return pointer; } \

#define CREATE_5(CLASS) \
public: template <class A0, class A1, class A2, class A3, class A4> static CLASS::Pointer create(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) \
{ Pointer pointer = make_intrusive<CLASS>(); if( !pointer || !pointer->init(a0,a1,a2,a3,a4) ) pointer.reset( nullptr ); return pointer; } \

#define CREATE_6(CLASS) \
public: template <class A0, class A1, class A2, class A3, class A4, class A5> static CLASS::Pointer create(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) \
{ Pointer pointer = make_intrusive<CLASS>(); if( !pointer || !pointer->init(a0,a1,a2,a3,a4,a5) ) pointer.reset( nullptr ); return pointer; } \

#define CREATE_7(CLASS) \
public: template <class A0, class A1, class A2, class A3, class A4, class A5, class A6> static CLASS::Pointer create(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) \
{ Pointer pointer = make_intrusive<CLASS>(); if( !pointer || !pointer->init(a0,a1,a2,a3,a4,a5,a6) ) pointer.reset( nullptr ); return pointer; } \

#define DECLARE_POINTER( CLASS ) \
public:\
typedef IntrusivePtr<CLASS> Pointer; \
typedef IntrusivePtr<CLASS> CLASS##Pointer; \
CLASS(); \
virtual ~CLASS(); \

#define PROTECTED_CONSTRUCTORS( CLASS ) \
	protected: \
	CLASS( ); \
	~CLASS( ); \
	CLASS( const CLASS & ); \
	CLASS& operator = (const CLASS &); \
	CLASS( CLASS && ); \

#define PRIVATED_CONSTRUCTORS( CLASS ) \
	private: \
	CLASS( ); \
	~CLASS( ); \
	CLASS( const CLASS & ); \
	CLASS& operator = (const CLASS &); \
	CLASS( CLASS && ); \



#endif