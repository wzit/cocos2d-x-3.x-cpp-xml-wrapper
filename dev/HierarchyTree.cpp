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

#include "HierarchyTree.h"

NS_CC_BEGIN;

namespace dev
{
	HierarchyTree::HierarchyTree()
	{}

	HierarchyTree::~HierarchyTree()
	{}

	HierarchyTree* HierarchyTree::root()
	{
		HierarchyTree * line( this );
		HierarchyTree * parent( nullptr );
		while( line )
		{
			parent = line;
			line = line->_parentLine;
		}
		return parent;
	}

	HierarchyTree* HierarchyTree::create( const std::string & text )
	{
		auto p = new HierarchyTree;
		p->init( text );
		p->autorelease();
		return p;
	}

	HierarchyTree* HierarchyTree::create( Node * node )
	{
		auto buildDesc = [node]()
		{
			std::string desc = node->getDescription();

			int l = desc.find( "<" ) + 1;
			int r = desc.find( " ", l );
			std::string type = desc.substr( l, r - l );


			return node->getName() + ":" + type;
		};

		
		std::string desc = buildDesc();

		HierarchyTree * line = create( desc );
		for( auto child : node->getChildren() )
		{
			if( dynamic_cast<HierarchyTree*>(child) )
				continue;
			line->addLine( create( child ) );
		}

		return line;
	}

	bool HierarchyTree::init( const std::string & text )
	{
		auto cb = std::bind( [this]( Ref* )mutable{this->switchState(); }, std::placeholders::_1 );
		addChild( _iconContent = Sprite::create( "dev/cont_folder.png" ) );
		addChild( _text = Label::createWithSystemFont( text, "Arial", textSize ) );
		addChild( _content = Node::create() );

		_iconStateOpen = MenuItemImage::create( "dev/state_open.png", "dev/state_open.png", cb );
		_iconStateClose = MenuItemImage::create( "dev/state_close.png", "dev/state_close.png", cb );

		((Sprite*)_iconStateOpen->getNormalImage())->getTexture()->setAliasTexParameters();
		((Sprite*)_iconStateOpen->getSelectedImage())->getTexture()->setAliasTexParameters();
		((Sprite*)_iconStateClose->getNormalImage())->getTexture()->setAliasTexParameters();
		((Sprite*)_iconStateClose->getSelectedImage())->getTexture()->setAliasTexParameters();
		_iconContent->getTexture()->setAliasTexParameters();

		auto menu = Menu::create( _iconStateOpen, _iconStateClose, nullptr );
		menu->setPosition( 0, 0 );
		addChild( menu );

		_iconStateOpen->setPosition( offsetX * 0.5f, offsetY * 0.5f );
		_iconStateClose->setPosition( offsetX * 0.5f, offsetY * 0.5f );
		_iconContent->setPosition( offsetX * 1.5f, offsetY * 0.5f );
		_text->setPosition( offsetX * 2.5f, offsetY * 0.5f );
		_text->setAnchorPoint( Point( 0.f, 0.5f ) );

		close();

		return 0;
	}

	void HierarchyTree::close()
	{
		setState( OpenState::close );
	}

	void HierarchyTree::open()
	{
		setState( OpenState::open );
	}

	void HierarchyTree::switchState()
	{
		OpenState state = _state == OpenState::open ? OpenState::close : OpenState::open;
		setState( state );
	}

	void HierarchyTree::setState( OpenState state )
	{
		_iconStateOpen->setVisible( state == OpenState::open && _childLines.empty() == false );
		_iconStateClose->setVisible( state == OpenState::close && _childLines.empty() == false );
		_content->setVisible( state == OpenState::open );
		_state = state;

		//HierarchyTree * line( this );
		//HierarchyTree * parent( nullptr );
		//while( line )
		//{
		//	parent = line;
		//	line = line->_parentLine;
		//}
		auto root = this->root();
		if( root )
		{
			root->view( root->getPosition() );
		}

	}

	void HierarchyTree::addLine( HierarchyTree * line )
	{
		_content->addChild( line );
		_childLines.push_back( line );
		line->setParentLine( this );
		Point pos( 10, -(int)_childLines.size() * 10 );
		line->setPosition( pos );
		setState( _state );
	}

	Size HierarchyTree::getSize()const
	{
		Size size;
		size.height = offsetY;
		if( _state == OpenState::open )
		{
			for( auto line : _childLines )
			{
				auto s = line->getSize();
				size.height += s.height;
			}
		}
		return size;
	}

	Point HierarchyTree::view( const Point & position )
	{
		Point point = position;

		setPosition( position );
		point.y -= offsetY;

		if( _state == OpenState::open )
		{
			Point pos( offsetX, -offsetY );
			for( auto line : _childLines )
			{
				line->view( pos );
				pos.y -= line->getSize().height;
			}
			point.y -= pos.y;
		}

		return point;
	}
	void HierarchyTree::setParentLine( HierarchyTree * line )
	{
		_parentLine = line;
	}

};

NS_CC_END;