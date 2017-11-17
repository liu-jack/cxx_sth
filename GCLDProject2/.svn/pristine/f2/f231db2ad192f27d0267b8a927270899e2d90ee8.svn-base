#include "PlayerValueOwner.h"

#include "System.h"
#include "OS.h"
#include "def/ObjectDefines.h"
#include "def/MmoAssert.h"
#include "boost/numeric/conversion/cast.hpp"
#include "Enum.pb.h"
#include "utility/Utility.h"
#include "PlayerDataManager.h"
#include "InterServer.pb.h"


#define  REPEAT_OPERATE_TIMESPACE  10
namespace {
    const int32 moneyIndex[eCurrencyNum] = {
	    pb::PLAYER_FIELD_COIN,                    // 硬币
	    pb::PLAYER_FIELD_SYS_GOLD,                 // 钻石
		pb::PLAYER_FIELD_IRON,					//矿石
		pb::PLAYER_FIELD_FOOD,					//粮食
		pb::PLAYER_FIELD_WOOD,					//木材
        pb::PLAYER_FIELD_HORNOR,				//荣誉点
		pb::PLAYER_FIELD_BUILDING_HORNOR,		//建筑荣誉点
        pb::PLAYER_FIELD_TRAINING,        //练兵令
        pb::PLAYER_FIELD_TROOPSNUM,       //增兵次数 
        pb::PLAYER_FIELD_FREECOLLECTNUM,    //免费征收次数  
        pb::PLAYER_FIELD_SUPERCOLLECTNUM,     //超级征收次数
        pb::PLAYER_FIELD_TROOPSODER,// 征兵令
		pb::PLAYER_FIELD_GOLDHAMMER,// 黄金锤
		pb::PLAYER_FIELD_COUPON,
		pb::PLAYER_FIELD_MARKETBUYNUM,
		pb::PLAYER_FIELD_FREEPURIFY,
		pb::PLAYER_FIELD_EXTRMEPURIFY,
		pb::PLAYER_FIELD_GODSTONEPURIFY,
		pb::PLAYER_FIELD_CASH_GOLD,
		pb::PLAYER_FIELD_CUR_XP,
		pb::PLAYER_FIELD_CUR_VIP_XP,
		pb::PLAYER_FIELD_DAILY_QUEST_SCORE,
		pb::PLAYER_FIELD_TRAINING_FIGHT,
		pb::PLAYER_FIELD_TRAINING_DOUBLE,
		pb::PLAYER_FIELD_TRAINING_REDUCE,
		pb::PLAYER_FIELD_PEOPLE_LOYAL,//民忠
		pb::PLAYER_FIELD_ZHENGWU_TIMES,//政务
		pb::PLAYER_FIELD_RENOWN,//名望
		pb::PLAYER_FIELD_STARCRYSTAL,//星耀晶
		pb::PLAYER_FIELD_BOARD,//集成板
		pb::PLAYER_FIELD_GENE_CODE,//基因码
		pb::PLAYER_FIELD_BEEF,//牛肉
		pb::PLAYER_FIELD_WINE,//美酒
    } ;

}

PlayerValueOwner::PlayerValueOwner()
: m_hasUpdateInfo( false)
, m_channelId( 0)
, m_platformId( 0)
, m_accountId( 0)
, m_values( pb::PLAYER_FIELD_32_BIT_COUNT, pb::PLAYER_FIELD_64_BIT_COUNT, pb::PLAYER_FIELD_FLOAT_COUNT, pb::PLAYER_FIELD_STR_COUNT )
, m_gmLevel( GM_LEVEL_NONE)
, m_operateLockTime( sOS.GetRealTime())
, m_isAutoCombat(false)
{
	m_accountId =0 ;
    // only for test



}

PlayerValueOwner::~PlayerValueOwner()
{

}

int PlayerValueOwner::GetRealValue(int valIdx, int val)
{
	if(valIdx == pb::PLAYER_FIELD_FREECOLLECTNUM)	//免费征收次数  不允许超过上限
	{
		if(val > GET_BASE_DEF_INT(pb::BD_FREE_DRAFT_LIMIT_TIMES))
			val = GET_BASE_DEF_INT(pb::BD_FREE_DRAFT_LIMIT_TIMES);
	}
	return val;
}

void PlayerValueOwner::SetValueInt(int32 valIdx, int val, bool isNew /* = true */)
{
	val = GetRealValue(valIdx,val);		//所有有上限的判断加在这里
	int old_value = MutableValues().GetInt32(valIdx);
	if(MutableValues().SetInt32(valIdx, val))
	{
		OnModifyInt(valIdx,val,old_value,isNew);
		m_hasUpdateInfo = true;
	}
}

void PlayerValueOwner::SetValuefloat(int valIdx, float val)
{
	
	if(MutableValues().SetFloat(valIdx, val))
	{
		OnModifyfloat(valIdx,val);
		 m_hasUpdateInfo = true;
	}
}

