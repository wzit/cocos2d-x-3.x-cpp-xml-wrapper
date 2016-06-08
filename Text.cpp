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

#include "Text.h"
#include "loadxml/xmlLoader.h"
#include "loadxml/xmlProperties.h"
#include "Language.h"
NS_CC_BEGIN

Text::Text()
: _fontSize(50)
, _systemFontName( "Tahoma" )
{
	useLocation( true );
}

Text::~Text( )
{
	if( Language::isLife() )
		Language::shared().observer.remove( _ID );
}

bool Text::init()
{
	return Label::init();
}

bool Text::init( const std::string& font, const std::string& text, float width, TextHAlignment alignment, const Vec2& imageOffset )
{
	if( font.empty() == false && Label::setBMFontFilePath( font, imageOffset ) )
	{
		setMaxLineWidth( width );
		setString( text );
		setAlignment( alignment );
		return true;
	}
	else
	{
		setString( text );
		setAlignment( alignment );
		setSystemFontName( "Arial" );
		setSystemFontSize( 27 );
		return true;
	}
	return false;
}

void Text::setString( const std::string& newString )
{
	if( _sourceString.empty() )
		_sourceString = newString;
	Label::setString( newString );
	strechNode( this, _strech );
}

void Text::setSourceString( const std::string& newString )
{
	_sourceString = newString;
}

void Text::useLocation( bool mode )
{
	if( mode )
		Language::shared().observer.add( _ID, std::bind( &Text::onChangeLocalisation, this ) );
	else
		Language::shared().observer.remove( _ID );
}

void Text::onChangeLocalisation()
{
	setBMFontFilePath( _bmFontPath );
	std::string string = Language::shared().string( xmlLoader::macros::parse( _sourceString ) );
	setString( string );
}

void Text::setTTFFontName( const std::string& font )
{
	_ttfFontName = font;
	TTFConfig ttfConfig( _ttfFontName.c_str(), _fontSize, GlyphCollection::DYNAMIC );
	setTTFConfig( ttfConfig );
	setString( getString() );
}

void Text::setSystemFontName( const std::string& font )
{
	_systemFontName = font;
	Label::setSystemFontSize( _fontSize );
	Label::setSystemFontName( _systemFontName );
}

const std::string& Text::getTTFFontName()const
{
	return _ttfFontName;
}

const std::string& Text::getSystemFontName()const
{
	return _systemFontName;
}

void Text::setFontSize( unsigned size )
{
	_fontSize = size;
	if( _ttfFontName.empty() == false )
	{
		TTFConfig ttfConfig( _ttfFontName.c_str(), _fontSize, GlyphCollection::DYNAMIC );
		setTTFConfig( ttfConfig );
		setString( getString() );
	}
	else if( _systemFontName.empty() == false && _bmFontPath.empty() )
	{
		Label::setSystemFontSize( _fontSize );
		Label::setSystemFontName( _systemFontName );
	}
}

unsigned Text::getFontSize()const
{
	return _fontSize;
}

NS_CC_END