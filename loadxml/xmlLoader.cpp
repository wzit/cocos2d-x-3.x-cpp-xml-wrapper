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

#include "ml/common.h"
#include "ml/ObjectFactory.h"
#include "ml/Events.h"
#include "ml/ScrollMenu.h"
#include "ml/Animation.h"
#include "ml/ParamCollection.h"
#include "xmlLoader.h"
#include "xmlProperties.h"
#include "Crypto.h"

NS_CC_BEGIN;


#ifdef BOOL 
#	undef BOOL
#endif

const std::string IniFile( "" );
namespace xmlLoader
{
	std::map<std::string, XmlDocPointer > _cache;

	XmlDocPointer getDoc(const std::string& path)
	{
#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
		return loadDoc( IniFile, path );
#endif
		auto iter = _cache.find( path );
		if( iter == _cache.end() )
		{
			auto doc = loadDoc( IniFile, path );
			if( doc )
			{
				auto pair = std::pair<std::string, XmlDocPointer>( path, doc );
				iter = _cache.insert( pair ).first;
			}
		}
		if( iter == _cache.end() )
			return std::make_shared<pugi::xml_document>();
		else
			return iter->second;
	}


	void load_scrollmenu_items( ScrollMenu* menu, const pugi::xml_node & xmlnode );
	void load_nonscissor_children( ScrollMenu* menu, const pugi::xml_node & xmlnode );
	void load_paralax_children( ParallaxNode* menu, const pugi::xml_node & xmlnode );

	std::string object_type( const std::string & file )
	{
		std::string path = file;
		do
		{
			auto doc = getDoc( path );
			auto root = doc->root().first_child();
			std::string type = root.attribute( ksType.c_str() ).as_string();
			if( type.empty() == false )
				return type;
			path = root.attribute( ksTemplate.c_str() ).as_string();
			if( path.empty() )
				break;
		}
		while( true );
		return "";
	}

	NodePointer load_node( const std::string & file )
	{
		auto doc = getDoc( file );
		auto root = doc->root().first_child();
		return root ? load_node( root ) : nullptr;
	}

	IntrusivePtr<Node> load_node( const pugi::xml_node & xmlnode )
	{
		const std::string& type = xmlnode.attribute( ksType.c_str() ).as_string();
		const std::string& template_ = xmlnode.attribute( ksTemplate.c_str() ).as_string();
		if( template_.empty() )
		{
			if( type.empty() )
			{
				log( "xmlLoader (load_node): empty type. xml node path: [%s]", xmlnode.path().c_str() );
			}
			auto node = Factory::shared().build<Node>( type );
			auto nodeext = dynamic_cast<NodeExt*>(node.ptr());
			if( nodeext )
				bookDirectory( nodeext );
			load( node.ptr(), xmlnode, false );
			if( nodeext )
				unbookDirectory();

			return node;
		}
		else
		{
			auto node = load_node( template_ );
			pugi::xml_node xmlnodem = xmlnode;
			load( node, xmlnodem, true );
			return node;
		}
	}

	IntrusivePtr<EventBase> load_event( const pugi::xml_node & xmlnode )
	{
		const std::string& type = xmlnode.name();

		auto event = EventBase::create( type );
		for( auto attr = xmlnode.first_attribute(); attr; attr = attr.next_attribute() )
		{
			auto name = attr.name();
			auto value = macros::parse( attr.value() );
			event->setParam( name, value );
		}
		for( auto child = xmlnode.first_child(); child; child = child.next_sibling() )
		{
			event->loadXmlEntity( child.name(), child );
		}
		return event;
	}

	void load( Node* node, const std::string & path )
	{
		auto doc = getDoc( path );
		load( node, doc->root().first_child() );
	}

