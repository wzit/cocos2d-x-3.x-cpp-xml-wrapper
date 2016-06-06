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

#include "cocos2d.h"
#include "Language.h"
#include "Observer.h"
#include "loadxml/xmlLoader.h"
#include "mlUserData.h"

NS_CC_BEGIN;

static bool Language_isLIfe( false );
bool Language::isLife()
{
	return Language_isLIfe;
}

Language::Language()
	: _currentID( -1 )
{
	Language_isLIfe = true;
};

Language::~Language()
{
	Language_isLIfe = false;
};

void Language::onCreate()
{
	_links.clear();
	_packs.clear();
	_current.clear();
	_currentID = -1;
	load();
}

std::string Language::string( const std::string & id, int depth )
{
	if( depth <= 0 )
		return id;
	if( _currentID == -1 )
		return id;
	std::string temp = id;
	std::string result;
	while( true )
	{
		size_t k0 = temp.find( "#" );
		size_t k1 = k0 != std::string::npos ? temp.substr( k0 + 1 ).find( "#" ) : std::string::npos;
		if( k0 != std::string::npos && k1 != std::string::npos )
		{
			std::string word = temp.substr( k0+1, k1 );
			word = this->string(word, depth - 1 );
			result += temp.substr(0,k0) + word;
			temp = temp.substr( k0+k1+2 );
		}
		else
		{
			if( _currentID != -1 )
			{
				auto it = _packs.find( id );
				if( it != _packs.end() )
				{
					result = xmlLoader::macros::parse( it->second.at( _currentID ) );
					result = string( result, depth - 1 );
				}
				else
				{
					result += temp;
				}
			}
			break;
		}
	};
	return result;
}

void Language::set( const std::string & lang )
{
	if( _packs.empty() )
		return;
	std::string language;
	unsigned index = getLanguageIndex( lang, &language );

	if( index != -1 )
	{
		assert( mlUserData::getInstance() );
		mlUserData::getInstance()->lang_set( language );
		xmlLoader::macros::set( "LOCALE", language );
		xmlLoader::macros::set( "LOCALE_DIR", language + "/" );
		_current = language;
		_currentID = index;
		observer.pushevent();
	}
	else
	{
		set( "en" );
	}
}

std::string Language::getCurrentLanguage()const
{
	return _current;
}

void Language::load()
{
	loadPacks( "lang/languages.xml" );
	loadPacks( "lang/languages_ext.xml" );

	pugi::xml_document doc;
	doc.load_file( "lang/lang.xml" );
	auto root = doc.root().first_child();
	std::string def = root.attribute( "default" ).as_string( "en" );
	std::string force = root.attribute( "force" ).as_string();

	for( auto node = root.first_child(); node; node = node.next_sibling() )
	{
		auto id = node.first_attribute().name();
		auto lk = node.first_attribute().as_string();
		_links.emplace( id, lk );
	}

	assert( mlUserData::getInstance() );
	std::string userlang = mlUserData::getInstance()->lang_get();
	if( userlang.empty() )
	{
		bool useSystemLang( true );
		if( getLanguageIndex( force, nullptr ) != -1 )
		{
			useSystemLang = false;
			set( force );
		}

		if( useSystemLang )
		{
			auto systemlang = Application::getInstance()->getCurrentLanguage();
			switch( systemlang )
			{
				case LanguageType::ENGLISH: set( "en" ); break;
				case LanguageType::RUSSIAN: set( "ru" ); break;
				case LanguageType::GERMAN: set( "de" ); break;

				case LanguageType::CHINESE: set( "ch" ); break;
				case LanguageType::FRENCH: set( "fr" ); break;
				case LanguageType::ITALIAN: set( "it" ); break;
				case LanguageType::SPANISH: set( "es" ); break;
				case LanguageType::DUTCH: set( "nl" ); break;
				case LanguageType::KOREAN: set( "ko" ); break;
				case LanguageType::JAPANESE: set( "ja" ); break;
				case LanguageType::HUNGARIAN: set( "hu" ); break;
				case LanguageType::PORTUGUESE: set( "pt" ); break;
				case LanguageType::NORWEGIAN: set( "no" ); break;
				case LanguageType::POLISH: set( "pl" ); break;
				case LanguageType::TURKISH: set( "tr" ); break;
				case LanguageType::UKRAINIAN: set( "uk" ); break;

				default: set( def );
			}
		}
	}
	else
	{
		set( userlang ); 
	}
}

void Language::loadPacks( const std::string& path )
{
	pugi::xml_document doc;
	doc.load_file( path.c_str() );
	auto root = doc.root().first_child();
	if( !root )
		log( "WARNING!!! Language wasnt loaded. Cannot open laguages path [%s]", path.c_str() );

	for( auto node = root.first_child(); node; node = node.next_sibling() )
	{
		loadRow( node );
	}
}

void Language::loadRow( const pugi::xml_node& node )
{
	auto xmlID = node.first_child();
	std::string id = xmlID.text().as_string();
	auto it = _packs.emplace( id, std::vector<std::string>() );
	auto iter = it.first;
	iter->second.clear();
	for( auto xmlValue = xmlID.next_sibling(); xmlValue; xmlValue = xmlValue.next_sibling() )
	{
		std::string value = xmlValue.text().as_string();
		iter->second.push_back( value );
	}
}

unsigned Language::getLanguageIndex( const std::string& lang, std::string* linkedLang )
{
	const auto iWord = _packs.find( "id" );
	if( iWord == _packs.end() )
		return -1;

	auto language = lang;
	if( _links.find( language ) != _links.end() )
		language = _links.find( language )->second;

	bool finded(false);
	unsigned index( 0 );
	for( ; index < iWord->second.size(); ++index )
	{
		std::string id = iWord->second.at( index );
		if( id == language )
		{
			finded = true;
			break;
		}
	}

	if( finded && linkedLang )
		*linkedLang = language;
	return finded ? index : -1;
}


NS_CC_END