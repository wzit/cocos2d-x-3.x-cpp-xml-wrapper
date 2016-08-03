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

#ifndef __Common_h__
#define __Common_h__
#include "cocos2d.h"
#include "stdio.h"
#include "Generics.h"

#define FOR_EACHXML(_node, _child) for( auto _child = _node.first_child(); _child; _child = _child.next_sibling() )
#define FOR_EACHXML_BYTAG(_node, _child, _tag) for( auto _child = _node.child(_tag); _child; _child = _child.next_sibling(_tag) ) 

#define sizearray(arr) sizeof(route) / sizeof ( route[0] )


class TimeCounter
{
public:
	TimeCounter() :m_counter( 0 ), m_timer( 0 ) {}
	void set( float time ) { m_timer = time; reset(); }
	void reset() { m_counter = m_timer; }
	void tick( float elapsetime ) { m_counter -= elapsetime; }
	float time()const { return m_counter; }
	float duration()const { return m_timer; }
	operator bool() const { return m_counter <= 0; }

private:
	operator float() const { return m_timer; }
private:
	float m_counter;
	float m_timer;
};

void log_once( const char * format, ... );

float getAngle( const cocos2d::Point & a, const cocos2d::Point & b );

cocos2d::Point getVectorByDirection( float direction );
float getDirectionByVector( const cocos2d::Point & radius );
float lerpDegrees( float start, float end, float amount );

float getDistance( const cocos2d::Point & point, const cocos2d::Point & A, const cocos2d::Point & B );

float distanse_pointToLineSegment(
	const cocos2d::Point & segmentA,
	const cocos2d::Point & segmentB,
	const cocos2d::Point & point );


cocos2d::FiniteTimeAction * createRouteAction( const std::vector<cocos2d::Point> route, float objectSpeed );

bool checkPointInNode( const cocos2d::Node * node, const cocos2d::Point & pointInParentSpace, int depth = -1 );

template <class STLcont>
std::string unite( const STLcont & cont, const char delimiter = ',' )
{
	std::string result;
	for( const auto& value : cont )
	{
		result += value;
		result += delimiter;
	}
	if( result.empty() == false )
		result.pop_back();
	return result;
}

void split( std::list<std::string> & out, const std::string & values, const char delimiter = ',' );
void split( std::vector<std::string> & out, const std::string & values, const char delimiter = ',' );

template <typename T>
void split_t( std::vector<T> & out, const std::string & values, const char delimiter = ',' )
{
	std::vector<std::string> ss;
	split( ss, values, delimiter );
	for( auto& s : ss )
		out.push_back( cocos2d::strTo<T>( s ) );
}

bool strToBool( const std::string &value );
std::string boolToStr( bool value );

int strToInt( const std::string &value );
std::string intToStr( int value );

float strToFloat( const std::string &value );
std::string floatToStr( float value );
std::string floatToStr2( float value );

cocos2d::Point strToPoint( const std::string & value );
const std::string pointToStr( const cocos2d::Point & point );

cocos2d::Size strToSize( const std::string & value );
const std::string sizeToStr( const cocos2d::Size & size );

cocos2d::Rect strToRect( const std::string & value );
const std::string rectToStr( const cocos2d::Rect & rect );

cocos2d::Color3B strToColor3B( const std::string & value );
cocos2d::Color4B strToColor4B( const std::string & value );

cocos2d::BlendFunc strToBlendFunc( const std::string & value );
std::string blendFuncToStr( const cocos2d::BlendFunc & blendFunc );

cocos2d::EventKeyboard::KeyCode strToKeyCode( const std::string & value );


struct Strech
{
	Strech();
	enum class Mode
	{
		min_scale,
		max_scale,
		only_x,
		only_y,
		both_xy,
		unknow,
	};
	Mode mode;
	cocos2d::Size boundingSize;
	float maxScaleX;
	float maxScaleY;
	float minScaleX;
	float minScaleY;

	bool empty()const;
};
Strech::Mode strToStrechMode( const std::string& mode );
Strech strToStrech( const std::string& string );

void strechNode( cocos2d::Node*node, const Strech& strech );

void fileLog( const std::string & s );

cocos2d::Point getRandPointInPlace( const cocos2d::Point & center, float radius );

std::string m_image_directory( const std::string & source, int posFromEnd, unsigned randomDiapason );
void computePointsByRadius( std::vector<cocos2d::Point> & out, float radius, unsigned countPoints, float startAngleInDegree = 0 );

cocos2d::Node * getNodeByTagsPath( cocos2d::Node * root, const std::list<int> & tagspath );
cocos2d::Node * getNodeByPath( cocos2d::Node * root, const std::string & path_names );

bool isFileExist( const std::string& path );

template <class T>
T * getNodeByTagsPath( cocos2d::Node * root, const std::list<int> & tagspath )
{
	auto node = getNodeByTagsPath( root, tagspath );
	return dynamic_cast<T*>(node);
}

template <class T>
T * getNodeByPath( cocos2d::Node * root, const std::string & path_names )
{
	auto node = getNodeByPath( root, path_names );
	return dynamic_cast<T*>(node);
}

void openUrl( const std::string & url );
void openStoreUrl();
float calculate( const std::string& expression, const std::map<std::string, float>& constants );

class ActionEnable : public cocos2d::ActionInstant
{
public:
	static ActionEnable * create();

	virtual void update( float time ) override;
	virtual ActionInstant* reverse() const override;
	virtual ActionEnable* clone() const override;

CC_CONSTRUCTOR_ACCESS:
	ActionEnable() {}
	virtual ~ActionEnable() {}
private:
	CC_DISALLOW_COPY_AND_ASSIGN( ActionEnable );
};

class ActionDisable : public cocos2d::ActionInstant
{
public:
	static ActionDisable * create();

	virtual void update( float time ) override;
	virtual ActionInstant* reverse() const override;
	virtual ActionDisable* clone() const override;

CC_CONSTRUCTOR_ACCESS:
	ActionDisable() {}
	virtual ~ActionDisable() {}
private:
	CC_DISALLOW_COPY_AND_ASSIGN( ActionDisable );
};
#endif