	void load( Node* node, const pugi::xml_node & xmlnode, bool ignoreTemplate )
	{
		const std::string& template_ = xmlnode.attribute( ksTemplate.c_str() ).as_string();
		if( ignoreTemplate == false && template_.empty() == false )
		{
			load( node, template_ );
		}

		for( auto attr = xmlnode.first_attribute(); attr; attr = attr.next_attribute() )
		{
			const std::string name = attr.name();
			xmlLoader::setProperty( node, name, attr.value() );
		}

		for( auto xmlentity : xmlnode )
		{
			const std::string& tag = xmlentity.name();
			bool result( false );
			NodeExt * nodeext = dynamic_cast<NodeExt*>(node);
			if( nodeext )
			{
				result = nodeext->loadXmlEntity( tag, xmlentity );
			}
			if( !result )
			{
				result = true;
				if( tag == "children" )
				{
					auto paralax = dynamic_cast<ParallaxNode*>(node);
					if( !paralax )
						load_children( node, xmlentity );
					else
						load_paralax_children( paralax, xmlentity );
				}
				else if( tag == "items" )
				{
					ScrollMenu* scrollmenu = dynamic_cast<ScrollMenu*>(node);
					assert( scrollmenu );
					load_scrollmenu_items( scrollmenu, xmlentity );
				}
				else if( tag == "children_nonscissor" )
				{
					ScrollMenu* scrollmenu = dynamic_cast<ScrollMenu*>(node);
					assert( scrollmenu );
					if( scrollmenu )
					{
						load_nonscissor_children( scrollmenu, xmlentity );
					}
				}
				else if( tag == "actions" )
				{
					NodeExt * nodeext = dynamic_cast<NodeExt*>(node);
					assert( nodeext );
					if( nodeext ) nodeext->loadActions( xmlentity );
				}
				else if( tag == "events" )
				{
					NodeExt * nodeext = dynamic_cast<NodeExt*>(node);
					assert( nodeext );
					if( nodeext ) nodeext->loadEvents( xmlentity );
				}
				else
				{
					result = false;
				}
				if( !result )
				{
					log_once( "xml node will not reading. path=[%s]", xmlentity.path().c_str() );
				}
			}

		}
	}

	NodePointer getorbuild_node( Node * node, const pugi::xml_node & xmlnode, bool& createdFromTemplate )
	{
		const std::string& type = xmlnode.attribute( ksType.c_str() ).as_string();
		const std::string& name = xmlnode.attribute( ksName.c_str() ).as_string();
		const std::string& path = xmlnode.attribute( ksPath.c_str() ).as_string();
		const std::string& template_ = xmlnode.attribute( ksTemplate.c_str() ).as_string();
		NodePointer child( nullptr );
		if( template_.empty() == false )
		{
			child = load_node( macros::parse( template_ ) );
			if( child )
				createdFromTemplate = true;
		}
		if( child == nullptr && path.empty() == false )
		{
			child = getNodeByPath( node, path );
		}
		if( child == nullptr && name.empty() == false )
		{
			child = node->getChildByName( name );
		}
		if( child == nullptr )
		{
			if( type.empty() )
			{
				log( "xmlLoader (getorbuild_node): empty type. xml node path: [%s]", xmlnode.path().c_str() );
			}
			child = Factory::shared().build<Node>( type );
		}
		return child;
	}

	void load_children( Node* node, const pugi::xml_node & xmlnode )
	{
		FOR_EACHXML_BYTAG( xmlnode, xmlchild, "node" )
		{
			bool createdFromTemplate( false );
			auto child = getorbuild_node( node, xmlchild, createdFromTemplate );
			if( !child )
				continue;
			assert( child );
			if( child == nullptr )
				continue;
			load( child, xmlchild, createdFromTemplate );
			if( child->getParent() == nullptr )
			{
				node->addChild( child, child->getLocalZOrder() );
			}
		}
	}

