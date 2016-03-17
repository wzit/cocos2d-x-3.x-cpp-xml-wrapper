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

#ifndef __JavaBind_h__
#define __JavaBind_h__

#include <string>
#include <map>
#include <assert.h>
#include "cocos2d.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
#include "platform/CCPlatformMacros.h"
#include "platform/CCCommon.h"
#include "platform/android/jni/JniHelper.h"

class JavaBind
{
public:
	JavaBind( const std::string& package, const std::string& className, const std::string& methodName, const std::string& argsFormat )
	{
		buildPackage( package, className );
		buildArgsFormat( argsFormat );
		_method = methodName;
	}

	void call()
	{
		findMethodInfoWithResult("void");
		_methodInfo.env->CallStaticVoidMethod( _methodInfo.classID, _methodInfo.methodID );
	}

	void call( const std::string& arg0 )
	{
		findMethodInfoWithResult("void");
		jstring jarg0 = _methodInfo.env->NewStringUTF( arg0.c_str() );
		_methodInfo.env->CallStaticVoidMethod( _methodInfo.classID, _methodInfo.methodID, jarg0 );
		_methodInfo.env->DeleteLocalRef( jarg0 );
	}

	template< class T >
	void call( const std::string& arg0, T arg1 )
	{
		findMethodInfoWithResult("void");
		jstring jarg0 = _methodInfo.env->NewStringUTF( arg0.c_str() );
		_methodInfo.env->CallStaticVoidMethod( _methodInfo.classID, _methodInfo.methodID, jarg0, arg1 );
		_methodInfo.env->DeleteLocalRef( jarg0 );
	}

	template< class T1, class T2 >
	void call( const std::string& arg0, T1 arg1, T2 arg2 )
	{
		findMethodInfoWithResult("void");
		jstring jarg0 = _methodInfo.env->NewStringUTF( arg0.c_str() );
		_methodInfo.env->CallStaticVoidMethod( _methodInfo.classID, _methodInfo.methodID, jarg0, arg1, arg2 );
		_methodInfo.env->DeleteLocalRef( jarg0 );
	}

	void call( const std::string& arg0, const std::string& arg1 )
	{
		findMethodInfoWithResult("void");
		jstring jarg0 = _methodInfo.env->NewStringUTF( arg0.c_str() );
		jstring jarg1 = _methodInfo.env->NewStringUTF( arg1.c_str() );
		_methodInfo.env->CallStaticVoidMethod( _methodInfo.classID, _methodInfo.methodID, jarg0, jarg1 );
		_methodInfo.env->DeleteLocalRef( jarg0 );
		_methodInfo.env->DeleteLocalRef( jarg1 );
	}

	void call( const std::string& arg0, const std::string& arg1, const std::string& arg2 )
	{
		findMethodInfoWithResult("void");
		jstring jarg0 = _methodInfo.env->NewStringUTF( arg0.c_str() );
		jstring jarg1 = _methodInfo.env->NewStringUTF( arg1.c_str() );
		jstring jarg2 = _methodInfo.env->NewStringUTF( arg2.c_str() );
		_methodInfo.env->CallStaticVoidMethod( _methodInfo.classID, _methodInfo.methodID, jarg0, jarg1, jarg2 );
		_methodInfo.env->DeleteLocalRef( jarg0 );
		_methodInfo.env->DeleteLocalRef( jarg1 );
		_methodInfo.env->DeleteLocalRef( jarg2 );
	}

	template< class T >
	void call( T arg )
	{
		findMethodInfoWithResult("void");
		_methodInfo.env->CallStaticVoidMethod( _methodInfo.classID, _methodInfo.methodID, arg );
	}

	template< class T >
	void call( const std::string& arg0, const std::string& arg1, T arg2 )
	{
		findMethodInfoWithResult("void");
		jstring jarg0 = _methodInfo.env->NewStringUTF( arg0.c_str() );
		jstring jarg1 = _methodInfo.env->NewStringUTF( arg1.c_str() );
		_methodInfo.env->CallStaticVoidMethod( _methodInfo.classID, _methodInfo.methodID, jarg0, jarg1, arg2 );
		_methodInfo.env->DeleteLocalRef( jarg0 );
		_methodInfo.env->DeleteLocalRef( jarg1 );
	}

	template< class T2, class T3 >
	void call( const std::string& arg0, const std::string& arg1, T2 arg2, T3 arg3 )
	{
		findMethodInfoWithResult("void");
		jstring jarg0 = _methodInfo.env->NewStringUTF( arg0.c_str() );
		jstring jarg1 = _methodInfo.env->NewStringUTF( arg1.c_str() );
		_methodInfo.env->CallStaticVoidMethod( _methodInfo.classID, _methodInfo.methodID, jarg0, jarg1, arg2, arg3 );
		_methodInfo.env->DeleteLocalRef( jarg0 );
		_methodInfo.env->DeleteLocalRef( jarg1 );
	}