void PlayerValueOwner::SetValueString(int valIdx,const string& val)
{
	if(MutableValues().SetString(valIdx, val))
	{
		OnModifyString(valIdx,val);
		 m_hasUpdateInfo = true;
	}
}

void PlayerValueOwner::SetValueUint64(int valIdx, uint64 val)
{
	if (MutableValues().SetUint64(valIdx, val))
	{
		OnModifyUint64(valIdx,val);
		m_hasUpdateInfo = true;
	}
}

uint64 PlayerValueOwner::GetAttrUint64( uint32 index ) const
{
	return m_values.GetUint64(index);
}

int32 PlayerValueOwner::GetAttrInt(uint32 index) const
{
	return Values().GetInt32(index);
}

uint32 PlayerValueOwner::GetAttrUint(uint32 index) const
{
	return static_cast<uint32>(Values().GetInt32(index));
}

float PlayerValueOwner::GetAttrFloat(uint32 index) const
{
	return Values().GetFloat(index);
}

const std::string& PlayerValueOwner::GetAttrString(uint32	index) const{
	return Values().GetString(index);
}

uint64 PlayerValueOwner::GetGuid() const
{
	return Values().GetUint64( pb::PLAYER_FIELD_PLAYER_ID ) ;
}

uint64 PlayerValueOwner::GetGuildId()
{
	return Values().GetUint64( pb::PLAYER_FIELD_GUILD_ID ) ;
}

uint64 PlayerValueOwner::GetLockTime() const
{
	return Values().GetUint64( pb::PLAYER_FIELD_LOCK_TIME ) ;
}

void PlayerValueOwner::SetLockTime(uint64 totime)
{
	SetValueUint64( pb::PLAYER_FIELD_LOCK_TIME,totime) ;
}

void PlayerValueOwner::SetGuildId(uint64 guildId)
{
	SetValueUint64( pb::PLAYER_FIELD_GUILD_ID,guildId) ;
}
uint64 PlayerValueOwner::GetPalaceTakeSalaryTime()
{
	return Values().GetUint64( pb::PLAYER_FIELD_PALACE_TAKE_SALARY_TIME ) ;
}

void PlayerValueOwner::SetPalaceTakeSalaryTime(uint64 timeSec)
{
	SetValueUint64( pb::PLAYER_FIELD_PALACE_TAKE_SALARY_TIME,timeSec) ;
}

const string& PlayerValueOwner::CharName()
{
	return Values().GetString( pb::PLAYER_FIELD_NAME ) ;
}

void PlayerValueOwner::SetGuideProcess( int32 process )
{
    SetValueInt( pb::PLAYER_FIELD_GUIDE_PROCESS, process);
}

void PlayerValueOwner::SetHeadIconId(int32 iconId)
{
	SetValueInt( pb::PLAYER_FIELD_HEAD_ICON_ID, iconId);
}

uint32 PlayerValueOwner::GetHeadIconId()
{
	return static_cast<uint32>(Values().GetInt32(pb::PLAYER_FIELD_HEAD_ICON_ID));
}

void PlayerValueOwner::SetCharName( const std::string& name )
{
    SetValueString( pb::PLAYER_FIELD_NAME, name);
}

bool PlayerValueOwner::IsInCrossWar()
{
	
	return IsRegistCrossServer()  && (GetAttrInt( pb::PLAYER_FIELD_IS_IN_CROSS_SERVER ) > 0);
}

void PlayerValueOwner::SetInCrossWar( bool isIn )
{
	LLOG("set in war %d", isIn);
	if (isIn)
	{
		if (IsRegistCrossServer())
		{
			SetValueInt( pb::PLAYER_FIELD_IS_IN_CROSS_SERVER, isIn ? 1 : 0);
		}
	} 
	else
	{
		SetValueInt( pb::PLAYER_FIELD_IS_IN_CROSS_SERVER, isIn ? 1 : 0);
	}
}

bool PlayerValueOwner::IsLock()
{
	bool isLock = (Values().GetInt32(pb::PLAYER_FIELD_LOCKREASON) == (int)pb::ACCOUNT_LOCK_TYPE_LOCKED);
	if (isLock)
	{
		if(GetLockTime() > 1)
		{
			if((uint64)sOS.GetRealTime() > GetLockTime())
			{
				SetLockState(false);
				isLock = (Values().GetInt32(pb::PLAYER_FIELD_LOCKREASON) == (int)pb::ACCOUNT_LOCK_TYPE_LOCKED);
			}
		}
	}
	return isLock;
}

void PlayerValueOwner::SetLockState( bool value )
{
	SetValueInt( pb::PLAYER_FIELD_LOCKREASON, value ? (int)pb::ACCOUNT_LOCK_TYPE_LOCKED : (int)pb::ACCOUNT_LOCK_TYPE_NONE);
	if (!value)
	{
		SetLockTime(0);
	}
}

