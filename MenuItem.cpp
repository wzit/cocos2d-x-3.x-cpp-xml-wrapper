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

#include "ml/MenuItem.h"
#include "ml/ImageManager.h"
#include "ml/Audio/AudioEngine.h"
#include "ml/Text.h"
#include "loadxml/xmlProperties.h"
using namespace cocos2d;

const std::string kNameText( "text" );
const std::string kNameImageNormal( "normal" );
const std::string kNameImageSelected( "selected" );
const std::string kNameImageDisabled( "disabled" );

mlMenuItem::mlMenuItem()
: _imageNormal()
, _imageSelected()
, _imageDisabled()
, _font()
, _fontTTF()
, _text()
, _sound()
, _fontSize(-1)
, _useScaleEffectOnSelected( true )
, _onClick( nullptr )
, _labelNormal()
, _labelSelected()
, _labelDisabled()
, _delayOnActivate(0)
{
	_sound = "##sound_button##";
}

mlMenuItem::~mlMenuItem()
{
	stopAllActions();
	removeAllChildrenWithCleanup( true );
}

mlMenuItem::Pointer mlMenuItem::create(
	const std::string & normalImage,
	const std::string & selectdImage,
	const std::string & disabledImage,
	const std::string & fontBMP,
	const std::string & text,
	const ccMenuCallback& callback )
{
	Pointer ptr = make_intrusive<mlMenuItem>();
	if( ptr && ptr->initWithNormalImage( normalImage, selectdImage, disabledImage, fontBMP, text, callback ) )
		return ptr;
	ptr.reset( nullptr );
	return ptr;
}

mlMenuItem::Pointer mlMenuItem::create(
	const std::string & normalImage,
	const std::string & selectedImage,
	const std::string & fontBMP,
	const std::string & text,
	const ccMenuCallback & callback )
{
	return create( normalImage, selectedImage, normalImage, fontBMP, text, callback );
}

mlMenuItem::Pointer mlMenuItem::create(
	const std::string & normalImage,
	const std::string & selectedImage,
	const ccMenuCallback & callback )
{
	const std::string kEmpty;
	return create( normalImage, selectedImage, normalImage, kEmpty, kEmpty, callback );
}

mlMenuItem::Pointer mlMenuItem::create(
	const std::string & normalImage,
	const ccMenuCallback & callback )
{
	const std::string kEmpty;
	return create( normalImage, normalImage, kEmpty, kEmpty, kEmpty, callback );
}

mlMenuItem::Pointer mlMenuItem::create()
{
	const std::string kEmpty;
	return create( kEmpty, kEmpty, kEmpty, kEmpty, kEmpty, nullptr );
}

bool mlMenuItem::initWithNormalImage(
	const std::string & normalImage,
	const std::string & selectedImage,
	const std::string & disabledImage,
	const std::string & fontBMP,
	const std::string & text,
	const ccMenuCallback& callback )
{
	MenuItemImage::initWithCallback( callback );
	setCallback( callback );
	NodeExt::init();

	setCascadeColorEnabled( true );
	setCascadeOpacityEnabled( true );

	setImageNormal( normalImage );
	setImageSelected( selectedImage );
	setImageDisabled( disabledImage );
	setFont( fontBMP );
	setText( text );

	return true;
}

void mlMenuItem::setCallback( const ccMenuCallback& callback )
{
	auto base = std::bind( &mlMenuItem::on_click, this, std::placeholders::_1 );
	MenuItem::setCallback( base );
	_onClick = callback;
}

void mlMenuItem::setCallbackOnSelected( const ccMenuCallback& callback )
{
	_onSelected = callback;
}

bool mlMenuItem::setProperty( int intproperty, const std::string & value )
{
	return NodeExt::setProperty( intproperty, value );
}

bool mlMenuItem::setProperty( const std::string & stringproperty, const std::string & value )
{
	if( stringproperty == "onselected" )
		_onSelected = xmlLoader::get_callback_by_description( value );
	else
		return NodeExt::setProperty( stringproperty, value );
	return true;
}

void mlMenuItem::setImageNormal( const std::string & imagefile )
{
	if( _imageNormal == imagefile )
		return;
	_imageNormal = imagefile;
	IntrusivePtr<Sprite> image = ImageManager::shared().sprite( _imageNormal );

	Vector<Node*> children;
	if( MenuItemImage::getNormalImage() )
		children = MenuItemImage::getNormalImage()->getChildren();
	for( auto child : children )
		child->removeFromParentAndCleanup(false);
	MenuItemImage::setNormalImage( image );
	for( auto child : children )
		image->addChild( child );
	if( image )
		image->setName( kNameImageNormal );
	if( _labelNormal )
	{
		auto center = image->getContentSize() / 2;
		_labelNormal->setPosition( center );
	}

	locateImages();
}

