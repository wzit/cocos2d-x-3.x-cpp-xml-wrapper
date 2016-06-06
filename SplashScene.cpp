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

#include "SplashScene.h"
#include "common.h"
#include "loadxml/xmlProperties.h"
NS_CC_BEGIN





SplashScene::SplashScene()
{
}

SplashScene::~SplashScene( )
{
}

bool SplashScene::init( const std::string& folder, const std::function<void()>& onFinish )
{
	do
	{
		CC_BREAK_IF( !onFinish );
		_onFinish = onFinish;

		auto createLayer = [folder]( int index )
		{
			std::string path = folder + "/splash" + toStr( index ) + ".jpg";
			if( FileUtils::getInstance()->isFileExist( path ) == false )
				return (Layer*)nullptr;

			auto layer = Layer::create();
			auto sprite = Sprite::create( path );
			Strech strech;
			strech.mode = Strech::Mode::max_scale;
			strech.boundingSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
			strechNode( sprite, strech );
			layer->addChild( sprite );
			sprite->setPosition( Point( strech.boundingSize / 2 ) );
			return layer;
		};

		int index( 1 );
		while( true )
		{
			auto layer = createLayer( index++ );
			if( !layer )break;
			_splashes.push_back( layer );
		}
		CC_BREAK_IF( _splashes.empty() );
		CC_BREAK_IF( !SmartScene::init( Layer::create() ) );
		pushLayer( _splashes.front(), true );

		return true;
	}
	while( false );
	return false;
}

void SplashScene::onLayerPushed( LayerPointer layer )
{
	auto action = Sequence::create(
		FadeIn::create( 1 ),
		DelayTime::create( 2 ),
		FadeOut::create( 1 ),
		RemoveSelf::create(),
		nullptr );
	layer->setCascadeOpacityEnabled( true );
	layer->setOpacity( 0 );
	layer->runAction( action );
}

void SplashScene::onLayerPoped( LayerPointer layer )
{
	assert( layer == _splashes.front() );
	_splashes.pop_front();
	if( _splashes.empty() )
	{
		_onFinish();
	}
	else
	{
		pushLayer( _splashes.front() );
	}
}


NS_CC_END