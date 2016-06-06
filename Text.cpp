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
: _ttfFontSize(50)
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
	TTFConfig ttfConfig( _ttfFontName.c_str(), _ttfFontSize, GlyphCollection::DYNAMIC );
	setTTFConfig( ttfConfig );
	setString( getString() );
}

const std::string& Text::getTTFFontName()const
{
	return _ttfFontName;
}

void Text::setTTFFontSize( unsigned size )
{
	_ttfFontSize = size;
	TTFConfig ttfConfig( _ttfFontName.c_str(), _ttfFontSize, GlyphCollection::DYNAMIC );
	setTTFConfig( ttfConfig );
	setString( getString() );
}

unsigned Text::getTTFFontSize()
{
	return _ttfFontSize;
}

NS_CC_END