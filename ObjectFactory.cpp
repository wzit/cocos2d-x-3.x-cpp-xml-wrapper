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

#include "ml/ObjectFactory.h"
#include "ml/Events.h"
#include "ml/ScrollMenu.h"
#include "ml/MenuItem.h"
#include "ml/NodeExt.h"
#include "ml/Panel.h"
#include "ml/Text.h"
NS_CC_BEGIN;

Factory::Factory()
{
	book<Node>("node");
	book<Sprite>( "sprite" );
	book<Text>( "bmlabel" );
	book<Menu>( "menu" );
	book<ScrollMenu>( "scrollmenu" );
	book<mlMenuItem>( "menuitem" );
	book<Layer>( "layer" );
	book<LayerExt>( "layerext" );
	book<NodeExt_>( "nodeext" );
	book<MenuExt>( "menuext" );
	book<Panel>( "panel" );
	book<Text>( "text" );
	book<ParticleSystemQuad>( "particle" );
	book<ParallaxNode>( "paralax" );
	book_pointer<ProgressTimer>( "progresstimer" );

	book<EventAction>( "action" );
	book<EventRunAction>( "runaction" );
	book<EventStopAction>( "stopaction" );
	book<EventStopAllAction>( "stopallaction" );
	book<EventSetProperty>( "setproperty" );
	book<EventPlaySound>( "playsound" );
	book<EventScene>( "scenestack" );
	book<EventCreateNode>( "createnode" );
	book<EventStatisticAccumulate>( "statistic_accumulate" );
}

IntrusivePtr<cocos2d::Ref> Factory::build( const std::string & key )
{
	bool isreg = m_objects.find( key ) != m_objects.end();
	if( !isreg )
		log( "Factory say: type [%s] not registred", key.c_str() );
	assert( isreg );
	return isreg ? m_objects[key]->build() : nullptr;
};


NS_CC_END