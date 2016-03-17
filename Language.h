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

#ifndef __ml_Language__
#define __ml_Language__

#include "Observer.h"
#include "Singlton.h"
#include "cocos2d.h"
#include "ml/pugixml/pugixml.hpp"
#include <string>
#include <memory>
#include <map>

#ifdef WORD
#	undef WORD
#endif
#define WORD(id) Language::shared().string(id)

NS_CC_BEGIN;

class Language : public Singlton<Language>
{
	typedef std::map<std::string, std::vector<std::string> > Packs;
public:
	Observer<Language, std::function<void()> > observer;
public:
	Language();
	virtual void onCreate()override;
	std::string string( const std::string & id, int depth = 10 );
	void set( const std::string & language );
	std::string getCurrentLanguage()const;
private:
	void load();
	void loadPacks( const std::string& path );
	void loadRow( const pugi::xml_node& node );

	unsigned getLanguageIndex( const std::string& language, std::string* linkedLang );
private:
	std::map<std::string, std::string> _links;
	Packs _packs;
	std::string _current;
	size_t _currentID;
};

NS_CC_END;

#endif /* defined(__JungleDefense__Language__) */
