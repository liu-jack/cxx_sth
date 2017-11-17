
/*
*Add BY:zhoulunhao 
*CREATE TIME:2017.9.27
*MAIL:zhoulunhao@hotmail.com
*/
#ifndef INCLUDE_CONTINUE_OCCUPY_CITY_LOG_H
#define INCLUDE_CONTINUE_OCCUPY_CITY_LOG_H

#include "def/TypeDef.h"
class Player;
namespace pb
{
	class GS2C_ContinueOccupyCity;
	class GxDB_ContinueOccupyCityInfo;
	class BaseContinueMsg;
}

struct LogOfContinueOccupyCity
{
	uint64 finish_time_;
	uint32 n_contin_city_;
	LogOfContinueOccupyCity()
		:finish_time_(0)
		,n_contin_city_(0){}
	LogOfContinueOccupyCity(uint64 finish_time,uint32 n_contin_city)
		:finish_time_(finish_time)
		,n_contin_city_(n_contin_city)
	{
	}
};

typedef std::map<time_t,LogOfContinueOccupyCity> LogOfContinueOccupyCityMap;//the key is a timestamp.

class ContinueOccupyCityLog
{
	LogOfContinueOccupyCityMap log_of_continue_occupy_city_;
	std::deque<std::pair<uint32,uint32> > queue_of_reward_;
public:
	ContinueOccupyCityLog();
	void ActiveConitinOccupyCity(Player& player,const time_t active_time);
	bool IsInContinueOccupyCity();
	void UpdateCityNum(Player& player,int value);
	//uint32 getLeftSeconds();
	void SaveTo(pb::GS2C_ContinueOccupyCity& msg);
	void SaveTo(pb::BaseContinueMsg& msg);
	void LoadFrom(const pb::GxDB_ContinueOccupyCityInfo& msg);
	void ResetLogMap(Player& player);
	uint32 getMaxContinueOccupyCity();
};

#endif ///INCLUDE_CONTINUE_OCCUPY_CITY_LOG_H