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
#ifndef __mlStatistic_h__
#define __mlStatistic_h__
#include "cocos2d.h"
#include "ml/macroses.h"
#include "ml/Singlton.h"
#include "ml/ParamCollection.h"
NS_CC_BEGIN





class mlStatistic : public Singlton<mlStatistic>
{
public:
	mlStatistic();
	~mlStatistic();
public:
	void onApplicationDidOpened();
	void onApplicationDidClosed();

	void send( const std::string& eventname, const ParamCollection& params );
	void accumulate( const std::string& eventname, const ParamCollection& params, bool push );
protected:
private:
	std::map<std::string, ParamCollection> _events;
};




NS_CC_END
#endif // #ifndef mlStatistic