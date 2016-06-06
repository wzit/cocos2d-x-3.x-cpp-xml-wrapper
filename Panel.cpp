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

#include "Panel.h"
#include "ImageManager.h"
#include "common.h"
NS_CC_BEGIN

Panel::Panel()
{
}

Panel::~Panel( )
{
}

bool Panel::init( )
{
	setCascadeColorEnabled( true );
	setCascadeOpacityEnabled( true );
	return true;
}

bool Panel::init( const std::string& folder )
{
	init();
	setFolder( folder );
	return true;
}

bool Panel::init( const std::string& folder, const Size& size )
{
	init( folder );
	setContentSize( size );
	return true;
}

void Panel::setFolder( const std::string& folder )
{
	_folder = folder;
	createFrames();
}

void Panel::setContentSize( const Size& size )
{
	Node::setContentSize( size );
	createFrames();
}

void Panel::destroyFrames()
{
	for( SpritePointer& frame : _frames )
	{
		removeChild( frame );
	}
	_frames.clear();
}

void Panel::createFrames()
{
	destroyFrames();
	if( _folder.empty() )return;
	if( getContentSize().equals( Size::ZERO ) )return;
	_frames.resize( 9 );

	auto size = getContentSize();
	auto w = size.width;
	auto h = size.height;
	static const Point anchors[9] =
	{
		Point( 0.0, 1 ),
		Point( 0.5, 1 ),
		Point( 1.0, 1 ),

		Point( 0.0, 0.5 ),
		Point( 0.5, 0.5 ),
		Point( 1.0, 0.5 ),

		Point( 0.0, 0 ),
		Point( 0.5, 0 ),
		Point( 1.0, 0 ),
	};
	
	int index( 0 );
	for( SpritePointer& frame : _frames )
	{
		std::string texture = _folder + toStr(index+1) + ".png";
		frame = ImageManager::sprite( texture );
		frame->setAnchorPoint( anchors[index] );
		frame->setPosition( anchors[index].x * w, anchors[index].y * h );
		addChild( frame );
		++index;
	}

	auto sx = w - (_frames[0]->getContentSize().width + _frames[8]->getContentSize().width);
	auto sy = h - (_frames[0]->getContentSize().height + _frames[8]->getContentSize().height);
	sx /= _frames[1]->getContentSize().width;
	sy /= _frames[4]->getContentSize().height;

	_frames[1]->setScaleX( sx );
	_frames[4]->setScaleX( sx );
	_frames[7]->setScaleX( sx );
	_frames[3]->setScaleY( sy );
	_frames[4]->setScaleY( sy );
	_frames[5]->setScaleY( sy );
}

NS_CC_END