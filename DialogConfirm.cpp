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

#include "DialogConfirm.h"
#include "ml/loadxml/xmlLoader.h"
#include "ml/SmartScene.h"
#include "ml/mlUserData.h"
#include "ml/MenuItem.h"
NS_CC_BEGIN

const std::string kUDDontAsk("dialog_dontask_");

const DialogConfirm::Result DialogConfirm::Yes = 0x1;
const DialogConfirm::Result DialogConfirm::No = 0x2;
const DialogConfirm::Result DialogConfirm::DontAsk = 0x4;

DialogConfirm::DialogConfirm()
: _dontask(false)
{
}

DialogConfirm::~DialogConfirm( )
{
}

bool DialogConfirm::init( const std::string & xmlfile, const std::string & nameID, bool showDontAsk )
{
	do
	{
		CC_BREAK_IF( !LayerExt::init() );
		_nameId = nameID;

		std::string key( "dontask_visible" );
		xmlLoader::macros::set( key, toStr( showDontAsk ) );
		load( xmlfile );
		xmlLoader::macros::erase( key );


		return true;
	}
	while( false );
	return false;
}

bool DialogConfirm::isDontAsk( const std::string& name )
{
	return mlUserData::getInstance()->get<bool>( kUDDontAsk + name );
}

void DialogConfirm::setDontAsk( const std::string& name )
{
	mlUserData::getInstance()->write( kUDDontAsk + name, true );
}

ccMenuCallback DialogConfirm::get_callback_by_description( const std::string & name )
{
	if( name == "ok" )return std::bind( &DialogConfirm::cb_select, this, std::placeholders::_1, true );
	else if( name == "cancel" )return std::bind( &DialogConfirm::cb_select, this, std::placeholders::_1, false );
	else if( name == "dontask" )return std::bind( &DialogConfirm::cb_dontask, this, std::placeholders::_1 );
	return LayerExt::get_callback_by_description( name );
}

void DialogConfirm::disappearance( bool force )
{
	auto menu = getChildByName<Menu*>( "menu" );
	if( menu ) 
		menu->setEnabled( false );

	observerAnswer.pushevent_result<bool>( No );
	LayerExt::disappearance();
}

void DialogConfirm::cb_select( Ref*, bool answer )
{
	Result r( 0 );
	r = r | (answer ? Yes : No);
	r = r | (_dontask ? DontAsk : 0);
	
	if( _dontask )
	{
		setDontAsk( _nameId );
	}

	bool exit = observerAnswer.getListenersSize() == 0;
	auto result = observerAnswer.pushevent_result<bool>( r );
	for( auto r : result )
	{
		if( r )
		{
			exit = true;
			break;
		}
	}
	if( exit )
		LayerExt::disappearance( true );
}

void DialogConfirm::cb_dontask( Ref* sender )
{
	mlMenuItem * item = static_cast<mlMenuItem*>(sender);
	assert( item );
	_dontask = !_dontask;
	std::string image = item->getParamCollection().get( _dontask ? "on" : "off" );
	item->setImageNormal( image );
}

NS_CC_END
