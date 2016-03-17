#include "mlStatistic.h"
#include "ml/services/plugins/ServicePlugin.h"
NS_CC_BEGIN





mlStatistic::mlStatistic()
{
}

mlStatistic::~mlStatistic( )
{
	onApplicationDidClosed();
}

void mlStatistic::onApplicationDidOpened()
{}

void mlStatistic::onApplicationDidClosed()
{
	for( auto event : _events )
	{
		send( event.first, event.second );
	}
	_events.clear();
}

void mlStatistic::send( const std::string& eventname, const ParamCollection& params )
{
	auto pc = params;
	pc["event"] = eventname;
	ServicePlugin::shared().sendStatistic( pc );
}

void mlStatistic::accumulate( const std::string& eventname, const ParamCollection& params, bool push )
{
	auto& value = _events[eventname];
	for( auto pair : params )
	{
		value[pair.first] = pair.second;
	}
	if( push )
	{
		send( eventname, value );
		_events.clear();
	}
}




NS_CC_END