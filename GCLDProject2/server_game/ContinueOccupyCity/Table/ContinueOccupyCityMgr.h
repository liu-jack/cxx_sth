/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#ifndef INCLUDE_CONTINUE_OCCUPY_CITY_H
#define INCLUDE_CONTINUE_OCCUPY_CITY_H

#include "def/TypeDef.h"
#include "SimpleSingleton.h"
#include "game_data/datastruct/struct_continue_occupy_city.h"
#include "ptr_container/PtrMap.h"

class ContinueOccupyCity
{
	const DB_ContinueOccupyCity& db_;
	IntPair p_;
public:
	explicit ContinueOccupyCity(const DB_ContinueOccupyCity& db);
	uint32 getRewardCount() const;
	const IntPair& getIntpair() const; 
	uint32 getContinCityNum() const;
	float getProbality() const;
	
};
typedef PtrMap<uint32,ContinueOccupyCity> ContinueOccupyCityMap;
class ContinueOccupyCityTableMgr:public SimpleSingleton<ContinueOccupyCityTableMgr>
{
	ContinueOccupyCityMap continue_occupy_city_map_;
	std::deque<std::pair<uint32,uint32> > queue_of_reward_; 
public:
	void Init();
	float getProbalityByCityNum(const int city_num);
	std::deque<std::pair<uint32,uint32> >& getQueue();
};

#define sContinueOccupyCityTableMgr (ContinueOccupyCityTableMgr::Instance())

#endif //INCLUDE_CONTINUE_OCCUPY_CITY_H