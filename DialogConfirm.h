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

#ifndef __DialogConfirm_h__
#define __DialogConfirm_h__
#include "cocos2d.h"
#include "ml/macroses.h"
#include "ml/NodeExt.h"
#include "ml/Observer.h"
NS_CC_BEGIN

class DialogConfirm : public LayerExt
{
public:
	typedef int Result;
	static const Result Yes;
	static const Result No;
	static const Result DontAsk;
private:
	typedef std::function<bool( Result )> Function;
	typedef Observer<DialogConfirm, Function>ObserverAnswer;

	DECLARE_BUILDER( DialogConfirm );
	CREATE_3(DialogConfirm);
	bool init( const std::string & xmlfile, const std::string & nameID, bool showDontAsk );
public:
	static bool isDontAsk( const std::string& name );
	static void setDontAsk( const std::string& name );
	ObserverAnswer observerAnswer;
protected:
	virtual ccMenuCallback get_callback_by_description( const std::string & name )override;
	virtual void disappearance( bool force )override;
	void cb_select( Ref*, bool );
	void cb_dontask( Ref* );
private:
	std::string _nameId;
	bool _dontask;
};

NS_CC_END
#endif // #ifndef DialogConfirm
