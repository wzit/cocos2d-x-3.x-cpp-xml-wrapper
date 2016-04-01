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

#include "Crypto.h"
#include "../common.h"
#include <fstream>
#if CC_TARGET_PLATFORM == CC_PLATFORM_WINRT
#	include "platform/winrt/CCWinRTUtils.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
//copy from cocos2d/cocos/platform/winrt/CCWinRTUtils.cpp
std::wstring CCUtf8ToUnicode( const char * pszUtf8Str, unsigned len = -1 )
{
	std::wstring ret;
	do
	{
		if( !pszUtf8Str ) break;
		// get UTF8 string length
		if( -1 == len )
		{
			len = strlen( pszUtf8Str );
		}
		if( len <= 0 ) break;

		// get UTF16 string length
		int wLen = MultiByteToWideChar( CP_UTF8, 0, pszUtf8Str, len, 0, 0 );
		if( 0 == wLen || 0xFFFD == wLen ) break;

		// convert string  
		wchar_t * pwszStr = new wchar_t[wLen + 1];
		if( !pwszStr ) break;
		pwszStr[wLen] = 0;
		MultiByteToWideChar( CP_UTF8, 0, pszUtf8Str, len, pwszStr, wLen + 1 );
		ret = pwszStr;
		CC_SAFE_DELETE_ARRAY( pwszStr );
	}
	while( 0 );
	return ret;
}
#endif

USING_NS_CC;

typedef int8_t TypeKey;
typedef int32_t TypeSize;
typedef CryptoDataType TypeData;

const TypeKey kCode1( 1 );
const TypeKey kCode2( 2 );
const TypeKey kCode3( 3 );
const TypeKey kCode4( 4 );
const TypeKey kCodeMax( 1 );

TypeData encode0( TypeData c ) 
{ 
	assert( c < 128 ); 
	return c; 
}
TypeData encode1( TypeData c ) 
{
	assert( c < 128 ); 
	return c * 2 + 1;
}
TypeData encode2( TypeData c ) 
{ 
	assert( c < 128 ); 
	return c * 2 + 2;
}
TypeData encode3( TypeData c )
{
	assert( c < 128 ); 
	return c * 2 - 1;
}
TypeData encode4( TypeData c )
{
	assert( c < 128 ); 
	return c * 2 - 2; 
}

TypeData decode0( TypeData c ) { return c; }
TypeData decode1( TypeData c ) { return (c - 1) / 2; }
TypeData decode2( TypeData c ) { return (c - 2) / 2; }
TypeData decode3( TypeData c ) { return (c + 1) / 2; }
TypeData decode4( TypeData c ) { return (c + 2) / 2; }

std::string toStr( const CryptoData& data )
{
	std::string string;
	string.resize( data.size() );
	char* raw = (char*)malloc( data.size() + 1 );
	memcpy( raw, data.data(), data.size() );
	raw[data.size()] = 0x0;
	string = raw;
	free( raw );
	return string;
}

CryptoData toData( const std::string& string )
{
	CryptoData data( string.size() );
	memcpy( data.data(), string.data(), string.size() );
	return data;
}

CryptoData& CryptoData::operator = (const std::string & string)
{
	*this = toData( string );
	return *this;
}

CryptoData::operator std::string()
{
	return toStr( *this );
}

CryptoData::CryptoData( const std::string & string )
{
	*this = toData( string );
}

TypeData encode( TypeKey key, TypeData c )
{
	//return encode0( c );
	if( key == kCode1 ) return encode1( c );
	if( key == kCode2 ) return encode2( c );
	if( key == kCode3 ) return encode3( c );
	if( key == kCode4 ) return encode4( c );
	return c;
}

TypeData decode( TypeKey key, TypeData c )
{
	//return decode0( c );
	if( key == kCode1 ) return decode1( c );
	if( key == kCode2 ) return decode2( c );
	if( key == kCode3 ) return decode3( c );
	if( key == kCode4 ) return decode4( c );
	return c;
}

