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

#ifndef __ml_MenuItem_h__
#define __ml_MenuItem_h__
#include "macroses.h"
#include "NodeExt.h"
#include "Observer.h"
#include "Text.h"

NS_CC_BEGIN;

class mlMenuItem : public MenuItemImage, public NodeExt
{
	DECLARE_POINTER(mlMenuItem)

	bool initWithNormalImage(
		 const std::string & normalImage,
		 const std::string & selectedImage,
		 const std::string & disabledImage,
		 const std::string & fontBMP,
		 const std::string & text,
		 const ccMenuCallback& callback);
public:
	static Pointer create(
		const std::string & normalImage,
		const std::string & selectedImage,
		const std::string & disabledImage,
		const std::string & fontBMP,
		const std::string & text,
		const ccMenuCallback & callback );

	static Pointer create(
		const std::string & normalImage,
		const std::string & selectedImage,
		const std::string & fontBMP,
		const std::string & text,
		const ccMenuCallback & callback );

	static Pointer create(
		const std::string & normalImage,
		const std::string & selectedImage,
		const ccMenuCallback & callback );

	static Pointer create(
		const std::string & normalImage,
		const ccMenuCallback & callback );
	static Pointer create();

	Observer<mlMenuItem, std::function<void()> > onSelect;
	Observer<mlMenuItem, std::function<void()> > onDeselect;
	Observer<mlMenuItem, std::function<void()> > onActivation;
public:
	virtual Rect rect() const override;

	virtual void selected()override;
	virtual void unselected()override;
	virtual void setEnabled( bool bEnabled )override;

	virtual void onEnter()override;
	virtual void onExit()override;

	virtual void setCallback( const ccMenuCallback& callback )override;
	virtual void setCallbackOnSelected( const ccMenuCallback& callback );
	virtual bool setProperty( int intproperty, const std::string & value )override;
	virtual bool setProperty( const std::string & stringproperty, const std::string & value )override;
	void setImageNormal( const std::string & imagefile );
	void setImageSelected( const std::string & imagefile );
	void setImageDisabled( const std::string & imagefile );
	void setText( const std::string & string );
	void setFont( const std::string & fontfile );
	void setTtfFontName( const std::string & fontname );
	void setFontSize( int fontsize );

	void setSound( const std::string sound );
	void useScaleEffect( bool mode ) { _useScaleEffectOnSelected = mode; }
protected:
	void switchAnimation();
	void buildText();
	void locateImages();
	void on_click( Ref*sender );
private:
	CC_SYNTHESIZE_READONLY_PASS_BY_REF( std::string, _imageNormal, ImageNormal );
	CC_SYNTHESIZE_READONLY_PASS_BY_REF( std::string, _imageSelected, ImageSelected );
	CC_SYNTHESIZE_READONLY_PASS_BY_REF( std::string, _imageDisabled, ImageDisabled );
	std::string _font;
	std::string _fontTTF;
	std::string _text;
	std::string _sound;
	int _fontSize;

	bool _useScaleEffectOnSelected;
	ccMenuCallback _onClick;
	ccMenuCallback _onSelected;

	CC_SYNTHESIZE( float, _delayOnActivate, DelayOnActivate );
	CC_SYNTHESIZE(IntrusivePtr<Text>, _labelNormal,   LabelNormal);
	CC_SYNTHESIZE(IntrusivePtr<Text>, _labelSelected, LabelSelected);
	CC_SYNTHESIZE(IntrusivePtr<Text>, _labelDisabled, LabelDisabled);
};

inline ActionInterval * actionMenuItemEnabled( float scalefactor = 1 )
{
	float scale = 1.f - 0.02f * scalefactor;
	
	ActionInterval * action =
	RepeatForever::create(
	Sequence::create(
		EaseInOut::create(ScaleTo::create(0.5f + CCRANDOM_MINUS1_1() * 0.1f, scale), 1.5),
		EaseInOut::create(ScaleTo::create(0.5f + CCRANDOM_MINUS1_1() * 0.1f, 1.00f), 1.5), nullptr
		) );
	return action;
}

NS_CC_END;

#endif