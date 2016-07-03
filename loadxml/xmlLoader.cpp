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

#include "common.h"
#include "../mlObjectFactory.h"
#include "Events.h"
#include "ScrollMenu.h"
#include "Animation.h"
#include "ParamCollection.h"
#include "ImageManager.h"
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
//#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
		return loadDoc( IniFile, path );
//#endif
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


	void load_scrollmenu_items( ScrollMenu* menu, pugi::xml_node xmlnode, int depth );
	void load_nonscissor_children( ScrollMenu* node, pugi::xml_node xmlnode, int depth );
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

	NodePointer load_node( const std::string & file, const std::string& topType, int depth )
	{
		log( "load node from doc [%s]", file.c_str() );
		auto doc = getDoc( file );
		auto root = doc->root().first_child();
		NodePointer result = root ? load_node( root, topType, depth ) : nullptr;
		return result;
	}

	IntrusivePtr<Node> load_node( pugi::xml_node xmlnode, const std::string& topType, int depth )
	{
		if( !xmlnode )
		{
			log( "warning: try loading node from emptt xmlnode" );
		}

		ParamCollection macroses( xmlnode.attribute( "macroses" ).as_string() );
		xmlnode.remove_attribute( "macroses" );
		for( auto pair : macroses )
			macros::set( pair.first, pair.second );

		const std::string& type = topType.empty() ? xmlnode.attribute( ksType.c_str() ).as_string() : topType;
		const std::string& template_ = macros::parse( xmlnode.attribute( ksTemplate.c_str() ).as_string() );
		
		IntrusivePtr<Node> result;
		if( template_.empty() )
		{
			if( type.empty() == false )
			{
				log( "try create object [%s]", type.c_str() );
				result = cocos2d::mlObjectFactory::shared().build<Node>( type );
				if( result )
					load( result.ptr(), xmlnode, depth );
			}
			else
			{
				log( "warning: try loading node with empty type" );
			}
		}
		else
		{
			result = load_node( template_, type, depth + 1 );
			pugi::xml_node xmlnodem = xmlnode;
			xmlnodem.remove_attribute( ksTemplate.c_str() );
			load( result, xmlnodem, depth + 1 );
		}
		if( depth == 0 && dynamic_cast<NodeExt*>(result.ptr()) )
		{
			dynamic_cast<NodeExt*>(result.ptr())->onLoaded();
		}
		return result;
	}

	void load( Node* node, const std::string & path, int depth )
	{
		log( "load from doc [%s]", path.c_str() );
		pugi::xml_document *doc( nullptr );
		doc = new pugi::xml_document;
		doc->load_file( path.c_str() );
		load( node, doc->root().first_child(), depth );
		delete doc;
	}

	void load( Node* node, pugi::xml_node xmlnode, int depth )
	{
		//const std::string& type = xmlnode.attribute( ksType.c_str() ).as_string();
		std::string template_ = xmlnode.attribute( ksTemplate.c_str() ).as_string();
		ParamCollection macroses( xmlnode.attribute( "macroses").as_string() );
		xmlnode.remove_attribute( "macroses" );
		for( auto pair : macroses )
		{
			macros::set( pair.first, pair.second );
		}
		
		if( template_.empty() == false )
		{
			load( node, template_, depth + 1 );
		}
		auto nodeext = dynamic_cast<NodeExt*>(node);
		if( nodeext)
			bookDirectory( nodeext );

		for( auto attr = xmlnode.first_attribute(); attr; attr = attr.next_attribute() )
		{
			const std::string name = attr.name();
			xmlLoader::setProperty( node, name, attr.value() );
		}
		while( true )
		{
			auto attr = xmlnode.first_attribute();
			if( attr )
				xmlnode.remove_attribute( attr );
			else
				break;
		}

		for( auto xmlentity : xmlnode )
		{
			const std::string& tag = xmlentity.name();
			if( tag == "children" )
				load_children( node, xmlentity, depth );
			else if( tag == "items" )/*&& type == "scrollmenu"*/ //remove checking type for template
			{
				ScrollMenu* scrollmenu = dynamic_cast<ScrollMenu*>(node);
				assert( scrollmenu );
				load_scrollmenu_items( scrollmenu, xmlentity, depth );
			}
			else if( tag == "children_nonscissor" )
			{
				ScrollMenu* scrollmenu = dynamic_cast<ScrollMenu*>(node);
				assert( scrollmenu );
				if( scrollmenu )
				{
					load_nonscissor_children( scrollmenu, xmlentity, depth );
				}
			}
			else if( tag == "actions" )
			{
				NodeExt * nodeext = dynamic_cast<NodeExt*>(node);
				assert( nodeext );
				nodeext->loadActions( xmlentity );
			}
			else if( tag == "events" )
			{
				NodeExt * nodeext = dynamic_cast<NodeExt*>(node);
				assert( nodeext );
				nodeext->loadEvents( xmlentity );
			}
			else if( tag == "macroses" )
			{
				for( auto xmlmacros : xmlentity )
				{
					auto name = xmlmacros.attribute( "name" ).as_string();
					auto value = xmlmacros.attribute( "value" ).as_string();
					macros::set( name, value );
				}
			}
			else
			{
				bool result( false );
				NodeExt * nodeext = dynamic_cast<NodeExt*>(node);
				if( nodeext )
				{
					result = nodeext->loadXmlEntity( tag, xmlentity );
				}

				if( !result )
				{
					log_once( "xml node will not reading. path=[%s]", xmlentity.path().c_str() );
				}
			}

		}

		if( nodeext )
		{
			unbookDirectory( nodeext );
		}
	}

	NodePointer getorbuild_node( Node * node, pugi::xml_node xmlnode, int depth )
	{
		const std::string& type = xmlnode.attribute( ksType.c_str() ).as_string();
		const std::string& name = xmlnode.attribute( ksName.c_str() ).as_string();
		const std::string& path = xmlnode.attribute( ksPath.c_str() ).as_string();
		const std::string& template_ = macros::parse( xmlnode.attribute( ksTemplate.c_str() ).as_string() );

		ParamCollection macroses( xmlnode.attribute( "macroses" ).as_string() );
		xmlnode.remove_attribute("macroses" );
		for( auto pair : macroses )
		{
			macros::set( pair.first, pair.second );
		}

		NodePointer child( nullptr );
		if( child == nullptr && path.empty() == false )
		{
			child = getNodeByPath( node, path );
		}
		if( child == nullptr && name.empty() == false )
		{
			child = node->getChildByName( name );
		}
		if( child && template_.empty() == false )
		{
			load( child, template_, depth );
		}
		if( child == nullptr && template_.empty() == false )
		{
			child = load_node( template_, type, depth );
			xmlnode.remove_attribute( ksTemplate.c_str() );
		}
		if( child && type.empty() == false )
		{
			//CCLOG( "Warning! type redifinited name[%s] type[%s]", name.c_str(), type.c_str() );
		}
		if( child == nullptr )
		{
			log( "try create child [%s]", type.c_str() );
			child = mlObjectFactory::shared().build<Node>( type );
		}
		if( !child )
		{
			CCLOG( "xmlLoader: getorbuild_node" );
			CCLOG( "name: [%s]", name.c_str() );
			CCLOG( "type: [%s]", type.c_str() );
			CCLOG( "path: [%s]", path.c_str() );
			CCLOG( "template: [%s]", template_.c_str() );
		}
		return child;
	}

	void load_children( Node* node, pugi::xml_node xmlnode, int depth )
	{
		FOR_EACHXML_BYTAG( xmlnode, xmlchild, "node" )
		{
			auto child = getorbuild_node( node, xmlchild, depth );
			assert( child );
			if( child == nullptr )
				continue;
			load( child, xmlchild, depth );
			if( child->getParent() == nullptr )
			{
				node->addChild( child, child->getLocalZOrder() );
			}
		}
	}

	void load_scrollmenu_items( ScrollMenu* menu, pugi::xml_node xmlnode, int depth )
	{
		for( auto xmlitem : xmlnode )
		{
			const std::string& imageN = macros::parse( xmlitem.attribute( "imageN" ).as_string() );
			const std::string& imageS = macros::parse( xmlitem.attribute( "imageS" ).as_string() );
			const std::string& imageD = macros::parse( xmlitem.attribute( "imageD" ).as_string() );
			const std::string& text = macros::parse( xmlitem.attribute( "text" ).as_string() );
			const std::string& font = macros::parse( xmlitem.attribute( "font" ).as_string() );
			const std::string& name = macros::parse( xmlitem.attribute( "name" ).as_string() );

			MenuItemPointer item( nullptr );
			item = menu->getItemByName( name );
			if( !item )
				item = menu->push( imageN, imageS, imageD, font, text, nullptr );
			load( item, xmlitem, depth );
		}
		menu->align( menu->getAlignedColums() );
	}
	void load_nonscissor_children( ScrollMenu* node, pugi::xml_node xmlnode, int depth )
	{
		FOR_EACHXML_BYTAG( xmlnode, xmlchild, "node" )
		{
			auto child = getorbuild_node( node, xmlchild, depth );
			assert( child );
			if( !child ) continue;
			load( child, xmlchild, depth );
			if( child->getParent() != node )
				node->addChildNotScissor( child, child->getLocalZOrder() );
		}
	}

	IntrusivePtr<EventBase> load_event( const pugi::xml_node xmlnode )
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
			auto name = child.name();
			event->loadXmlEntity( name, child );
		}
		return event;
	}

	template <typename T> static T parseValue( const std::vector <std::string> &attrs, size_t index )
	{
		const std::string& value = attrs[index];
		return strTo <T>( value );
	}

	template <typename T> static T parseOptionalValue(const std::vector <std::string> &attrs, size_t index, const T& defaultValue)
	{
		return index < attrs.size() ? parseValue <T>(attrs, index) : defaultValue;
	}

	template <> bool parseValue( const std::vector <std::string> &attrs, size_t index )
	{
		const std::string& value = attrs[index];
		return strTo<bool>(value);
	}


	ActionPointer load_action_from_file( const std::string & path )
	{
		pugi::xml_document doc;
		doc.load_file( path.c_str() );
		auto root = doc.root().first_child();
		return load_action( root );
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
			auto k = desc.find( "[" );
            if( k != std::string::npos )
				type = desc.substr( 0, k );
			return type;
		};
		auto getParams = []( const std::string & desc )
		{
			std::string params;
			auto k = desc.find( "[" );
			if( k != std::string::npos )
			{
				int count( 1 );
				int l = k + 1;
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
			int l = 0;
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
				auto k = string.find( "folder:" );
				if( k == 0 || k == 1 )
				{
					unsigned l = 0;
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
			auto _frames = []( std::string & string, const std::string& folder )
			{
				auto _list = [string, folder]()mutable
				{
					std::list<std::string> list;
					std::vector<std::string> frames;
					auto k = string.find( "frames:" );
					if( k == 0 || k == 1 )
					{
						string = string.substr( k + 7 );
					}
					if( string.back() == ']' )
						string.pop_back();
					split( list, string );
					for( auto & frame : list )
					{
						frames.push_back( folder + frame );
					}
					return frames;
				};
				auto _indexes = [string, folder]()mutable
				{
					std::string _indexes( "indexes:" );
					std::list<std::string> list;
					std::vector<std::string> frames;
					auto k = string.find( _indexes );
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
							int index = strTo<int>( string );
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
							int l = strTo<int>( a );
							int r = strTo<int>( b );
							for( int i = l; i != r; (r > l ? ++i : --i) )
							{
								indexes.push_back( i );
							}
							indexes.push_back( r );
						}
						list.pop_front();
					}

					std::string format( "%0" + toStr( (int)indexformat.size() ) + "d" );
					for( auto i : indexes )
					{
						char buffer[8];
						sprintf( buffer, format.c_str(), i );
						std::string frameext = frame + buffer + ext;
						std::string name = folder + frameext;
						auto frame = ImageManager::shared().spriteFrame( name );
						if( frame )
						{
							frames.push_back( name );
						}
						else if( FileUtils::getInstance()->isFileExist( name ) )
						{
							auto sprite = ImageManager::shared().sprite( name );
							if( sprite )
							{
								SpriteFrame* frame = SpriteFrame::create( name, Rect( 0, 0, sprite->getContentSize().width, sprite->getContentSize().height ) );
								if( frame )
									frames.push_back( name );
							}
						}
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

			static std::map<std::string, Animation*> _cash;
#ifndef _DEBUG
			auto iter = _cash.find( value );
			if( iter != _cash.end() )
				return iter->second->clone();
#endif

			auto str = value;
			auto folder = _folder( str );
			auto frames = _frames( str, folder );

			auto animation = createAnimation( frames, duration );
			if( animation )
			{
				animation->retain();
				_cash[value] = animation;
				animation = animation->clone();
			}
			return animation;
		};

		const std::string& cleared_desc = macros::parse( remove_spaces( desc ) );
		const std::string& type = getType( cleared_desc );
		const std::string& params = getParams( cleared_desc );
		const std::vector<std::string>& attr = getAttrs( params );

		auto FLOAT = [&attr] (int index) { return parseValue <float>(attr, index); };
		auto INT = [&attr] (int index) { return parseValue <int>(attr, index); };
		auto BOOL = [&attr] (int index) { return parseValue <bool>(attr, index); };

		auto OPTIONAL_FLOAT = [&attr]( size_t index, float defaultValue ) { return parseOptionalValue <float>( attr, index, defaultValue ); };
		auto OPTIONAL_INT = [&attr]( size_t index, int defaultValue ) { return parseOptionalValue <int>( attr, index, defaultValue ); };
		auto OPTIONAL_BOOL = [&attr]( size_t index, bool defaultValue ) { return parseOptionalValue <bool>( attr, index, defaultValue ); };
		auto OPTIONAL_STRING = [&attr]( size_t index ) { return index < attr.size() ? attr[index] : ""; };

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
		else if( type == k::ActionAnimate ) { return Animate::create( buildAnimation( FLOAT( 0 ), attr[1] ) ); }
		else if( type == k::ActionBezier ) {
			EaseBezierAction * bezier = EaseBezierAction::create( action_interval( attr[0] ) );
			bezier->setBezierParamer( FLOAT( 1 ), FLOAT( 2 ), FLOAT( 3 ), FLOAT( 4 ) );
			return bezier;
		}

		//action instant
		else if( type == k::ActionRemoveSelf ) { return RemoveSelf::create(); }
		else if( type == k::ActionShow ) { return Show::create(); }
		else if( type == k::ActionHide ) { return Hide::create(); }
		else
		{
#if USE_CHEATS == 1
			std::string message = "undefinited action type [" + type + "] \n";
			message += "action string: \n";
			message += cleared_desc;
			log_once( "%s", message.c_str() );
			MessageBox( message.c_str(), "Error creating action" );
			assert( 0 );
#endif
		}

		return nullptr;
	}

	ActionPointer load_action( const pugi::xml_node xmlnode )
	{
		auto body = xmlnode.attribute( "value" ).as_string();
		return load_action( body );
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

	void load_paramcollection( ParamCollection& params, const std::string & path )
	{
		pugi::xml_document doc;
		doc.load_file( path.c_str() );
		auto root = doc.root().first_child();
		load_paramcollection( params, root );
	}
	void load_paramcollection( ParamCollection& params, const pugi::xml_node xmlnode )
	{
		for( auto child : xmlnode )
		{
			std::string name = child.attribute( "name" ).as_string();
			std::string value = child.attribute( "value" ).as_string();
			if( name.empty() )
				name = child.name();
			if( value.empty() )
				value = child.text().as_string();
			params[name] = macros::parse(value);
		}
	}
};

NS_CC_END