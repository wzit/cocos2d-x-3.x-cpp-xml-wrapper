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

#ifndef __Dev_Hierarchy_Tree__
#define __Dev_Hierarchy_Tree__

#include "2d/CCNode.h"
#include "2d/CCSprite.h"
#include "2d/CCLabel.h"
#include "2d/CCMenu.h"

#include "ml/IntrusivePtr.h"

NS_CC_BEGIN;

/*
using:

auto tree = dev::HierarchyTree::create(this);
addChild( tree );
tree->close();

*/


namespace dev
{
	const float offsetX( 20 );
	const float offsetY( 16 );
	const float textSize( 16 );

	class HierarchyTree : public Node
	{
		enum class OpenState { open, close, };
	protected:
		HierarchyTree();
	public:
		virtual ~HierarchyTree();

		HierarchyTree* root();

		static HierarchyTree* create( const std::string & text );
		static HierarchyTree* create( Node * node );
		bool init( const std::string & text );

		void close();
		void open();
		void switchState();
		void setState( OpenState state );
		void addLine( HierarchyTree * line );
		Size getSize()const;
		Point view( const Point & position );
		void setParentLine( HierarchyTree * line );
		
	private:
		IntrusivePtr<Node> _capturedNode;
		MenuItemImage * _iconStateOpen;
		MenuItemImage * _iconStateClose;
		Sprite * _iconContent;
		Label * _text;
		OpenState _state;
		Node * _content;
		std::vector< HierarchyTree* > _childLines;
		HierarchyTree * _parentLine = nullptr;
	};
};

NS_CC_END;
#endif