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

#include "NodeExt.h"
#include "ml/common.h"
#include "ml/loadxml/xmlProperties.h"
#include "ml/loadxml/Crypto.h"
#include "ml/SmartScene.h"
NS_CC_BEGIN

NodeExt::NodeExt()
: _events()
, _actions()
{}

NodeExt::~NodeExt()
{}

bool NodeExt::init()
{
	return true;
}

Node* NodeExt::as_node_pointer()
{
	return dynamic_cast<Node*>(this);
}

Node * NodeExt::getChildByPath( const std::string & path_names )
{
	Node * self = as_node_pointer();
	Node * result = self ? getNodeByPath( self, path_names ) : nullptr;
	return result;
}

Node * NodeExt::getChildByPath( const std::list<int> path_tags )
{
	Node * self = as_node_pointer();
	Node * result = self ? getNodeByTagsPath( self, path_tags ) : nullptr;
	return result;
}

ccMenuCallback NodeExt::get_callback_by_description( const std::string & name )
{
	if( name.find( "runevent:" ) == 0 )
	{
		std::string eventname = name.substr( std::string( "runevent:" ).size() );
		return std::bind( [this, eventname]( Ref* ){runEvent( eventname ); }, std::placeholders::_1 );
	}
	else if( name.find( "openurl" ) == 0 )
	{
		std::string url = name.substr( std::string( "openurl:" ).size() );
		return std::bind( []( Ref*, const std::string& url ){openUrl( url ); }, std::placeholders::_1, url );
		
	}
	return nullptr;
}

ParamCollection& NodeExt::getParamCollection()
{
	return _params;
}

const ParamCollection& NodeExt::getParamCollection()const
{
	return _params;
}

void NodeExt::load( const std::string & path, const std::string & xmlFile )
{
	std::string file = path;
	if( file.empty() == false && file.back() != '/' )
		file += '/';
	file += xmlFile;
	load( file );
}

void NodeExt::load( const std::string & file )
{
	auto doc = xmlLoader::getDoc( file );
	auto root = doc->root().first_child();
	
	load( root );
}

void NodeExt::load( const pugi::xml_node & root )
{
	xmlLoader::bookDirectory( this );
	xmlLoader::load( as_node_pointer(), root );
	xmlLoader::unbookDirectory();
}

bool NodeExt::runEvent( const std::string & eventname )
{
	bool result( false );
	auto iter = _events.find( eventname );
	if( iter != _events.end() )
	{
		iter->second.execute( this );
		result = true;
	}
	else
	{
		std::string name = as_node_pointer() ? as_node_pointer()->getName() : "Not node inherited";
		//log_once( "NodeExt[%s]: event with name [%s] not dispatched", name.c_str( ), eventname.c_str( ) );
	}
	return result;
}

ActionPointer NodeExt::getAction( const std::string & name )
{
	auto iter = _actions.find( name );
	if( iter != _actions.end() )
		return iter->second;
	return nullptr;
}

bool NodeExt::setProperty( int intproperty, const std::string & value )
{
	return false;
}

bool NodeExt::setProperty( const std::string & stringproperty, const std::string & value )
{
	auto iter = _propertyLinks.find( stringproperty );
	if( iter != _propertyLinks.end() )
	{
		auto target = getNodeByPath( this->as_node_pointer(), iter->second.target );
		if( target )
			xmlLoader::setProperty( target, iter->second.property, value );
	}
	return false;
}

void NodeExt::loadActions( const pugi::xml_node & xmlnode )
{
	for( auto xmlaction : xmlnode )
	{
		std::string actionname = xmlaction.attribute( "name" ).as_string();
		auto action = xmlLoader::load_action( xmlaction );
		assert( action );
		_actions[actionname] = action;
	}
}

