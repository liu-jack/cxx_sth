#include "PlayerDataManager.h"

#include "DbTool.h"
#include "mysqldata.h"

#include "BaseDefineMgr.h"
#include "BaseDefine.pb.h"
#include "Enum.pb.h"
#include "../Trigger/trigger.h"
#include "utility/Utility.h"
using namespace pb;


void PlayerDataMgr::Init()
{
	m_maxVipLevel = 0;
    m_basePlayerVec.reserve( 100);
    FOREACH_DB_ITEM( base_player, DB_BasePlayer)
    {
        if (m_basePlayerVec.size() <= base_player->level)
        {
            m_basePlayerVec.resize( base_player->level + 1);
        }
        m_basePlayerVec[ base_player->level] = base_player;
		IntPairVec vec;
		Utility::SplitStr2(base_player->level_up_reward, vec);
		m_basePlayerRewardMap.insert(std::pair<uint32,IntPairVec>(base_player->level,vec));
    }
    MMO_ASSERT( GetMaxLevel() + 1 == m_basePlayerVec.size());

    FOREACH_DB_ITEM( base_vip, DB_VipLevelPro)
    {
        m_baseVipMap[ base_vip->vip_level] = base_vip;
		ParsserPrivilege(base_vip->reward01_type,base_vip->reward01_id,base_vip->reward01_num,m_priVipMap);
		ParsserPrivilege(base_vip->reward02_type,base_vip->reward02_id,base_vip->reward02_num,m_priVipMap);
		ParsserPrivilege(base_vip->reward03_type,base_vip->reward03_id,base_vip->reward03_num,m_priVipMap);
		if (m_maxVipLevel < base_vip->vip_level)
		{
			m_maxVipLevel = base_vip->vip_level;
		}
    }

	FOREACH_DB_ITEM( HeadIcon, DB_HeadIconUnlock)
	{
		m_headIconUnlockReqMap[ HeadIcon->role_image_id] = HeadIcon;
	}
	FOREACH_DB_ITEM( month_card, DB_MonthCard)
	{
		m_monthCardMap[month_card->id] = month_card;
	}

	FOREACH_DB_ITEM( ResourceItem, DB_ResourceInit)
	{
		m_InitResourceMap[ResourceItem->item_id] = ResourceItem->init_num;
	}

	FOREACH_DB_ITEM( rewadItem, DB_FirstPay)
	{
		m_firstPayVec.push_back(rewadItem);
	}
	InitClientCrossWhiteList();
	InitClientGameWhiteList();
	InitGameClientBlackList();
}

void PlayerDataMgr::InitClientCrossWhiteList()
{
	m_wCOpCodeList.push_back(CMSG_CHAT_MESSAGE);
	m_wCOpCodeList.push_back(CMSG_ALL_CHARACTER_INFO);
	m_wCOpCodeList.push_back(CMSG_HERO_MOVE);
	m_wCOpCodeList.push_back(CMSG_STOP_HERO_MOVE);
	m_wCOpCodeList.push_back(CMSG_CONSCRIPTION);
	m_wCOpCodeList.push_back(CMSG_PLAYER_VALUE_UPDATE);
	m_wCOpCodeList.push_back(CMSG_CHARACTER_UPDATE);
	m_wCOpCodeList.push_back(CMSG_COMBAT_BEGIN);
	m_wCOpCodeList.push_back(CMSG_COMBAT_REQUEST_NEXT_LST);
	m_wCOpCodeList.push_back(CMSG_COMBAT_ANI_FINISHED);
	m_wCOpCodeList.push_back(CMSG_COMBAT_SELECT_TACTIC);
	m_wCOpCodeList.push_back(CMSG_COMBAT_UNWATCH);
	m_wCOpCodeList.push_back(CMSG_COMBAT_ATTRACT_ENEMY);
	m_wCOpCodeList.push_back(CMSG_COMBAT_RUSH);
	m_wCOpCodeList.push_back(CMSG_COMBAT_RUNAWAY);
	m_wCOpCodeList.push_back(CMSG_COMBAT_ADD_SOLDIER);
	m_wCOpCodeList.push_back(CMSG_COMBAT_CALL_REQUEST);
	m_wCOpCodeList.push_back(CMSG_COMBAT_TRY);
	m_wCOpCodeList.push_back(CMSG_AUTO_COMBAT);
	m_wCOpCodeList.push_back(CMSG_GET_HERO_STATUS);
	m_wCOpCodeList.push_back(CMSG_COMBAT_REQUEST_TERR);
	m_wCOpCodeList.push_back(CMSG_DONATION_GOODS);
	m_wCOpCodeList.push_back(CMSG_CITY_ALL_INFO);
	m_wCOpCodeList.push_back(CMSG_REQUEST_FIGHT_LST);
	m_wCOpCodeList.push_back(CMSG_ENTER_CROSS_WAR);
	m_wCOpCodeList.push_back(CMSG_COUNTRY_INFO);
	m_wCOpCodeList.push_back(CMSG_QUIT_CROSS_WAR);
	m_wCOpCodeList.push_back(CMSG_DONATION_GOODS_INFOS);
	m_wCOpCodeList.push_back(CMSG_CROSS_ATTACK_CAPITAL);
	m_wCOpCodeList.push_back(CMSG_CROSS_USE_TROOPS);
	m_wCOpCodeList.push_back(CMSG_CROSS_TAKE_WAR_REWARD);
	m_wCOpCodeList.push_back(CMSG_CROSS_COUNTRY_INFO);
	m_wCOpCodeList.push_back(CMSG_CROSS_WAR_STATE);
	m_wCOpCodeList.push_back(CMSG_CROSS_ALL_DESTROY_TASK_INFO);
	m_wCOpCodeList.push_back(CMSG_CROSS_TAKE_DESTROY_TASK_AWARD);
	m_wCOpCodeList.push_back(CMSG_CROSS_KILL_TASK_INFO);
	m_wCOpCodeList.push_back(CMSG_CROSS_TAKE_KILL_TASK_AWARD);
	m_wCOpCodeList.push_back(CMSG_CROSS_ALL_AMMO_INFO);
	m_wCOpCodeList.push_back(CMSG_CROSS_BUY_AMMO);
	m_wCOpCodeList.push_back(CMSG_CROSS_KILL_ENEMY_RANK);
	m_wCOpCodeList.push_back(CMSG_CROSS_USE_ATTACK_HUMMER);
	m_wCOpCodeList.push_back(CMSG_SEIGE_FORCE_ALL_INFO);
	m_wCOpCodeList.push_back(CMSG_SEIGE_FORCE_USE);
}

