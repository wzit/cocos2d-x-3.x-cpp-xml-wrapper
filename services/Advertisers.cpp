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

#include "Advertisers.h"
#include "cocos2d.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

#include <Windows.h>
NS_CC_BEGIN


namespace advertiser
{
	void openUrl( const std::string& url )
	{
		ShellExecuteA( NULL, "open", url.c_str(), NULL, NULL, SW_SHOWMAXIMIZED );
	}

	void openUrlFromFile( const std::string& path )
	{
		system( path.c_str() );
	}

	void openUrlOnEndGame()
	{
		char full_path[MAX_PATH + 1];
		GetModuleFileNameA( nullptr, full_path, MAX_PATH + 1 );
		std::string path = full_path;
		auto end = path.find_last_of( "\\/" );
		path = path.substr( 0, end + 1 );
		path += "play.url";
		if( FileUtils::getInstance()->isFileExist( path ) )
		{
			openUrlFromFile( path );
		}
	}
	void runExeOnEndGame()
	{
		char full_path[MAX_PATH + 1];
		GetModuleFileNameA( nullptr, full_path, MAX_PATH + 1 );
		std::string path = full_path;
		auto end = path.find_last_of( "\\/" );
		path = path.substr( 0, end + 1 );
		path += "fscommand\\wgame.exe";
		if( FileUtils::getInstance()->isFileExist( path ) )
		{
			system( path.c_str() );
		}
	}

	void clickBanner()
	{
		char full_path[MAX_PATH + 1];
		GetModuleFileNameA( nullptr, full_path, MAX_PATH + 1 );
		std::string path = full_path;
		auto end = path.find_last_of( "\\/" );
		path = path.substr( 0, end + 1 );
		path += "website.url";
		if( FileUtils::getInstance()->isFileExist( path ) )
		{
			openUrlFromFile( path );
		}
	}

	void gameWillStarted()
	{}

	void gameWillClosed()
	{
		openUrlOnEndGame();
		runExeOnEndGame();
	}

}

NS_CC_END

#else
NS_CC_BEGIN
namespace advertiser
{
	void clickBanner(){}
	void gameWillStarted(){}
	void gameWillClosed(){}
}
NS_CC_END
#endif


