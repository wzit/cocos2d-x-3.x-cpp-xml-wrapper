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

#include "ml/SmartScene.h"
#include "ml/ImageManager.h"
NS_CC_BEGIN

const std::string kImageShadow( "images/square.png" );

SmartScene::SmartScene()
: _nowBlockedTopLayer( false )
, _useShadow(true)
, _autoPushShadow(true)
, _durationShadowFade( 0.2f )
, _opacityShadowFade( 128 )
, _colorShadow( 0, 0, 0 )
{}

SmartScene::~SmartScene()
{}

bool SmartScene::init( LayerPointer mainlayer )
{
	Scene::init();

	_shadowResource = kImageShadow;
	resetMainLayer( mainlayer );

	return true;
}

void SmartScene::onEnter()
{
	Scene::onEnter();

	for( size_t i = 0; i < _stack.size() - 1; ++i )
	{
		auto layers = _stack[i];
		for( auto layer : layers )
		{
			if( layer->isRunning() )
				layer->onExit();
		}
	}
	if( _stack.empty() == false )
	{
		auto layers = _stack.back();
		for( auto layer : layers )
		{
			if( layer->isRunning() == false )
				layer->onEnter();
		}
	}
	_dispachStack = true;
}

void SmartScene::onExit()
{
	_dispachStack = false;
	if( _stack.empty() == false )
	{
		auto layers = _stack.back();
		for( auto layer : layers )
			layer->onExit();
	}

	Scene::onExit();
}

void SmartScene::resetMainLayer( Layer* mainlayer )
{
	if( mainlayer == _mainlayer ) return;

	while( _stack.size() > 1 )
	{
		auto layers = _stack.back();
		for( auto& layer : layers )
			removeChild( layer );
	}
	if( _mainlayer )
	{
		removeChild( _mainlayer );
		assert( _stack.size() == 1 );
		_stack.clear();
	}
	_mainlayer = mainlayer;
	if( _mainlayer )
	{
		addChild( _mainlayer, 0 );
		_stack.push_back( std::deque<LayerPointer>() );
		_stack.back().push_back( _mainlayer );
	}

}

void SmartScene::pushLayer( Layer* layer, bool exitPrevios, bool waitShadow )
{
	if( waitShadow )
	{
		assert( _tempLayerData.layer == nullptr );
		_tempLayerData.layer = layer;
		_tempLayerData.exitPrevios = exitPrevios;

		if( exitPrevios )
		{
			_tempLayerData.dummy = Layer::create();
			pushLayer( _tempLayerData.dummy, true, false );
		}
		return;
	}
	if( layer )
	{
		assert( _stack.empty() == false );
		auto top = _stack.back();
		assert( top.empty() == false );
		int z = top.back()->getZOrder() + 2;

		layer->setOnExitCallback( std::bind( &SmartScene::on_layerClosed, this, layer ) );
		addChild( layer, z );


		if( exitPrevios )
		{
			_stack.push_back( std::deque<LayerPointer>() );
			_stack.back().push_back( layer );

			auto layers = _stack[_stack.size() - 2];
			for( auto& layer : layers )
				layer->onExit();
			if( _autoPushShadow )
				pushShadow();	
		}
		else
		{
			_stack.back().push_back( layer );
		}
		onLayerPushed( layer );
	}
}

void SmartScene::on_layerClosed( Layer* layer )
{
	if( _nowBlockedTopLayer )return;
	if( !_dispachStack )return;

	if( layer == _stack.back().back() )
	{
		if( _stack.size() >= 2 )
		{
			_stack.pop_back();
			auto layers = _stack.back();
			for( auto& layer : layers )
				layer->onEnter();
			if( _tempLayerData.dummy.ptr() != layer )
				popShadow();
			onLayerPoped( layer );
		}
	}
	if( _stack.empty() == false )
	{
		auto top = _stack.back();
	}
}

void SmartScene::pushShadow()
{
	if( _useShadow )
	{
		auto dessize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
		auto z = _stack.empty() == false ? _stack.back().back()->getZOrder() - 1 : 1;
		SpritePointer shadow = ImageManager::sprite( _shadowResource );
		shadow->setName( "shadow" );
		shadow->setScaleX( dessize.width );
		shadow->setScaleY( dessize.height );
		shadow->setColor( _colorShadow );
		shadow->setOpacity( 0 );
		shadow->setPosition( Point( dessize / 2 ) );
		_shadows.push_back( shadow );
		shadow->runAction( Sequence::createWithTwoActions( FadeTo::create( _durationShadowFade, _opacityShadowFade ), CallFunc::create( [this](){onShadowAppearanceEnded(); } ) ) );
		addChild( shadow, z );
	}
}

void SmartScene::popShadow()
{
	if( _shadows.empty() == false )
	{
		auto shadow = _shadows.back();
		shadow->runAction( Sequence::create( FadeOut::create( _durationShadowFade ), RemoveSelf::create(), nullptr ) );
		_shadows.pop_back();
	}
}

void SmartScene::onShadowAppearanceEnded()
{
	if( _tempLayerData.dummy )
	{
		auto dummy = _tempLayerData.dummy;
		auto layer = _tempLayerData.layer;

		_tempLayerData.layer.reset( nullptr );

		_autoPushShadow = false;
		dummy->removeFromParent();
		pushLayer( layer, _tempLayerData.exitPrevios, false );
		_autoPushShadow = true;
		_tempLayerData.dummy.reset( nullptr );
	}
}

NS_CC_END