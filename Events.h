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

#ifndef __Events_h__
#define __Events_h__
#include "ml/macroses.h"
#include "ml/pugixml/pugixml.hpp"
#include "ml/IntrusivePtr.h"
#include "ml/ParamCollection.h"
NS_CC_BEGIN;

class NodeExt;

class EventBase : public cocos2d::Ref
{
	DECLARE_POINTER( EventBase );
public:
	static Pointer create( const std::string & type );

	virtual void execute( NodeExt * context ) = 0;
	virtual void setParam( const std::string & name, const std::string & value );
	virtual std::string getParam( const std::string & name ) = 0;
	virtual bool loadXmlEntity( const std::string& tag, const pugi::xml_node& node );

	virtual Node* getTarget( NodeExt * context );
private:
	CC_SYNTHESIZE_PASS_BY_REF( std::list<int>, _targetTags, TagsToTarget );
	CC_SYNTHESIZE_PASS_BY_REF( std::string, _targetPath, PathToTarget );
};

class EventsList : public std::list<EventBase::Pointer>
{
public:
	void execute( NodeExt * context );
};

class EventAction : public EventBase
{
	DECLARE_BUILDER( EventAction );
	CREATE_0(EventAction);
	bool init( );
public:
	virtual void execute( NodeExt * context );
	virtual void setParam( const std::string & name, const std::string & value );
	virtual std::string getParam( const std::string & name );
private:
private:
	std::string _actionname;
	std::string _state;
};

class EventRunAction : public EventAction
{
	DECLARE_BUILDER( EventRunAction );
	CREATE_0(EventRunAction);
	bool init( );
};

class EventStopAction : public EventAction
{
	DECLARE_BUILDER( EventStopAction );
	CREATE_0(EventStopAction);
	bool init( );
};

class EventStopAllAction : public EventAction
{
	DECLARE_BUILDER( EventStopAllAction );
	CREATE_0(EventStopAllAction);
	bool init( );
public:
	virtual void execute( NodeExt * context );
};

class EventSetProperty : public EventBase
{
	DECLARE_BUILDER( EventSetProperty );
	CREATE_0(EventSetProperty);
	bool init( );
public:
	virtual void execute( NodeExt * context );
	virtual void setParam( const std::string & name, const std::string & value );
	virtual std::string getParam( const std::string & name );
private:
	int _property;
	std::string _stringproperty;
	std::string _value;
};

class EventPlaySound : public EventBase
{
	DECLARE_BUILDER( EventPlaySound );
	CREATE_0(EventPlaySound);
	bool init( );
public:
	virtual void execute( NodeExt * context );
	virtual void setParam( const std::string & name, const std::string & value );
	virtual std::string getParam( const std::string & name );
private:
	void play( float dt );
	void stop( float dt );
private:
	std::string _sound;
	unsigned int _soundID;
	bool _looped;
	float _predelay;
	float _duration;
	float _panoram;
};

class EventScene : public EventBase
{
	DECLARE_BUILDER( EventScene );
	CREATE_0(EventScene);
	bool init();
public:
	static std::function<IntrusivePtr<Scene>( const std::string& )> GetScene;
public:
	virtual void execute( NodeExt * context );
	virtual void setParam( const std::string & name, const std::string & value );
	virtual std::string getParam( const std::string & name );
private:
private:
	std::string _action;
	std::string _scene;
};

class EventCreateNode : public EventBase
{
	DECLARE_BUILDER( EventCreateNode );
	CREATE_0( EventCreateNode );
	bool init();
public:
	virtual void execute( NodeExt * context );
	virtual void setParam( const std::string & name, const std::string & value );
	virtual std::string getParam( const std::string & name );
	virtual bool loadXmlEntity( const std::string& tag, const pugi::xml_node& node )override;
private:
	struct PositionInfo
	{
		Point offset;
		enum class PosType{ 
			byContext ,
			offset,
		} method;
	}_positionInfo;
	std::vector<NodePointer> _nodes;
	std::vector<std::string> _paths;
	int _additionalZOrder;
};

class EventStatisticAccumulate : public EventBase
{
	DECLARE_BUILDER( EventStatisticAccumulate );
	CREATE_0( EventStatisticAccumulate );
	bool init();
public:
	virtual void execute( NodeExt * context );
	virtual void setParam( const std::string & name, const std::string & value );
	virtual std::string getParam( const std::string & name );
private:
	bool _push;
	std::string _event;
	ParamCollection _params;
};

NS_CC_END
#endif