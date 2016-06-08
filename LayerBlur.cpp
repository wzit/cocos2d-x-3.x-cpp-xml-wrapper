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
#include "LayerBlur.h"
#include "ShadersCache.h"

NS_CC_BEGIN;


LayerBlur::LayerBlur()
	: _useBlur( false )
	, _blurCounter( 0 )
	, _blurSamples( 10 )
{}

bool LayerBlur::init()
{
	return true;
}

void LayerBlur::visitWithBlur( Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags )
{
	assert( _useBlur );
	assert( _rendererBloor );

	if( isBlurActive() )
	{
		if( _blurCounter == 0 )
		{
			renderer->render();
			_rendererBloor->beginWithClear( 0, 0, 0, 0 );
			visitContent( renderer, parentTransform, parentFlags );
			_rendererBloor->end();
			renderer->render();
		}
		else if( _blurCounter < _blurSamples )
		{
			_rendererBloor->begin();
			_rendererBloor->getSprite()->visit( renderer, parentTransform, parentFlags );
			_rendererBloor->end();
			renderer->render();
		}
		_rendererBloor->visit( renderer, parentTransform, parentFlags );
		renderer->render();
		_blurCounter++;
	}
	else
	{
		_blurCounter = 0;
		visitContent( renderer, parentTransform, parentFlags );
	}
}

void LayerBlur::setIsUseBlur( bool var )
{
	_useBlur = var;
	if( _useBlur )
	{
		auto desSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
		auto program = CustomShadersCache::shared().program( "shaders/bloor" );
		_rendererBloor = RenderTexture::create( desSize.width, desSize.height );
		_rendererBloor->setPosition( Point( desSize / 2 ) );
		_rendererBloor->getSprite()->setGLProgram( program );
	}
	else
	{
		_rendererBloor.reset( nullptr );
	}
}

bool LayerBlur::getIsUseBlur()const
{
	return _useBlur;
}


NS_CC_END