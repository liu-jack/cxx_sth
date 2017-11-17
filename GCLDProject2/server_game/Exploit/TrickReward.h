/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#ifndef __TRICK_REWARD__H
#define __TRICK_REWARD__H

#include "common/def/TypeDef.h"

class TrickReward
{
public:
	uint32 id_;
	uint32 trick_times_;
	uint32 item_id_;
	uint32 item_num_;
	TrickReward(uint32 id,uint32 trick_times,uint32 item_id,uint32 item_num);
};
class  OccupyReward
{
public:
	uint32 id_;
	uint32 occupy_nums_;
	uint32 item_id_;
	uint32 item_num_;
	OccupyReward(uint32 id,uint32 occupy_nums,uint32 item_id,uint32 item_num);
};

struct  RankSort{
	uint8 country_id_;
	uint32 ranknum_;
	uint32 level_;
	uint32 vip_level_;
	uint32 headIconId_;
	uint64 playerid_;
	explicit RankSort(uint8 country_id,uint32 ranknum,uint32 level,uint32 vip_level,uint64 playerid,uint32 headIconId = 0);
	friend bool operator==(const RankSort& other,const RankSort& another);
	friend bool operator<(const RankSort& other,const RankSort& another);
};

typedef std::map<RankSort,string> ranksortMap;

#endif