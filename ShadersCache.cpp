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

#include "ShadersCache.h"

NS_CC_BEGIN;

void CustomShadersCache::onCreate()
{
	Director::getInstance()->getEventDispatcher()->addCustomEventListener( EVENT_RENDERER_RECREATED, [&]( EventCustom* ){
		reload();
	} );

	//{//uncoment this block for initiate reloading
	//	EventListenerKeyboard * event1 = EventListenerKeyboard::create();
	//	event1->onKeyReleased = std::bind(
	//		[&]( EventKeyboard::KeyCode key, Event* )mutable
	//	{
	//		if( key == EventKeyboard::KeyCode::KEY_A )
	//		{
	//			for( auto pair : _programs )
	//			{
	//				pair.second->reset();
	//			}
	//		}
	//		if( key == EventKeyboard::KeyCode::KEY_S )
	//		{
	//			cocos2d::EventCustom recreatedEvent( EVENT_RENDERER_RECREATED );
	//			Director::getInstance()->getEventDispatcher()->dispatchEvent( &recreatedEvent );
	//		}
	//	}, std::placeholders::_1, std::placeholders::_2 );
	//	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority( event1, 1 );
	//}
}

IntrusivePtr<GLProgram> CustomShadersCache::program( const std::string& path )
{
	IntrusivePtr<GLProgram> program = GLProgram::createWithFilenames( path + ".vsh", path + ".fsh" );
	add( path, program );
	return program;
}

void CustomShadersCache::add( const std::string& path, GLProgram* program )
{
	_programs.emplace( path, program );
}

void CustomShadersCache::reload()
{
	for( auto iter = _programs.begin(); iter != _programs.end(); )
	{
		//auto& program = *iter;
		//auto vsh = program.first + ".vsh";
		//auto fsh = program.first + ".fsh";
		//
		//auto state = GLProgramState::getOrCreateWithGLProgram( program.second );
		//auto node = state->getReferenceCount() > 1 ? state->getNodeBinding() : nullptr;
		//
		//log( "CustomShadersCache: reload custom shader: [%s:%d], any using[%s]", program.first.c_str(), program.second->_ID, node ? "yes" : "no" );
		//if( node )
		//{
		//	program.second = GLProgram::createWithFilenames( vsh, fsh );
		//	node->setGLProgram( program.second );
		//	++iter;
		//}
		//else
		//{
		//	iter = _programs.erase( iter );
		//}

		auto vsh = iter->first + ".vsh";
		auto fsh = iter->first + ".fsh";
		auto program = iter->second;
		program->reset();
		program->initWithFilenames( vsh, fsh );
		program->link();
		program->updateUniforms();
		log( "CustomShadersCache: reload custom shader: [%s:%d]", iter->first.c_str(), program->_ID );
		++iter;
	}
}


NS_CC_END;