void PlayerDataMgr::InitClientGameWhiteList()
{
	m_wGopCodeList.push_back(CMSG_MARKET_ALL_INFO);
	m_wGopCodeList.push_back(CMSG_MARKET_BUY);
}

void PlayerDataMgr::InitGameClientBlackList()
{
	m_bOpCodeList.push_back(SMSG_CHARACTER_UPDATE);
	m_bOpCodeList.push_back(SMSG_HERO_MOVE);
	m_bOpCodeList.push_back(SMSG_HERO_POS);
	m_bOpCodeList.push_back(SMSG_COMBAT_HERO_STATUS);
}


bool PlayerDataMgr::CheckClientCrossWhiteList(pb::Opcode opcode)
{
	for(std::vector<pb::Opcode>::iterator it = m_wCOpCodeList.begin(); it != m_wCOpCodeList.end(); ++it)
	{
		if(*it == opcode)
			return true;
	}
	return false;
}

bool PlayerDataMgr::CheckClientGameWhiteList(pb::Opcode opcode)
{
	for(std::vector<pb::Opcode>::iterator it = m_wGopCodeList.begin(); it != m_wGopCodeList.end(); ++it)
	{
		if(*it == opcode)
			return true;
	}
	return false;
}

bool PlayerDataMgr::CheckGameClientBlackList(pb::Opcode opcode)
{
	for(std::vector<pb::Opcode>::iterator it = m_bOpCodeList.begin(); it != m_bOpCodeList.end(); ++it)
	{
		if(*it == opcode)
			return false;
	}
	return true;
}


const DB_HeadIconUnlock* PlayerDataMgr::GetHeadIconUnLock(uint32 headIconId) 
{
	IconId2UnLockReq::const_iterator it = m_headIconUnlockReqMap.find(headIconId);
	if ( it != m_headIconUnlockReqMap.end())
	{
		return it->second;
	}
	return NULL;
}

bool PlayerDataMgr::TryGetUpgradeExp( uint32 level, uint32& out_exp ) const
{
    if ( level >= m_basePlayerVec.size())
        return false;

    const DB_BasePlayer* player = m_basePlayerVec[ level];
    if ( player)
    {
        out_exp = player->xp;
        return true;
    }
    return false;
}



bool PlayerDataMgr::TryGetVipUpgradeExp( uint32 level, uint32& out_value ) const
{
	VipMap::const_iterator vipIter =   m_baseVipMap.find(level);
	if (vipIter == m_baseVipMap.end())
	{
		return false;
	}
	const DB_VipLevelPro* vip = vipIter->second;
    if ( vip && vip->vip_exp)
    {
        out_value = vip->vip_exp;
        return true;
    }
    return false;   
}



uint32 PlayerDataMgr::GetMaxLevel() const
{
    if ( m_basePlayerVec.empty())
    {
        return 1;
    }
    else
        return (m_basePlayerVec.back())->level;
        
}

