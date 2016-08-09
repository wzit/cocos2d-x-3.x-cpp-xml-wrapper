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

#include <cstdint>
#include "cocos2d.h"
#include "Generics.h"
#include "common.h"
//#include "json/rapidjson.h"
//#include "json/document.h"
//#include "json/stringbuffer.h"
//#include "json/writer.h"

NS_CC_BEGIN;

// from string

template <> std::string strTo(const std::string &value)
{
	return value;
}
template <> float strTo(const std::string &value)
{
	return strToFloat(value);
}
template <> int strTo(const std::string &value)
{
	return strToInt(value);
}
template <> uint32_t strTo(const std::string &value)
{
	return static_cast<uint32_t>(strToInt(value));
}
template <> uint64_t strTo(const std::string &value)
{
	return static_cast<uint64_t>(strToInt(value));
}
template <> bool strTo(const std::string &value)
{
	return strToBool(value);
}
template <> cocos2d::Point strTo(const std::string &value)
{
	return strToPoint(value);
}
template <> cocos2d::Size strTo(const std::string &value)
{
	return strToSize(value);
}
template <> cocos2d::Rect strTo(const std::string &value)
{
	return strToRect(value);
}
template <> cocos2d::Color3B strTo(const std::string &value)
{
	return strToColor3B( value );
}
template <> cocos2d::Color4B strTo(const std::string &value)
{
	return strToColor4B( value );
}
template <> cocos2d::BlendFunc strTo( const std::string &value )
{
	return strToBlendFunc( value );
}
template <> Strech strTo( const std::string &value )
{
	return strToStrech( value );
}
template <> Strech::Mode strTo( const std::string &value )
{
	return strToStrechMode( value );
}
//template <> rapidjson::Document strTo( const std::string &value )
//{
//	rapidjson::Document doc;
//	doc.Parse( rapidjson::StringRef( value.c_str() ) );
//	return doc;
//}
template <> EventKeyboard::KeyCode strTo( const std::string &value )
{
	return strToKeyCode( value );
}


// to string

template <> std::string toStr(std::string value)
{
	return value;
}
template <> std::string toStr( char const * value )
{
	return std::string( value ? value : "" );
}
template <> std::string toStr(const std::string &value)
{
	return value;
}
template <> std::string toStr(int value)
{
	return intToStr(value);
}
template <> std::string toStr(uint32_t value)
{
	return intToStr(static_cast<int>(value));
}
template <> std::string toStr(uint64_t value)
{
	return intToStr(static_cast<int>(value));
}
template <> std::string toStr(bool value)
{
	return boolToStr( value );
}
template <> std::string toStr( float value )
{
	return floatToStr( value );
}
template <> std::string toStr( cocos2d::Point value )
{
	return pointToStr( value );
}
template <> std::string toStr( cocos2d::Size value )
{
	return sizeToStr( value );
}
template <> std::string toStr( cocos2d::Rect value )
{
	return rectToStr( value );
}
template <> std::string toStr( cocos2d::BlendFunc value )
{
	return blendFuncToStr( value );
}
//template <> std::string toStr( const rapidjson::Document& doc )
//{
//	rapidjson::StringBuffer buffer;
//	rapidjson::Writer<rapidjson::StringBuffer> writer( buffer );
//	doc.Accept( writer );
//	std::string output = buffer.GetString();
//	return output;
//}

NS_CC_END;