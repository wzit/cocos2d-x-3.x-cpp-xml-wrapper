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

#include "SimpleAudioEngine.h"
#include "ml/Audio/AudioMenu.h"
#include "ml/Audio/AudioEngine.h"
NS_CC_BEGIN;
using namespace CocosDenshion;

AudioMenu::AudioMenu()
{
	auto desSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
	
	Menu::init();
	setPosition( Point::ZERO );

	auto cb0 = std::bind( [](){ AudioEngine::shared().soundEnabled(false); } );
	auto cb1 = std::bind( [](){ AudioEngine::shared().soundEnabled(true);  AudioEngine::shared().playEffect2( "" ); } );
	auto cb2 = std::bind( [](){ AudioEngine::shared().musicEnabled(false); AudioEngine::shared().playEffect2( "" ); } );
	auto cb3 = std::bind( [](){ AudioEngine::shared().musicEnabled(true);  AudioEngine::shared().playEffect2( "" ); } );

	m_soundOn = MenuItemImage::create ( "",  "", cb0 );
	m_soundOff = MenuItemImage::create( "", "", cb1 );
	m_musicOn = MenuItemImage::create( "", "", cb2 );
	m_musicOff = MenuItemImage::create( "", "", cb3 );

	addChild( m_soundOn );
	addChild( m_soundOff );
	addChild( m_musicOn );
	addChild( m_musicOff );

	float K(75);
	m_soundOn->setPosition( Point(desSize.width - K*2, desSize.height - K) );
	m_soundOff->setPosition( Point(desSize.width - K*2, desSize.height - K) );
	m_musicOn->setPosition( Point(desSize.width - K, desSize.height - K) );
	m_musicOff->setPosition( Point(desSize.width - K, desSize.height - K) );

}

AudioMenu::~AudioMenu()
{
}

void AudioMenu::check()
{
}


NS_CC_END;