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

#include "ImageManager.h"
NS_CC_BEGIN;

class __SpriteFrameCache2 : public SpriteFrameCache
{
public:
	std::map< std::string, SpriteFrame* > getFramesList( )
	{
		std::map< std::string, SpriteFrame* > map;
		for( auto frame : _spriteFrames )
		{
			map[frame.first] = frame.second;
		}
		return map;
	}
};

ImageManager::ImageManager()
{
}

ImageManager::~ImageManager()
{
}

ImageManager::ImageManager( const ImageManager& )
{
	assert( 0 );
}

const ImageManager& ImageManager::operator = ( const ImageManager& )
{
	assert( 0 );
	return *this;
}

void ImageManager::onCreate()
{
	push();
}

void ImageManager::push()
{
	_resources.emplace_back();
}

void ImageManager::pop()
{
	Resources& top = _resources.back();
	auto tc = Director::getInstance()->getTextureCache();
	tc->removeUnusedTextures();
	for( auto texture : top.textures )
	{
		tc->removeTexture(texture.second);
	}
	_resources.pop_back();
}

void ImageManager::load_plist( const std::string & path, const std::string & _name )
{
	__SpriteFrameCache2 cache;
	cache.init();
	cache.addSpriteFramesWithFile( path );

	std::string nameplist = _name.empty() ? path : _name;
	Resources& top = _resources.back();

	if( top.atlases.find( nameplist ) != top.atlases.end() )
		return;
	top.atlases[nameplist] = path;

	auto frames = cache.getFramesList();
	for( auto it : frames )
	{
		std::string name = it.first;
		SpriteFrame * frame = it.second;
		std::string key = nameplist + "::" + name;
			
		auto result = top.frames.insert( std::pair<std::string, SpriteFrame*>( key, frame ) );
		assert( result.second );
		result.first->second->retain();
	}
	if(frames.size() > 0 )
	{
		top.textures[path] = frames.begin()->second->getTexture();
	}
}

void unload_plist( const std::string & name )
{
}

Texture2D * ImageManager::textureForPLIST( const std::string & path )
{
	for( auto& res : _resources )
	{
		auto iter = res.textures.find( path );
		if( iter == res.textures.end() )
		{
			log( "texture for plist[%s] not found", path.c_str() );
			return nullptr;
		}

		return iter->second;
	}
	return nullptr;
}

SpriteFrame* ImageManager::spriteFrame( const std::string & key )
{
	for( auto& res : _resources )
	{
		auto it = res.frames.find( key );
		SpriteFrame* frame = it == res.frames.end() ? nullptr : it->second;
		return frame;
	}
	return nullptr;
}

Texture2D * ImageManager::texture( const std::string & texture )
{
	auto texturecache = Director::getInstance( )->getTextureCache( );
	assert( texturecache );
	assert( shared( ).spriteFrame( texture ) == nullptr );
	return texturecache->addImage( texture );
}

Sprite * ImageManager::sprite( const std::string & key )
{
	Sprite * sprite(nullptr);
	auto frame = ImageManager::shared().spriteFrame( key );
	if( frame )
	{
		sprite = Sprite::createWithSpriteFrame( frame );
	}
	else
	{
		sprite = Sprite::create( key );
	}

	if( !sprite )
	{
		log( "Sprite with resource [%s] not created.", key.c_str() );
	}
	assert( sprite );
	return sprite;
}


NS_CC_END;
