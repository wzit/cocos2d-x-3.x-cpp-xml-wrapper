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

#ifndef __Crypto_h__
#define __Crypto_h__
#include "macroses.h"
#include "pugixml/pugixml.hpp"

typedef uint8_t CryptoDataType;

class CryptoData : public std::vector < CryptoDataType >
{
public:
	CryptoData(){};
	CryptoData( size_t size ) :std::vector < CryptoDataType >( size ) {};
	CryptoData( const std::string& string );

	CryptoData& operator = (const std::string& string);
	operator std::string();
};

typedef std::shared_ptr<pugi::xml_document> XmlDocPointer;

CryptoData decode( const CryptoData& data );
CryptoData encode( const CryptoData& data );

CryptoData readFromFile( const std::string& pathToCryptoFile, const std::string& filename );
void appendToFile( const std::string& pathToCryptoFile, const std::string& filename, const CryptoData& data, bool append = true );

XmlDocPointer loadDoc( const std::string& datFile, const std::string& file );
void saveDoc( const std::string& datFile, const std::string& file, const XmlDocPointer& doc, bool append = true );


bool CryptoTest();

bool prepareXmls();

#endif