void NodeExt::loadEvents( const pugi::xml_node & xmlnode )
{
	for( auto xmllist : xmlnode )
	{
		std::string listname = xmllist.attribute( "name" ).as_string();
		for( auto xmlevent : xmllist )
		{
			auto event = xmlLoader::load_event( xmlevent );
			assert( event );
			_events[listname].push_back( event );
		}
	}
}

void NodeExt::loadParams( const pugi::xml_node & xmlnode )
{
	for( auto xmlparam : xmlnode )
	{
		std::string name = xmlparam.name();
		const auto& attr = xmlparam.attribute( "value" );
		std::string value = attr ? attr.as_string() : xmlparam.text().as_string();
		_params[name] = value;
	}
}

void NodeExt::loadPropertyLinks( const pugi::xml_node & xmlnode )
{
	/*
	<link name="health" target="menu/health/normal/text" property="text"/>
	*/
	for( auto xmllink : xmlnode )
	{
		std::string name = xmllink.attribute( "name" ).as_string();
		PropertyLink& link = _propertyLinks[name];
		link.name = name;
		link.target = xmllink.attribute( "target" ).as_string();
		link.property = xmllink.attribute( "property" ).as_string();
	}
}

bool NodeExt::loadXmlEntity( const std::string & tag, const pugi::xml_node & xmlnode )
{
	if( tag == xmlLoader::k::xmlTag::ParamCollection )
	{
		loadParams( xmlnode );
	};
	if( tag == xmlLoader::k::xmlTag::PropertyLinks )
	{
		loadPropertyLinks( xmlnode );
	}
	return false;
}


LayerExt::LayerExt()
: _firstEntrance(true)
, _dispatchKeyBack(true)
{}

LayerExt::~LayerExt() {}

bool LayerExt::init() 
{
	do
	{
		CC_BREAK_IF( !Layer::init() );
		CC_BREAK_IF( !NodeExt::init() );

		setContentSize( Size::ZERO );

		auto lambda = [this]( EventKeyboard::KeyCode key, Event* )mutable
		{
			if( _dispatchKeyBack && key == EventKeyboard::KeyCode::KEY_BACK )
				this->disappearance();
		};
		EventListenerKeyboard * event = EventListenerKeyboard::create();
		event->onKeyReleased = std::bind( lambda, std::placeholders::_1, std::placeholders::_2 );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( event, this );

		return true;
	}
	while( false );
	return false;

}

void LayerExt::onEnter()
{
	Layer::onEnter();
	if( _firstEntrance )
	{
		appearance();
		_firstEntrance = false;
	}
}

SmartScene* LayerExt::getSmartScene()
{
	auto scene = Layer::getScene();
	auto smart = static_cast<SmartScene*>(scene);
	return smart;
}

ccMenuCallback LayerExt::get_callback_by_description( const std::string & name )
{
	if( name == "close" )
	{
		return std::bind( [this]( Ref* ){this->disappearance(); }, std::placeholders::_1 );
	}
	else if( name.find( "pushlayer:" ) == 0 )
	{
		std::string path = name.substr( strlen( "pushlayer:" ) );
		auto push = []( Ref*, std::string& path )
		{
			auto layer = xmlLoader::load_node<Layer>( path );
			auto scene = static_cast<SmartScene*>(Director::getInstance()->getRunningScene());
			if( layer && scene )
				scene->pushLayer( layer, true );
		};
		return std::bind( push, std::placeholders::_1, path );
	}
	else
		return NodeExt::get_callback_by_description( name );
}

void LayerExt::appearance()
{
	runEvent( "appearance" );
}

void LayerExt::disappearance( bool force )
{
	if( force || isRunning() )
	{
		if( !runEvent( "disappearance" ) )
			runAction( CallFunc::create( std::bind( [this](){ removeFromParent(); } ) ) );
	}
}


NodeExt_::NodeExt_() {}
NodeExt_::~NodeExt_() {}

MenuExt::MenuExt() {}
MenuExt::~MenuExt() {}
NS_CC_END