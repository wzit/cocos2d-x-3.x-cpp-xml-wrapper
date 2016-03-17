#ifndef __Timer_h__
#define __Timer_h__
#include "cocos2d.h"
#include "macroses.h"
NS_CC_BEGIN





class mlTimer
{
public:
	struct SmartInterval
	{
		SmartInterval( mlTimer& timer, const std::string& intervalName )
			: _timer( timer )
			, _interval( intervalName )
		{
			_timer.push( intervalName );
		}
		~SmartInterval()
		{
			_timer.pop( _interval );
		}
		mlTimer& _timer;
		std::string _interval;
	};
public:
	void log();
	void push( const std::string& intervalName );
	void pop( const std::string& intervalName );
protected:
private:
	struct Interval
	{
		Interval() : active( false ), counter(0) {}
		std::chrono::time_point < std::chrono::system_clock > startTime;
		std::chrono::duration<double> duration;
		bool active;
		int counter;
	};
	std::map<std::string, Interval> _intervals;
};




NS_CC_END
#endif // #ifndef Timer