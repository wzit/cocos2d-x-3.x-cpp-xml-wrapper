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

#include "ml/Audio/AudioEngine.h"
#include "ml/loadxml/xmlLoader.h"
NS_CC_BEGIN;
using namespace CocosDenshion;

#define SOUND 1

#ifdef WIN32
bool _soundEnabled( true );
bool _musicEnabled( true );
#endif

static float kDefaultVolumeSounds(0.8f);
static float kDefaultVolumeMusic(0.8f);

struct SoundInfo
{
	SoundInfo() :id( 0 ), counter( 0 ) {}
	unsigned id;
	unsigned counter;
};
std::map<std::string, SoundInfo> _loopedSounds;

Observer < AudioEngine, std::function<void( bool )> > AudioEngine::SoundEnableChanged;
Observer < AudioEngine, std::function<void( bool )> > AudioEngine::MusicEnableChanged;
std::function< bool() > AudioEngine::IsSoundEnabled;
std::function< bool() > AudioEngine::IsMusicEnabled;

AudioEngine::AudioEngine()
{}

void AudioEngine::onCreate()
{
	soundEnabled( isSoundEnabled() );
	musicEnabled( isMusicEnabled() );

	Director::getInstance()->getScheduler()->scheduleUpdate( this, 0, false );
}

AudioEngine::~AudioEngine()
{}

void AudioEngine::soundEnabled( bool mode )
{
	float volume = mode ? kDefaultVolumeSounds : 0.f;
	SimpleAudioEngine::getInstance()->setEffectsVolume( volume );
	SoundEnableChanged.pushevent( mode );
#ifdef WIN32
	_soundEnabled = mode;
	if( !_soundEnabled )
	{
		SimpleAudioEngine::getInstance()->stopAllEffects();
		_loopedSounds.clear();
	}
#endif
}

void AudioEngine::musicEnabled( bool mode )
{
	float volume = mode ? kDefaultVolumeMusic : 0.f;
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume( volume );
	MusicEnableChanged.pushevent( mode );
#ifdef WIN32
	_musicEnabled = mode;
	if( !_musicEnabled )
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	else if( _currentMusic.empty() == false )
	{
		auto path = _currentMusic;
		_currentMusic.clear();
		playMusic( path, true );
	}
#endif
}

bool AudioEngine::isSoundEnabled()const
{
	return IsSoundEnabled ? IsSoundEnabled() : true;
}

bool AudioEngine::isMusicEnabled()const
{
	return IsMusicEnabled ? IsMusicEnabled() : true;
}

void AudioEngine::playMusic( const std::string & path, bool lopped )
{
#ifdef WIN32
	if( !_musicEnabled )
	{
		_currentMusic = path;
		return;
	}
#endif
	if( path == _currentMusic )
		return;
	_currentMusic = path;
	std::string pathmusic = path;
	pathmusic = xmlLoader::macros::parse( pathmusic );
	if( FileUtils::getInstance()->isFileExist(pathmusic) == false )
		return;
	pathmusic = FileUtils::getInstance()->fullPathForFilename( pathmusic );

#if SOUND == 1
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	SimpleAudioEngine::getInstance()->playBackgroundMusic( pathmusic.c_str(), lopped );
#endif
}

int AudioEngine::playEffect( const std::string & path, bool lopped, float pan )
{
#ifdef WIN32
	if( !_soundEnabled ) 
		return -1;
#endif
	const std::string& sound = xmlLoader::macros::parse( path );
	
	if( _soundRunInFrameTick.end() != _soundRunInFrameTick.find(sound) )
		return -1;
	_soundRunInFrameTick.insert( sound );

	if( FileUtils::getInstance()->isFileExist( sound ) == false )
		return -1;

	if( lopped  == false )
	{
		return SimpleAudioEngine::getInstance()->playEffect( sound.c_str(), lopped );
	}
	else
	{
		auto ins = _loopedSounds.insert( std::pair<std::string, SoundInfo>( sound, SoundInfo() ) );
		auto& info = ins.first->second;
		if( info.counter == 0 )
		{
			info.id = SimpleAudioEngine::getInstance()->playEffect( sound.c_str(), lopped );
		}
		++info.counter;
		return info.id;
	}
}

void AudioEngine::playEffect2( const std::string & path )
{
	playEffect( path, false, 0 );
}

void AudioEngine::stopEffect( int id )
{
	auto iter = _loopedSounds.begin();
	for( ; iter != _loopedSounds.end(); ++iter )
	{
		if( iter->second.id == id )
			break;
	}
	if( iter != _loopedSounds.end() )
	{
		--iter->second.counter;
		if( iter->second.counter == 0 )
		{
			SimpleAudioEngine::getInstance()->stopEffect( id );
		}
	}
	else
	{
		SimpleAudioEngine::getInstance()->stopEffect( id );
	}

}

void AudioEngine::pauseEffect( int id )
{
	SimpleAudioEngine::getInstance()->pauseEffect( id );
}

void AudioEngine::resumeEffect( int id )
{
	SimpleAudioEngine::getInstance()->resumeEffect( id );
}

void AudioEngine::stopMusic()
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void AudioEngine::pauseAllEffects()
{
	SimpleAudioEngine::getInstance()->pauseAllEffects();
}

void AudioEngine::resumeAllEffects()
{
	SimpleAudioEngine::getInstance()->resumeAllEffects();
}

void AudioEngine::update( float dt )
{
	_soundRunInFrameTick.clear();
}

NS_CC_END;
