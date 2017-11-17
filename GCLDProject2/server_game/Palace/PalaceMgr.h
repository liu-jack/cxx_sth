#pragma once
#include "def/TypeDef.h"
#include "def/ConstDef.h"
#include "Base.h"

class Player;
namespace Combat
{
    class CombatGroup;
}
namespace pb
{
    class GS2C_Palace_All_Info;
	class PalaceAchievementInfo;
	class GxDB_PalaceAchievement;
	class GS2C_Palace_Take_Reward_Rsp;
}


// struct OfficeId
// {
// 	uint8 head_icon_id_;
// 	uint8 office_id_;
// 	uint32 level_;
// 	uint32 vip_level_;
// 	uint32 military_power_;
// 	uint64 playerid_;
// 	OfficeId()
// 		:head_icon_id_(),office_id_(),level_(),vip_level_(),military_power_(),playerid_()
// 	{
// 
// 	}
// 	OfficeId(uint8 head_icon_id,uint8 office_id,uint32 level,uint32 vip_level,uint32 military_power,uint64 playerid)
// 		:head_icon_id_(head_icon_id),office_id_(office_id),level_(level),vip_level_(vip_level),military_power_(military_power),playerid_(playerid)
// 	{
// 
// 	}
// 
// 	friend bool operator<(const OfficeId& another,const OfficeId& other)
// 	{
// 		if(another.office_id_ == 0 && other.office_id_ != 0) return false;
// 		if(another.office_id_ > other.office_id_) return true;
// 		else if(another.office_id_ < other.office_id_) return false;
// 		else
// 		{
// 			if(another.level_ < other.level_ ) return true;
// 			else if(another.level_ > other.level_ ) return false;
// 			else
// 			{
// 				if(another.military_power_ < other.military_power_) return true;
// 				else if(another.military_power_ > other.military_power_) return false;
// 				else
// 				{	
// 					if(another.vip_level_ < other.vip_level_) return true;
// 					else if(another.vip_level_ > other.vip_level_) return false;
// 					else
// 					{	
// 						if(another.playerid_ > other.playerid_) return true;
// 						else if(another.playerid_ < other.playerid_) return false;
// 						else
// 						{
// 							return false;
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}
// 	friend bool operator==(const OfficeId& another,const OfficeId& other)
// 	{
// 		return another < other && other < another;
// 	}
// 
// };

class PalaceMgr {
public:
    struct OfficeInfo {
        OfficeInfo(){ clear(); }
        void clear(){ ZeroMemoryThis; }
        uint64          playerId;
        uint64          skillUseTime;
        uint64          aideLst[5];
        uint64          aideApplyLst[10];
        Combat::CombatGroup*   pCombatGroup;
		bool			isInFighting;		//正在攻打，无法申请再次攻打		
    };
	

public:
    std::map<uint8, OfficeInfo>     m_OfficeMap;    //官员列表
    //侍从、申请侍从的人员记录，只是为了方便判断
    //重启后，根据数据库信息重组
    std::map<uint64, uint8>         m_aideInfoMap;
	std::map<uint8, uint32>			m_awardMap;
    PalaceMgr();
public:
    static PalaceMgr* Instance(uint countryId){ 
        if (countryId > MAX_PLAYER_COUNTRY_ID) return NULL;
        static PalaceMgr T[MAX_PLAYER_COUNTRY_ID]; return &T[countryId-1];
    }
    void SaveTo(int countryId, pb::GS2C_Palace_All_Info& msg);
    void SaveTo(int countryId, int officeId, pb::GS2C_Palace_All_Info& msg);
    void LoadFrom(int countryId, const pb::GS2C_Palace_All_Info& msg);
	
    int GetOfficeId(uint64 playerId);
    OfficeInfo* GetOfficeInfo(uint64 playerId);
    Player* GetOfficePlayer(int officeId);
	bool IsPlayerOffical(uint64 playerId);

	void FightNewOffice(Player& player, std::vector<uint32>& heroIds, int officeId);  //打新官职
    void _OnFightEnd(Combat::CombatGroup* pGroup, bool isAttackWin, Player* player, int officeId);
	void _OnFightCanceled(Combat::CombatGroup* pGroup,int officeId);
    void CancelOffice(Player& host);                //放弃官职
    void ApplyAide(Player& player, int officeId);   //申请侍从
    void CancelAide(Player& aide);                  //放弃侍从
    void CancelApplyAide(Player& player);           //取消申请
    void AgreeAide(Player& host, uint64 aideId);    //同意申请
    void RefuseAide(Player& host, uint64 aideId);   //拒绝申请
    void FireAide(Player& host, uint64 aideId);     //解除侍从
    IntPair TakeSalary(Player& player);             //领取俸禄

	bool IsOfficalCanFight(Player& host, int officeid, int& errcode);

	void UpdateOfficalAwardsPerHour();
private:
    void ChangeOffice(Player& player, uint8 newOfficeId);
    void ClearOffice(int officeId);
    void ClearAideApplyLst(OfficeInfo& info);
	void ClearAideLst(OfficeInfo& info);
    bool CanApplyAide(int officeId, Player& player);
    bool AddAide(int officeId, uint64 playerId);
};
#define sPalaceMgr(countryId) PalaceMgr::Instance(countryId)