TypeKey generateKey()
{
	return kCode1 + rand() % kCodeMax;
}

/*
Format:
**  header  **
1: Crypto key
2: Data size (without header
**  Body  **
3: Data
*/
CryptoData decode( const CryptoData & data )
{
	CryptoData crypt;
	if( data.size() >= sizeof( TypeKey ) + sizeof( TypeSize ) )
	{
		TypeKey key;
		TypeSize size;
		auto * raw = data.data();
		memcpy( &key, raw, sizeof( TypeKey ) ); raw += sizeof( TypeKey );
		memcpy( &size, raw, sizeof( TypeSize ) ); raw += sizeof( TypeSize );
		assert( size <= static_cast<TypeSize>(data.size()) );
		crypt.resize( size );
		auto cryptraw = crypt.data();
		for( TypeSize i = 0; i < size; ++i )
		{
			cryptraw[i] = decode( key, raw[i] );
		}
	}
	return crypt;
}

CryptoData encode( const CryptoData & data )
{
	TypeKey key = generateKey();
	TypeSize size = data.size();
	CryptoData result;

	if( data.size() >= sizeof( TypeKey ) + sizeof( TypeSize ) )
	{
		auto * raw = data.data();

		std::vector<TypeData> crypt;
		crypt.resize( sizeof( TypeKey ) + sizeof( TypeSize ) + size );
		int8_t* cryptraw = (int8_t*)crypt.data();

		memcpy( cryptraw, &key, sizeof( TypeKey ) ); cryptraw += sizeof( TypeKey );
		memcpy( cryptraw, &size, sizeof( TypeSize ) ); cryptraw += sizeof( TypeSize );
		for( TypeSize i = 0; i < size; ++i )
		{
			cryptraw[i] = encode( key, raw[i] );
		}
		result.resize( crypt.size() );
		memcpy( result.data(), crypt.data(), crypt.size() );
	}
	return result;
}

//CryptoData decode( const std::string & string )
//{
//	CryptoData data = toData( string );
//	data = decode( data );
//	return data;
//}
//
//CryptoData encode( const std::string & string )
//{
//	CryptoData data = toData( string );
//	data = encode( data );
//	return data;
//}

bool readChunk( CryptoData & chunk, unsigned char ** data )
{
	bool result( true );
	TypeSize size( 0 );

	memcpy( &size, *data, sizeof(TypeSize) );
	*data += sizeof( TypeSize );
	chunk.resize( size );
	memcpy( chunk.data(), *data, sizeof( CryptoDataType ) * size );
	*data += sizeof( CryptoDataType ) * size;
	return result;
}

bool skipChunk( unsigned char ** data )
{
	bool result( true );
	TypeSize size( 0 );
	memcpy( &size, *data, sizeof( TypeSize ) );
	*data += sizeof( TypeSize );
	*data += sizeof( CryptoDataType ) * size;
	return result;
}

bool writeChunk( const CryptoData & chunk, unsigned char ** data )
{
	bool result( true );
	TypeSize size = chunk.size();
	memcpy( *data, &size, sizeof( TypeSize ) );
	*data += sizeof( TypeSize );
	memcpy( *data, chunk.data(), sizeof( CryptoDataType ) * size );
	*data += sizeof( CryptoDataType ) * size;
	return result;
}