	std::string string_call()
	{
		findMethodInfoWithResult("%s");
		auto result = _methodInfo.env->CallStaticObjectMethod( _methodInfo.classID, _methodInfo.methodID );
		auto ret = string(result);
		return ret;
	}

	std::string string_call( const std::string& arg0 )
	{
		findMethodInfoWithResult("%s");
		jstring jarg0 = _methodInfo.env->NewStringUTF( arg0.c_str() );
		auto result = _methodInfo.env->CallStaticObjectMethod( _methodInfo.classID, _methodInfo.methodID, jarg0 );
		auto ret = string(result);
		_methodInfo.env->DeleteLocalRef( jarg0 );
		return ret;
	}

	template< class T1 >
	std::string string_call( const std::string& arg0, T1 arg1 )
	{
		findMethodInfoWithResult("%s");
		jstring jarg0 = _methodInfo.env->NewStringUTF( arg0.c_str() );
		auto result = _methodInfo.env->CallStaticObjectMethod( _methodInfo.classID, _methodInfo.methodID, jarg0, arg1 );
		auto ret = string(result);
		_methodInfo.env->DeleteLocalRef( jarg0 );
		return ret;
	}

	bool bool_call()
	{
		findMethodInfoWithResult("%b");
		auto result = _methodInfo.env->CallStaticBooleanMethod( _methodInfo.classID, _methodInfo.methodID );
		return result;
	}
private:
	void buildPackage( const std::string& package, const std::string& className )
	{
		_packageClass = package + "." + className;
		int k( std::string::npos );
		do
		{
			k = _packageClass.find( "." );
			if( k != std::string::npos )
				_packageClass[k] = '/';
		}
		while( k != std::string::npos );
	}

	std::string cFormatArgToJniFormat( const std::string& arg )
	{
		static std::map<std::string, std::string> hash;
		if( hash.empty() )
		{
			hash["%b"] = "Z";
			hash["%d"] = "I";
			hash["%f"] = "F";
			hash["%c"] = "C";
			hash["%s"] = "Ljava/lang/String;";
			hash["void"] = "V";
		}
		return hash.at( arg );
	}

	void findMethodInfoWithResult( const std::string& resultType )
	{
		std::string format = _argsFormat;
		format += cFormatArgToJniFormat( resultType );
		cocos2d::log("JavaBind: find method [ %s %s(%s) ] ", _packageClass.c_str(), _method.c_str(), format.c_str() );
		bool find = cocos2d::JniHelper::getStaticMethodInfo( _methodInfo, _packageClass.c_str(), _method.c_str(), format.c_str() );
		cocos2d::log("JavaBind: find method [ %s %s(%s) ] %s", _packageClass.c_str(), _method.c_str(), format.c_str(), find ? "found" : "not found" );
		assert( find );
	}

	std::string string(void*javastring)
	{
		jstring returnedString = (jstring)javastring;
		const char * cstring = _methodInfo.env->GetStringUTFChars( returnedString, 0 );
		return std::string( cstring );
	}

	void buildArgsFormat( const std::string& format )
	{
		_argsFormat = "(";
		int l = format.find( "%" );
		bool exit = l == std::string::npos;
		while( !exit )
		{
			int k = format.find( '%', l + 1 );
			if( k == std::string::npos )
			{
				exit = true;
				k = format.size();
			}
			std::string arg = format.substr( l, k - l );
			_argsFormat += cFormatArgToJniFormat( arg );
			l = k;
		}
		_argsFormat += ")";
	}

	std::string _packageClass;
	std::string _method;
	std::string _argsFormat;
	cocos2d::JniMethodInfo _methodInfo;
};
#else

class JavaBind
{
public:
	JavaBind( const std::string& package, const std::string& className, const std::string& methodName, const std::string& argsFormat ) {};
	void call() {};
	void call( const std::string& arg0 ) {};
	template< class T > void call( const std::string& arg0, T arg1 ) {};
	template< class T1, class T2 > void call( const std::string& arg0, T1 arg1, T2 arg2 ) {};
	void call( const std::string& arg0, const std::string& arg1 ) {};
	void call( const std::string& arg0, const std::string& arg1, const std::string& arg2 ) {};
	template< class T > void call( T arg ) {};
	template< class T > void call( const std::string& arg0, const std::string& arg1, T arg2 ) {};
	template< class T2, class T3 > void call( const std::string& arg0, const std::string& arg1, T2 arg2, T3 arg3 ) {};
	std::string string_call() { return ""; };
	std::string string_call( const std::string& arg0 ) { return ""; };
	template< class T1 > std::string string_call( const std::string& arg0, T1 arg1 ) {};
private:
	void buildPackage( const std::string& package, const std::string& className ) {};
	std::string cFormatArgToJniFormat( const std::string& arg ) { return ""; };
	void findMethodInfoWithResult( const std::string& resultType ) {};
	std::string string( void*javastring ) { return ""; };
	void buildArgsFormat( const std::string& format ) {};
};

#endif

#endif
