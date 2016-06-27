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

#include "mlUserData.h"
#include "cocos2d.h"

std::string phoneID( "1238977688sdaf8768646-asfd" );

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
extern "C"
{
#define VOID_NATIVE(method) JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_##method

	VOID_NATIVE( nativeSetPhoneID )(JNIEnv * env, jobject thiz, jstring jid)
	{
		const char *id = env->GetStringUTFChars( jid, 0 );
		phoneID = id;
	}
}
#endif

NS_CC_BEGIN;


const std::string kUser_Level_prefix( "level_" );
const std::string kUser_Level_launchmode( "launchmode" );
const std::string kUser_Gamemode_prefix( "mode_" );
const std::string kUser_Complite_suffix( "complite_" );
const std::string kUser_Scores_suffix( "scores_" );
const std::string kUser_Replay_suffix( "replay_" );
const std::string kUser_ReplayBeforeWin_suffix( "replaybw_" );
const std::string kUser_passedLevels( "level_passed" );
const std::string kUserValue_CompliteYes( "complite_yes" );
const std::string kUserValue_CompliteNo( "complite_no" );
const std::string kUserSoundEnabled( "sound_enabled" );
const std::string kUserMusicEnabled( "music_enabled" );
const std::string kUserSessionNumber( "session_number" );

static const std::string kFile( "savedata.xml" );
static std::string kFileForDev( "savedata.xml" );
static std::string kFileDat( "savedata.dat" );
static std::string kFileDatArchive( "savedata2.dat" );

namespace __userdata
{
	static bool isInitialised( false );
	XmlDocPointer Doc;

	void init()
	{
		isInitialised = true;
		std::string path = FileUtils::getInstance()->getWritablePath();
		kFileForDev = path + kFileForDev;
		kFileDat = path + kFileDat;
		kFileDatArchive = path + kFileDatArchive;
	}
	bool validateDoc()
	{
		if( !Doc )return true;
		auto root = Doc->root().first_child();
		auto device = root.child( "device" );
		if( device )
		{
			return phoneID == device.attribute( "id" ).as_string();
		}
		else
		{
			if( !root )
				root = Doc->root().append_child( "root" );
			device = root.append_child( "device" );
			device.append_attribute( "id" ).set_value( phoneID.c_str() );
		}
		return true;
	}
	void openDoc()
	{
		if( isInitialised == false )init();
		if( Doc == nullptr )
		{
			//if( FileUtils::getInstance()->isFileExist( kFileDat ) )
			auto dir = kFileDat;
			auto k = dir.find_last_of( "/" );
			dir = dir.substr( 0, k );
			Doc = ::loadDoc( /*kFileDat*/"", dir+ "/" + kFile );
			if( validateDoc() == false )
			{
				Doc.reset( new pugi::xml_document );
			}

			if( Doc && Doc->root().first_child() )
				::saveDoc( kFileDatArchive, kFile, Doc, false );
			else
			{
#ifdef WIN32
				if( FileUtils::getInstance()->isFileExist( kFileDatArchive ) )
					Doc = ::loadDoc( kFileDatArchive, kFile );
				else
					Doc = ::loadDoc( "", FileUtils::getInstance()->getWritablePath() + kFile );
#else
				Doc = ::loadDoc( kFileDatArchive, kFile );
#endif
				if( validateDoc() == false || !Doc->root().first_child() )
				{
					Doc.reset( new pugi::xml_document );
				}
			}
		}
	}
	void saveDoc()
	{
		if( isInitialised == false )init();
		auto dir = kFileDat;
		auto k = dir.find_last_of( "/" );
		dir = dir.substr( 0, k );
		FileUtils::getInstance()->createDirectory(dir);
		::saveDoc( /*kFileDat*/"", dir + "/" + kFile, Doc, false );
#ifdef WIN32
#if DEV == 1
		auto path = FileUtils::getInstance()->getWritablePath() + kFile;
		Doc->save_file( path.c_str() );
#endif
#endif
	}
	void clearDoc()
	{
		Doc = std::make_shared<pugi::xml_document>();
	}
}