CryptoData readFromFile( const std::string & pathToCryptoFile, const std::string & afilename )
{
	CryptoData data;

	Data filedata;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	filedata = FileUtils::getInstance()->getDataFromFile( pathToCryptoFile );
#endif

	if( filedata.getBytes() == nullptr )
	{
#	if CC_TARGET_PLATFORM == CC_PLATFORM_WINRT || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
		auto wide = CCUtf8ToUnicode(pathToCryptoFile.c_str());
		std::fstream file( wide, std::fstream::binary | std::fstream::in );
#	else
		std::fstream file( pathToCryptoFile, std::fstream::binary | std::fstream::in );
#	endif

		if( !file.is_open() )
			return data;

		file.seekg( 0, file.end );
		int size = file.tellg();
		file.seekg( 0, file.beg );

		char *raw = new char[size];
		file.read( raw, size );
		filedata.fastSet( (unsigned char*)(raw), size );
	}

	if( filedata.getSize() > 0 )
	{
		auto size = filedata.getSize();
		auto begin = filedata.getBytes();
		auto curr = filedata.getBytes();

		while( (curr-begin) < size )
		{
			CryptoData filename;

			if( !readChunk( filename, &curr ) )
				break;
			filename = decode( filename );
			std::string fn = toStr( filename );

			if( afilename == fn )
			{
				readChunk( data, &curr );
				data = decode( data );
				break;
			}
			else
			{
				skipChunk( &curr );
			}
		}
	}
	return data;
}

void appendToFile( const std::string & pathToCryptoFile, const std::string & afilename, const CryptoData & adata, bool append )
{
	std::string fullpathToDat = cocos2d::FileUtils::getInstance()->fullPathForFilename( pathToCryptoFile );
	if( fullpathToDat.empty() ) fullpathToDat = pathToCryptoFile;

	auto mode = std::fstream::binary | std::fstream::out | (append ? std::fstream::ate : std::fstream::trunc);
#if CC_TARGET_PLATFORM == CC_PLATFORM_WINRT || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	auto wide = CCUtf8ToUnicode( pathToCryptoFile.c_str() );
	std::fstream file( wide, mode );
#else
	std::fstream file( pathToCryptoFile, mode );
#endif

	if( !file.is_open() )
		return;

	CryptoData filename;
	CryptoData data;
	data = encode( adata );
	filename = encode( afilename );

	std::vector<unsigned char> rawdata;
	size_t size( 0 );
	size += filename.size() + sizeof( TypeSize );
	size += data.size() + sizeof( TypeSize );

	rawdata.resize( size );
	unsigned char* ucdata = rawdata.data();

	writeChunk( filename, &ucdata );
	writeChunk( data, &ucdata );

	file.write( (const char*)rawdata.data(), rawdata.size());
}

