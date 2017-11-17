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
	COMMON_FUBEN,     //��ͨ 0
	RESOURCE_FUBEN,   //��Դ 1
	GROUP_FUBEN,      //���� 2
	FOG_FUBEN,        //���� 3
	CRUSADE_FUBEN,	//Զ�� 4
	CRUSADE_GIFTS_FUBEN,	//Զ������ 5
	BRANCH_FUBEN,		//֧��
	HIDE_FUBEN,			//����
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