bool PlayerValueOwner::IsShutUp()
{
	if (IsLock())
	{
		return true;
	}
	bool isShutUp = (GetAttrInt( pb::PLAYER_FIELD_LOCKREASON ) == (int)pb::ACCOUNT_LOCK_SPEAK);
	if (isShutUp)
	{
		if(GetLockTime() > 1)
		{
			if((uint64)sOS.GetRealTime() > GetLockTime())
			{
				SetShutUpState(false);
				isShutUp = (GetAttrInt( pb::PLAYER_FIELD_LOCKREASON ) == (int)pb::ACCOUNT_LOCK_SPEAK);
			}
		}
	}
	return isShutUp;
}

void PlayerValueOwner::SetShutUpState( bool isIn )
{
	SetValueInt( pb::PLAYER_FIELD_LOCKREASON, isIn ? (int)pb::ACCOUNT_LOCK_SPEAK : (int)pb::ACCOUNT_LOCK_TYPE_NONE);
	if (!isIn)
	{
		SetLockTime(0);
	}
}

const string& PlayerValueOwner::GuildName()
{
	return Values().GetString( pb::PALYER_FIELD_GUILD_NAME ) ;
}

void PlayerValueOwner::SetGuildName(string guildName)
{
	SetValueString( pb::PALYER_FIELD_GUILD_NAME,guildName) ;
}

uint64 PlayerValueOwner::AccountId() const
{
	return m_accountId;
}

int32 PlayerValueOwner::GetLevel() const
{
	return GetAttrInt( pb::PLAYER_FIELD_LEVEL);
}

int32 PlayerValueOwner::GetVipLevel() const
{
	return GetAttrInt( pb::PLAYER_FIELD_VIP_LEVEL);
}

int32 PlayerValueOwner::GetXP() const
{
	return GetAttrInt( pb::PLAYER_FIELD_CUR_XP);
}


int32 PlayerValueOwner::GetVipXP() const
{
    return GetAttrInt( pb::PLAYER_FIELD_CUR_VIP_XP);
}


void PlayerValueOwner::AddXP(uint32 number)
{
	int32 newXp = MultiValueArray::ModifyInt(GetXP(), number ) ;
    int32 newLevel = GetLevel();
    int32 maxLevel = sPlayerDataMgr.GetMaxLevel();

    
    while( true)
    {
        if ( newLevel >= maxLevel )
            break;
        
        uint32 upgradeXp = 0;
        if ( !sPlayerDataMgr.TryGetUpgradeExp(newLevel, upgradeXp))
            break;

        if ( newXp < (int)upgradeXp)
            break;

        newXp -= upgradeXp;
        ++newLevel;
    }

	SetCurExp( newXp);
    if ( newLevel != GetLevel())
    {
        _SetLevel( newLevel);
    }
}

void PlayerValueOwner::AddVipXp( uint32 number )
{
	int32 newXp = MultiValueArray::ModifyInt(GetVipXP(), number ) ;
    int32 newLevel = GetVipLevel();
    int32 maxLevel = sPlayerDataMgr.GetMaxVipLevel();

    while( true)
    {
        if ( newLevel >= maxLevel )
            break;

        uint32 upgradeXp = 0;
		if ( !sPlayerDataMgr.TryGetVipUpgradeExp(newLevel+1, upgradeXp))
            break;

        if ( newXp < (int)upgradeXp)
            break;
        ++newLevel;
		_SetVipLv( newLevel);
    }

    SetCurVipExp( newXp);
    if ( newLevel != GetVipLevel())
    {
        _SetVipLv( newLevel);
    }
}


int32 PlayerValueOwner::GetCombatPower() const
{
	return 1;//GetAttrInt( pb::PLAYER_FIELD_COMBAT_POWER);
}

void PlayerValueOwner::SetCombatPower( int combatpower )
{
  _SetCombatPower(combatpower);
}

int32 PlayerValueOwner::GetCurrency( int32 type ) const
{
	if( !IsValidCurrencyType( type ) )
	{
		MMO_ASSERT( false ) ;
		return 0;
	}

	return GetAttrInt( moneyIndex[type] );
}

int32 PlayerValueOwner::GetGold() const
{
	return GetCurrency(eSysGold) + GetCurrency(eCashGold);
}

bool PlayerValueOwner::EnoughGold(uint32 number ) const
{
	int32 goldValue = GetCurrency(eSysGold) + GetCurrency(eCashGold);
	return goldValue >= static_cast<int32>(number) ;
}

bool PlayerValueOwner::EnoughCurrency( int32 type, uint32 number ) const
{
	if (GetCurrency( type ) >= static_cast<int32>(number))
	{
		return true;
	}
	else
	{
		if (type == eSysGold)
		{
			return EnoughGold(number);
		}
		return false;
	}
}

