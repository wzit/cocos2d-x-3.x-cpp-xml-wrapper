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

#include "types.h"
#include "common.h"
#include "ScrollMenu.h"
#include "MenuItem.h"
#include "ImageManager.h"
#include "NodeExt.h"
#include "Language.h"
#include "Text.h"
#include "ShadersCache.h"
#include "xmlProperties.h"
#include "LayerBlur.h"


NS_CC_BEGIN;

namespace xmlLoader
{
	static std::deque<NodeExt*> _directories;
	static std::map<std::string, const int> properties;
	struct __autofillproperties
	{
		void fill()
		{
			bookProperty( ksType, kType );
			bookProperty( ksVisible, kVisible );
			bookProperty( ksPos, kPos );
			bookProperty( ksPosX, kPosX );
			bookProperty( ksPosY, kPosY );
			bookProperty( ksScale, kScale );
			bookProperty( ksStrech, kStrech ) ;
			bookProperty( ksSize, kSize );
			bookProperty( ksRotation, kRotation  );
			bookProperty( ksLocalZ, kLocalZ ) ;
			bookProperty( ksGlobalZ, kGlobalZ  );
			bookProperty( ksCenter, kCenter ) ;
			bookProperty( ksTag, kTag ) ;
			bookProperty( ksCascadeOpacity, kCascadeOpacity  );
			bookProperty( ksCascadeColor, kCascadeColor  );
			bookProperty( ksImage, kImage  );
			bookProperty( ksBlending, kBlending );
			bookProperty( ksOpacity, kOpacity );
			bookProperty( ksColor, kColor  );
			bookProperty( ksAnimation, kAnimation  );
			bookProperty( ksName, kName );
			bookProperty( ksAlignCols, kAlignCols ) ;
			bookProperty( ksImageNormal, kImageNormal ) ;
			bookProperty( ksImageSelected, kImageSelected  );
			bookProperty( ksImageDisabled, kImageDisabled  );
			bookProperty( ksText, kText  );
			bookProperty( ksFont, kFont  );
			bookProperty( ksFontTTF, kFontTTF );
			bookProperty( ksFontSize, kFontSize );
			bookProperty( ksMenuCallBack, kMenuCallBack );
			bookProperty( ksEnabled, kEnabled );
			bookProperty( ksTextWidth, kTextWidth  );
			bookProperty( ksTextAlign, kTextAlign );
			bookProperty( ksEnableShadow, kEnableShadow );
			bookProperty( ksEnableOutline, kEnableOutline );
			bookProperty( ksEnableGlow, kEnableGlow);
			bookProperty( ksDisableShadow, kDisableShadow );
			bookProperty( ksScaleEffect, kScaleEffect  );
			bookProperty( ksSound, kSound  );
			bookProperty( ksTemplate, kTemplate );
			bookProperty( ksPath, kPath );
			bookProperty( ksAlignStartPosition, kAlignStartPosition );
			bookProperty( ksGridSize, kGridSize );
			bookProperty( ksScissorRect, kScissorRect );
			bookProperty( ksScissorEnabled, kScissorEnabled );
			bookProperty( ksScrollEnabled, kScrollEnabled );
			bookProperty( ksAllowScrollByX, kAllowScrollByX );
			bookProperty( ksAllowScrollByY, kAllowScrollByY );
			bookProperty( ksMouseScrollEnabled, kMouseScrollEnabled );
			bookProperty( ksMouseScrollSpeed, kMouseScrollSpeed );
			bookProperty( ksProgressType, kProgressType );
			bookProperty( ksPercent, kPercent );
			bookProperty( ksMidPoint, kMidPoint );
			bookProperty( ksBarChangeRate, kBarChangeRate );
			bookProperty( ksUseBlur, kUseBlur );
			bookProperty( ksProgressImage, kProgressImage );
			bookProperty( ksShaderProgram, kShaderProgram );
		}
	};
	__autofillproperties __autofillproperties__;

	void bookProperty( const std::string & name, const int iname )
	{
#ifdef _DEBUG
		for( auto& pair : properties )
		{
			int second = pair.second;
			assert( pair.second != iname );
			assert( pair.first != name );
		}
#endif
		properties.insert( std::pair<std::string, const int>( name, iname ) );
	}

