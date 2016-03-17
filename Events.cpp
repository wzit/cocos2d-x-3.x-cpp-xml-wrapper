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

#include "ml/Events.h"
#include "ml/ObjectFactory.h"
#include "ml/common.h"
#include "ml/common.h"
#include "ml/NodeExt.h"
#include "ml/loadxml/xmlProperties.h"
#include "ml/loadxml/xmlLoader.h"
#include "ml/Audio/AudioEngine.h"
#include "ml/services/mlStatistic.h"

NS_CC_BEGIN;

EventBase::Pointer EventBase::create( const std::string & type )
{
	return Factory::shared().build<EventBase>( type );
}

EventBase::EventBase()
{}

EventBase::~EventBase()
{}

void EventBase::setParam( const std::string & name, const std::string & value )
{
	if( name == "targettags" )
	{
		std::list<std::string>list;
		split( list, value );
		for( auto tag : list )
			_targetTags.push_back( strToInt( tag ) );
	}
	else if( name == "target" )
	{
		_targetPath = value;
	}
}

bool EventBase::loadXmlEntity( const std::string& tag, const pugi::xml_node& node )
{
	return false;
}

Node* EventBase::getTarget( NodeExt * context )
{
	Node * target = nullptr;
	if( _targetTags.empty() == false )
		target = context->getChildByPath( _targetTags );
	else if( _targetPath.empty() == false )
		target = context->getChildByPath( _targetPath );
	else
		target = context->as_node_pointer();
	return target;
}


/****************************************************************************************/
//MARK:	EventAction
/****************************************************************************************/
EventAction::EventAction()
{}

EventAction::~EventAction()
{}

bool EventAction::init()
{
	return true;
}


void EventAction::execute( NodeExt * context )
{
	assert( context );
	auto action = context->getAction( _actionname );

	Node * target = getTarget( context );

	if( target == nullptr )
		log( "EventAction::execute: cannot find target. context: [%s], path: [%s]",
		context->as_node_pointer()->getName().c_str(), getPathToTarget().c_str() );
	if( action == nullptr )
		log( "EventAction::execute: cannot find action. context: [%s], action name: [%s]",
		context->as_node_pointer()->getName().c_str(), _actionname.c_str() );

#ifdef WIN32
	assert( action && target );
#endif
	if( !(action && target) )
		return;

	if( _state == "run" )
	{
		auto clone = action->clone();
		//auto tag = static_cast<int>(*((int*)action.ptr()));
		auto tag = action->_ID;
		target->runAction( clone );
		clone->setTag( tag );
	}
	else if( _state == "stop" )
	{
		//auto tag = static_cast<int>(*((int*)action.ptr()));
		auto tag = action->_ID;
		target->stopActionByTag( tag );
	}
	else
		assert( 0 );
}

void EventAction::setParam( const std::string & name, const std::string & value )
{
	if( name == "action" )
		_actionname = value;
	if( name == "state" )
	{
		_state = value;
		assert( _state == "run" || _state == "stop" );
	}
	else
		EventBase::setParam( name, value );
}

std::string EventAction::getParam( const std::string & name )
{
	if( name == "action" )
		return _actionname;
	return "";
}

EventRunAction::EventRunAction() {}
EventRunAction::~EventRunAction() {}
bool EventRunAction::init()
{
	if( EventAction::init() ) { setParam( "state", "run" ); return true; }
	return false;
}

EventStopAction::EventStopAction() {}
EventStopAction::~EventStopAction() {}
bool EventStopAction::init()
{
	if( EventAction::init() ) { setParam( "state", "stop" ); return true; }
	return false;
}

/****************************************************************************************/
//MARK:	EventStopAllAction
/****************************************************************************************/

EventStopAllAction::EventStopAllAction() {}
EventStopAllAction::~EventStopAllAction() {}
bool EventStopAllAction::init() { return true; }
void EventStopAllAction::execute( NodeExt * context )
{
	Node * target = getTarget( context );
	assert( target );
	if( target )
		target->stopAllActions();
}

/****************************************************************************************/
//MARK:	EventSetProperty
/****************************************************************************************/
EventSetProperty::EventSetProperty()
{}

EventSetProperty::~EventSetProperty()
{}

