#ifndef __FubenMgr_h__
#define __FubenMgr_h__
#include "def/TypeDef.h"
#include <boost/unordered_map.hpp>
#include "ptr_container/PtrMap.h"
#include "Fuben.h"
#include "FubenStage.h"

typedef PtrMap<uint32, Fuben> FubenMap;
typedef PtrMap<uint32, FubenStage> StageMap;
typedef	std::map<uint32, std::vector<const FubenStage*> > FubenStageMap;

class FubenDataMgr {

    FubenMap        m_fubenMap;			//副本信息 所有
    StageMap		m_stageMap;			//关卡信息 所有
    FubenStageMap   m_FubenStageList;	//有序的关卡信息，指针  根据副本id来分组 普通关、资源关、军团关
	FubenStageMap	m_CrusadeStageList;	//有序的远征信息
	FubenStageMap	m_BranchStageList;
	FubenStageMap	m_HideStageList;
public:
    static FubenDataMgr& Instance(){ static FubenDataMgr T; return T; }	//静态单例

    void Init();
    const Fuben*      GetFuben(uint32 fubenId) const;	//获得指向副本信息的指针
    const FubenStage* GetStage(uint32 stageId) const;	//获得指向关卡信息的指针
    const Fuben&      GetFirstFuben() const;	
	const FubenMap& GetFubenMap() const;
    const FubenStage& GetFirstStage() const;
    const Fuben*      GetFubenByStageID(uint32 stageId) const;	//根据关卡id获得指向副本信息的指针
    const std::vector<const FubenStage*>* GetFubenStageList(uint32 fubenId) const;
	const std::vector<const FubenStage*> *GetCrusadeFubenStageList(uint32 fubenId) const;
	const std::vector<const FubenStage*> *GetBranchFubenStageList(uint32 fubenId) const;
	const std::vector<const FubenStage*> *GetHideFubenStageList(uint32 fubenId) const;

	const FubenStage* GetFirstCrusadeStageByFubenID(uint32 fubenID) const;//通过副本id获得当前远征第一个关卡
	const FubenStage* GetFirstCrusadeStage() const;//默认第一个远征副本
	const uint32	  GetPrestageID(uint32 stageid) const;

   // bool IsResourceStage(uint32 stageId);
	const uint32 GetBranchFubenGroupId(const uint32 stageId) const;
	bool GetNextStageID(uint32& rFubenId, uint32& rStageId) const;
	uint32 GetNextBranchStageId(uint32 FubenId,const uint32 stageId) const;
	void GetUnlockCharacter(const uint32 rFubenId,const uint32 rStageId,std::set<uint32>& unlockChar) const;
	void initCrusadeTable();
	//bool GetFubenFogReward(const uint32 num,IntPair* pair);
	const StageMap& GetStageMap() const;


	uint32 GetCurrentFubenStageNum(uint32 stageID);//当前关卡Id对应的远征副本的总观卡数，包括远征奖励
	uint32 GetCurrentCrusadeStageNum(uint32 stageId);//当前关卡Id对应的远征副本的远征关卡数
	uint32 GetCurrentCrusadeRewardStageNum(uint32 stageId);//当前关卡Id对应的远征副本的远征奖励关卡数
};
#define sFubenDataMgr FubenDataMgr::Instance()		//静态单例

#endif //__FubenMgr_h__