	int strToPropertyType( const std::string &property )
	{
		static bool first( true );
		if( first )
		{
			__autofillproperties__.fill();
			first = false;
		}
		return properties[property];
	}

	std::string propertyTypeToStr( const int property )
	{
		for( auto pair : properties )
		{
			if( pair.second == property )
			{
				return pair.first;
			}
		}
		return "";
	}

	void setProperty( Node* node, const std::string &property, const std::string &value )
	{
		if( property == ksTemplate )
			return;

		const int iproperty = strToPropertyType( property );
		if( false == setProperty( node, iproperty, value ) )
		{
			auto nodeext = dynamic_cast<NodeExt*>(node);
			if( nodeext)
				nodeext->setProperty( property, value );
		}
	}

	bool setProperty( Node* node, const int property, const std::string & rawvalue )
	{
		bool result( false );
		assert( node );
		static Language& language = Language::shared();

		Sprite * sprite = dynamic_cast<Sprite*>(node);
		ScrollMenu* scrollmenu = dynamic_cast<ScrollMenu*>(node);
		mlMenuItem * menuitem = dynamic_cast<mlMenuItem*>(node);
		ProgressTimer* progress = dynamic_cast<ProgressTimer*>(node);
		Text* text = dynamic_cast<Text*>(node);

		const std::string& value = xmlLoader::macros::parse( rawvalue );

		Point point;
		Size size;
		SpriteFrame* frame( nullptr );
		//Texture2D * texture( nullptr );
		NodeExt * nodeext( nullptr );

		nodeext = dynamic_cast<NodeExt*>(node);
		if( nodeext )
			result = nodeext->setProperty( property, value );
		
		if( result == false )
		{
			result = true;
			switch( property )
			{
				//for node:
				case kType:
					break;
				case kName:
					node->setName( value );
					break;
				case kVisible:
					node->setVisible( strTo<bool>( value ) );
					break;
				case kPos:
					node->setPosition( strTo<Point>( value ) );
					break;
				case kPosX: 
					node->setPositionX( strTo<float>( value ) );
					break;
				case kPosY: 
					node->setPositionY( strTo<float>( value ) ); 
					break;
				case kScale:
				{
					if( value.find( "x" ) != std::string::npos )
					{
						point = strTo<Point>( value );
						node->setScale( point.x, point.y );
					}
					else
					{
						float s = strTo<float>( value );
						node->setScale( s );
					}
					break;
				}
				case kRotation:
					node->setRotation( strTo<float>( value ) );
					break;
				case kCenter:
					node->setAnchorPoint( strTo<Point>( value ) );
					break;
				case kStrech:
				{
					auto strech = strTo<Strech>( value );
					strechNode( node, strech );
					if( text )
						text->setStrech( strech );
					break;
				}
				case kSize:
					size.width = strTo<Point>( value ).x;
					size.height = strTo<Point>( value ).y;
					node->setContentSize( size );
					break;
				case kTag:
					node->setTag( strTo<int>( value ) );
					break;
				case kCascadeColor:
					node->setCascadeColorEnabled( strTo<bool>( value ) );
					break;
				case kCascadeOpacity:
					node->setCascadeOpacityEnabled( strTo<bool>( value ) );
					break;
				case kLocalZ:
					node->setLocalZOrder( strTo<int>( value ) );
					break;
				case kGlobalZ:
					node->setGlobalZOrder( strTo<int>( value ) );
					break;
					//for sprite:
				case kImage:
					if( sprite )
					{
						frame = ImageManager::shared().spriteFrame( value );
						if( frame )
							sprite->setSpriteFrame( frame );
						else
							sprite->setTexture( value );
					}
					else if( progress )
					{
						auto sprite = ImageManager::sprite( value );
						if( sprite )
							progress->setSprite( sprite );
					}
					break;
					//for scroll menu:
				case kBlending:
					assert( sprite || progress );
					if( sprite )
						sprite->setBlendFunc( strTo<BlendFunc>(value) );
					else if( progress && progress->getSprite() )
						progress->getSprite()->setBlendFunc( strTo<BlendFunc>( value ) );
					break;
				case kOpacity:
					node->setOpacity( strTo<int>( value ) );
					break;
				case kColor:
					node->setColor( strTo<Color3B>( value ) );
					break;
				case kAnimation:
					node->runAction( xmlLoader::load_action( value ) );
					break;
				case kAlignCols:
					assert( scrollmenu );
					scrollmenu->setAlignedColums( strTo<int>( value ) );
					break;
					//for mlMenuItem & mlSlider:
				case kImageNormal:
					assert( menuitem  );
					if( menuitem )
						menuitem->setImageNormal( value );
					break;
				case kImageSelected:
					assert( menuitem );
					if( menuitem )
						menuitem->setImageSelected( value );
					break;
				case kImageDisabled:
					assert( menuitem );
					if( menuitem )
						menuitem->setImageDisabled( value );
					break;
				case kMenuCallBack:
					assert( menuitem );
					for( auto iter = _directories.rbegin(); iter != _directories.rend(); ++iter )
					{
						auto node = *iter;
						auto callback = node->get_callback_by_description( value );
						if( menuitem )
							menuitem->setCallback( callback );
						if( callback )
							break;
					}
					break;
				case kEnabled:
					if( menuitem )
						menuitem->setEnabled( strTo<bool>( value ) );
					else
						result = false;
					break;
				case kSound:
					assert( menuitem );
					menuitem->setSound( value );
					break;
				case kText:
				{
					assert( menuitem || text );
					std::string string = macros::parse( value );
					string = language.string( string );
					string = macros::parse( string );
					if( menuitem ) menuitem->setText( string );
					else if( text )
					{
						text->setString( string );
						text->setSourceString( rawvalue );
					}
					break;
				}
				case kFont:
					assert( menuitem || text );
					if( text )
					{
						if( value.find( ".fnt" ) != std::string::npos )
							text->setBMFontFilePath( value );
						else if( value.find( ".ttf" ) != std::string::npos )
							text->setTTFFontName( value );
						else
							text->setSystemFontName( value );
					}
					else if( menuitem )
					{
						menuitem->setFont( value );
					}
					break;
				case kFontTTF:
					assert( menuitem || text );
					if( text ) text->setTTFFontName( value );
					else menuitem->setTtfFontName( value );
					break;
				case kFontSize:
					assert( menuitem || text );
					if( text ) text->setFontSize( strTo<int>( value ) );
					else if( menuitem ) menuitem->setFontSize( strTo<int>( value ) );
					break;
				case kTextWidth:
					assert( text );
					if( text ) text->setWidth( strTo<float>( value ) );
					break;
				case kTextAlign:
					assert( text );
					if( text )
					{
						TextHAlignment align;
#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
						if( value == "center" ) align = TextHAlignment::CENTER;
						else if( value == "right" ) align = TextHAlignment::RIGHT;
						else align = TextHAlignment::LEFT;
#else
						if( value == "center" ) align = TextHAlignment::CENTER;
						else if( value == "right" ) align = TextHAlignment::LEFT;
						else align = TextHAlignment::RIGHT;
#endif
						text->setAlignment( align );
					}
					break;
				case kEnableShadow:
					assert( text );
					if( text )
					{
						//"color:000000FF,offset:2x-2,blurradius:2"
						ParamCollection pc( value );
						auto color = strTo<Color4B>( pc.get( "color", "000000FF" ) );
						auto offset = strTo<Size>( pc.get( "offset", "2x-2" ) );
						auto blurradius = strTo<int>( pc.get( "blurradius", "0" ) );
						text->enableShadow( color, offset, blurradius );
					}
					break;
				case kEnableOutline:
					assert( text );
					if( text )
					{
						ParamCollection pc( value );
						auto color = strTo<Color4B>( pc.get( "color", "000000FF" ) );
						auto width = strTo<int>( pc.get( "width", "1" ) );
						text->enableOutline( color, width );
					}
					break;
				case kEnableGlow:
					assert( text );
					if( text )
					{
						auto color = strTo<Color4B>( value );
						text->enableGlow( color );
					}
					break;
				case kDisableShadow:
					assert( text );
					if( text ) text->disableEffect( LabelEffect::SHADOW );
					break;
				case kDisableOutline:
					assert( text );
					if( text ) text->disableEffect( LabelEffect::OUTLINE );
					break;
				case kDisableGlow:
					assert( text );
					if( text ) text->disableEffect( LabelEffect::GLOW );
					break;
				case kScaleEffect:
					assert( menuitem );
					menuitem->useScaleEffect( strTo<bool>( value ) );
					break;
				case kAlignStartPosition:
					assert( scrollmenu );
					scrollmenu->setAlignedStartPosition( strTo<Point>( value ) );
					break;
				case kGridSize:
					assert( scrollmenu );
					scrollmenu->setGrisSize( strTo<Size>( value ) );
					break;
				case kScissorRect:
					assert( scrollmenu );
					scrollmenu->setScissorRect( strTo<Rect>( value ) );
					break;
				case kScrollEnabled:
					assert( scrollmenu );
					scrollmenu->setScrollEnabled( strTo<bool>( value ) );
					break;
				case kScissorEnabled:
					assert( scrollmenu );
					scrollmenu->setScissorEnabled( strTo<bool>( value ) );
					break;
				case kAllowScrollByX:
					assert( scrollmenu );
					scrollmenu->setAllowScrollByX( strTo<bool>( value ) );
					break;
				case kAllowScrollByY:
					assert( scrollmenu );
					scrollmenu->setAllowScrollByY( strTo<bool>( value ) );
					break;
				case kMouseScrollEnabled:
					assert( scrollmenu && 0 );
					//scrollmenu->setMouseScrollEnabled( strTo<bool>( value ) );
					break;
				case kMouseScrollSpeed:
					assert( scrollmenu && 0 );
					//scrollmenu->setMouseScrollSpeed( strTo<float>( value ) );
					break;
				case kProgressType:
					if( progress )
						progress->setType( value == "radial" ? ProgressTimer::Type::RADIAL : ProgressTimer::Type::BAR );
					else
						result = false;
					break;
				case kPercent:
					if( progress )
						progress->setPercentage( strTo<float>( value ) );
					else
						result = false;
					break;
				case kMidPoint:
					if( progress )
						progress->setMidpoint( strTo<Point>( value ) );
					else
						result = false;
					break;
				case kBarChangeRate:
					if( progress )
						progress->setBarChangeRate( strTo<Point>( value ) );
					else
						result = false;
					break;
				case kUseBlur:
					assert( dynamic_cast<LayerBlur*>(node) );
					dynamic_cast<LayerBlur*>(node)->setIsUseBlur( strTo<bool>( value ) );
					break;
				case kProgressImage:
					break;
				case kShaderProgram:
				{
					GLProgram* program( nullptr );
					if( value == "grayscale" )
						program = ShaderCache::getInstance()->getGLProgram( GLProgram::SHADER_NAME_POSITION_GRAYSCALE );
					else if( value == "default" )
						program = ShaderCache::getInstance()->getGLProgram( GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP );
					else
						program = CustomShadersCache::shared().program( value );

					if( program )
						node->setGLProgram( program );
					else
						log( "cannot create GL shader program by name or path: [%s]", value.c_str() );
					break;
				}
				default:
					result = false;
					log_once( "property with name[%d] not dispathed node by name[%s]", property, node->getName().c_str() );
					break;
			}
		}
		return result;
	}

	void bookDirectory( NodeExt* node )
	{
		_directories.push_back( node );
		if( node && node->as_node_pointer() )
			node->as_node_pointer()->retain();
	}

	void unbookDirectory( NodeExt* node )
	{
		auto iter = std::find( _directories.begin(), _directories.end(), node );
		if( iter != _directories.end() )
		{
			if( node && node->as_node_pointer() )
				node->as_node_pointer()->release();
			_directories.erase( iter );
		}
	}

	ccMenuCallback get_callback_by_description( const std::string& description )
	{
		for( auto iter = _directories.rbegin(); iter != _directories.rend(); ++iter )
		{
			auto node = *iter;
			auto callback = node->get_callback_by_description( description );
			if( callback )
			{
				return callback;
			}
		}
		return nullptr;
	}
};

NS_CC_END