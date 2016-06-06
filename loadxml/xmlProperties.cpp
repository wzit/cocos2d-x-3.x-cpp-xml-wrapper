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
#include "Panel.h"
#include "Text.h"
#include "mlUserData.h"
#include "xmlProperties.h"

NS_CC_BEGIN;

namespace xmlLoader
{
	static std::deque<NodeExt *> _directories;
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
			bookProperty( ksPosInterval, kPosInterval );
			bookProperty( ksPosRadius, kPosRadius );
			bookProperty( ksScale, kScale  );
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
			bookProperty( ksAlias, kAlias );
			bookProperty( ksOpacity, kOpacity );
			bookProperty( ksColor, kColor  );
			bookProperty( ksAnimation, kAnimation );
			bookProperty( ksAction, kAction );
			bookProperty( ksName, kName );
			bookProperty( ksAlignCols, kAlignCols ) ;
			bookProperty( ksImageNormal, kImageNormal ) ;
			bookProperty( ksImageSelected, kImageSelected  );
			bookProperty( ksImageDisabled, kImageDisabled  );
			bookProperty( ksEnabled, kEnabled );
			bookProperty( ksText, kText  );
			bookProperty( ksFont, kFont );
			bookProperty( ksFontTTF, kFontTTF );
			bookProperty( ksFontSize, kFontSize );
			bookProperty( ksMenuCallBack, kMenuCallBack  );
			bookProperty( ksTextWidth, kTextWidth  );
			bookProperty( ksTextAlign, kTextAlign );
			bookProperty( ksScaleEffect, kScaleEffect  );
			bookProperty( ksSound, kSound  );
			bookProperty( ksTemplate, kTemplate );
			bookProperty( ksAlignStartPosition, kAlignStartPosition );
			bookProperty( ksGridSize, kGridSize );
			bookProperty( ksScissorRect, kScissorRect );
			bookProperty( ksScissorEnabled, kScissorEnabled );
			bookProperty( ksScrollEnabled, kScrollEnabled );
			bookProperty( ksAllowScrollByX, kAllowScrollByX );
			bookProperty( ksAllowScrollByY, kAllowScrollByY );
			bookProperty( ksPath, kPath);
			bookProperty( ksDelayOnActivate, kDelayOnActivate );
			bookProperty( ksFolder, kFolder );
			bookProperty( ksDuration, kDuration );
			bookProperty( ksHotLocalisation, kHotLocalisation );
			bookProperty( ksMidPoint, kMidPoint );
			bookProperty( ksPercent, kPercent );
			bookProperty( ksBarRate, kBarRate );
			bookProperty( ksProgressType, kProgressType );
			bookProperty( ksResource, kResource );
		}
	};


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

	std::string toStringProperty( int value )
	{
		for( auto property : properties )
			if( property.second == value )
				return property.first;
		return "property noot found";
	}

	int strToPropertyType( const std::string &property )
	{
		static bool first( true );
		if( first )
		{
			__autofillproperties __autofillproperties;
			__autofillproperties.fill();
			first = false;
		}
		return properties[property];
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
		Menu * menu = dynamic_cast<Menu*>(node);
		mlMenuItem * menuitem = dynamic_cast<mlMenuItem*>(node);
		Panel* panel = dynamic_cast<Panel*>(node);
		Text* text = dynamic_cast<Text*>(node);
		ProgressTimer* timer = dynamic_cast<ProgressTimer*>(node);

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
					node->setVisible( strToBool( value ) );
					break;
				case kPos:
					node->setPosition( strToPoint( value ) );
					break;
				case kPosX:
					node->setPositionX( strToFloat( value ) );
					break;
				case kPosY:
					node->setPositionY( strToFloat( value ) );
					break;
				case kPosInterval:
				{
					int k = value.find( "x" );
					if( k != std::string::npos )
					{
						auto interval = []( const std::string& s )
						{
							int k = s.find( ".." );
							if( k == std::string::npos )
								return strTo<float>( s );
							float l = strTo<float>( s.substr( 0, k ) );
							float r = strTo<float>( s.substr( k + 2 ) );
							if( l < r )
							{
								float v = r - l;
								float result = CCRANDOM_0_1() * v + l;
								return result;
							}
							return l;
						};
						std::string x = value.substr( 0, k );
						std::string y = value.substr( k + 1 );
						Point point( interval( x ), interval( y ) );
						node->setPosition( point );
					}
					break;
				}
				case kPosRadius:
				{
					//pos="pos:50x50,radius:50,scale:1x0.5,circlepos=5,circlecount=10"
					ParamCollection pc( value );
					float radius = strTo<float>( pc.get( "radius" ) );
					int pos = strTo<int>( pc.get( "circlepos" ) );
					int count = strTo<int>( pc.get( "circlecount" ) );
					std::vector<Point> p;
					computePointsByRadius( p, radius, count, 0 );
					Point point = p[pos];
					if( pc.isExist( ksScale ) )
					{
						auto s = strToPoint( pc.get( ksScale ) );
						point.x *= s.x;
						point.y *= s.y;
					}
					point += strToPoint( pc.get( ksPos ) );
					node->setPosition( point );
					break;
				}

				case kScale:
					point = strToPoint( value );
					node->setScale( point.x, point.y );
					break;
				case kRotation:
					node->setRotation( strToFloat( value ) );
					break;
				case kCenter:
					node->setAnchorPoint( strToPoint( value ) );
					break;
				case kStrech:
				{
					auto strech = strToStrech( value );
					strechNode( node, strech );
					if( text )
						text->setStrech( strech );
					break;
				}
				case kSize:
					size.width = strToPoint( value ).x;
					size.height = strToPoint( value ).y;
					node->setContentSize( size );
					break;
				case kTag:
					node->setTag( strToInt( value ) );
					break;
				case kCascadeColor:
					node->setCascadeColorEnabled( strToBool( value ) );
					break;
				case kCascadeOpacity:
					node->setCascadeOpacityEnabled( strToBool( value ) );
					break;
				case kLocalZ:
					node->setLocalZOrder( strToInt( value ) );
					break;
				case kGlobalZ:
					node->setGlobalZOrder( strToInt( value ) );
					break;
					//for sprite:
				case kImage:
					frame = ImageManager::shared().spriteFrame( value );
					if( sprite )
					{
						if( frame )
							sprite->setSpriteFrame( frame );
						else
							sprite->setTexture( value );
					}
					else if( timer )
					{
						auto sprite = ImageManager::sprite( value );
						if( sprite )
							timer->setSprite( sprite );
					}
					break;
					//for scroll menu:
				case kBlending:
					assert( sprite );
					sprite->setBlendFunc( strToBlendFunc(value) );
					break;
				case kAlias:
					assert( sprite );
					if( sprite && sprite->getTexture() )
					{
						bool fullscreen = mlUserData::getInstance()->get<bool>( "use_fullscreen", true );

						if( fullscreen == false )
						{
							if( strToBool( value ) )
								sprite->getTexture()->setAliasTexParameters();
							else
								sprite->getTexture()->setAntiAliasTexParameters();
						}
					}
					break;
				case kOpacity:
					node->setOpacity( strToInt( value ) );
					break;
				case kColor:
					node->setColor( strToColor3B( value ) );
					break;
				case kAnimation:
				case kAction:
				{
					auto action = xmlLoader::load_action( value );
					if( action )
						node->runAction( action );
					break;
				}
				case kAlignCols:
					assert( scrollmenu );
					scrollmenu->setAlignedColums( strToInt( value ) );
					break;
					//for mlMenuItem:
				case kImageNormal:
					assert( menuitem );
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
				case kEnabled:
					if( menuitem ) menuitem->setEnabled( strToBool( value ) );
					if( menu ) menu->setEnabled( strToBool( value ) );
					if( scrollmenu ) scrollmenu->setEnabled( strToBool( value ) );
					break;
				case kMenuCallBack:
					assert( menuitem );
					menuitem->setCallback( get_callback_by_description( value ) );
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
					if(menuitem) menuitem->setText( string );
					else if( text )
					{
						text->setString( string );
						text->setSourceString( rawvalue );
					}
					break;
				}
				case kFont:
					assert( menuitem || text );
					if( text ) text->setBMFontFilePath( value );
					else menuitem->setFont( value );
					break;
				case kFontTTF:
					assert( menuitem || text );
					if( text ) text->setTTFFontName( value );
					else menuitem->setSystemFontName( value );
					break;
				case kFontSize:
					assert( menuitem || text );
					if( text ) text->setTTFFontSize( strToInt( value ) );
					else menuitem->setSystemFontSize( strToInt( value ) );
					break;
				case kTextWidth:
					assert( text );
					if( text ) text->setMaxLineWidth( strToFloat( value ) );
					break;
				case kTextAlign:
					assert( text );
					if( text )
					{
						TextHAlignment align;
						if( value == "center" ) align = TextHAlignment::CENTER;
						else if( value == "right" ) align = TextHAlignment::RIGHT;
						else align = TextHAlignment::LEFT;
						text->setAlignment( align );
					}
					break;
				case kScaleEffect:
					assert( menuitem );
					menuitem->useScaleEffect( strToBool( value ) );
					break;
				case kAlignStartPosition:
					assert( scrollmenu );
					scrollmenu->setAlignedStartPosition( strToPoint( value ) );
					break;
				case kGridSize:
					assert( scrollmenu );
					scrollmenu->setGrisSize( strToSize( value ) );
					break;
				case kScissorRect:
					assert( scrollmenu );
					scrollmenu->setScissorRect( strToRect( value ) );
					break;
				case kScrollEnabled:
					assert( scrollmenu );
					scrollmenu->setScrollEnabled( strToBool( value ) );
					break;
				case kScissorEnabled:
					assert( scrollmenu );
					scrollmenu->setScissorEnabled( strToBool( value ) );
					break;
				case kAllowScrollByX:
					assert( scrollmenu );
					scrollmenu->setAllowScrollByX( strToBool( value ) );
					break;
				case kAllowScrollByY:
					assert( scrollmenu );
					scrollmenu->setAllowScrollByY( strToBool( value ) );
					break;
				case kDelayOnActivate:
					assert( menuitem );
					menuitem->setDelayOnActivate( strToFloat( value ) );
					break;
				case kFolder:
					assert( panel );
					panel->setFolder( value );
					break;
				case kHotLocalisation:
					assert( text );
					if( text )
						text->useLocation( strToBool( value ) );
					break;
				case kMidPoint:
					assert( timer );
					if( timer )timer->setMidpoint( strToPoint( value ) );
					break;
				case kPercent:
					assert( timer );
					if( timer )timer->setPercentage( strToFloat( value ) );
					break;
				case kBarRate:
					assert( timer );
					if( timer )timer->setBarChangeRate( strToPoint( value ) );
					break;
				case kProgressType:
					assert( timer );
					if( timer )
					{
						ProgressTimer::Type type( ProgressTimer::Type::RADIAL );
						if( value == "radial" )type = ProgressTimer::Type::RADIAL;
						else if( value == "bar" )type = ProgressTimer::Type::BAR;
						else assert( 0 );
						timer->setType( type );
					}
					break;
				case kResource:
				{
					auto particle = dynamic_cast<ParticleSystemQuad*>(node);
					if( particle )
					{
						particle->initWithFile( value );
					}
				}

				default:
					result = false;
					log_once( "property with name[%s] not dispathed node by name[%s]", toStringProperty( property ).c_str(), node->getName().c_str() );
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

	void unbookDirectory()
	{
		if( _directories.empty() )
			assert( 0 );
		auto nodeext = _directories.back();
		if( nodeext && nodeext->as_node_pointer() )
			nodeext->as_node_pointer()->release();
		_directories.pop_back();
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