	ActionPointer load_action( const std::string & desc )
	{
		auto remove_spaces = []( const std::string & desc )
		{
			std::string result = desc;
			const std::string spaces( " \n\t\r" );
			std::string::size_type k( 0 );
			while( (k = result.find_last_of( spaces )) != std::string::npos )
			{
				result.erase( k, 1 );
			}
			return result;
		};
		auto getType = []( const std::string & desc )
		{
			std::string type;
			size_t k = desc.find( "[" );
			if( k != std::string::npos )
				type = desc.substr( 0, k );
			return type;
		};
		auto getParams = []( const std::string & desc )
		{
			std::string params;
			size_t k = desc.find( "[" );
			if( k != std::string::npos )
			{
				int count( 1 );
				size_t l = k + 1;
				for( ; l < (int)desc.size() && count != 0; ++l )
				{
					if( desc[l] == '[' )++count;
					else if( desc[l] == ']' )--count;
				}
				params = desc.substr( k + 1, l - k - 2 );
			}
			return params;
		};
		auto getAttrs = []( const std::string & params )
		{
			std::vector<std::string> attr;
			int count = 0;
			size_t l = 0;
			for( unsigned r = 0; r < params.size(); ++r )
			{
				if( params[r] == '[' )++count;
				else if( params[r] == ']' )--count;
				if( count == 0 && params[r] == ',' )
				{
					attr.push_back( params.substr( l, r - l ) );
					l = r + 1;
				}
			}
			attr.push_back( params.substr( l ) );
			return attr;
		};
		auto buildAnimation = []( float duration, const std::string value )
		{
			auto _folder = []( std::string & string )
			{
				std::string result;
				size_t k = string.find( "folder:" );
				if( k == 0 || k == 1 )
				{
					size_t l = 0;
					for( l = 0; l < string.size(); ++l )
					{
						if( string[k + l] == ',' )
							break;
					}
					result = string.substr( k + 7, l - 7 );
					string = string.substr( k + l + 1 );
				}
				return result;
			};
			auto _frames = []( std::string & string )
			{
				auto _list = [string]()mutable
				{
					std::list<std::string> list;
					std::vector<std::string> frames;
					size_t k = string.find( "frames:" );
					if( k == 0 || k == 1 )
					{
						string = string.substr( k + 7 );
					}
					if( string.back() == ']' )
						string.pop_back();
					split( list, string );
					for( auto & frame : list )
					{
						frames.push_back( frame );
					}
					return frames;
				};
				auto _indexes = [string]()mutable
				{
					std::string _indexes( "indexes:" );
					std::list<std::string> list;
					std::vector<std::string> frames;
					int k = string.find( _indexes );
					if( k == 0 || k == 1 )
						string = string.substr( k + _indexes.size() );
					if( string.back() == ']' )
						string.pop_back();
					split( list, string );
					assert( list.size() >= 2 );

					std::string frame = list.front();
					std::string ext;
					k = frame.find_last_of( "." );
					ext = frame.substr( k );
					frame = frame.substr( 0, k );
					list.pop_front();

					std::string indexformat;
					std::vector<int> indexes;
					while( list.empty() == false )
					{
						std::string string = list.front();
						auto k = string.find( ":" );
						if( k == std::string::npos )
						{
							int index = strToInt( string );
							indexes.push_back( index );

							if( indexformat.size() < string.size() )
								indexformat = string;
						}
						else
						{
							std::string a = string.substr( 0, k );
							std::string b = string.substr( k + 1 );
							if( indexformat.size() < a.size() ) indexformat = a;
							if( indexformat.size() < b.size() ) indexformat = b;
							int l = strToInt( a );
							int r = strToInt( b );
							for( int i = l; i != r; (r > l ? ++i : --i) )
							{
								indexes.push_back( i );
							}
							indexes.push_back( r );
						}
						list.pop_front();
					}

					std::string format( "%0" + toStr( indexformat.size() ) + "d" );
					for( auto i : indexes )
					{
						char buffer[8];
						sprintf( buffer, format.c_str(), i );
						std::string frameext = frame + buffer + ext;
						frames.push_back( frameext );
					}

					return frames;
				};
				if( string.find( "frames:" ) != std::string::npos )
					return _list();
				else if( string.find( "indexes:" ) != std::string::npos )
					return _indexes();
				assert( 0 );
				return std::vector<std::string>();
			};
			auto str = value;
			auto folder = _folder( str );
			auto frames = _frames( str );
			for( auto & frame : frames )
				frame = folder + frame;

			return createAnimation( frames, duration );
		};

		const std::string& cleared_desc = macros::parse( remove_spaces( desc ) );
		const std::string& type = getType( cleared_desc );
		const std::string& params = getParams( cleared_desc );
		const std::vector<std::string>& attr = getAttrs( params );

		auto FLOAT = [attr]( int index )
		{
			const std::string& value = attr[index];
			std::string::size_type k = value.find( ".." );
			if( k == std::string::npos )
			{
				return strToFloat( value );
			}
			else
			{
				const float l = strToFloat( value.substr( 0, k ) );
				const float r = strToFloat( value.substr( k + 2 ) );
				const float v = CCRANDOM_0_1() * (r - l) + l;
				assert( l <= r );
				assert( v >= l && v <= r );
				return v;
			}
		};
		auto INT = [attr]( int index )
		{
			const std::string& value = attr[index];
			std::string::size_type k = value.find( ".." );
			if( k == std::string::npos )
			{
				return strToInt( value );
			}
			else
			{
				const int l = strToFloat( value.substr( 0, k ) );
				const int r = strToFloat( value.substr( k + 2 ) );
				const int v = static_cast<int>(CCRANDOM_0_1() * (r - l) + l);
				assert( l <= r );
				assert( v >= l && v <= r );
				return v;
			}
		};
		auto BOOL = [attr]( int index )
		{
			const std::string& value = attr[index];
			return strToBool( value );
		};

		auto action_interval = []( const std::string & desc ) { return static_cast<ActionInterval*>(load_action( desc ).ptr()); };
		//auto action_finitetime = []( const std::string & desc ) { return static_cast<FiniteTimeAction*>(load_action( desc ).ptr()); };

		if( type == k::ActionSequence || type == k::ActionSpawn )
		{
			const std::vector<std::string> & sactions = getAttrs( params );
			Vector<FiniteTimeAction*>actions;
			for( auto& saction : sactions )
			{
				auto action = load_action( saction );
				auto fta = dynamic_cast<FiniteTimeAction*>(action.ptr());
				assert( fta );
				if( fta )
					actions.pushBack( fta );
			}

			if( type == k::ActionSequence )
				return Sequence::create( actions );
			else
				return Spawn::create( actions );
		}
		else if( type == k::ActionDelayTime ) { return DelayTime::create( FLOAT( 0 ) ); }
		else if( type == k::ActionScaleTo ) { return ScaleTo::create( FLOAT( 0 ), FLOAT( 1 ), FLOAT( 2 ) ); }
		else if( type == k::ActionScaleBy ) { return ScaleBy::create( FLOAT( 0 ), FLOAT( 1 ), FLOAT( 2 ) ); }
		else if( type == k::ActionSkewTo ) { return SkewTo::create( FLOAT( 0 ), FLOAT( 1 ), FLOAT( 2 ) ); }
		else if( type == k::ActionSkewBy ) { return SkewBy::create( FLOAT( 0 ), FLOAT( 1 ), FLOAT( 2 ) ); }
		else if( type == k::ActionMoveTo ) { return MoveTo::create( FLOAT( 0 ), Point( FLOAT( 1 ), FLOAT( 2 ) ) ); }
		else if( type == k::ActionMoveBy ) { return MoveBy::create( FLOAT( 0 ), Point( FLOAT( 1 ), FLOAT( 2 ) ) ); }
		else if( type == k::ActionRotateTo ) { return RotateTo::create( FLOAT( 0 ), FLOAT( 1 ) ); }
		else if( type == k::ActionRotateBy ) { return RotateBy::create( FLOAT( 0 ), FLOAT( 1 ) ); }
		else if( type == k::ActionJumpTo ) { return JumpTo::create( FLOAT( 0 ), Point( FLOAT( 1 ), FLOAT( 2 ) ), FLOAT( 3 ), INT( 4 ) ); }
		else if( type == k::ActionJumpBy ) { return JumpBy::create( FLOAT( 0 ), Point( FLOAT( 1 ), FLOAT( 2 ) ), FLOAT( 3 ), INT( 4 ) ); }
		else if( type == k::ActionBlink ) { return Blink::create( FLOAT( 0 ), INT( 1 ) ); }
		else if( type == k::ActionFadeTo ) { return FadeTo::create( FLOAT( 0 ), INT( 1 ) ); }
		else if( type == k::ActionFadeIn ) { return FadeIn::create( FLOAT( 0 ) ); }
		else if( type == k::ActionFadeOut ) { return FadeOut::create( FLOAT( 0 ) ); }
		else if( type == k::ActionTintTo ) { return TintTo::create( FLOAT( 0 ), INT( 1 ), INT( 2 ), INT( 3 ) ); }
		else if( type == k::ActionTintBy ) { return TintBy::create( FLOAT( 0 ), INT( 1 ), INT( 2 ), INT( 3 ) ); }

		else if( type == k::ActionRepeatForever ) { return RepeatForever::create( action_interval( attr[0] ) ); }
		else if( type == k::ActionRepeat ) { return Repeat::create( action_interval( attr[0] ), INT( 1 ) ); }
		else if( type == k::ActionEaseIn ) { return EaseIn::create( action_interval( attr[0] ), FLOAT( 1 ) ); }
		else if( type == k::ActionEaseOut ) { return EaseOut::create( action_interval( attr[0] ), FLOAT( 1 ) ); }
		else if( type == k::ActionEaseInOut ) { return EaseInOut::create( action_interval( attr[0] ), FLOAT( 1 ) ); }
		else if( type == k::ActionBounceIn ) { return EaseBounceIn::create( action_interval( attr[0] ) ); }
		else if( type == k::ActionBounceOut ) { return EaseBounceOut::create( action_interval( attr[0] ) ); }
		else if( type == k::ActionBounceInOut ) { return EaseBounceInOut::create( action_interval( attr[0] ) ); }
		else if( type == k::ActionBackIn ) { return EaseBackIn::create( action_interval( attr[0] ) ); }
		else if( type == k::ActionBackOut ) { return EaseBackOut::create( action_interval( attr[0] ) ); }
		else if( type == k::ActionBackInOut ) { return EaseBackInOut::create( action_interval( attr[0] ) ); }
		else if( type == k::ActionSineIn ) { return EaseSineIn::create( action_interval( attr[0] ) ); }
		else if( type == k::ActionSineOut ) { return EaseSineOut::create( action_interval( attr[0] ) ); }
		else if( type == k::ActionSineInOut ) { return EaseSineInOut::create( action_interval( attr[0] ) ); }
		else if( type == k::ActionAnimate ) 
		{ 
			auto animation = buildAnimation( FLOAT( 0 ), attr[1] );
			return animation ? Animate::create( animation ) : nullptr;
		}

		//action instant
		else if( type == k::ActionRemoveSelf ) { return RemoveSelf::create(); }
		else if( type == k::ActionShow ) { return Show::create(); }
		else if( type == k::ActionHide ) { return Hide::create(); }
		else if( type == k::ActionEnable ) { return ActionEnable::create(); }
		else if( type == k::ActionDisable ) { return ActionDisable::create(); }
		else
		{
			std::string message = "undefinited action type [" + type + "] \n";
			message += "action string: \n";
			message += cleared_desc;
			log_once( "%s", message.c_str() );
			assert( 0 );
		}

		return nullptr;
	}

