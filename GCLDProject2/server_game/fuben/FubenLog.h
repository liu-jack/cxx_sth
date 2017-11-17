#ifndef __FubenLog_h__
#define __FubenLog_h__
#include "def/TypeDef.h"
#include "def/ConstDef.h"
class Player;
class FubenStage;
namespace Combat
{
    class CombatGroup;
}
namespace pb
{
    class GS2C_Fuben_All_Info;		//���и�����Ϣ
    class GS2C_Resource_Stage;		//��ԴС�ؿ�
	class GS2C_Combat_Crusade;		//Զ������
	class GS2C_Select_Ret;
	class GS2C_Combat_Crusade_Add_Hero;
	class GS2C_ResetResource;
	class GS2C_TakeResource;
	class C2GS_Combat_Crusade_Recover_Times;
	class GxDB_Combat_Crusade;
	class GS2C_Crusade_Take_Reward_Rsp;
	class GS2C_take_branch_fuben_star_rsp;
	class BranchFubenInfo;
	class HideFubenInfo;
}

//�ؿ���Դ����
// struct ResourceLog
// {
// 	ResourceLog()
// 		:quest_times(0),times(0),validTime(0)
// 	{}
//     void Reset();
// 	void Bezero();
// 	uint32 quest_times; //�������
//     uint32 times;       //����Դ�ر���ȡ�Ĵ���
//     uint64 validTime;   //����ȡ����Чʱ�� sec
// };

struct stage_info
{
	uint32 pass_count_;
	uint32 is_take_rewrd_;
	stage_info()
		:pass_count_(0)
		,is_take_rewrd_(0)
	{
	}
};

//Զ����Ϣ
struct CrusadeLog
{
	uint32 currCrusade;	//��ǰ��һԶ������
	uint32 currCruStage;	//��ǰԶ����һ��
	uint32 isCruDead;	//��ǰԶ�������Ƿ���	0��1��
	uint32 restReset;	//ʣ�����ô���
	uint32 addHeroTimes;	//ʣ������佫����
	uint32 award_state;	//������ȡ���
	uint32 isCrusadeOver;	//��ǰԶ���Ƿ��޷��ٴ�   ��ʼ��Ϊ0,��˵����ʧ����Ϊ1���������ٴ�

	VecInt heroAddTimes;
	IntPairVec heroAddTimesBackup;	//���ͻ��˷��͵���Ϣ
	CrusadeLog();
	void Reset();
	void AddHeroAddTimes(uint32 times);
};
enum BRANCH_TYPE
{
	BRANCH_EASY = 1,
	BRANCH_COMMON,
	BRANCH_HARD,
};
struct BranchBaseInfo
{
	bool is_passed_;
	bool is_take_award_;
	uint32 branch_group_id_;
	BranchBaseInfo()
		:is_passed_(0),is_take_award_(0),branch_group_id_(0){}
	BranchBaseInfo(uint32 branch_group_id)
		:is_passed_(0),is_take_award_(0),branch_group_id_(branch_group_id)
	{
	}
};


struct HideFubenBaseInfo
{
	uint8 free_time_;
	uint32 today_fight_times_;
	uint32 kill_monsters_;
	HideFubenBaseInfo()
		:free_time_(1),today_fight_times_(0),kill_monsters_(0){}
};

class FubenLog { //�������ȼ�¼�������������
public:
   // typedef std::map<uint32, ResourceLog>   ResourceLogMap;	//�ؿ���Դ������
    typedef std::map<uint32,stage_info>     PassStageSet;	//���������ؿ�
	typedef std::map<uint32,BranchBaseInfo >     BranchFubenMap;	//���������ؿ�
	typedef std::map<uint32,HideFubenBaseInfo >     HideFubenMap;	//���������ؿ�
public:
    uint32 m_curFubenID; //���½�
    uint32 m_curStageID; //�½����С��
	uint32 NR_stars_[18];
    //ResourceLogMap	m_mapResourceLog;
    PassStageSet	m_passStageLog; //���������ؿ������Źء�������
	CrusadeLog m_crusadeLog;
	BranchFubenMap branchFubenMap_;
	HideFubenMap hideFubenMap_;
    FubenLog();
	bool Init();
    void SaveTo(Player* player,pb::GS2C_Fuben_All_Info& msg);
	void SaveTo(pb::GS2C_Combat_Crusade& msg);
	void SaveTo(pb::GxDB_Combat_Crusade& msg);
	void SaveTo(const uint32 stageId,const BranchBaseInfo& info,pb::BranchFubenInfo& msg);
	void SaveTo(const uint32 stageId,const HideFubenBaseInfo& info,pb::HideFubenInfo& msg);
	void LoadFrom(const pb::GS2C_Fuben_All_Info& msg);
	void LoadFrom(const pb::GxDB_Combat_Crusade& msg);
	//���Ͷ˷��͹������佫Id�����heroAddTimesBackup
	void AddHeroToVector(std::vector<uint32>& vec);

	uint32 GetDistance(const VecInt::iterator& iter,const VecInt& vec);
	void SyncVecs(Player& player);
	bool EnterStage(Player& player, const uint32 stageId, std::vector<uint32>& heroIds);
    void OnStageEnd(Player* player, uint32 stageId,const uint32 npc_hp, Combat::CombatGroup* pGroup, bool isAttackWin); //С�ؿ����ظ���
    bool _GoToNextStage();
	void GetUnlockCharacter(std::set<uint32>& unlockChar,const uint32 branch_fuben_id = 0);
	//void OnCombatGroupRelease(Player* player, IntPairVec heroVec);
	void OnCrusadeEnd(Player* player, const uint32 stageId, Combat::CombatGroup* pGroup, bool isAttackWin);
	void OnCrusadeBegin(Player* player,Combat::CombatGroup* pGroup);

	void FillStageInBranchAndHide();
    uint32 IsPassStage(uint32 stageId) const;
	bool IsCursadePassed(const uint32 CrusadeStageId)const;
	void SelectCrusade(Player& player, uint32 crusadeid,pb::GS2C_Select_Ret& msg);
	void GotoNextCrusade(CrusadeLog& log,const uint32 stageId);

	bool EnterCrusadeStage(Player& player, uint32 stageId, std::vector<uint32>& heroIds);

	void RecoverTimes(Player& player,const pb::C2GS_Combat_Crusade_Recover_Times& msg);
	void InitCrusadeData(Player* player);
	bool AddShadowInCrusade(Player& player,uint32 heroid, pb::GS2C_Combat_Crusade_Add_Hero& msg);
	void TakeCrusadeReward(Player& player,const uint32 stageId,pb::GS2C_Crusade_Take_Reward_Rsp& msg);

	//bool IsBranchFubenPassed(const uint32 stageId);
	void UpdateBranchFubenPassed(Player& player,const uint32 stageId,const bool passed);
	void UpdateBranchFubenTakendReward(Player& player,const uint32 stageId,const bool taken_star);
	void TakeBranchReward(Player& player,const uint32 stageId,pb::GS2C_take_branch_fuben_star_rsp& msg);
	void ResetHideFubenFightTimes(Player& player);
	void AddHideFubenFightTimes(Player& player,const uint32 stageId);
	void SetHideFubenKillMonsters(Player& player,const uint32 stageId,const uint32 npc_hp,const uint32 kill_npc);
private:
	void FightWithNpc(Player& player, const FubenStage* pStage, std::vector<uint32>& heroIds);
	void CombatInCrusade(Player& player, const FubenStage* pStage, std::vector<uint32>& heroIds);
};

#endif //__FubenLog_h__