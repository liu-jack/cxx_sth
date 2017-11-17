//////////////////////////////
//子建筑
//////////////////////////////
#ifndef BuildingPro_h__
#define BuildingPro_h__
#include "def/TypeDef.h"
#ifndef USE_PTR_HASH_MAP
#include "ptr_container/PtrMap.h"
#else
#include "ptr_container/PtrHashMap.h"
#endif // USE_PTR_HASH_MAP
struct DB_BuildingProto;
class PlayerValueOwner;
class BuildingLevel;
class Building
{
public:
	Building(const DB_BuildingProto& dbBuildingProto);
	~Building();
public:    
	uint32 id() const;	//子建筑id
	uint32 maxLv() const { return m_maxLv; }	//最大等级
	uint32 moduleId() const;	//所属大建筑
	void AddBuildingLevel(BuildingLevel* buildLv);//添加子建筑不同等级
	uint32 LvUpTime( uint32 curLv ) const;		//升级需消耗的时间
	uint32 LvReq() const;						//开放等级
	uint32 Honor(uint32 lv) const;				//升级功勋奖励
	uint32 Exp(uint32 lv) const;				//升级经验奖励
	uint32 Icome(uint32 lv) const;				//恢复速度量
	uint32 IcomeLimit(uint32 lv) const;			//存储上限
	uint32 NextRebuildId() const;				//abandon--改造顺序
	uint32 RebuildLaborNeed() const;			//abandon--改造需要的劳工数
	uint32 RebuildNeedTime() const;				//abandon--改造需要时间
	uint32 RebuildResAdd() const;				//abandon--改造资源产量加成
	bool LvUpReq(uint32 lv,uint32& coinReq, uint32& woodReq) const;	//升级需要消耗的银币与木材

private:
#ifndef USE_PTR_HASH_MAP
	typedef PtrMap<uint32, BuildingLevel> BuildingLvMap;
#else
	typedef PtrHashMap<uint32, BuildingLevel> BuildingLvMap;
#endif // USE_PTR_HASH_MAP
	const DB_BuildingProto& m_dbBuildingProto;	//子建筑属性
	BuildingLvMap  m_mapBuildingLv;		//该子建筑所有等级对应属性列表
	uint32 m_maxLv;						//最大等级
};

#endif //BuildingPro_h__

