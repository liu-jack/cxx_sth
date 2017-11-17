//////////////////////////////
//子建筑对应等级
//////////////////////////////
#ifndef BuildingLvPro_h__
#define BuildingLvPro_h__

#include "def/TypeDef.h"
struct DB_BuildingLevelUp;


class PlayerValueOwner;

class BuildingLevel
{
public:
	BuildingLevel(const DB_BuildingLevelUp& dbBuildingLv);
	~BuildingLevel();
public:    
	uint32 id() const;
	uint32 lv() const;		//等级
	uint32 honor() const;	//升级功勋奖励
	uint32 exp() const;		//升级经验奖励
	uint32 building_id() const;	//所属子建筑id
	uint32 lvUpTime() const;	//升级所需时间
	uint32 Icome() const;		//速度产出量
	uint32 IcomLimit() const;	//存储上限贡献
	uint32 coinReq() const;		//需求银币
	uint32 woodReq() const;		//需求木材
private:
	const DB_BuildingLevelUp& m_dbBuildingLvProto;

};

#endif //BuildingLvPro_h__