static mlUserData* instanse( nullptr );
Observer< mlUserData, std::function<void()> > mlUserData::onChangeProfile;

mlUserData::mlUserData()
{
	instanse = this;

	unsigned session = getSessionNumber();
	++session;
	write( kUserSessionNumber, session );
}

mlUserData::~mlUserData()
{
	instanse = nullptr;
}

mlUserData* mlUserData::getInstance()
{
	return instanse;
}

void mlUserData::setProfile( const std::string& profile )
{
	_profileName = profile;
	onChangeProfile.pushevent();
}

const std::string& mlUserData::getProfile()const
{
	return _profileName;
}

template <> bool mlUserData::get<bool>( const std::string & key, bool def )
{ return strToBool( get_str( key, toStr( def ) ) ); }

template <> int32_t mlUserData::get<int32_t>( const std::string & key, int32_t def )
{ return strTo<int32_t>( get_str( key, toStr( def ) ) ); }

template <> uint32_t mlUserData::get<uint32_t>( const std::string & key, uint32_t def )
{ return static_cast<uint32_t>(strTo<uint32_t>( get_str( key, toStr( def ) ) )); }

template <> float mlUserData::get<float>( const std::string & key, float def )
{ return strToFloat( get_str( key, toStr( def ) ) ); }

template <> bool mlUserData::commondata_get<bool>( const std::string & key, bool def )
{ return strToBool( commondata_get_str( key, toStr( def ) ) ); }

template <> int32_t mlUserData::commondata_get<int32_t>( const std::string & key, int32_t def )
{ return strTo<int32_t>( commondata_get_str( key, toStr( def ) ) ); }

template <> uint32_t mlUserData::commondata_get<uint32_t>( const std::string & key, uint32_t def )
{ return static_cast<uint32_t>(strTo<uint32_t>( commondata_get_str( key, toStr( def ) ) )); }

template <> float mlUserData::commondata_get<float>( const std::string & key, float def )
{ return strToFloat( commondata_get_str( key, toStr( def ) ) ); }

void mlUserData::write( const std::string & key, const std::string & value )
{
	__userdata::openDoc();
	auto Root = __userdata::Doc->root().child( "root" );
	if( !Root )
		Root = __userdata::Doc->root().append_child( "root" );
	if( _profileName.empty() == false )
		Root = Root.child( _profileName.c_str() );
	if( !Root )
	{
		Root = __userdata::Doc->root().child( "root" );
		Root = Root.append_child( _profileName.c_str() );
	}
	auto node = Root.child( key.c_str() );
	if( !node )
		node = Root.append_child( key.c_str() );
	auto attr = node.attribute( "value" );
	if( !attr )
		attr = node.append_attribute( "value" );
	attr.set_value( value.c_str() );
}

std::string mlUserData::get_str( const std::string & key, const std::string & defaultvalue )
{
	__userdata::openDoc();
	auto Root = __userdata::Doc->root().child( "root" );
	if( _profileName.empty() == false )
		Root = Root.child( _profileName.c_str() );
	auto node = Root.child( key.c_str() );
	std::string value = node.attribute( "value" ).as_string( "" );
	if( value.empty() )
		value = defaultvalue;
	return value;
}

void mlUserData::commondata_write( const std::string & key, const std::string & value )
{
	auto t = _profileName;
	_profileName.clear();
	write( key, value );
	_profileName = t;
}

std::string mlUserData::commondata_get_str( const std::string & key, const std::string & defaultvalue )
{
	auto t = _profileName;
	_profileName.clear();
	auto r = get_str( key, defaultvalue );
	_profileName = t;
	return r;
}

void mlUserData::save()
{
	__userdata::saveDoc();
}

unsigned mlUserData::getSessionNumber()
{
	return get<uint32_t>( kUserSessionNumber, 0 );
}

void mlUserData::lang_set( const std::string& id )
{
	write( "userlanguage", id );
}

std::string mlUserData::lang_get()
{
	return get_str( "userlanguage" );
}

void mlUserData::level_complite( unsigned index )
{
	std::string key = kUser_Level_prefix + toStr( index ) + kUser_Complite_suffix;
	write( key, kUserValue_CompliteYes );
	int countPasses = level_getCountPassed();
	if( (int)index == countPasses ) 
		level_incrementPassed();
}

