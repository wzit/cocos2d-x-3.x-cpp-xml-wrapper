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

#include <ctime>
#include "ScoreCounter.h"
#include "ml/mlUserData.h"
#include "ml/loadxml/Crypto.h"

NS_CC_BEGIN;

namespace
{
	static const std::string xTeaKeyStr( "AKLJsw23faf32f48hrn124134tgvwsfb" );
	static xtea::KeyBlock xTeaKey;
}

ScoreCounter::ScoreCounter( void )
{
	xtea::convertKeyToXtea( xTeaKeyStr, xTeaKey );
}

void ScoreCounter::onCreate()
{
	int scoreIdMin = 0;
	int scoreIdMax = mlUserData::getInstance()->get<int>( "scores_index_max" );
	for( unsigned i = scoreIdMin; i <= scoreIdMax; ++i )
	{
		int scores( 0 );

		std::string s1( kUser_Scores_suffix + "L" + toStr( i ) );
		std::string s2( kUser_Scores_suffix + "R" + toStr( i ) );

		xtea::Word defaultvalue = { 0, 0 };
		xtea::crypt( defaultvalue, xTeaKey );
		xtea::Word word = {
			mlUserData::getInstance()->get<uint32_t>( s1, defaultvalue[0] ),
			mlUserData::getInstance()->get<uint32_t>( s2, defaultvalue[1] ),
		};
		xtea::decrypt( word, xTeaKey );
		scores = static_cast<int>(word[0]);

		if( scores > 0 )
			addMoney( i, scores, true );
	}
}

ScoreCounter::~ScoreCounter( void )
{}

void ScoreCounter::change( int id, int value, bool saveValueToUserData )
{
	int scores = getMoney( id ) + value;

	auto i = m_scores.find( id );
	if( i == m_scores.end() )
	{
		XteaWord word = { 0, 0 };
		i = m_scores.emplace( id, word ).first;
	}
	assert( i != m_scores.end() );
	xtea::Word word;
	word[0] = static_cast<uint32_t>(scores);
	word[1] = 1;
	xtea::crypt( word, xTeaKey );
	i->second._0 = word[0];
	i->second._1 = word[1];
	auto& observer = this->observer( id );
	observer.pushevent( scores );

	int scoreIdMax = mlUserData::getInstance()->get<int>( "scores_index_max" );
	if( scoreIdMax != id )
	{
		scoreIdMax = std::max( scoreIdMax, id );
		mlUserData::getInstance()->write<int>( "scores_index_max", scoreIdMax );
	}

	if( saveValueToUserData )
	{
		std::string s1( kUser_Scores_suffix + "L" + toStr( id ) );
		std::string s2( kUser_Scores_suffix + "R" + toStr( id ) );
		mlUserData::getInstance()->write( s1, toStr<uint32_t>( word[0] ) );
		mlUserData::getInstance()->write( s2, toStr<uint32_t>( word[1] ) );
		mlUserData::getInstance()->save();
	}
}

void ScoreCounter::clear()
{
	m_scores.clear();
	onCreate();
}

void ScoreCounter::setMoney( int id, int value, bool saveValueToUserData )
{
	int current = getMoney( id );
	addMoney( id, -current + value, saveValueToUserData );
}

void ScoreCounter::addMoney( int id, int value, bool saveValueToUserData )
{
	change( id, value, saveValueToUserData );
}

void ScoreCounter::subMoney( int id, int value, bool saveValueToUserData )
{
	change( id, -value, saveValueToUserData );
}

int ScoreCounter::getMoney( int id )const
{
	auto i = m_scores.find( id );
	if( i != m_scores.end() )
	{
		xtea::Word word = { i->second._0, i->second._1 };
		xtea::decrypt( word, xTeaKey );
		int scores = static_cast<int>(word[0]);
		return scores;
	}
	else
	{
		return 0;
	}
}

ScoreCounter::Observer_OnChangeScore& ScoreCounter::observer( int id )
{
	return _onChangeScores[id];
}

NS_CC_END;