void mlMenuItem::setImageSelected( const std::string & imagefile )
{
	if( _imageSelected == imagefile )
		return;
	_imageSelected = imagefile;
	IntrusivePtr<Sprite> image = ImageManager::shared().sprite( imagefile );
	MenuItemImage::setSelectedImage( image );
	image->setName( kNameImageSelected );
	locateImages();
	if( _labelSelected )
	{
		auto center = image->getContentSize() / 2;
		_labelSelected->setPosition( center );
	}
}

void mlMenuItem::setImageDisabled( const std::string & imagefile )
{
	if( _imageDisabled == imagefile )
		return;
	_imageDisabled = imagefile;
	IntrusivePtr<Sprite> image = ImageManager::shared().sprite( imagefile );
	MenuItemImage::setDisabledImage( image );
	image->setName( kNameImageDisabled );
	locateImages();
	if( _labelDisabled )
	{
		auto center = image->getContentSize() / 2;
		_labelDisabled->setPosition( center );
	}
}

void mlMenuItem::setText( const std::string & string )
{
	if( _text == string )return;
	_text = string;
	buildText();
}

void mlMenuItem::setFont( const std::string & fontfile )
{
	if( _font == fontfile )return;
	_font = fontfile;
	buildText();
}

void mlMenuItem::setSystemFontName( const std::string & fontname )
{
	if( _fontTTF == fontname )return;
	_fontTTF = fontname;
	buildText();
}

void mlMenuItem::setSystemFontSize( int fontsize )
{
	if( _fontSize == fontsize )return;
	_fontSize = fontsize;
	buildText();
}

void mlMenuItem::buildText()
{
	//if( _font.empty() )
	//	return;
	auto allocate = [this]( Text::Pointer& label, Node * parent )
	{
		assert( parent );

		Point center = Point( parent->getContentSize() / 2 );

		if( !label )
		{
			label.reset( Text::create() );
			parent->addChild( label );
			label->setName( kNameText );
			label->setCascadeColorEnabled( true );
			label->setCascadeOpacityEnabled( true );
			
		}

		if( label->getStrech().empty() )
		{
			Strech strechInfo;
			strechInfo.boundingSize = parent->getContentSize();
			strechInfo.boundingSize.width *= 0.7f;
			strechInfo.boundingSize.height *= 0.8f;
			strechInfo.mode = Strech::Mode::min_scale;
			label->setStrech( strechInfo );
		}
		if( _font.empty() == false )
		{
			label->setBMFontFilePath( _font );
		}
		else if( _fontTTF.empty() == false && _fontSize != -1 )
		{
			TTFConfig ttfConfig( _fontTTF.c_str(), _fontSize, GlyphCollection::DYNAMIC );
			label->setTTFConfig( ttfConfig );
		}
		if( label->getPosition().isZero() )
			label->setPosition( center );
		label->setString( _text );
		label->updateContent();
	};

	if( getNormalImage() )allocate( _labelNormal, getNormalImage() );
	if( getSelectedImage() )allocate( _labelSelected, getSelectedImage() );
	if( getDisabledImage() )allocate( _labelDisabled, getDisabledImage() );
	if( _labelNormal )_labelNormal->setVisible( _text.empty() == false );
	if( _labelSelected )_labelSelected->setVisible( _text.empty() == false );
	if( _labelDisabled )_labelDisabled->setVisible( _text.empty() == false );
}

void mlMenuItem::locateImages()
{
	if( !getNormalImage() )
		return;

	Point center = Point( getNormalImage()->getContentSize() / 2 );

	Node * node( nullptr );
	if( (node = getNormalImage()) )
	{
		node->setAnchorPoint( Point( 0.5f, 0.5f ) );
		node->setPosition( center );
		node->setCascadeColorEnabled( true );
		node->setCascadeOpacityEnabled( true );
	}
	if( (node = getSelectedImage()) )
	{
		node->setAnchorPoint( Point( 0.5f, 0.5f ) );
		node->setPosition( center );
		node->setCascadeColorEnabled( true );
		node->setCascadeOpacityEnabled( true );
	}
	if( (node = getDisabledImage()) )
	{
		node->setAnchorPoint( Point( 0.5f, 0.5f ) );
		node->setPosition( center );
		node->setCascadeColorEnabled( true );
		node->setCascadeOpacityEnabled( true );
	}
}

