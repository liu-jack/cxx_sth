#pragma once

#include "OS.h"
#include "def/TypeDef.h"
#include "memory_buffer/MultiValueArray.h"
#include "utility/TimePeriod.h"
#include "Enum.pb.h"
#include "boost/scoped_ptr.hpp"
#include "BaseDefineMgr.h"
#include "BaseDefine.pb.h"


#define GROWUP_LAST_TIME_LONG (GET_BASE_DEF_UINT(pb::BD_GROWUP_QUEST_LAST_TIME) * ONE_HOUR_SECOND )////second
#define FINISH_STAGE_AND_GIVE_GOLD_LONG (GET_BASE_DEF_UINT(pb::BD_PASS_STAGE_ACTIVITY_LAST_TIME) * ONE_HOUR_SECOND)/////second
#define OPEN_FOG_LAST_LONG (GET_BASE_DEF_UINT(pb::BD_ACTIVITY_OPEN_FOG_LAST_TIME) * ONE_HOUR_SECOND)////second
const static uint32 ENTER_WORLD = 18;//function_open function Id

enum KILL_TYPE
{
	ENUM_TYPE_KILL = 1,
	ENUM_TYPE_DEATH	= 2
};
namespace pb
{
	class ObjectFullValue ;
    class ObjectValueUpdate;
}

class PlayerValueOwner
{
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
public:
	PlayerValueOwner();
    virtual ~PlayerValueOwner();
    
	//GUID
    virtual uint64 GetGuid() const;
    void SetGuid( uint64 id);

    const std::string& CharName();
    void SetCharName( const std::string& name);

	//等级
    int32 GetLevel() const;	
    int32 GetXP() const;
    void  AddXP( uint32 number);
	void AddTodayXP(uint32 number);
	uint32 GetTodayXP();
	void resetTodayAddXP();
	void AddXPByLimit(uint32 number,uint32 limit);
    
	//VIP
    int32 GetVipLevel() const;
    int32 GetVipXP() const;	
    void  AddVipXp( uint32 number);

	//战斗力
	int32 GetCombatPower() const;
    void  SetCombatPower( int combatpower);
	//PVP 排名
    void SetPVPRank( int rank);
	void SetPVPHistoryRank( int rank );
    
	//公会
    uint64 GetGuildId();
	const string& GuildName();
    void SetGuildName(string guildName);
	void SetGuildId(uint64 guildId);

	uint64 GetLockTime() const;
	void SetLockTime(uint64 totime);
	//新手引导
    void SetGuideProcess( int32 process);
    
	void SetHeadIconId(int32 iconId);
	uint32 GetHeadIconId();
	//Flag 字段
    void SetOld();
    bool IsNew() const;

    bool TestFlag( uint32 flag) const;
    void SetFlag( uint32 flag);
    void RemoveFlag( uint32 flag);

    //////////////////////////////////////////////////////////////////////////
	//金钱
    int32 GetCurrency( int32 type) const;
	bool EnoughGold(uint32 number ) const;
	bool EnoughCurrency( int32 type, uint32 number ) const ;
    bool EnoughCurrency( const CurrType2Count& curr2count) const;
	int32 GetGold() const;
    //////////////////////////////////////////////////////////////////////////
	//皇城低阶官职
    int  GetPlaceLowOfficeId() const;
    void SetPlaceLowOfficeId(int officeId);


    //////////////////////////////////////////////////////////////////////////
	//登录
    int64 GetLastLogin() const;
	int64 GetRegistTime() const;
    void SetLastLoginToNow();
    int32 GetContinLoginDay() const;
    void SetContinLoginDay( int day);
	void SetLoginDaysNumber(const int times);//
	uint32 GetLoginDaysNumber() const;//登录天数

    //////////////////////////////////////////////////////////////////////////
	//AP奖励
    int64 GetLastApBonusTime() const;
    void SetLastApBounsTime( int64 timeStamp);

    //////////////////////////////////////////////////////////////////////////
	//登录奖励
    uint32 GetTakeLogin7DayCount() const;
    void SetTakeLogin7DayCount(uint32 count);

    uint32 GetTakeLoginMonthlyCount() const;
    void SetTakeLoginMonthlyCount(uint32 count);

    time_t GetTakeLogin7DayTime() const;
    void SetTakeLogin7DayTime(time_t takeTime);

	time_t GetEquipShopBuyCoolDownTime() const;
	void SetEquipShopBuyCoolDownTime( time_t takeTime );
	time_t GetTakeLoginMonthlyTime() const;
    void SetTakeLoginMonthlyTime(time_t takeTime);
	//月卡
	void SetTakeNormalMonthCardRewardTime(time_t takeTime);
	void SetTakeAdvanceMonthCardRewardTime(time_t takeTime);
	time_t GetTakeNormalMonthCardRewardTime();
	time_t GetTakeAdvanceMonthCardRewardTime();
	void SetBuyNormalMonthCardTime(time_t buyTime);
	void SetBuyAdvanceMonthCardTime(time_t buyTime);
	time_t GetBuyNormalMonthCardTime();
	time_t GetBuyAdvanceMonthCardTime();

	uint64 GetCongratulateActiveTime();
	void SetCongratulateActiveTime(uint64 time_date);

	//建筑
	uint32 GetAutoHammerCount() const;
	void SetAutoHammerCount( uint32 count);
	uint32 GetGoldHammerCount() const;
	void SetGoldHammerCount(uint32 count);

	//武将经验加成
	uint32 GetHerosExpUpPercent();
	void SetHerosExpIpPercent(uint32 expPer);
	//玩家经验加成
	uint32 GetPlayerExpUpPercent();
	void SetPlayerExpUpPercent(uint32 number);
////异星宝箱等级
	uint32 GetActivityStarBoxLevel();
	uint32 GetTodayContinueOccupyCity();
    //////////////////////////////////////////////////////////////////////////