bool EventSetProperty::init()
{
	return true;
}

void EventSetProperty::execute( NodeExt * context )
{
	Node * target = getTarget( context );
	if( target )
	{
		if( _property != 0 )
			xmlLoader::setProperty( target, _property, _value );
		else
			xmlLoader::setProperty( target, _stringproperty, _value );
	}
}

void EventSetProperty::setParam( const std::string & name, const std::string & value )
{
	if( name == "property" )
	{
		_property = xmlLoader::strToPropertyType( value );
		if( _property == 0 )
			_stringproperty = value;
	}
	else if( name == "value" ) _value = value;
	else EventBase::setParam( name, value );
}

std::string EventSetProperty::getParam( const std::string & name )
{
	assert( 0 );
	return "";
}


/****************************************************************************************/
//MARK:	EventPlaySound
/****************************************************************************************/

EventPlaySound::EventPlaySound()
: _sound()
, _soundID( -1 )
, _looped( 0 )
, _predelay( 0 )
, _duration( 0 )
, _panoram( 0 )
{}

EventPlaySound::~EventPlaySound()
{}

bool EventPlaySound::init()
{
	return true;
}

void EventPlaySound::execute( NodeExt * context )
{
	assert( _looped ? _duration > 0 : true );
	retain();
	if( _predelay == 0 )
	{
		play( 0 );
	}
	else
	{
		auto func = std::bind( &EventPlaySound::play, this, std::placeholders::_1 );
		auto key = "EventPlaySound::play" + toStr( _ID );
		if( Director::getInstance()->getScheduler()->isScheduled( key, this ) == false )
		{
			Director::getInstance()->getScheduler()->schedule( func, this, _predelay, 0, 0, false, key );
		}
	}

}

void EventPlaySound::setParam( const std::string & name, const std::string & value )
{
	if( name == "sound" ) _sound = xmlLoader::macros::parse( value );
	else if( name == "looped" ) _looped = strToBool( xmlLoader::macros::parse( value ) );
	else if( name == "predelay" ) _predelay = strToFloat( xmlLoader::macros::parse( value ) );
	else if( name == "duration" ) _duration = strToFloat( xmlLoader::macros::parse( value ) );
	else if( name == "panoram" ) _panoram = strToFloat( xmlLoader::macros::parse( value ) );
	assert( _panoram >= -1 && _panoram <= 1 );
}

std::string EventPlaySound::getParam( const std::string & name )
{
	if( name == "sound" ) return _sound;
	else if( name == "looped" ) return toStr( _looped );
	else if( name == "predelay" ) return toStr( _predelay );
	else if( name == "duration" ) return toStr( _duration );
	else if( name == "panoram" ) return toStr( _panoram );
	return "";
}

void EventPlaySound::play( float dt )
{
	_soundID = AudioEngine::shared().playEffect( _sound, _looped, _panoram );
	if( _looped )
	{
		auto func = std::bind( &EventPlaySound::stop, this, std::placeholders::_1 );
		auto key = "EventPlaySound::stop" + toStr( _ID );
		if( Director::getInstance()->getScheduler()->isScheduled( key, this ) == false )
		{
			retain();
			Director::getInstance()->getScheduler()->schedule( func, this, _duration, 0, 0, false, key );
		}
	}
	release();
}

void EventPlaySound::stop( float dt )
{
	if( _soundID != -1 )
	{
		AudioEngine::shared().stopEffect( _soundID );
	}
	release();
}

/****************************************************************************************/
//MARK:	EventScene
/****************************************************************************************/

std::function<IntrusivePtr<Scene>( const std::string& )> EventScene::GetScene;

EventScene::EventScene()
{}

EventScene::~EventScene()
{}

bool EventScene::init()
{
	return true;
}

void EventScene::execute( NodeExt * context )
{
	auto push = []( Scene*scene ){ Director::getInstance()->pushScene( scene ); };
	auto replace = []( Scene*scene ){ Director::getInstance()->replaceScene( scene ); };
	auto pop = [](){ Director::getInstance()->popScene(); };

	if( _action == "push" )
	{
		assert( GetScene );
		push( GetScene( _scene ) );
	}
	else if( _action == "replace" )
	{
		assert( GetScene );
		replace( GetScene( _scene ) );
	}
	else if( _action == "pop" )
	{
		pop();
	}
	else
	{
		GetScene( _scene );
		//assert( 0 );
	}
}

