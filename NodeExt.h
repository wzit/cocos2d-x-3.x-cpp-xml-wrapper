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

#ifndef __NodeExt_h__
#define __NodeExt_h__
#include "cocos2d.h"
#include "macroses.h"
#include "Events.h"
#include "loadxml/xmlLoader.h"
#include "common.h"
#include "ParamCollection.h"

NS_CC_BEGIN

class SmartScene;

struct PropertyLink
{
	std::string name;
	std::string target;
	std::string property;
};

class NodeExt
{
public:
	bool runEvent( const std::string & eventname );
	ActionPointer getAction( const std::string & name );
	virtual Node* as_node_pointer();

	Node * getChildByPath( const std::string & path_names );
	Node * getChildByPath( const std::list<int> path_tags );
	virtual ccMenuCallback get_callback_by_description( const std::string & name );

	ParamCollection& getParamCollection();
	const ParamCollection& getParamCollection()const;
protected:
	NodeExt();
	virtual ~NodeExt();
	bool init();

public:
	virtual void load( const std::string & directory, const std::string & xmlFile )final;
	virtual void load( const std::string & pathToXmlFile )final;
	virtual void load( const pugi::xml_node & root );

public:
	virtual bool setProperty( int intproperty, const std::string & value );
	virtual bool setProperty( const std::string & stringproperty, const std::string & value );
	virtual bool loadXmlEntity( const std::string & tag, const pugi::xml_node & xmlnode );
	void loadActions( const pugi::xml_node & xmlnode );
	void loadEvents( const pugi::xml_node & xmlnode );
	void loadParams( const pugi::xml_node & xmlnode );
	void loadPropertyLinks( const pugi::xml_node & xmlnode );
private:
	std::map<std::string, EventsList> _events;
	std::map<std::string, ActionPointer> _actions;
	std::map<std::string, PropertyLink> _propertyLinks;
	ParamCollection _params;
};

class LayerExt: public Layer, public NodeExt
{
	DECLARE_BUILDER( LayerExt );
	CREATE_0(LayerExt);
	virtual bool init() override;
public:
	virtual void onEnter()override;
	SmartScene* getSmartScene();

	virtual void appearance();
	virtual void disappearance( bool force = false );
protected:
	virtual ccMenuCallback get_callback_by_description( const std::string & name )override;
private:
	bool _firstEntrance;
	CC_SYNTHESIZE( bool, _dispatchKeyBack, DispatchKeyBack );
};

class NodeExt_: public Node, public NodeExt
{
	DECLARE_BUILDER( NodeExt_ );
	CREATE_0(NodeExt_);
	bool init() { return true; }
};

class MenuExt : public Menu, public NodeExt
{
public:
	DECLARE_BUILDER( MenuExt );
	CREATE_0( MenuExt );
	bool init() { return Menu::init(); }
};


NS_CC_END
#endif // #ifndef NodeExt