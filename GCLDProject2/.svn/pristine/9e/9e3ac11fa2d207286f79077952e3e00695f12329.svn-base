#ifndef ModuleLogPro_h__
#define ModuleLogPro_h__
#include "def/TypeDef.h"
#include "BuildingDefine.h"
#ifndef USE_PTR_HASH_MAP
#include "ptr_container/PtrMap.h"
#else
#include "ptr_container/PtrHashMap.h"
#endif // USE_PTR_HASH_MAP
class Module;
class BuildingLog;
namespace pb
{
	class GxDB_Module;
	class GS2C_Building_State;
	class GxDB_Building;
	class GxDB_Buildings_Info;
}
#ifndef USE_PTR_HASH_MAP
typedef PtrMap<uint32, BuildingLog> BuildingLogMap;
#else
typedef PtrHashMap<uint32, BuildingLog> BuildingLogMap;
#endif // USE_PTR_HASH_MAP
class ModuleLog
{
public:
	ModuleLog(const Module* moduleProto);
	~ModuleLog();
	uint32 id() const;
	void SaveTo(pb::GxDB_Module& msg) const;
	void SaveBuildInfoTo(uint32 buildId, pb::GxDB_Building& msg ) const;
	void SaveBuildInfoTo(pb::GxDB_Buildings_Info& msg );
	void SaveToState(pb::GS2C_Building_State& msg);
	void LoadFrom(const pb::GxDB_Module& msg);
	void AddBuildingLog(BuildingLog* buildingLog);		//添加子建筑逻辑
	BuildingLog* GetBuildingLog(uint32 buildindId);		//获得子建筑逻辑
	uint32 GetLvUpTime(uint32 BuildingId);				//获得子建筑升级所需时间
	uint32 BuildLvUp(uint32 buildId);					//子建筑升级
	void CheckAutoLvUpBuildings();						//检测可以升级的子建筑
	bool CanBuildingLvUp(uint32 buildId,uint32 curPlayerLv);	//当前子建筑可以升级
	uint32 TimeType() const;							//恢复资源的时间类型，分钟/小时
	uint32 IncomeType() const;							//收益资源类型
	uint32 Icome() const;								//大建筑实际收益资源速度
	uint32 IcomeBase() const;							//大建筑基础收益速度
	uint32 IcomeLimit() const;							//大建筑资源存储量
	uint32 CurGainBuffId() const;						//收益buffid
	void   SetSartGainTimerTime(time_t time);			//设置开始收益的起始时间
	time_t GetSartGainTimerTime();						//获得……
	void   SetGainValue(uint32 value);					//设置收益值
	uint32 GetGainValue();								//获得……
	void SetRebuildState( bool isRebuild );				//abandon
	void ResetExpireTime();
	uint32 GetBuildCurLv(uint32 buildindId);			//获得子建筑当前等级
	bool   AddExpireTime(uint32 buffid, uint32 expTime);
	void GetMinLevelUpBuildIds(std::vector<uint32>& vec,uint32 moduleId,uint32 limitTimes,uint32 curPlayerLv, ModuleLvUpInfo& curLvUpIds);
	bool GetBuildingLvUpReq(uint32 buildId,uint32& coinReq, uint32& woodReq);
	void CheckBuildingIsUnlock(uint32 player_lv);
	const BuildingLogMap& GetBuildingLogMap() const;
	bool IsBuildingUnlock();
private:
	const Module*       m_pModuleProto;
	BuildingLogMap     m_BuildingLogMap;
	uint32 m_curGainMode;
	time_t m_expireTime;
	bool   m_isRebuild;
	time_t m_starGainTime;
	uint32 m_gainValue;
};
#endif //ModuleLogPro_h__