#ifndef __FubenStage_h__
#define __FubenStage_h__

#include "def/TypeDef.h"
struct DB_FubenStage;

namespace pb
{
    class C2S_Stage_KillSoldier;
    class C2S_Pass_Stage;
    class C2S_UnlockHero;
}
enum EType {
	COMMON_FUBEN,     //普通 0
	RESOURCE_FUBEN,   //资源 1
	GROUP_FUBEN,      //军团 2
	FOG_FUBEN,        //迷雾 3
	CRUSADE_FUBEN,	//远征 4
	CRUSADE_GIFTS_FUBEN,	//远征奖励 5
	BRANCH_FUBEN,		//支线
	HIDE_FUBEN,			//隐藏
};
class FubenStage {
    const DB_FubenStage& m_dbFubenStage;
public:
    IntPairVec  m_npcLst;

    
    FubenStage(const DB_FubenStage& db);
	uint32 Id() const;
    int32 LandForm() const;
    uint32 ReqLv() const;
    uint32 FubenId() const;
    uint32 UnlockeHeroId() const;
	void GetUnlockeHeroId(std::set< uint32>&heroIdSet) const;
	uint32 PreStage() const;
    void GetCost(IntPair& cost, uint32 times) const;
    void GetReward(IntPairVec& reward) const;
	uint32 GetStageType() const;
	bool IsCommonStage() const;
    bool IsResourceStage() const;
    bool IsGroupStage() const;
    bool IsFogStage() const;
	bool IsCrusadeStage() const;
	bool IsCruGiftsStage() const;
	bool IsBranchStage() const;
	bool IsHideStage() const;
	uint32 HideUnlockStar() const;
	uint32 BranchGroupId() const;
	const string& GetAwardList() const;
};

#endif //__FubenStage_h__