bool PlayerValueOwner::EnoughCurrency( const CurrType2Count& curr2count ) const
{
    for ( CurrType2Count::const_iterator it = curr2count.begin();
        it != curr2count.end(); ++it)
    {
        if ( !EnoughCurrency( it->first, it->second))
        {
            return false;
        }
    }
    return true;
}

void PlayerValueOwner::ModifyCurrency( uint32 type, int32 number )
{
	if( !IsValidCurrencyType( type ) )
	{
		MMO_ASSERT( false ) ;
		return  ;
	}

	if(type == ePlayerExp && number > 0)	//玩家经验 需要升级
	{
		AddXP(number);
		return;
	}

	if (type == eSysGold)
	{
		if (number < 0)
		{
			//先扣除rmbcoin，再扣除syscoin
			int curRmbGold = GetAttrInt( moneyIndex[eCashGold]);
			if (curRmbGold < -number)
			{
				int32 curMoney = MultiValueArray::ModifyInt( GetAttrInt( moneyIndex[eCashGold] ), -curRmbGold ) ;
				SetValueInt( moneyIndex[eCashGold], curMoney );
				curMoney = MultiValueArray::ModifyInt( GetAttrInt( moneyIndex[type] ), curRmbGold + number ) ;
				SetValueInt( moneyIndex[type], curMoney );
				return;
			}
			else
			{
				int32 curMoney = MultiValueArray::ModifyInt( GetAttrInt( moneyIndex[eCashGold] ), number ) ;
				SetValueInt( moneyIndex[eCashGold], curMoney );
				return;
			}
		}
		else
		{
			int32 curMoney = MultiValueArray::ModifyInt( GetAttrInt( moneyIndex[type] ), number ) ;
			//LLOG("orgnum=%d,num=%d,curMoney=%d",GetAttrInt( moneyIndex[type] ),number,curMoney);
			SetValueInt( moneyIndex[type], curMoney ) ;
		}
	}
	else
	{
		int32 curMoney = MultiValueArray::ModifyInt( GetAttrInt( moneyIndex[type] ), number ) ;
		//LLOG("orgnum=%d,num=%d,curMoney=%d",GetAttrInt( moneyIndex[type] ),number,curMoney);
		SetValueInt( moneyIndex[type], curMoney ) ;
	}
}

void PlayerValueOwner::AddCurrency(  uint32 type, uint32 number )
{
	int32 value =boost::numeric_cast<int32>( number ) ;
	ModifyCurrency( type, value ) ;
}



bool PlayerValueOwner::TryDeductCurrency( uint32 type, uint32 number)
{
	if( EnoughCurrency( type, number ) )
	{
		int32 value = - boost::numeric_cast<int32>( number );
		MMO_ASSERT( value <= 0 ) ;
		ModifyCurrency( type, value ) ;
		return true ;
	}
    else
    {
        DLOG("NOT ENOUGH CURRENCY, TYPE : %d , NEED COUNT: %d",  type, number);
        return false ;
    }	
}

bool PlayerValueOwner::TryDeductCurrency( const CurrType2Count& curr2count )
{
    if ( EnoughCurrency( curr2count))
    {
        for ( CurrType2Count::const_iterator it = curr2count.begin();
            it != curr2count.end(); ++it)
        {
            int32 value = - boost::numeric_cast<int32>( it->second );
            MMO_ASSERT( value <= 0 ) ;
            ModifyCurrency( it->first, value ) ;
        }
        return true;
    }
    return false;
}

int PlayerValueOwner::GetPlaceLowOfficeId() const
{
    return GetAttrInt(pb::PlAYER_FIELD_PALACE_LOW_OFFICE_ID);
}
void PlayerValueOwner::SetPlaceLowOfficeId(int officeId)
{
    SetValueInt(pb::PlAYER_FIELD_PALACE_LOW_OFFICE_ID, officeId);
}


void PlayerValueOwner::SaveValue( pb::ObjectFullValue &fullValue )
{
	Values().BuildFullUpdateMsg( fullValue ) ;
}




void PlayerValueOwner::LoadValue( const pb::ObjectFullValue &fullValue )
{
	MutableValues().ApplyFullUpdateMsg( fullValue );
	SetUnmodified();
}

void PlayerValueOwner::SetUnmodified()
{
	m_values.SetUnmodified();
    m_hasUpdateInfo = false;
}

bool PlayerValueOwner::HasUpdateInfo() const
{
	return m_hasUpdateInfo;
}

bool PlayerValueOwner::IsAutoIncreaseSolders()
{
	return GetAttrInt( pb::PlAYER_FIELD_IS_AUTO_INCREASE_TROOPS ) > 0;
}

void PlayerValueOwner::SetAutoIncreaseSolders( bool isAuto)
{
	SetValueInt( pb::PlAYER_FIELD_IS_AUTO_INCREASE_TROOPS, isAuto ? 1 : 0);
}

