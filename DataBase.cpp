#include "DataBase.h"
#include "mlUserData.h"
#include "loadxml/xmlLoader.h"
NS_CC_BEGIN

size_t DataBase::max( const std::string& table, const std::string& row )const
{
	_checkAndLog( table, row, 0 );
	int a = id( table );
	int b = id( row );
	return _raw.at( a ).at( b ).size();
}

std::set<std::string> DataBase::getRows( const std::string& table )const
{
	std::set<std::string> result;
	if( !isExist( table ) )
		return result;
	for( auto pair : _raw.at( id( table ) ) )
	{
		auto id = pair.first;
		for( auto pairId : _ids )
		{
			if( pairId.second == id )
				result.insert( pairId.first );
		}
	}
	return result;
}

bool DataBase::isExist( const std::string& table )const
{
	if( _ids.find( table ) == _ids.end() ) return false;
	auto iTable = _raw.find( id( table ) );
	if( iTable == _raw.end() )return false;
	return true;
}

bool DataBase::isExist( const std::string& table, const std::string& row )const
{
	if( _ids.find( table ) == _ids.end() ) return false;
	if( _ids.find( row ) == _ids.end() ) return false;
	auto iTable = _raw.find( id( table ) );
	if( iTable == _raw.end() )return false;
	auto iRow = iTable->second.find( id( row ) );
	if( iRow == iTable->second.end() )return false;
	return true;
}

bool DataBase::isExist( const std::string& table, const std::string& row, unsigned index )const
{
	if( _ids.find( table ) == _ids.end() ) return false;
	if( _ids.find( row ) == _ids.end() ) return false;
	auto iTable = _raw.find( id( table ) );
	if( iTable == _raw.end() )return false;
	auto iRow = iTable->second.find( id( row ) );
	if( iRow == iTable->second.end() )return false;
	return index < iRow->second.size();
}

int DataBase::getIndex( const std::string& table, const std::string& row, const std::string& parameter )const
{
	_checkAndLog( table, row, 0 );
	auto iTable = _raw.find( id( table ) );
	auto iRow = iTable->second.find( id( row ) );
	int index( 0 );
	for( auto& value : iRow->second )
	{
		if( value == parameter )
			break;
		++index;
	}
	return index;
}

template <> bool DataBase::get( const std::string& table, const std::string& row, unsigned index )const
{
	_checkAndLog( table, row, index );
	return strTo<bool>( _raw.at( id( table ) ).at( id( row ) ).at( index ) );
}

template <> int32_t DataBase::get( const std::string& table, const std::string& row, unsigned index )const
{
	_checkAndLog( table, row, index );
	return strTo<int32_t>( _raw.at( id( table ) ).at( id( row ) ).at( index ) );
}

template <> uint32_t DataBase::get( const std::string& table, const std::string& row, unsigned index )const
{
	_checkAndLog( table, row, index );
	return strTo<uint32_t>( _raw.at( id( table ) ).at( id( row ) ).at( index ) );
}

template <> float DataBase::get( const std::string& table, const std::string& row, unsigned index )const
{
	_checkAndLog( table, row, index );
	return _base.at( id( table ) ).at( id( row ) ).at( index );
}

template <> std::string DataBase::get( const std::string& table, const std::string& row, unsigned index )const
{
	_checkAndLog( table, row, index );
	return _raw.at( id( table ) ).at( id( row ) ).at( index );
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

void DataBase::_checkAndLog( const std::string& table, const std::string& row, unsigned index )const
{
#ifdef _DEBUG
	if( isExist( table, row, index ) == false )
	{
		log( "==================================" );
		log( "table [%s] is %s", table.c_str(), isExist( table ) ? "exist" : "not exist" );
		log( "table [%s] is %s", (table + "/" + row).c_str(), isExist( table, row ) ? "exist" : "not exist" );
		log( "table [%s] is %s", (table + "/" + row + "/" + toStr( index )).c_str(), isExist( table, row, index ) ? "exist" : "not exist" );
		log( "==================================" );
	}
#endif
}

void DataBase::onCreate()
{
	xmlLoader::macros::set( "lt", "<" );
	xmlLoader::macros::set( "rt", ">" );

	auto doc = xmlLoader::getDoc( "ini/database.xml" );
	auto root = doc->root().first_child();
	for( auto xmltable = root.first_child(); xmltable; xmltable = xmltable.next_sibling() )
	{
		std::string stable = xmltable.name();
		auto table = id( stable, true );
		for( auto xmlrow = xmltable.first_child(); xmlrow; xmlrow = xmlrow.next_sibling() )
		{
			std::string srow = xmlrow.name();
			auto row = id( srow, true );
			std::string text = xmlrow.text().get();
			int k( 0 );
			while( (k = text.find( "," )) != std::string::npos )
				text[k] = '.';
			std::vector<std::string> svalues;
			char divider( ' ' );
			if( text.find( "\t" ) != std::string::npos )
				divider = '\t';
			text = xmlLoader::macros::parse( text );
			split( svalues, text, divider );

			for( auto value : svalues )
			{
				float v = strTo<float>( value );
				_raw[table][row].push_back( value );
				_base[table][row].push_back( v );
			}
		}
	}

	xmlLoader::macros::erase( "lt" );
	xmlLoader::macros::erase( "rt" );
}

NS_CC_END