	ActionPointer load_action_from_file( const std::string& path )
	{
		auto doc = getDoc( path );
		return load_action( doc->root().first_child() );
	}

	ActionPointer load_action( const pugi::xml_node & xmlnode )
	{
		auto body = xmlnode.attribute( "value" ).as_string();
		return load_action( body );
	}

	void load_scrollmenu_items( ScrollMenu* menu, const pugi::xml_node & xmlnode )
	{
		for( auto xmlitem : xmlnode )
		{
			const std::string& imageN = macros::parse( xmlitem.attribute( "imageN" ).as_string() );
			const std::string& imageS = macros::parse( xmlitem.attribute( "imageS" ).as_string() );
			const std::string& imageD = macros::parse( xmlitem.attribute( "imageD" ).as_string() );
			const std::string& text = macros::parse( xmlitem.attribute( "text" ).as_string() );
			const std::string& font = macros::parse( xmlitem.attribute( "font" ).as_string() );

			MenuItemPointer item( nullptr );
			item = menu->getItemByName( xmlitem.attribute( "name" ).as_string() );
			if( !item )
				item = menu->push( imageN, imageS, imageD, font, text, nullptr );
			load( item, xmlitem );
		}
		menu->align( menu->getAlignedColums() );
	}
	void load_nonscissor_children( ScrollMenu* node, const pugi::xml_node & xmlnode )
	{
		FOR_EACHXML_BYTAG( xmlnode, xmlchild, "node" )
		{
			bool createdFromTemplate(false);
			auto child = getorbuild_node( node, xmlchild, createdFromTemplate );
			if( !child ) 
				continue;
			load( child, xmlchild, createdFromTemplate );
			if( child->getParent() != node )
				node->addChildNotScissor( child, child->getLocalZOrder() );
		}
	}
	void load_paralax_children( ParallaxNode* node, const pugi::xml_node & xmlnode )
	{
		for( auto xmlchild : xmlnode )
		{
			auto ratio = strToPoint( xmlchild.attribute( "ratio" ).as_string() );
			auto offset = strToPoint( xmlchild.attribute( "offset" ).as_string() );
			bool createdFromTemplate( false );
			auto child = getorbuild_node( node, xmlchild, createdFromTemplate );
			if( !child )
				continue;
			load( child, xmlchild, createdFromTemplate );
			if( child->getParent() )
				child->removeFromParent();
			node->addChild( child, child->getLocalZOrder(), ratio, offset );
		}
	}

