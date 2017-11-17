#pragma once

#include "SimpleSingleton.h"
#include "def/TypeDef.h"
#include <vector>
#include <map>
#include <set>
#include "Opcode.pb.h"

struct DB_BasePlayer;
struct DB_VipLevelPro;
struct DB_HeadIconUnlock;
struct DB_FirstPay;
struct DB_MonthCard;
class Player;

class PlayerDataMgr : public SimpleSingleton< PlayerDataMgr> 
{
public:
    typedef std::vector<const DB_BasePlayer*> BasePlayerVec;
	typedef std::map<uint32, const DB_VipLevelPro*>   VipMap;
	typedef std::map<uint32, const DB_HeadIconUnlock*>   IconId2UnLockReq;
	typedef std::map<uint32, IntPairVec> BasePlayerRewardMap;
	typedef std::vector<const DB_FirstPay*> FirstPayVec;
	typedef std::map<uint32,const DB_MonthCard*> MonthCardMap;
public:
    void Init();

	//void InitCreatPlayerData(Player* player);
	//bool CheckNewFreeHeadIconUnLock(Player* player, std::set<uint32> & unlockFreeIds);

	const IconId2UnLockReq& GetHeadIconMap() { return m_headIconUnlockReqMap;}
	const ItemID2Count& GetInitItemMap() { return m_InitResourceMap;}
	// vip�ȼ�
    bool TryGetUpgradeExp( uint32 level, uint32& out_exp) const;
    uint32 GetMaxLevel() const;
    
    // vip�ȼ�
    bool TryGetVipUpgradeExp( uint32 level, uint32& out_value) const;
    uint32 GetMaxVipLevel() const;

    // ��������
    uint32 GetVipExp( uint32 viplv) const;//
	bool GetRewardItems(uint32 viplv, IntV3Vec& retMap ) const;
	bool GetPrivilege( uint32 viplv,std::map<int,int>& retMap) const;
	int  GetPrivilegeValue(int priId) const;

	//�׳�
	bool GetFirstPayRewards(IntV3Vec& itemV,std::map<int,int>& priMap) const;

	//�¿�
	int getMonthCardRewardAndLastDays(const uint32 cardType,IntPairVec& vec,uint32& lastDays);
	
    
    // ���ܿ���
    bool IsReachTrainingAddtionRequire( int vipLv, int lv, uint32 addtion) const;
    bool IsUnlockSweep( int lv) const;
    bool IsUnlockCharReinforce( int lv) const;
    bool IsUnlockEquipReinforce( int lv) const;
    bool IsUnlockAccReinforce( int lv ) const;

    bool IsUnlockPVPArena( int lv) const;
    bool IsUnlockPVPAcc( int lv) const;
    bool IsUnlockPVEInfinity( int lv) const;
    bool IsUnlockPVEGold( int lv) const;
	const DB_HeadIconUnlock* GetHeadIconUnLock(uint32 headIconId);

//��������
	bool GetRewardByLevel(uint32 level,IntPairVec* vec) const;
private:
private:
	bool ParsserItems(uint32 rType, const std::string& rId, const std::string& rNum,IntV3Vec& retMap) const;
	bool ParsserPrivilege(uint32 rType, const std::string& rId, const std::string& rNum,std::map<int,int>& retMap) const;
private:
    BasePlayerVec m_basePlayerVec;
    VipMap    m_baseVipMap;
	IconId2UnLockReq m_headIconUnlockReqMap;
	ItemID2Count m_InitResourceMap;
	BasePlayerRewardMap m_basePlayerRewardMap;
	std::map< int, int>     m_priVipMap;
	uint32 m_maxVipLevel;
	FirstPayVec m_firstPayVec;
	MonthCardMap m_monthCardMap;
//����д���
public:
	void InitClientCrossWhiteList();		//���˷����������Ϣ
	void InitClientGameWhiteList();			//�����ɱ����������Ϣ	������У�
	void InitGameClientBlackList();			//������Ϸ�����ͻ��˵ĺ�����
	bool CheckClientCrossWhiteList(pb::Opcode opcode);	//�ͻ��˷�������İ�����
	bool CheckClientGameWhiteList(pb::Opcode opcode);	//�ͻ��˷������ط�����İ�����
	bool CheckGameClientBlackList(pb::Opcode opcode);		//��Ϸ�������ͻ��˵ĺ����� �������Ϸ������һ��
	std::vector<pb::Opcode> m_wCOpCodeList;
	std::vector<pb::Opcode> m_wGopCodeList;
	std::vector<pb::Opcode> m_bOpCodeList;
};

#define sPlayerDataMgr PlayerDataMgr::Instance() 