uint32 PlayerValueOwner::GetCurRebuildId()
{
	return GetAttrUint( pb::PLAYER_FIELD_CUR_REBUILD_ID );
}

void PlayerValueOwner::SetCurRebuildId( int buildId )
{
	SetValueInt(pb::PLAYER_FIELD_CUR_REBUILD_ID,buildId);
}

bool PlayerValueOwner::IsBeginRebuild()
{
  return GetAttrInt( pb::PLAYER_FIELD_IS_BEGIN_REBUILD ) > 0;
}

void PlayerValueOwner::SetBeginRebuild(bool isBegin)
{
	SetValueInt( pb::PLAYER_FIELD_IS_BEGIN_REBUILD, isBegin ? 1 : 0);
}

bool PlayerValueOwner::IsRegistCrossServer()
{
	return GetAttrInt( pb::PLAYER_FIELD_IS_REGIST_CROSS_SERVER ) > 0;
}

void PlayerValueOwner::SetRegistCrossServer( bool rigest )
{
	SetValueInt( pb::PLAYER_FIELD_IS_REGIST_CROSS_SERVER, rigest ? 1 : 0);
}

void PlayerValueOwner::WriteUpdateInfoTo( pb::ObjectValueUpdate& msg ) const
{
	m_values.BuildUpdateMsg(msg);
}

// 机器人客户端用
void PlayerValueOwner::ApplyUpdateInfoFrom( const pb::ObjectValueUpdate& msg )
{
    m_values.ApplyUpdateMsg(msg);
}


bool PlayerValueOwner::IsGM() const
{
	return m_gmLevel != GM_LEVEL_NONE;
}

void PlayerValueOwner::GM_SetLevel( int level )
{
    _SetLevel( level);
	SetCurExp( 0);
}


void PlayerValueOwner::GM_SetVipLevel( int vipLv )
{
#ifdef _MMO_SERVER_
	uint32 vipExp = 0;
	if (sPlayerDataMgr.TryGetVipUpgradeExp(vipLv,vipExp))
	{
		uint32 curVipXp = (uint32)GetVipXP();
		if (vipExp > curVipXp)
		{
			uint32 newXp = vipExp - curVipXp;
			AddVipXp(newXp);
		}
	}
#endif
}


bool PlayerValueOwner::IsTakeVipReward( int vipLv )
{
	int curLv = GetVipLevel();
	if (vipLv <= curLv)
	{
		int curTakeFlag =  GetAttrInt( pb::PLAYER_FIELD_TAKE_VIP_REWARD_FLAG );
		return (curTakeFlag & (1 << vipLv)) != 0;
	}
	else
	{
		return false;
	}
}

bool PlayerValueOwner::TakedVipReward( int vipLv )
{
	int curLv = GetVipLevel();
	if (vipLv <= curLv)
	{
		int curTakeFlag =  GetAttrInt( pb::PLAYER_FIELD_TAKE_VIP_REWARD_FLAG );
		curTakeFlag = curTakeFlag | (1 << vipLv);
		SetValueInt(pb::PLAYER_FIELD_TAKE_VIP_REWARD_FLAG,curTakeFlag);
		return true;
	}
	return false;
}

uint32 PlayerValueOwner::GetBagCapacity()
{
	return GetAttrInt( pb::PLAYER_FIELD_BAG_CAPACITY );
}

void PlayerValueOwner::SetBagCapacity( int num )
{
	ASSERT(num > GetAttrInt( pb::PLAYER_FIELD_BAG_CAPACITY ));
	SetValueInt( pb::PLAYER_FIELD_BAG_CAPACITY, num);
}

uint32 PlayerValueOwner::GetPanZerId()
{
	return GetAttrInt( pb::PLAYER_FIELD_PANZER_ID );
}

void PlayerValueOwner::SetPanZerId( int num )
{
	if (num > 0)
	{
		SetValueInt( pb::PLAYER_FIELD_PANZER_ID, num);
	}
}

void PlayerValueOwner::_SetLevel( int level )
{
    SetValueInt( pb::PLAYER_FIELD_LEVEL, level);
}

//void PlayerValueOwner::_SetIconID(int headid)
//{
//	SetValueInt( pb::PLAYER_FIELD_HEAD_ICON_ID, headid);
//}

void PlayerValueOwner::_SetVipLv( int vipLv )
{
    SetValueInt( pb::PLAYER_FIELD_VIP_LEVEL, vipLv);
}
void PlayerValueOwner::_SetCombatPower( int power )
{
    //SetValueInt( pb::PLAYER_FIELD_COMBAT_POWER, power);
}


void PlayerValueOwner::SetCurExp( int exp )
{
    return SetValueInt( pb::PLAYER_FIELD_CUR_XP, exp);
}