	namespace macros
	{
		static ParamCollection _macroses;

		void set( const std::string & name, const std::string & value )
		{
			_macroses[name] = value;
		}

		const std::string& get( const std::string & name )
		{
			auto iter = _macroses.find( name );
			if( iter != _macroses.end() )
				return iter->second;

			return name;
		}

		void clear()
		{
			_macroses.clear();
		}

		void erase( const std::string & name )
		{
			auto iter = _macroses.find( name );
			if( iter != _macroses.end() )
				_macroses.erase( iter );
		}

		const std::string parse( const std::string & string )
		{
			std::string result = string;
			while( true )
			{
				const std::string::size_type l = result.find( delimiter );
				const std::string::size_type r = result.find( delimiter, l + delimiter_size );

				if( l != std::string::npos && r != std::string::npos )
				{
					const std::string& macros = result.substr( l + delimiter_size, r - (l + delimiter_size) );
					const std::string& substitution = parse( get( macros ) );
					result.replace( l, r - l + delimiter_size, substitution );
				}
				else
				{
					break;
				}
			}

			return result;
		}

	}

	void load( ParamCollection * params, const std::string & path )
	{
		assert( params );
		auto doc = getDoc( path );
		auto root = doc->root().first_child();
		for( auto child : root )
		{
			std::string name = child.attribute( "name" ).as_string();
			std::string value = child.attribute( "value" ).as_string();
			(*params)[name] = value;
		}
	}


