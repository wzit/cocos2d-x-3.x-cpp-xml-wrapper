#include "DataBase.h"
#include "mlUserData.h"
#include "loadxml/xmlLoader.h"
NS_CC_BEGIN

size_t DataBase::max( const std::string& group, const std::string& param )const
{
	_checkAndLog( group, param, 0 );
	int a = id( group );
	int b = id( param );
	return _raw.at( a ).at( b ).size();
}

bool DataBase::isExist( const std::string& group )const
{
	if( _ids.find( group ) == _ids.end() ) return false;
	auto iGroup = _raw.find( id( group ) );
	if( iGroup == _raw.end() )return false;
	return true;
}

bool DataBase::isExist( const std::string& group, const std::string& param )const
{
	if( _ids.find( group ) == _ids.end() ) return false;
	if( _ids.find( param ) == _ids.end() ) return false;
	auto iGroup = _raw.find( id( group ) );
	if( iGroup == _raw.end() )return false;
	auto iParam = iGroup->second.find( id( param ) );
	if( iParam == iGroup->second.end() )return false;
	return true;
}

bool DataBase::isExist( const std::string& group, const std::string& param, unsigned index )const
{
	if( _ids.find( group ) == _ids.end() ) return false;
	if( _ids.find( param ) == _ids.end() ) return false;
	auto iGroup = _raw.find( id( group ) );
	if( iGroup == _raw.end() )return false;
	auto iParam = iGroup->second.find( id( param ) );
	if( iParam == iGroup->second.end() )return false;
	return index < iParam->second.size();
}

template <> bool DataBase::get( const std::string& group, const std::string& param, unsigned index )const
{
	_checkAndLog( group, param, index );
	return strToBool( _raw.at( id( group ) ).at( id( param ) ).at( index ) );
}

template <> int32_t DataBase::get( const std::string& group, const std::string& param, unsigned index )const
{
	_checkAndLog( group, param, index );
	return strTo<int32_t>( _raw.at( id( group ) ).at( id( param ) ).at( index ) );
}

template <> uint32_t DataBase::get( const std::string& group, const std::string& param, unsigned index )const
{
	_checkAndLog( group, param, index );
	return strTo<uint32_t>( _raw.at( id( group ) ).at( id( param ) ).at( index ) );
}

template <> float DataBase::get( const std::string& group, const std::string& param, unsigned index )const
{
	_checkAndLog( group, param, index );
	return _base.at( id( group ) ).at( id( param ) ).at( index );
}

template <> std::string DataBase::get( const std::string& group, const std::string& param, unsigned index )const
{
	_checkAndLog( group, param, index );
	return _raw.at( id( group ) ).at( id( param ) ).at( index );
}

int DataBase::id( const std::string& value, bool create )const
{
	auto iter = _ids.find( value );
	if( _ids.end() == iter )
		_ids[value] = _ids.size();
	return _ids[value];
}

int DataBase::id( const std::string& value )const
{
	return _ids.at( value );
}

void DataBase::_checkAndLog( const std::string& group, const std::string& param, unsigned index )const
{
#ifdef _DEBUG
	if( isExist( group, param, index ) == false )
	{
		log( "==================================" );
		log( "group [%s] is %s", group.c_str(), isExist( group ) ? "exist" : "not exist" );
		log( "group [%s] is %s", (group + "/" + param).c_str(), isExist( group, param ) ? "exist" : "not exist" );
		log( "group [%s] is %s", (group + "/" + param + "/" + toStr( index )).c_str(), isExist( group, param, index ) ? "exist" : "not exist" );
		log( "==================================" );
	}
#endif
}

void DataBase::onCreate()
{
	auto doc = xmlLoader::getDoc( "ini/database.xml" );
	auto root = doc->root().first_child();
	for( auto xmlgroup = root.first_child(); xmlgroup; xmlgroup = xmlgroup.next_sibling() )
	{
		std::string sgroup = xmlgroup.name();
		auto group = id( sgroup, true );
		for( auto xmlparam = xmlgroup.first_child(); xmlparam; xmlparam = xmlparam.next_sibling() )
		{
			std::string sparam = xmlparam.name();
			auto param = id( sparam, true );
			std::string text = xmlparam.text().get();
			int k( 0 );
			while( (k = text.find( "," )) != std::string::npos )
				text[k] = '.';
			std::vector<std::string> svalues;
			split( svalues, text, ' ' );
			for( auto value : svalues )
			{
				float v = strToFloat( value );
				_raw[group][param].push_back( value );
				_base[group][param].push_back( v );
			}
		}
	}
}

NS_CC_END