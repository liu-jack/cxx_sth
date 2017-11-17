#ifndef BuildingLogger_h__
#define BuildingLogger_h__
#include "def/TypeDef.h"
#include "BuildingDefine.h"
#include <boost/function.hpp>
#ifndef USE_PTR_HASH_MAP
#include "ptr_container/PtrMap.h"
#else
#include "ptr_container/PtrHashMap.h"
#endif // USE_PTR_HASH_MAP

#include "boost/scoped_ptr.hpp"


namespace pb
{
	class GxDB_Modules_Info;
	class GxDB_Buildings_Info;
	class GS2C_Building_State;
	class GxDB_Module;
	class GxDB_Building;
}

class PlayerValueOwner;
class ModuleLog;
class BuildingLog;
class Player;
class BuildingLogMgr
{
public:
	BuildingLogMgr();
	~BuildingLogMgr();
private:
#ifndef USE_PTR_HASH_MAP
	typedef PtrMap<uint32, ModuleLog> ModuleLogMap;
#else
	typedef PtrHashMap<uint32, ModuleLog> ModuleLogMap;
#endif // USE_PTR_HASH_MAP

public:
	void Init();
	void LoadModuleFrom(const pb::GxDB_Modules_Info& logInfo);
	void LoadBuildingFrom(const pb::GxDB_Buildings_Info& logInfo);
	void SaveModuInfoTo(pb::GxDB_Modules_Info& logInfo);
	void SaveBuildingTo(pb::GxDB_Buildings_Info& logInfo);
	void SaveModuInfoStateTo(uint32 moduleId,uint32 autoHammerCount, pb::GS2C_Building_State& logInfo);
	void SaveModuleInfo(uint32 moduleId,pb::GxDB_Module& moduldMsg);
	void SaveBuildingInfo(uint32 buildId,pb::GxDB_Building& buildMsg);
	uint32 GetLvUpTime(uint32 moduleId, uint32 buildId);		//�����������ʱ��
	uint32 BuildLvUp( uint32 moduleId, uint32 buildId );		//�ӽ�������
	uint32 GetBuildCurLv(uint32 moduleId, uint32 buildId );		//�ӽ�����ǰ�ȼ�
	uint32 GetBuildCurLv(uint32 buildId );						//�ӽ�����ǰ�ȼ�
	bool CanBuildingLvUp(uint32 buildId,uint32 curPlayerLv);	//�ӽ����ܷ�����
	bool CanBuildingLvUp(uint32 moduleId, uint32 buildId,uint32 curPlayerLv);//�ӽ����ܷ�����
	bool GetBuildingLvUpReq(uint32 moduleId, uint32 buildId,uint32& coinReq, uint32& woodReq);//�ӽ���������Ҫ��Դ
	void UpdateAutoLvUpTimes(PlayerValueOwner* pValue,uint32 moduleId);		//�����Զ���������
	uint32 CurLvUpBuildingCount() { return m_curLvUpBuildIds.size(); }		//��ǰ���������Ľ�������
	bool IsBuildingInLvUp(uint32 moduleId,uint32 buildId);				//��ǰ�ӽ����Ƿ���������
	bool AddLvUpBuilding(uint32 moduleId,uint32 buildId);				//���������ӽ���
	void GetLvUpBuildingIds(PlayerValueOwner* pValue, uint32 moduleId, std::vector<uint32>& buildIds,uint32 curPlayerLv);	//��ÿ��������Ľ���
	bool RemoveLvUpBuilding(uint32 moduleId,uint32 buildId);	//�Ƴ����������������б��еĽ���
	bool IsRechBuildingLvUpMaxCount();
	bool CurHasBuildingLvUp();
	void GoldHammerLvUp(boost::function<void(uint32,uint32)> f);
    uint32 GetModuleIncome(uint32 moduleId);
    uint32 GetModuleIncomeBase(uint32 moduleId);
    uint32 GetModuleIncomeLimit(uint32 moduleId);
	uint32 GetIcomeType(uint32 moduleId);
	uint32 GetCurGainBuffId( uint32 moduleId );
	bool AddCurGainBuffTime( uint32 moduleId, uint32 gainBuffId, uint32 moreTime);
	void UpdateModuleSartGainTimerTime(uint32 moduleId,uint64 time);
	void UpdateModuleGainValue(Player* player,const uint32 moduleId,const uint32 value);
	void UpdateGrowUpQuestInfo(const uint32 moduleId,Player* player,const uint32 value);
	time_t GetModuleSartGainTimerTime( uint32 moduleId);
	uint32 GetModuleGainValue( uint32 moduleId);
	void UpdateModuleRebuildState( uint32 moduleId,bool isRebuild );
	void UpdateBuildingRebuildState( uint32 moduleId, uint32 buildId, bool isRebuild );
	uint32 CurAutoLvUpModule() const { return m_curAutoLvUpBuildModule;}
	void SetCurAutoLvUpModule(uint32 moduleId);
	//zhoulunhao �ѽ��������ȼ��ﵽ
	uint32 GetUnlockBuildingCurlv(uint32 ModuleId);
	uint32 GetUnlockBuildingSize(uint32 ModuleId);
	void CheckBuildingIsUnlock(Player* player,uint32 player_lv);
	bool IsBuildingUnlock(uint32 ModuleId);
protected:
	ModuleLogMap	    m_ModuleLogMap;
	ModuleLvUpInfo		m_curLvUpBuildIds;
	uint32              m_curAutoLvUpBuildModule;
};

#endif // BuildingLogger_h__