    int GenPrebattleID();
    bool IsMatchPreBattleId( int id) const;
    void ResetPrebattleId();


    //////////////////////////////////////////////////////////////////////////
	//属性获取
	int32 GetAttrInt(uint32 index) const;
	uint32 GetAttrUint(uint32 index) const;
	float GetAttrFloat(uint32 index) const;
    uint64 GetAttrUint64(uint32 index) const;
	const std::string& GetAttrString(uint32 index) const;

	int GetRealValue(int valIdx, int val);	//获得正确的值  上限内

	void SetValueInt(int valIdx, int new_val, bool isNew = true);
	void SetValuefloat(int valIdx, float val);
	void SetValueUint64(int valIdx, uint64 val);
	void SetValueString(int valIdx, const string& val);

	//增加没有上限的属性如： 功勋值
	bool TryAddValueIsInLimit(uint32 index,int val,int limit);
	void AddValueLimitByLimit(uint32 index,int val,int limit);
	bool IsReachLimit(uint32 index,int limit);	

	uint64 GetPalaceTakeSalaryTime();
	void SetPalaceTakeSalaryTime(uint64 timeSec);

	time_t IsInRepeatOperateTime();

	virtual uint64 AccountId() const;
	void SetAccountId( uint64 accountId ) {   m_accountId =accountId ;   }

	void SaveValue( pb::ObjectFullValue &fullValue ) ;
	void LoadValue( const pb::ObjectFullValue &fullValue ) ;

	virtual uint32 GetChannelId( void ) const         {   return m_channelId ;   }
	void SetChannelId( uint32 id )      {   m_channelId =id ;   }

	virtual uint32 GetPlatformId( void ) const       {   return m_platformId ;   }
	void SetPlatformId( uint32 id )     {   m_platformId =id ;   }

	bool IsAutoCombat() const { return m_isAutoCombat; }
	void SetIsAutoCombat(bool isauto) { m_isAutoCombat = isauto; }
	
    //////////////////////////////////////////////////////////////////////////
	void SetGMLevel(int level)      {   m_gmLevel = level;}
	int GMLevel()                   {   return m_gmLevel;}
	bool IsGM() const;

    //////////////////////////////////////////////////////////////////////////
    void GM_SetLevel(int level);
    void GM_SetVipLevel(int vipLv);


	bool IsTakeVipReward(int vipLv);
	bool TakedVipReward( int vipLv );
	//每日任务
	uint32	GetDailyQuestScore() const;
	void   AddDailyQuestScore(int value);

	//背包
	uint32 GetBagCapacity();
	void SetBagCapacity(int num);

	//攻城器械
	uint32 GetPanZerId();
	void   SetPanZerId(int num);

	//是否自动增兵
	bool  IsAutoIncreaseSolders();
	void  SetAutoIncreaseSolders(bool isAuto);

	//主城改造
	uint32 GetCurRebuildId();
	void   SetCurRebuildId(int buildId);
	bool   IsBeginRebuild();
	void   SetBeginRebuild(bool isBegin);

	//跨服战
	bool  IsRegistCrossServer();
	void  SetRegistCrossServer(bool rigest);
	bool IsInCrossWar();
	void SetInCrossWar(bool isIn);

	bool IsLock();
	void   SetLockState(bool value);

	bool IsShutUp();
	void SetShutUpState(bool isIn);

	void SetEnterWorldTime(time_t time_enter);
	time_t GetEnterWorldTime();
/////功勋榜
	uint32 GetExploitValue();
	uint32 GetTrickValue();
	uint32 GetOccupyValue();
	uint32 GetAttackCityValue();
	void SetExploitValue(int value);
	void SetTrickValue(int value);
	void SetOccupyValue(int value);

	void setCurrRewardCharId(const int char_id);
	uint32 getCurrRewardCharId();

    void    WriteUpdateInfoTo(pb::ObjectValueUpdate& msg) const;
	//机器人客户端用
    void    ApplyUpdateInfoFrom( const pb::ObjectValueUpdate& msg);

private:
    void ModifyCurrency( uint32 type, int32 number ) ;




protected:
    //////////////////////////////////////////////////////////////////////////
    void    SetUnmodified();
    bool    HasUpdateInfo() const;
	//请用player中的带reason 版
    void AddCurrency( uint32 type, uint32 number );
    bool TryDeductCurrency( uint32 type, uint32 number ) ;
    bool TryDeductCurrency( const CurrType2Count& curr2count);

    
    virtual void _SetLevel( int level);
    virtual void _SetVipLv( int vipLv);
    virtual void _SetCombatPower( int power);
    void    SetCurExp( int exp);
    void    SetCurVipExp( int exp);

	//virtual void _SetIconID(int headid);

protected:
    virtual void	OnModifyInt(int valIdx, int new_val, int old_value,bool isNew = true) {};
	virtual void	OnModifyfloat(int valIdx, float val) {};
	virtual void	OnModifyString(int valIdx, const string& new_val) {};
	virtual void	OnModifyUint64(int valIdx, uint64 val) {};

private:
	MultiValueArray& MutableValues()        {   return m_values ;   }
public:
	const MultiValueArray& Values() const   {   return m_values ;   }

private:
    bool                 m_hasUpdateInfo;
	uint32               m_channelId ;
	uint32               m_platformId ;
	uint64               m_accountId ;
	MultiValueArray      m_values;
	int                  m_gmLevel;
	time_t				 m_operateLockTime;
	bool				 m_isAutoCombat;

};