void PlayerValueOwner::SetCurVipExp( int exp )
{
    return SetValueInt( pb::PLAYER_FIELD_CUR_VIP_XP, exp);
}


bool PlayerValueOwner::TestFlag( uint32 flag ) const
{
    uint32 value = GetAttrInt( pb::PLAYER_FIELD_FLAGS) ;
    return Utility::HasFlag( value, flag) ;
}

void PlayerValueOwner::SetFlag( uint32 flag )
{
    uint32 value =GetAttrInt( pb::PLAYER_FIELD_FLAGS) ; 
    Utility::SetFlag( value, flag ) ; 

    SetValueInt( pb::PLAYER_FIELD_FLAGS, value );
}

void PlayerValueOwner::RemoveFlag( uint32 flag )
{
    uint32 value =GetAttrInt( pb::PLAYER_FIELD_FLAGS) ; 
    Utility::RemoveFlag( value, flag ) ; 

    SetValueInt( pb::PLAYER_FIELD_FLAGS, value );
}


void PlayerValueOwner::SetOld()
{
    RemoveFlag( pb::PLAYER_FLAG_IS_NEW );
}

bool PlayerValueOwner::IsNew() const
{
    return TestFlag( pb::PLAYER_FLAG_IS_NEW ) ;
}

time_t PlayerValueOwner::IsInRepeatOperateTime()
{
	time_t curTime = sOS.GetRealTime();
   if ((curTime - m_operateLockTime) > REPEAT_OPERATE_TIMESPACE)
   {
	   m_operateLockTime = curTime;
	   return false;
   }
   else
   {
	   return true;
   }
}

void PlayerValueOwner::SetGuid( uint64 id )
{
    SetValueUint64( pb::PLAYER_FIELD_PLAYER_ID, id);
}

void PlayerValueOwner::SetPVPRank( int rank )
{
   // SetValueInt( pb::PLAYER_FIELD_PVP_RANK, rank);
}

void PlayerValueOwner::SetPVPHistoryRank( int rank )
{
    //SetValueInt( pb::PLAYER_FIELD_PVP_HISTORY_RANK, rank);
}

void PlayerValueOwner::SetLastLoginToNow()
{
    SetValueUint64( pb::PLAYER_FIELD_LAST_LOGIN_TIME, static_cast<uint64>(sOS.GetRealTime()) );
}

int64 PlayerValueOwner::GetLastLogin() const
{
    return static_cast<int64>( GetAttrUint64( pb::PLAYER_FIELD_LAST_LOGIN_TIME));
}

int64 PlayerValueOwner::GetRegistTime() const
{
  return static_cast<int64>( GetAttrUint64( pb::PLAYER_FIELD_REGIST_TIME));
}


void PlayerValueOwner::SetContinLoginDay( int day)
{
    SetValueInt(pb::PLAYER_FIELD_CONTINUOUSLY_LOGIN_DAY, day);
}

void PlayerValueOwner::SetLoginDaysNumber(const int times)
{
	SetValueInt(pb::PLAYER_FIELD_LOGIN_DAYS,times);
}

uint32 PlayerValueOwner::GetLoginDaysNumber() const
{
	return GetAttrUint(pb::PLAYER_FIELD_LOGIN_DAYS);
}

int32 PlayerValueOwner::GetContinLoginDay() const
{
    return GetAttrInt( pb::PLAYER_FIELD_CONTINUOUSLY_LOGIN_DAY );
}

bool PlayerValueOwner::TryAddValueIsInLimit( uint32 index,int val,int limit )
{
	int32 v = GetAttrInt( index) + val;
	return v <= limit;
}

void PlayerValueOwner::AddValueLimitByLimit( uint32 index,int val,int limit )
{
	int32 v = GetAttrInt(index);
	if ( v <= limit )
	{
		int32 w = v + val;
		SetValueInt(index, (std::min)(w, limit) );
	}
}

bool PlayerValueOwner::IsReachLimit( uint32 index,int limit )
{
	return GetAttrInt(index) >= limit;
}

void PlayerValueOwner::SetLastApBounsTime( int64 timeStamp )
{
    //SetValueUint64( pb::PLAYER_FIELD_TAKE_AP_BONUS_TIME, timeStamp);
}

int64 PlayerValueOwner::GetLastApBonusTime() const
{
    return 0;// GetAttrUint64( pb::PLAYER_FIELD_TAKE_AP_BONUS_TIME );
}

uint32 PlayerValueOwner::GetTakeLogin7DayCount() const
{
    return GetAttrInt( pb::PLAYER_FIELD_LOGIN_7_DAY_REWARD_TAKE_COUNT );
}

void PlayerValueOwner::SetTakeLogin7DayCount( uint32 count)
{
    SetValueInt( pb::PLAYER_FIELD_LOGIN_7_DAY_REWARD_TAKE_COUNT , count);
}

