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

#ifndef __ScrollMenu_h__
#define __ScrollMenu_h__

#include "cocos2d.h"
#include "ml/macroses.h"
#include <map>
#include "Scissor.h"
#include "ml/common.h"
#include "MenuItem.h"

NS_CC_BEGIN;

class ScrollMenu : public Scissor<Layer>
{
	DECLARE_BUILDER( ScrollMenu );
	CREATE_0(ScrollMenu);
	bool init();
public:
	bool onTouchBegan( Touch*, Event* );
	void onTouchEnded( Touch*, Event* );
	void onTouchMoved( Touch*, Event* );
	void onTouchCancelled( Touch*, Event* );

	void setEnabled( bool var );
	bool isEnabled()const;

	void setTouchEnabled( bool var );
	bool isTouchEnabled( )const;
	void setScrollEnabled( bool var );
	bool isScrollEnabled( )const;

	mlMenuItem::Pointer push(
		const std::string & normalImage,
		const std::string & selectedImage,
		const std::string & disabledImage,
		const std::string & fontBMP,
		const std::string & text,
		const ccMenuCallback & callback );

	mlMenuItem::Pointer push(
		const std::string & normalImage,
		const std::string & selectedImage,
		const std::string & fontBMP,
		const std::string & text,
		const ccMenuCallback & callback );

	mlMenuItem::Pointer push(
		const std::string & normalImage,
		const std::string & selectedImage,
		const ccMenuCallback & callback );

	mlMenuItem::Pointer push(
		const std::string & normalImage,
		const ccMenuCallback & callback );

	void addItem( mlMenuItem::Pointer item );
	void removeItem( mlMenuItem::Pointer item );
	void removeAllItems();

	virtual Node* getChildByName( const std::string& name ) const override;

	void align( int cols );
public:
	size_t getItemsCount( )const;
	mlMenuItem* getItem( size_t index );
	const mlMenuItem* getItem( size_t index )const;
	mlMenuItem* getItemByName( const std::string & name );
	const mlMenuItem* getItemByName( const std::string & name )const;

	void scroll( int x, int y );

	void scrollLock();
	void scrollUnLock();
protected:
	mlMenuItem* getItemForTouch( Touch *touch );

	virtual void onScrollBegan( Touch*touch, const Point & shift ) {}
	virtual void onScrollMoved( Touch*touch, const Point & shift ) {}
	virtual void onScrollEnded( Touch*touch, const Point & shift ) {}
private:
	void scrollBegan( Touch* );
	void scrollMoved( Touch* );
	void scrollEnded( Touch* );
	void scrollCanceled( Touch* );

	Point fitPosition( const Point & pos );
	Point fitPositionByGrid( const Point & pos );

	void select( mlMenuItem * item );
	void unselect( mlMenuItem * item );
	void activate( mlMenuItem * item );

private:
	bool _touchEnabled;
	bool _scrollEnabled;

	mlMenuItem::Pointer _selected;
	mlMenuItem::Pointer _selectedOnTouchBegan;
	std::vector<mlMenuItem::Pointer> _items;

	bool _scrolled;
	bool _lockScroll;
	Point _scrollAreaPos;

	CC_PROPERTY_PASS_BY_REF( int, _alignedCols, AlignedColums );

	CC_SYNTHESIZE( bool, _allowScrollX, AllowScrollByX );
	CC_SYNTHESIZE( bool, _allowScrollY, AllowScrollByY );

	CC_SYNTHESIZE_PASS_BY_REF( Size, _gridSize, GrisSize );
	CC_SYNTHESIZE_PASS_BY_REF( Point, _alignedStartPosition, AlignedStartPosition );

};

NS_CC_END;
#endif