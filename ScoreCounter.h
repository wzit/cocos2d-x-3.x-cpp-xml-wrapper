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

#ifndef __SCORE_COUNTER__
#define __SCORE_COUNTER__

#include "ml/Observer.h"
#include "ml/Singlton.h"
#include "ml/xtea/xtea.h"

NS_CC_BEGIN;

class ScoreCounter : public Singlton < ScoreCounter >
{
	friend class Singlton < ScoreCounter > ;
	typedef Observer< ScoreCounter, std::function<void( int )> > Observer_OnChangeScore;
protected:
	ScoreCounter( void );
	~ScoreCounter( void );
	virtual void onCreate();
public:
	void clear();
	void setMoney( int id, int value, bool saveValueToUserData );
	void addMoney( int id, int value, bool saveValueToUserData );
	void subMoney( int id, int value, bool saveValueToUserData );
	int getMoney( int id )const;
	Observer_OnChangeScore& observer( int id );
private:
	void change( int id, int value, bool saveValueToUserData );
protected:
	struct XteaWord
	{
		uint32_t _0;
		uint32_t _1;
	};
	std::map<int, XteaWord> m_scores;

	std::map<int, Observer_OnChangeScore> _onChangeScores;
};

NS_CC_END;
#endif