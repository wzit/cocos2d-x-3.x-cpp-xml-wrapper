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

#ifndef __Text_h__
#define __Text_h__
#include "cocos2d.h"
#include "macroses.h"
#include "NodeExt.h"
#include "loadxml/xmlProperties.h"
#include "common.h"
NS_CC_BEGIN

//Label::createWithBMFont
class Text : public Label, public NodeExt
{
	friend bool xmlLoader::setProperty( Node* node, const int property, const std::string & rawvalue );
	DECLARE_BUILDER( Text );
	CREATE_0(Text);
	CREATE_2(Text);
	CREATE_5(Text);
	bool init()override;
	bool init( const std::string& font, const std::string& text, float width = 0, TextHAlignment alignment = TextHAlignment::LEFT, const Vec2& imageOffset = Vec2::ZERO );
public:
	virtual void setString( const std::string& newString ) override;
	void setSourceString( const std::string& newString );
	void useLocation( bool mode );
protected:
	void onChangeLocalisation();
private:
	std::string _fontBM;
	std::string _sourceString;
	CC_SYNTHESIZE_PASS_BY_REF( Strech, _strech, Strech );
	CC_PROPERTY_PASS_BY_REF( std::string, _ttfFontName, TTFFontName );
	CC_PROPERTY( unsigned, _ttfFontSize, TTFFontSize );
};

NS_CC_END
#endif // #ifndef Text
