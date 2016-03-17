#include "ml/Timer.h"
NS_CC_BEGIN





//mlTimer::mlTimer()
//{
//}
//
//mlTimer::~mlTimer( )
//{
//}

void mlTimer::log()
{
	cocos2d::log( "\n\nTimer: intervals begin:" );
	for( auto& pair : _intervals )
	{
		cocos2d::log( "\t %s: [%f], count: [%d]", pair.first.c_str(), pair.second.duration.count(), pair.second.counter );
	}
	cocos2d::log( "\n\nTimer: intervals end." );
}

void mlTimer::push( const std::string& intervalName )
{
	//auto insertPair = _intervals.insert( std::pair<std::string, Interval>( intervalName, Interval() ) );
	//auto& iter = insertPair.first->second;
	//assert( iter.active == false );
	//iter.active = true;
	//iter.counter++;
	//iter.startTime = std::chrono::system_clock::now();
}

void mlTimer::pop( const std::string& intervalName )
{
	//auto iter = _intervals.find( intervalName );
	//assert( iter != _intervals.end() );
	//assert( iter->second.active );
	//iter->second.active = false;
	//auto now = std::chrono::system_clock::now();
	//iter->second.duration += now - iter->second.startTime;
}

NS_CC_END