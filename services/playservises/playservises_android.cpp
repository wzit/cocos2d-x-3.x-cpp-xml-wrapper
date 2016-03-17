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

#include "ml/JniBind.h"
#include "cocos2d.h"

USING_NS_CC;

const std::string& package("com.google.example.games.basegameutils");
const std::string& javaClass("PlayServises");

namespace PlayServises
{
	static bool _isInitialised = false;
	static bool _isConnected = false;
	static std::list< std::function<void()> > _callbackOnConnected;
	void init()
	{
		if( _isInitialised )
			return;
		JavaBind bind( package, javaClass, "connect", "" );
		bind.call();
		_isInitialised = true;
	}

	bool isConnected()
	{
		return _isConnected;
	}

	void addCallbackOnConnected( std::function<void()> function )
	{
		if( function )
			_callbackOnConnected.push_back( function );
	}

	namespace Leaderboard
	{
		void record( const std::string& leaderBoardID, int value )
		{
			if( !_isInitialised )
				return;
			JavaBind bind( package, javaClass, "leaderboardRecord", "%s%d" );
			bind.call( leaderBoardID, value );
		}
		void open( const std::string& leaderBoardID )
		{
			if( !_isInitialised )
				return;
			JavaBind bind( package, javaClass, "leaderboardOpen", "%s" );
			bind.call( leaderBoardID );
		}
	}

	namespace Achievements
	{
		void unlock( const std::string& id)
		{
			if( !_isInitialised )
				return;
			JavaBind bind( package, javaClass, "achievementsUnlock", "%s" );
			bind.call( id );
		}
		void increment( const std::string& id, int value )
		{
			if( !_isInitialised )
				return;
			JavaBind bind( package, javaClass, "achievementsIncrement", "%s%d" );
			bind.call( id, value );
		}
		void open()
		{
			if( !_isInitialised )
				return;
			JavaBind bind( package, javaClass, "achievementsOpen", "" );
			bind.call();
		}
	}
}

#define METHOD(name)  Java_com_google_example_games_basegameutils_PlayServises_##name
extern
"C"
{
	JNIEXPORT void JNICALL  METHOD(playConnected)(JNIEnv*  env, jobject thiz )
	{
		PlayServises::_isConnected = true;

		for( auto func: PlayServises::_callbackOnConnected )
		{
			func();
		}
	}

	JNIEXPORT void JNICALL  METHOD(playConnectFailed)(JNIEnv*  env, jobject thiz )
	{
		PlayServises::_isConnected = false;
	}

	JNIEXPORT void JNICALL  METHOD(playConnectionSuspended)(JNIEnv*  env, jobject thiz )
	{
		//cocos2d::MessageBox("playConnectionSuspended", "PlayServises");
	}

	JNIEXPORT void JNICALL  METHOD(activityConnectionFailed)(JNIEnv*  env, jobject thiz )
	{
		//cocos2d::MessageBox("activityConnectionFailed", "PlayServises");
	}

	JNIEXPORT void JNICALL  METHOD(activityConnectionSuspended)(JNIEnv*  env, jobject thiz )
	{
		//cocos2d::MessageBox("activityConnectionSuspended", "PlayServises");
	}

}
#undef METHOD