uint32 PlayerValueOwner::GetTakeLoginMonthlyCount() const
{
    return GetAttrInt( pb::PLAYER_FIELD_LOGIN_MONTLY_REWARD_TAKE_COUNT );
}

void PlayerValueOwner::SetTakeLoginMonthlyCount( uint32 count)
{
    SetValueInt( pb::PLAYER_FIELD_LOGIN_MONTLY_REWARD_TAKE_COUNT , count);
}

time_t PlayerValueOwner::GetTakeLogin7DayTime() const
{
    return GetAttrUint64( pb::PLAYER_FIELD_LOGIN_7_DAY_REWARD_TAKE_TIME );
}

void PlayerValueOwner::SetTakeLogin7DayTime( time_t takeTime )
{
    SetValueUint64( pb::PLAYER_FIELD_LOGIN_7_DAY_REWARD_TAKE_TIME , takeTime);
}

time_t PlayerValueOwner::GetEquipShopBuyCoolDownTime() const
{
	return GetAttrUint64( pb::PLAYER_FIELD_NEXT_RECOVER_MARKET_BUY_NUM_TIME );
}

void PlayerValueOwner::SetEquipShopBuyCoolDownTime( time_t takeTime )
{
	SetValueUint64( pb::PLAYER_FIELD_NEXT_RECOVER_MARKET_BUY_NUM_TIME , takeTime);
}


time_t PlayerValueOwner::GetTakeLoginMonthlyTime() const
{
    return GetAttrUint64( pb::PLAYER_FIELD_LOGIN_MONTLY_REWARD_TAKE_TIME );
}

void PlayerValueOwner::SetTakeLoginMonthlyTime( time_t takeTime )
{
    SetValueUint64( pb::PLAYER_FIELD_LOGIN_MONTLY_REWARD_TAKE_TIME , takeTime);

}

void PlayerValueOwner::SetTakeNormalMonthCardRewardTime(time_t takeTime)
{
	SetValueUint64(pb::PLAYER_FIELD_TAKE_MONTH_CARD_NORMAL,takeTime);
}

void PlayerValueOwner::SetTakeAdvanceMonthCardRewardTime(time_t takeTime)
{
	SetValueUint64(pb::PLAYER_FIELD_TAKE_MONTH_CARD_ADVANCE,takeTime);
}

time_t PlayerValueOwner::GetTakeNormalMonthCardRewardTime()
{
	 return GetAttrUint64( pb::PLAYER_FIELD_TAKE_MONTH_CARD_NORMAL );
}

time_t PlayerValueOwner::GetTakeAdvanceMonthCardRewardTime()
{
	return GetAttrUint64( pb::PLAYER_FIELD_TAKE_MONTH_CARD_ADVANCE );
}

void PlayerValueOwner::SetBuyNormalMonthCardTime(time_t buyTime)
{
	SetValueUint64(pb::PLAYER_FIELD_BUY_MONTH_CARD_NORMAL,buyTime);
}

void PlayerValueOwner::SetBuyAdvanceMonthCardTime(time_t buyTime)
{
	SetValueUint64(pb::PLAYER_FIELD_BUY_MONTH_CARD_ADVANCE,buyTime);
}

time_t PlayerValueOwner::GetBuyNormalMonthCardTime()
{
	return GetAttrUint64( pb::PLAYER_FIELD_BUY_MONTH_CARD_NORMAL );
}

time_t PlayerValueOwner::GetBuyAdvanceMonthCardTime()
{
	return GetAttrUint64( pb::PLAYER_FIELD_BUY_MONTH_CARD_ADVANCE );
}

uint64 PlayerValueOwner::GetCongratulateActiveTime()
{
	return GetAttrUint64(pb::PLAYER_FIELD_CONGRATULATE_ACTIVE_TIME);
}

void PlayerValueOwner::SetCongratulateActiveTime(uint64 time_date)
{
	SetValueUint64(pb::PLAYER_FIELD_CONGRATULATE_ACTIVE_TIME,time_date);
}

uint32 PlayerValueOwner::GetAutoHammerCount() const
{
	return GetAttrInt( pb::PlAYER_FIELD_AUTO_HAMMER );
}

void PlayerValueOwner::SetAutoHammerCount( uint32 count)
{
	SetValueInt( pb::PlAYER_FIELD_AUTO_HAMMER , count);
}

uint32 PlayerValueOwner::GetGoldHammerCount() const
{
 return GetAttrInt( pb::PLAYER_FIELD_GOLDHAMMER );
}

void PlayerValueOwner::SetGoldHammerCount( uint32 count )
{
	SetValueInt( pb::PLAYER_FIELD_GOLDHAMMER , count);
}

uint32 PlayerValueOwner::GetHerosExpUpPercent()
{
	return GetAttrInt(pb::PLYAER_FIELD_HEROS_EXPUP_PERCENT);
}