XmlDocPointer loadDoc( const std::string& datFile_, const std::string& file )
{
	auto doc = std::make_shared<pugi::xml_document>();

	std::string datFile;
	if( datFile_.empty() == false )
		datFile = datFile_;
	else if( file.find( "ini/" ) == 0 )
		datFile = "dat/" + file.substr( 4, file.size() - 8 ) + ".dat";

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	if( FileUtils::getInstance()->isFileExist( file ) )
	{
		bool rewrite( true );
		if( FileUtils::getInstance()->isFileExist( datFile ) )
		{
			FILETIME xmlCreate, xmlAccess, xmlWrite;
			FILETIME datCreate, datAccess, datWrite;
			HANDLE xmlHandle = CreateFileA( FileUtils::getInstance()->fullPathForFilename( file ).c_str(), 0x00, 0x00, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
			HANDLE datHandle = CreateFileA( FileUtils::getInstance()->fullPathForFilename( datFile ).c_str(), 0x00, 0x00, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
			GetFileTime( xmlHandle, &xmlCreate, &xmlAccess, &xmlWrite );
			GetFileTime( datHandle, &datCreate, &datAccess, &datWrite );
			CloseHandle( xmlHandle );
			CloseHandle( datHandle );
	
			bool cmp = xmlWrite.dwHighDateTime > datWrite.dwHighDateTime;
	
			rewrite = cmp;
		}
		else
		{
			std::string fullpath = FileUtils::getInstance()->fullPathForFilename( file );
			rewrite = rewrite && fullpath.find( "Resources/ini" ) != std::string::npos;
		}
		if( rewrite )
		{
			auto iniXml = std::make_shared<pugi::xml_document>();
			iniXml->load_file( file.c_str() );
			std::string dir = datFile.substr( 0, datFile.find_last_of( "/" ) );
			if( FileUtils::getInstance()->isDirectoryExist( "dat" ) )
			{
				FileUtils::getInstance()->createDirectory( dir );
				saveDoc( datFile, file, iniXml, false );
			}
		}
	}
#endif

	std::string data = readFromFile( datFile, file );
	if( data.empty() || doc->load( data.c_str() ).status != pugi::status_ok )
	{
		doc->load_file( file.c_str() );
		//log( "%s", data.c_str() );
	}
	return doc;
}

void saveDoc( const std::string& datFile, const std::string& file, const XmlDocPointer& doc, bool append )
{
#ifdef _DEBUG
	unsigned int flag = pugi::format_no_declaration;
#else
	unsigned int flag = pugi::format_raw | pugi::format_no_escapes | pugi::format_no_declaration;
#endif
	if( datFile.empty() == false )
	{
		std::stringstream ss;
		doc->save( ss, "", flag );
		appendToFile( datFile, file, ss.str(), append );
	}
	else
	{
		doc->save_file( file.c_str(), "", flag );
	}
}

bool test1()
{
	std::string string;
	string.resize( 100 + rand() % 100 );
	for( auto& c : string )
		c = rand() % 64 + 32;
	std::vector<unsigned char> matrix( string.size() );
	memcpy( matrix.data(), string.data(), string.size() );

	CryptoData source( matrix.size() );
	memcpy( source.data(), matrix.data(), matrix.size() );
	//source.copy( (unsigned char*)matrix.data(), matrix.size() );
	CryptoData crypt = encode( source );
	CryptoData decrypt = decode( crypt );

	matrix.resize( decrypt.size() );
	memcpy( matrix.data(), decrypt.data(), decrypt.size() );

	bool identity( true );
	identity = identity && 0 == memcmp( matrix.data(), string.data(), matrix.size() );
	identity = identity && 0 == memcmp( matrix.data(), string.data(), string.size() );
	return identity;
}

bool test2()
{
	std::string string;
	string.resize( 100 + rand() % 100 );
	for( auto& c : string ) c = rand() % 64 + 32;

	CryptoData d = toData( string );
	std::string s1 = toStr( d );
	bool result = s1 == string;
	return result;
}

bool test3()
{
	std::string string;
	string.resize( 100 + rand() % 100 );
	for( auto& c : string ) c = rand() % 64 + 32;

	CryptoData data1 = encode( string );
	CryptoData data2 = decode( data1 );
	std::string string1 = toStr( data2 );

	bool result = string == string1;
	return result;
}

bool test4()
{
	auto genStr = []()
	{
		std::string string;
		string.resize( 1000 + rand() % 1000 );
		for( auto& c : string ) c = rand() % 64 + 32;
		return string;
	};

	std::string s1 = genStr();
	std::string s2 = genStr();
	std::string s3 = genStr();

	std::string f1 = "filename1";
	std::string f2 = "filename2";
	std::string f3 = "filename3";

	std::string file = "test.dat";

	remove( file.c_str() );
	appendToFile( file, f1, toData( s1 ) );
	appendToFile( file, f2, toData( s2 ) );
	appendToFile( file, f3, toData( s3 ) );

	auto d1 = readFromFile( file, f1 );
	auto d2 = readFromFile( file, f2 );
	auto d3 = readFromFile( file, f3 );

	bool result( true );
	result = result && toStr( d1 ) == s1;
	result = result && toStr( d2 ) == s2;
	result = result && toStr( d3 ) == s3;
	return result;
}

bool test5()
{
	auto genStr = []()
	{
		std::string string;
		string.resize( 1000 + rand() % 1000 );
		for( auto& c : string ) c = rand() % 64 + 32;
		return string;
	};

	std::string s1 = genStr();
	std::string s2 = genStr();
	std::string s3 = genStr();

	std::string f1 = "filename1";
	std::string f2 = "filename2";
	std::string f3 = "filename3";

	std::string file = "test.dat";

	appendToFile( file, f1, s1 );
	appendToFile( file, f2, s2 );
	appendToFile( file, f3, s3 );

	std::string d1 = readFromFile( file, f1 );
	std::string d2 = readFromFile( file, f2 );
	std::string d3 = readFromFile( file, f3 );

	bool result( true );
	result = result && d1 == s1;
	result = result && d2 == s2;
	result = result && d3 == s3;
	return result;
}

bool testXml()
{
	std::string file( "test.dat" );
	remove( file.c_str() );

	int value( 123 );
	pugi::xml_document doc1;
	doc1.root().append_child( "test" ).append_child( "test" ).append_attribute( "t" ).set_value( value );
	std::stringstream ss;
	doc1.save( ss );
	
	appendToFile( file, "ini/f1.xml", ss.str() );
	appendToFile( file, "ini/f2.xml", ss.str() );

	pugi::xml_document doc2; doc2.load( ((std::string)readFromFile( file, "ini/f1.xml" )).c_str() );
	pugi::xml_document doc3; doc3.load( ((std::string)readFromFile( file, "ini/f2.xml" )).c_str() );

	bool result( true );
	result = result && doc2.root().child( "test" ).child( "test" ).attribute( "t" ).as_int() == value;
	result = result && doc3.root().child( "test" ).child( "test" ).attribute( "t" ).as_int() == value;
	return result;
};

bool test6()
{
	remove( "C:/Users/Volodar/Documents/map.dat" );
	auto doc = std::make_shared<pugi::xml_document>();
	doc->load_file( "ini/mapscene/flag.xml" );
	saveDoc( "C:/Users/Volodar/Documents/map.dat", "ini/mapscene/flag.xml", doc );
	loadDoc( "C:/Users/Volodar/Documents/map.dat", "ini/mapscene/flag.xml" );

	return true;
}

void perfomance()
{
	auto doc = std::make_shared<pugi::xml_document>();
	doc->load_file( "C:/~MSVS/GreeceDefense/Debug.win32/xmlfile_pugi.xml" );
	saveDoc( "C:/~MSVS/GreeceDefense/Debug.win32/xmlfile_pugi.dattest", "C:/~MSVS/GreeceDefense/Debug.win32/xmlfile_pugi.xml", doc, false );
	doc.reset();

	while( true )
	{
		auto p = loadDoc( "C:/~MSVS/GreeceDefense/Debug.win32/xmlfile_pugi.dattest", "C:/~MSVS/GreeceDefense/Debug.win32/xmlfile_pugi.xml" );
		saveDoc( "C:/~MSVS/GreeceDefense/Debug.win32/xmlfile_pugi.dattest", "C:/~MSVS/GreeceDefense/Debug.win32/xmlfile_pugi.xml", p, false );
	}
}

bool CryptoTest()
{
	//perfomance();
	bool result( true );
	//result = result && test6();		if( !result ) cocos2d::MessageBox( "test6", "CryptoTest" );
	//result = result && test1();		if( !result ) cocos2d::MessageBox( "test1", "CryptoTest" );
	//result = result && test2();		if( !result ) cocos2d::MessageBox( "test2", "CryptoTest" );
	//result = result && test3();		if( !result ) cocos2d::MessageBox( "test3", "CryptoTest" );
	//result = result && test4();		if( !result ) cocos2d::MessageBox( "test4", "CryptoTest" );
	//result = result && test5();		if( !result ) cocos2d::MessageBox( "test5", "CryptoTest" );
	//result = result && testXml();	if( !result ) cocos2d::MessageBox( "testXml", "CryptoTest" );
	return result;
}

bool prepareXmls()
{
	return true;
};