uint32 PlayerDataMgr::GetMaxVipLevel() const
{
    return  m_maxVipLevel; 
}

uint32 PlayerDataMgr::GetVipExp( uint32 viplv ) const
{
	VipMap::const_iterator vipIter =   m_baseVipMap.find(viplv);
	if (vipIter == m_baseVipMap.end())
	{
		return 0;
	}
	const DB_VipLevelPro* vip = vipIter->second;
	return vip->vip_exp;
}

bool PlayerDataMgr::GetRewardItems( uint32 viplv, IntV3Vec& retMap ) const
{
	VipMap::const_iterator vipIter =   m_baseVipMap.find(viplv);
	if (vipIter == m_baseVipMap.end())
	{
		return false;
	}
	const DB_VipLevelPro* vip = vipIter->second;
	bool isGet = false;
	isGet = ParsserItems(vip->reward01_type,vip->reward01_id,vip->reward01_num,retMap) ? true : isGet;
	isGet = ParsserItems(vip->reward02_type,vip->reward02_id,vip->reward02_num,retMap) ? true : isGet;
	isGet = ParsserItems(vip->reward03_type,vip->reward03_id,vip->reward03_num,retMap) ? true : isGet;
	return isGet;
}

bool PlayerDataMgr::GetPrivilege( uint32 viplv,std::map<int,int>& retMap ) const
{
	VipMap::const_iterator vipIter =   m_baseVipMap.find(viplv);
	if (vipIter == m_baseVipMap.end())
	{
		return false;
	}
	const DB_VipLevelPro* vip = vipIter->second;
	bool isGet = false;
	isGet = ParsserPrivilege(vip->reward01_type,vip->reward01_id,vip->reward01_num,retMap) ? true : isGet;
	isGet = ParsserPrivilege(vip->reward02_type,vip->reward02_id,vip->reward02_num,retMap) ? true : isGet;
	isGet = ParsserPrivilege(vip->reward03_type,vip->reward03_id,vip->reward03_num,retMap) ? true : isGet;
	return isGet;
}

int PlayerDataMgr::GetPrivilegeValue(int priId ) const
{
	std::map< int, int>::const_iterator IterPri = m_priVipMap.find(priId);
	if (IterPri != m_priVipMap.end())
	{
		return IterPri->second;
	}
	return 0;
}

bool PlayerDataMgr::GetFirstPayRewards( IntV3Vec& itemV,std::map<int,int>& priMap ) const
{
	bool isGet = false;
	FirstPayVec::const_iterator firPayIter = m_firstPayVec.begin();
	for (; firPayIter != m_firstPayVec.end(); ++firPayIter)
	{
		const DB_FirstPay* curReward = (*firPayIter);
		if (curReward->reward01_type == 1)
		{
			isGet = ParsserPrivilege(curReward->reward01_type,curReward->reward01_id,curReward->reward01_num,priMap) ? true : isGet;
		}
		else if (curReward->reward01_type == 2)
		{
			isGet = ParsserItems(curReward->reward01_type,curReward->reward01_id,curReward->reward01_num,itemV) ? true : isGet;
		}
	}
	return isGet;
}



int PlayerDataMgr::getMonthCardRewardAndLastDays(const uint32 cardType,IntPairVec& vec,uint32& lastDays)
{
	MonthCardMap::const_iterator iter = m_monthCardMap.find(cardType);
	if(iter != m_monthCardMap.end())
	{
		Utility::SplitStr2(iter->second->award,vec);
		lastDays = iter->second->time;
		return 0;
	}
	return -1;
}

bool PlayerDataMgr::IsReachTrainingAddtionRequire( int vipLv, int lv, uint32 addtion ) const
{
    if ( addtion != 1 && addtion != 5)
    {
        return false;
    }
    if ( addtion == 1 )
    {
        if ( lv >= GET_BASE_DEF_INT( BD_UNLOCK_PLAYER_FUNC_CHAR_RANDOM_1) )
        {
            return true;
        }
    }
    if ( addtion == 5 )
    {
        if ( vipLv >= GET_BASE_DEF_INT( BD_UNLOCK_VIP_FUNC_CHAR_RANDOM_5) )
        {
            return true;
        }
    }
    return false;
}

bool PlayerDataMgr::IsUnlockSweep( int lv ) const
{
    return lv >= GET_BASE_DEF_INT( BD_UNLOCK_PLAYER_FUNC_SWEEP);
}

bool PlayerDataMgr::IsUnlockCharReinforce( int lv ) const
{
    return lv >= GET_BASE_DEF_INT( BD_UNLOCK_PLAYER_FUNC_CHAR_REINFORCE);
}

bool PlayerDataMgr::IsUnlockEquipReinforce( int lv ) const
{
    return lv >= GET_BASE_DEF_INT( BD_UNLOCK_PLAYER_FUNC_EQUIP_REINFORCE);
}