void PlayerValueOwner::SetHerosExpIpPercent(uint32 expPer)
{
	SetValueInt(pb::PLYAER_FIELD_HEROS_EXPUP_PERCENT,expPer);
}
int PlayerValueOwner::GenPrebattleID()
{
    SetValueInt( pb::PlAYER_FIELD_PRE_BATTLE_ID, System::Rand(1, 1000000) );
    return GetAttrInt( pb::PlAYER_FIELD_PRE_BATTLE_ID);
}

bool PlayerValueOwner::IsMatchPreBattleId( int id ) const
{
    return GetAttrInt( pb::PlAYER_FIELD_PRE_BATTLE_ID) == id;
}

void PlayerValueOwner::ResetPrebattleId()
{
    SetValueInt( pb::PlAYER_FIELD_PRE_BATTLE_ID, 0 );
}

uint32 PlayerValueOwner::GetPlayerExpUpPercent()
{
	return 0;
	///return GetAttrInt(pb::PLAYER_FIELD_PLAYER_EXPUP_PERCENT);
}

void PlayerValueOwner::SetPlayerExpUpPercent(uint32 expPer)
{
	//SetValueInt(pb::PLAYER_FIELD_PLAYER_EXPUP_PERCENT,expPer);
}

void PlayerValueOwner::AddXPByLimit( uint32 number,uint32 limit )
{
	AddTodayXP(number);
	if(GetTodayXP() <= limit)
	{
		AddXP(number);
	}
}

void PlayerValueOwner::AddTodayXP( uint32 number )
{
	int32 safeVal = MultiValueArray::ModifyInt( GetAttrInt( pb::PLAYER_FIELD_TODAY_ADD_XP), number ) ;
	SetValueInt(pb::PLAYER_FIELD_TODAY_ADD_XP,safeVal);
}

uint32 PlayerValueOwner::GetTodayXP()
{
	return GetAttrInt(pb::PLAYER_FIELD_TODAY_ADD_XP);
}

void PlayerValueOwner::resetTodayAddXP()
{
	SetValueInt(pb::PLAYER_FIELD_TODAY_ADD_XP,0);
}

uint32 PlayerValueOwner::GetDailyQuestScore() const
{
	return GetAttrUint(pb::PLAYER_FIELD_DAILY_QUEST_SCORE);
}

void PlayerValueOwner::AddDailyQuestScore(int value)
{
	if(value >= 0)
	{
		int32 safeVal = MultiValueArray::ModifyInt( GetAttrInt( pb::PLAYER_FIELD_DAILY_QUEST_SCORE), value ) ;
		SetValueInt(pb::PLAYER_FIELD_DAILY_QUEST_SCORE,safeVal);
	}
}



uint32 PlayerValueOwner::GetActivityStarBoxLevel()
{
	return GetAttrUint(pb::PLAYER_FIELD_ACTIVITY_STAR_BOX_LEVEL);
}

uint32 PlayerValueOwner::GetTodayContinueOccupyCity()
{
	return GetAttrUint(pb::PLAYER_FIELD_TODAY_CONTINUE_OCCPUY_CITY);
}

void PlayerValueOwner::SetEnterWorldTime( time_t time_enter )
{
	SetValueUint64(pb::PLAYER_FIELD_ENTER_WORLD_TIME,time_enter);
}

time_t PlayerValueOwner::GetEnterWorldTime()
{
	return GetAttrUint64(pb::PLAYER_FIELD_ENTER_WORLD_TIME);
}

uint32 PlayerValueOwner::GetExploitValue()
{
	return GetAttrUint(pb::PLAYER_FIELD_EXPLOIT_RANK_VALUE);
}

uint32 PlayerValueOwner::GetTrickValue()
{
	return GetAttrUint(pb::PLAYER_FIELD_TRICK_RANK_VALUE);
}

uint32 PlayerValueOwner::GetOccupyValue()
{
	return GetAttrUint(pb::PLAYER_FIELD_OCCUPY_RANK_VALUE);
}

uint32 PlayerValueOwner::GetAttackCityValue()
{
	return 0;
}

void PlayerValueOwner::SetExploitValue(int value)
{
	SetValueInt(pb::PLAYER_FIELD_EXPLOIT_RANK_VALUE,value);
}

void PlayerValueOwner::SetTrickValue(int value)
{
	SetValueInt(pb::PLAYER_FIELD_TRICK_RANK_VALUE,value);
}

void PlayerValueOwner::SetOccupyValue(int value)
{
	SetValueInt(pb::PLAYER_FIELD_OCCUPY_RANK_VALUE,value);
}

void PlayerValueOwner::setCurrRewardCharId(const int char_id)
{
	SetValueInt(pb::PLAYER_FIELD_CURR_REWARD_CHAR_ID,char_id);
}

uint32 PlayerValueOwner::getCurrRewardCharId()
{
	return GetAttrUint(pb::PLAYER_FIELD_CURR_REWARD_CHAR_ID);
}
