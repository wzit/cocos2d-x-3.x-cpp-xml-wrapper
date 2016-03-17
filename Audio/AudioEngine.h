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

#ifndef __TowerDefence__AudioEngine__
#define __TowerDefence__AudioEngine__
#include "SimpleAudioEngine.h"
#include "ml/Singlton.h"
#include "ml/Observer.h"
#include <string>
NS_CC_BEGIN;

class AudioEngine : public Singlton<AudioEngine>
{
	friend class Singlton<AudioEngine>;
	friend class AudioMenu;
public:
	static Observer < AudioEngine, std::function<void( bool )> > SoundEnableChanged;
	static Observer < AudioEngine, std::function<void( bool )> > MusicEnableChanged;
	static std::function< bool() > IsSoundEnabled;
	static std::function< bool() > IsMusicEnabled;
public:
	virtual ~AudioEngine();
	void soundEnabled( bool mode );
	void musicEnabled( bool mode );
	bool isSoundEnabled()const;
	bool isMusicEnabled()const;
	
	void playMusic( const std::string & path, bool lopped = true );
	int  playEffect( const std::string & path, bool lopped = false, float pan = 0 );

	void playEffect2( const std::string & path );
	void stopEffect( int id );
	void pauseEffect( int id );
	void resumeEffect( int id );
	
	void stopMusic();
	
	void pauseAllEffects();
	void resumeAllEffects();
	void update( float dt );
protected:
private:
	AudioEngine();
	virtual void onCreate();
private:
	std::string _currentMusic;
	std::set<std::string> _soundRunInFrameTick;
};

NS_CC_END;
#endif /* defined(__TowerDefence__AudioEngine__) */