	typedef int8_t SaveTypeCode;
	typedef int32_t SaveTypeSize;
	typedef uint8_t SaveTypeString;

	const SaveTypeCode kCode1( 1 );
	const SaveTypeCode kCode2( 2 );
	const SaveTypeCode kCode3( 3 );
	const SaveTypeCode kCode4( 4 );
	const SaveTypeCode kCodeMax( 4 );

	SaveTypeString encode1( SaveTypeString c ) { return c * 2 + 1; }
	SaveTypeString encode2( SaveTypeString c ) { return c * 2 + 2; }
	SaveTypeString encode3( SaveTypeString c ) { return c * 2 - 1; }
	SaveTypeString encode4( SaveTypeString c ) { return c * 2 - 2; }

	SaveTypeString decode1( SaveTypeString c ) { return (c - 1) / 2; }
	SaveTypeString decode2( SaveTypeString c ) { return (c - 2) / 2; }
	SaveTypeString decode3( SaveTypeString c ) { return (c + 1) / 2; }
	SaveTypeString decode4( SaveTypeString c ) { return (c + 2) / 2; }

	SaveTypeString encode( SaveTypeCode code, SaveTypeString c )
	{
		if( code == kCode1 ) return encode1( c );
		if( code == kCode2 ) return encode2( c );
		if( code == kCode3 ) return encode3( c );
		if( code == kCode4 ) return encode4( c );
		if( code > kCodeMax ) return c;
		return c;
	}

	SaveTypeString decode( SaveTypeCode code, SaveTypeString c )
	{
		if( code == kCode1 ) return decode1( c );
		if( code == kCode2 ) return decode2( c );
		if( code == kCode3 ) return decode3( c );
		if( code == kCode4 ) return decode4( c );
		if( code > kCodeMax ) return c;
		return c;
	}
};

NS_CC_END