void EventScene::setParam( const std::string & name, const std::string & value )
{
	if( name == "action" )
		_action = value;
	else if( name == "scene" )
		_scene = value;
	else
		EventBase::setParam( name, value );
}

std::string EventScene::getParam( const std::string & name )
{
	return "";
}


/****************************************************************************************/
//MARK:	EventCreateNode
/****************************************************************************************/

EventCreateNode::EventCreateNode()
	: _additionalZOrder( 0 )
{
	_positionInfo.method = PositionInfo::PosType::byContext;
}

EventCreateNode::~EventCreateNode()
{}

bool EventCreateNode::init()
{
	return true;
}

void EventCreateNode::execute( NodeExt * context )
{
	auto target = getTarget( context );
	if( !target )
		return;

	auto create = [target, context, this]( Node* node )
	{
		target->addChild( node );
		switch( _positionInfo.method )
		{
			case PositionInfo::PosType::byContext:
			{
				Node* connode = context ? context->as_node_pointer() : nullptr;
				Point pos = connode ? connode->getPosition() : Point::ZERO;
				pos += _positionInfo.offset + node->getPosition();
				node->setPosition( pos );
				break;
			}
			case PositionInfo::PosType::offset:
			{
				node->setPosition( _positionInfo.offset );
				break;
			}
		}
		node->setLocalZOrder( -node->getPositionY() + _additionalZOrder );
	};

	for( auto node : _nodes )
	{
		if( target && node && node->getParent() == nullptr )
			create( node );
	}
	for( auto path : _paths )
	{
		xmlLoader::bookDirectory( context );
		auto node = xmlLoader::load_node( path );
		xmlLoader::unbookDirectory();
		if( node )
			create( node );
	}
}

void EventCreateNode::setParam( const std::string & name, const std::string & value )
{
	if( 0 );
	else if( name == "additionalzorder" ) _additionalZOrder = strToInt( value );
	else if( name == "posinfo_offset" ) _positionInfo.offset = strToPoint( value );
	else if( name == "posinfo_method" )
	{
		if( value == "" || value == "bycontext" ) _positionInfo.method = PositionInfo::PosType::byContext;
		if( value == "offset" ) _positionInfo.method = PositionInfo::PosType::offset;
	}
	EventBase::setParam( name, value );
}

std::string EventCreateNode::getParam( const std::string & name )
{
	//return EventBase::getParam( name );
	return "";
}

bool EventCreateNode::loadXmlEntity( const std::string& tag, const pugi::xml_node& node )
{
	if( tag == "node" )
	{
		auto obj = xmlLoader::load_node( node );
		_nodes.push_back( obj );
	}
	else if( tag == "nodes" )
	{
		for( auto child : node )
		{
			auto obj = xmlLoader::load_node( child );
			_nodes.push_back( obj );
		}
	}
	else if( tag == "path" )
	{
		_paths.push_back( node.attribute( "file" ).as_string() );
	}

	else
		return EventBase::loadXmlEntity( tag, node );
	return true;
}


/****************************************************************************************/
//MARK:	EventStatisticAccumulate
/****************************************************************************************/

EventStatisticAccumulate::EventStatisticAccumulate()
: _push( false )
{}

EventStatisticAccumulate::~EventStatisticAccumulate()
{}

bool EventStatisticAccumulate::init()
{
	return true;
}

void EventStatisticAccumulate::execute( NodeExt * context )
{
	mlStatistic::shared().accumulate( _event, _params, _push );
}

void EventStatisticAccumulate::setParam( const std::string & name, const std::string & value )
{
	if( name == "event" )
		_event = value;
	else if( name == "params" )
		_params.parse( value );
	else if( name == "push" )
		_push = strToBool( value );
	else
		EventBase::setParam( name, value );
}

std::string EventStatisticAccumulate::getParam( const std::string & name )
{
	return "";
}

/****************************************************************************************/
//MARK:	EventsList
/****************************************************************************************/

void EventsList::execute( NodeExt * context )
{
	for( auto event : *this )
	{
		event->execute( context );
	}
}

NS_CC_END;