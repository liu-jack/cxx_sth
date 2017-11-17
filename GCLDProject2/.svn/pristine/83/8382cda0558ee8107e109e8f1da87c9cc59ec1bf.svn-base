#ifndef BuildingLogPro_h__
#define BuildingLogPro_h__
#include "def/TypeDef.h"
namespace pb
{
	class GxDB_Building;
}

class PlayerValueOwner;
class Building;
class BuildingLog
{
public:
	BuildingLog(const Building* buildingProto);
	~BuildingLog();
public:    
	uint32 id() const;
	uint32 ModuleId() const;		//子建筑所属建筑id
	uint32 LvUpTime() const;		//升级所需时间
	void SaveTo(pb::GxDB_Building& msg) const;
	void LoadFrom(const pb::GxDB_Building& msg);
	uint32 BuildLvUp();		//子建筑升级
	uint32 CurLv() const { return m_curBuildLv;}	//当前子建筑等级
	bool  IsRechMaxLv() const;		//是否达到最大等级
	uint32 PlayerLvReq() const;		//该子建筑开放等级
	uint32 Icome() const;			//该子建筑当前等级收益
	uint32 IcomeLimit() const;		//存储上限
	uint32 SetRebuildState(bool isRebuild);	//abandon--改造
	bool LvUpReq(uint32& coinReq, uint32& woodReq) const;	//升级所需的银币与木材
	void CheckBuildingIsUnlock(uint32 player_lv);		//检查是否等级达到解锁要求
	bool IsUnlock() const;		//是否解锁
private:
	const Building* m_pBuildingProto;
	uint32 m_curBuildLv;
	time_t m_lvUpStartTime;
	bool m_isRebuild;
	bool m_isUnlock;
};

#endif //BuildingLogPro_h__