void mlMenuItem::on_click( Ref*sender )
{
	if( _sound.empty() == false )
	{
		AudioEngine::shared().playEffect( _sound, false, 0 );
	}
	if( _onClick )
	{
		if( _delayOnActivate == 0 )
			_onClick( sender );
		else
		{
			auto delay = DelayTime::create( _delayOnActivate );
			auto call = CallFunc::create( [this, sender](){_onClick( sender ); } );
			auto action = Sequence::create( delay, call, nullptr );
			runAction( action );
		}
	}

	runEvent( "activated" );

	onActivation.pushevent();
}

Rect mlMenuItem::rect() const
{
	//MenuItem::rect();
	Rect result;
	result = Rect( _position.x, _position.y,
				 _contentSize.width, _contentSize.height );

	Node const* node = getNormalImage();
	if( node == nullptr )
		node = this;
	auto size = node->getContentSize();
	auto pos = getPosition();
	auto center = node->getAnchorPoint();
	result.origin = -Point( size.width * center.x, size.height * center.y );
	result.origin += pos;
	result.size = size;

	return result;
}

void mlMenuItem::selected()
{
	MenuItemImage::selected();

	if( runEvent( "selected" ) == false )
	{
		if( _useScaleEffectOnSelected )
		{
			int tag = 0x123;
			auto actionN = EaseIn::create( ScaleTo::create( 0.3f, 0.8f ), 2 );
			auto actionS = actionN->clone();
			actionN->setTag( tag );
			actionS->setTag( tag );
			if( getNormalImage() )
			{
				getNormalImage()->stopActionByTag( tag );
				getNormalImage()->runAction( actionN );
			}
			if( getSelectedImage() )
			{
				getSelectedImage()->stopActionByTag( tag );
				getSelectedImage()->runAction( actionS );
			}
		}
	}

	if( _onSelected )
		_onSelected(this);
	onSelect.pushevent();
}

void mlMenuItem::unselected()
{
	MenuItemImage::unselected();

	if( runEvent( "unselected" ) == false )
	{
		if( _useScaleEffectOnSelected )
		{
			int tag = 0x123;
			auto actionN = EaseIn::create( ScaleTo::create( 0.2f, 1.0f ), 2 );
			auto actionS = actionN->clone();
			actionN->setTag( tag );
			actionS->setTag( tag );
			if( getNormalImage() )
			{
				getNormalImage()->stopActionByTag( tag );
				getNormalImage()->runAction( actionN );
			}
			if( getSelectedImage() )
			{
				getSelectedImage()->stopActionByTag( tag );
				getSelectedImage()->runAction( actionS );
			}
		}
	}

	onDeselect.pushevent();
}

void mlMenuItem::setEnabled( bool bEnabled )
{
	if( isEnabled() == bEnabled )
		return;

	MenuItemImage::setEnabled( bEnabled );
	switchAnimation();
}

void mlMenuItem::onEnter()
{
	MenuItemImage::onEnter();
	switchAnimation();
}

void mlMenuItem::onExit()
{
	MenuItemImage::onExit();
	switchAnimation();
}

void mlMenuItem::setSound( const std::string sound )
{
	_sound = sound;
}

void mlMenuItem::switchAnimation()
{
	return;
	//	stopActionByTag( kActionTagMenuItemImageWithText_Enabled );
	//
	//	if( isEnabled() )
	//	{
	//		float part = (CCRANDOM_MINUS1_1() * 0.1f + 1) / 2;
	//		float s = 1.05f + CCRANDOM_0_1() * 0.05f;
	//		float t0 = 1 * part;
	//		float t1 = 2 * part;
	//		float t2 = 1 * part;
	//
	//		auto a0 = ScaleTo::create( t0, 1 * s, 1 / s );
	//		auto a1 = ScaleTo::create( t1, 1 / s, 1 * s );
	//		auto a2 = ScaleTo::create( t2, 1, 1 );
	//		auto action = RepeatForever::create( Sequence::create( a0, a1, a2, nullptr ) );
	//		runAction( action );
	//		action->setTag( kActionTagMenuItemImageWithText_Enabled );
	//	}
}