bool mlUserData::level_isComplite( unsigned index )
{
	std::string key = kUser_Level_prefix + toStr( index ) + kUser_Complite_suffix;
	return get_str( key ) == kUserValue_CompliteYes;
}

int mlUserData::level_incrementPassed()
{
	int count = level_getCountPassed();
	level_setCountPassed( count + 1 );
	return count + 1;
}

int mlUserData::level_getCountPassed()
{
	return get<int>( kUser_passedLevels, 0 );
}

int mlUserData::level_getScoresByIndex( int levelIndex, unsigned gamemode )
{
	std::string key =
		kUser_Level_prefix + toStr( levelIndex ) +
		kUser_Gamemode_prefix + toStr( gamemode ) +
		kUser_Scores_suffix;
	return get<int>( key, 0 );
}

void mlUserData::level_setCountPassed( int value )
{
	write( kUser_passedLevels, value );
}

void mlUserData::level_setScoresByIndex( int levelIndex, unsigned gamemode, int value )
{
	std::string key =
		kUser_Level_prefix + toStr( levelIndex ) +
		kUser_Gamemode_prefix + toStr( gamemode ) +
		kUser_Scores_suffix;
	int saved = level_getScoresByIndex( levelIndex, gamemode );
	if( value >= saved )
		write( key, value );
	if( saved == 0 && value > 0 )
		level_saveReplayCountBeforeWin( levelIndex, gamemode );
}

int mlUserData::level_getReplayCount( int levelIndex, unsigned gamemode )
{
	std::string key =
		kUser_Level_prefix + toStr( levelIndex ) +
		kUser_Gamemode_prefix + toStr( gamemode ) +
		kUser_Replay_suffix;
	return get<int>( key );
}

int mlUserData::level_incReplayCount( int levelIndex, unsigned gamemode )
{
	std::string key =
		kUser_Level_prefix + toStr( levelIndex ) +
		kUser_Gamemode_prefix + toStr( gamemode ) +
		kUser_Replay_suffix;
	int i = get<int>( key );
	write( key, i + 1 );
	return i + 1;
}

int mlUserData::level_getReplayCountSmart( int levelIndex, unsigned gamemode )
{
	std::string key =
		kUser_Level_prefix + toStr( levelIndex ) +
		kUser_Gamemode_prefix + toStr( gamemode ) +
		kUser_Replay_suffix + "smart";
	return get<int>( key );
}

int mlUserData::level_incReplayCountSmart( int levelIndex, unsigned gamemode )
{
	std::string key =
		kUser_Level_prefix + toStr( levelIndex ) +
		kUser_Gamemode_prefix + toStr( gamemode ) +
		kUser_Replay_suffix + "smart";
	int i = get<int>( key );
	write( key, i + 1 );
	return i + 1;
}

void mlUserData::level_saveReplayCountBeforeWin( int levelIndex, unsigned gamemode )
{
	std::string key =
		kUser_Level_prefix + toStr( levelIndex ) +
		kUser_Gamemode_prefix + toStr( gamemode ) +
		kUser_ReplayBeforeWin_suffix;
	int curr = get<int>( key, 0 );
	if( curr == 0 )
		write( key, level_getReplayCount( levelIndex, gamemode ) );
}

int mlUserData::level_getReplayCountBeforeWin( int levelIndex, unsigned gamemode )
{
	std::string key =
		kUser_Level_prefix + toStr( levelIndex ) +
		kUser_Gamemode_prefix + toStr( gamemode ) +
		kUser_ReplayBeforeWin_suffix;
	return get<int>( key, 0 );;
}

unsigned mlUserData::level_getLastLaunchMode()
{
	std::string key = kUser_Level_prefix + kUser_Level_launchmode;
	return get<int>( key );
}

void mlUserData::level_setLastLaunchMode( unsigned gamemode )
{
	std::string key = kUser_Level_prefix + kUser_Level_launchmode;
	write( key, gamemode );
}

NS_CC_END;
