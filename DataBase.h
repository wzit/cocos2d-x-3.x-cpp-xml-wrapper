#ifndef __DataBase_h__
#define __DataBase_h__
#include "cocos2d.h"
#include "macroses.h"
#include "Singlton.h"
NS_CC_BEGIN


class DataBase : public Singlton<DataBase>
{
public:
	void onCreate();
	size_t max( const std::string& group, const std::string& param )const;

	bool isExist( const std::string& table )const;
	bool isExist( const std::string& table, const std::string& row )const;
	bool isExist( const std::string& table, const std::string& row, unsigned index )const;

	int getIndex( const std::string& table, const std::string& row, const std::string& parameter )const;

	template <class TYPE>
	TYPE get( const std::string& table, const std::string& row, unsigned index = 0 )const;
protected:
	int id( const std::string& value, bool create )const;
	int id( const std::string& value )const;
	void _checkAndLog( const std::string& table, const std::string& row, unsigned index )const;
private:
	mutable std::map<std::string, int> _ids;
	std::map<int, std::map<int, std::vector<float>>> _base;
	std::map<int, std::map<int, std::vector<std::string>>> _raw;
};

NS_CC_END
#endif // #ifndef DataBase