bool PlayerDataMgr::IsUnlockAccReinforce( int lv ) const
{
    return lv >= GET_BASE_DEF_INT( BD_UNLOCK_PLAYER_FUNC_ACC_REINFORCE);
}

bool PlayerDataMgr::IsUnlockPVPArena( int lv ) const
{
    return lv >= GET_BASE_DEF_INT( BD_UNLOCK_PLAYER_FUNC_PVP_ARENA);
}

bool PlayerDataMgr::IsUnlockPVPAcc( int lv ) const
{
    return lv>= GET_BASE_DEF_INT( BD_UNLOCK_PLAYER_FUNC_PVP_ACC);
}

bool PlayerDataMgr::IsUnlockPVEInfinity( int lv ) const
{
    return lv >= GET_BASE_DEF_INT( BD_UNLOCK_PLAYER_FUNC_PVE_INFINITY);
}

bool PlayerDataMgr::IsUnlockPVEGold( int lv ) const
{
    return lv >= GET_BASE_DEF_INT( BD_UNLOCK_PLAYER_FUNC_PVE_GOLD);
}

bool PlayerDataMgr::GetRewardByLevel( uint32 level ,IntPairVec* vec) const
{
	BasePlayerRewardMap::const_iterator iter = m_basePlayerRewardMap.find(level);
	if(iter != m_basePlayerRewardMap.end())
	{
		*vec = iter->second;
		return true;
	}
	return false;
}

bool PlayerDataMgr::ParsserItems( uint32 rType, const std::string& rId, const std::string& rNum,IntV3Vec& retMap ) const
{
	if (rType == 2)
	{
		std::vector<string> retVecId;
		std::vector<int> retVecNum;
		Utility::SplitStr(rId, retVecId, ',');
		Utility::SplitStr(rNum, retVecNum, ',');
		int idCount = (int)retVecId.size();
		int numCount = (int)retVecNum.size();
		if (idCount == 0 || numCount == 0 || idCount != numCount)
		{
			return false;
		}
		for (int i=0; i< idCount; i++)
		{
			IntV3 tmp;
			std::string curItem  = retVecId[i];
			std::vector<string> retItemAttr;
			Utility::SplitStr(curItem, retItemAttr, '|');
			int nAttr = (int)retItemAttr.size();
			if (nAttr == 1)
			{
				tmp.first = atoi(retItemAttr[0].c_str());
				tmp.third = "0";
			}
			else if (nAttr == 2)
			{
				tmp.first = atoi(retItemAttr[0].c_str());
				tmp.third = retItemAttr[1];
			}
			else
			{
				continue;
			}
			tmp.second = retVecNum[i];
			retMap.push_back(tmp);
		}
		return true;
	}
	return false;
}

bool PlayerDataMgr::ParsserPrivilege( uint32 rType, const std::string& rId, const std::string& rNum,std::map<int,int>& retMap ) const
{
	if (rType == 1)
	{
		std::vector<int> retVecId;
		std::vector<int> retVecNum;
		Utility::SplitStr(rId, retVecId, ',');
		Utility::SplitStr(rNum, retVecNum, ',');
		int idCount = (int)retVecId.size();
		int numCount = (int)retVecNum.size();
		if (idCount == 0 || numCount == 0 || idCount != numCount)
		{
			return false;
		}
		bool hasPri = false;
		for (int i=0; i< idCount; i++)
		{
			int priId = retVecId[i];
			uint32 priflag = 0;
			switch(priId)
			{
			case 1:
				priflag = pb::PLAYER_GET_AUTO_BUILD;
				break;
			case 2:
				priflag = pb::PLAYER_GET_TROOPS_ODER_EVERYDAY;
				break;
			case 3:
				priflag = pb::PLAYER_OPEN_ADVANCED_RES_FUBEN;
				break;
			case 4:
				priflag = pb::PLAYER_UP_KILL_ENEMY_HORNOR;
				break;
			case 5:
				priflag = pb::PLAYER_UP_RES_CAPCITY;
				break;
			case 6:
				priflag = pb::PLAYER_UP_WORD_MAP_MOVE_SPEED;
				break;
			case 7:
				priflag = pb::PLAYER_UP_COUNTRY_WAR_TIME;
				break;
			case 8:
				priflag = pb::PLAYER_CAN_FAR_TRIGE_PERSONAL_EVENT;
				break;
			case 9:
				priflag = pb::PLAYER_NO_SHOP_COOL_DOWN_TIME;
				break;
			case 10:
				priflag = pb::PLAYER_CRUSADE_ADD_HERO_TIMES;
				break;
			}
			if (priflag > 0)
			{
				retMap[priflag] = retVecNum[i];
				hasPri = true;
			}
		}
		return hasPri;
	}
	return false;
}
