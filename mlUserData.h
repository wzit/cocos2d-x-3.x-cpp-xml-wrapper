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

#ifndef __ml_USER_DATA__
#define __ml_USER_DATA__

#include "cocos2d.h"
#include "pugixml/pugixml.hpp"
#include "loadxml/Crypto.h"
#include "Singlton.h"
#include "Observer.h"
#include "common.h"
#include "types.h"
NS_CC_BEGIN;

class UserData;

extern const std::string kUser_Level_prefix;
extern const std::string kUser_Level_launchmode;
extern const std::string kUser_Gamemode_prefix;
extern const std::string kUser_Complite_suffix;
extern const std::string kUser_Scores_suffix;
extern const std::string kUser_Replay_suffix;
extern const std::string kUser_ReplayBeforeWin_suffix;
extern const std::string kUser_passedLevels;
extern const std::string kUserValue_CompliteYes;
extern const std::string kUserValue_CompliteNo;
extern const std::string kUserSoundEnabled;
extern const std::string kUserMusicEnabled;

namespace __userdata
{
	void openDoc();
	void saveDoc();
	void clearDoc();
	extern XmlDocPointer Doc;
}


class mlUserData
{
public:
	mlUserData();
	virtual ~mlUserData();
	static mlUserData* getInstance();
	static Observer< mlUserData, std::function<void()> > onChangeProfile;

	void setProfile( const std::string& profile );
	const std::string& getProfile()const;

	void write( const std::string & key, const std::string & value );
	std::string get_str( const std::string & key, const std::string & defaultvalue = "" );
	void commondata_write( const std::string & key, const std::string & value );
	std::string commondata_get_str( const std::string & key, const std::string & defaultvalue = "" );
	virtual void save();

	template <class T> void write( const std::string & key, T value )
	{
		write( key, toStr( value ) );
	}

	template <class T> T get( const std::string & key, T def = 0 );

	template <class T> void commondata_write( const std::string & key, T def = 0 )
	{
		commondata_write( key, toStr( def ) );
	}
	template <class T> T commondata_get( const std::string & key, T def = 0 );


	unsigned getSessionNumber();
	void lang_set( const std::string& id );
	std::string lang_get();
	void level_complite( unsigned index );
	bool level_isComplite( unsigned index );
	int level_incrementPassed();
	int level_getCountPassed();
	int level_getScoresByIndex( int levelIndex, unsigned gamemode );
	void level_setCountPassed( int value );
	void level_setScoresByIndex( int levelIndex, unsigned gamemode, int value );
	
	int level_getReplayCount( int levelIndex, unsigned gamemode );
	int level_incReplayCount( int levelIndex, unsigned gamemode );
	int level_getReplayCountSmart( int levelIndex, unsigned gamemode );
	int level_incReplayCountSmart( int levelIndex, unsigned gamemode );

	void level_saveReplayCountBeforeWin( int levelIndex, unsigned gamemode );
	int level_getReplayCountBeforeWin( int levelIndex, unsigned gamemode );
	unsigned level_getLastLaunchMode();
	void level_setLastLaunchMode( unsigned gamemode );
private:
	std::string _profileName;
};


NS_CC_END;
#endif
