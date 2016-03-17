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

#ifndef __XML_PROPERTIES__
#define __XML_PROPERTIES__
#include "ml/types.h"


NS_CC_BEGIN;

class NodeExt;

#define __DECLARE_INT_PROPERTY( name, value ) const int k##name = __LINE__; const std::string ks##name(value);

namespace xmlLoader
{
	__DECLARE_INT_PROPERTY( Type, "type" );
	__DECLARE_INT_PROPERTY( Pos, "pos" );
	__DECLARE_INT_PROPERTY( PosX, "x" );
	__DECLARE_INT_PROPERTY( PosY, "y" );
	__DECLARE_INT_PROPERTY( PosInterval, "posinterval" );
	__DECLARE_INT_PROPERTY( PosRadius, "posradius" );
	__DECLARE_INT_PROPERTY( Scale, "scale" );
	__DECLARE_INT_PROPERTY( Rotation, "rotation" );
	__DECLARE_INT_PROPERTY( Strech, "strech" );
	__DECLARE_INT_PROPERTY( Size, "size" );
	__DECLARE_INT_PROPERTY( Visible, "visible" );
	__DECLARE_INT_PROPERTY( LocalZ, "z" );
	__DECLARE_INT_PROPERTY( GlobalZ, "globalzorder" );
	__DECLARE_INT_PROPERTY( Center, "center" );
	__DECLARE_INT_PROPERTY( Tag, "tag" );
	__DECLARE_INT_PROPERTY( CascadeColor, "cascadecolor" );
	__DECLARE_INT_PROPERTY( CascadeOpacity, "cascadeopacity" );
	__DECLARE_INT_PROPERTY( Name, "name" );
	__DECLARE_INT_PROPERTY( Image, "image" );
	__DECLARE_INT_PROPERTY( Blending, "blending" );
	__DECLARE_INT_PROPERTY( Alias, "alias" );
	__DECLARE_INT_PROPERTY( Opacity, "opacity" );
	__DECLARE_INT_PROPERTY( Color, "color" );
	__DECLARE_INT_PROPERTY( Animation, "animation" );
	__DECLARE_INT_PROPERTY( Action, "action" );
	__DECLARE_INT_PROPERTY( ImageNormal, "imageN" );
	__DECLARE_INT_PROPERTY( ImageSelected, "imageS" );
	__DECLARE_INT_PROPERTY( ImageDisabled, "imageD" );
	__DECLARE_INT_PROPERTY( Enabled, "enabled" );
	__DECLARE_INT_PROPERTY( Text, "text" );
	__DECLARE_INT_PROPERTY( Font, "font" );
	__DECLARE_INT_PROPERTY( FontTTF, "fontttf" );
	__DECLARE_INT_PROPERTY( FontSize, "fontsize" );
	__DECLARE_INT_PROPERTY( TextWidth, "textwidth" );
	__DECLARE_INT_PROPERTY( TextAlign, "textalign" );
	__DECLARE_INT_PROPERTY( MenuCallBack, "callback" );
	__DECLARE_INT_PROPERTY( ScaleEffect, "scale_effect" );
	__DECLARE_INT_PROPERTY( Sound, "sound" );
	__DECLARE_INT_PROPERTY( Path, "path" );
	__DECLARE_INT_PROPERTY( Template, "template" );
	__DECLARE_INT_PROPERTY( AlignCols, "cols" );
	__DECLARE_INT_PROPERTY( AlignStartPosition, "alignstartpos" );
	__DECLARE_INT_PROPERTY( GridSize, "gridsize" );
	__DECLARE_INT_PROPERTY( ScissorRect, "scissorrect" );
	__DECLARE_INT_PROPERTY( ScissorEnabled, "scissorenabled" );
	__DECLARE_INT_PROPERTY( ScrollEnabled, "scrollenabled" );
	__DECLARE_INT_PROPERTY( AllowScrollByX, "allowscrollbyx" );
	__DECLARE_INT_PROPERTY( AllowScrollByY, "allowscrollbyy" );
	__DECLARE_INT_PROPERTY( DelayOnActivate, "delayonactivate" );
	__DECLARE_INT_PROPERTY( Folder, "folder" );
	__DECLARE_INT_PROPERTY( Duration, "duration" );
	__DECLARE_INT_PROPERTY( HotLocalisation, "hotlocalisation" );
	__DECLARE_INT_PROPERTY( MidPoint, "midpoint" );
	__DECLARE_INT_PROPERTY( Percent, "percent" );
	__DECLARE_INT_PROPERTY( BarRate, "barrate" );
	__DECLARE_INT_PROPERTY( ProgressType, "progresstype" );
	__DECLARE_INT_PROPERTY( Resource, "resource" );

	//declare other properties as UserProperties + int constant
	__DECLARE_INT_PROPERTY( UserProperties, "" );

	void bookDirectory( NodeExt* node );
	void unbookDirectory();

	int strToPropertyType( const std::string &property );
	void setProperty( Node* node, const std::string &property, const std::string &value );
	bool setProperty( Node* node, const int property, const std::string &value );

	void bookProperty( const std::string & name, const int iname );

	ccMenuCallback get_callback_by_description( const std::string& description );
};

NS_CC_END

#ifdef __DECLARE_INT_PROPERTY
#	undef __DECLARE_INT_PROPERTY
#endif

#endif