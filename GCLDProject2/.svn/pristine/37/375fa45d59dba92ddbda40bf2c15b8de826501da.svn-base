#include "CharacterProto.h"

#include "datastruct/struct_character.h"
#include "def/MmoAssert.h"  
#include "def/ObjectDefines.h"
#include "utility/Utility.h"
#include "Logger.h"


#include "BaseDefineMgr.h"
#include "BaseDefine.pb.h"
using namespace pb;


CharacterProto::CharacterProto( 
    const DB_CharProto& dbCharProto,
    const DB_CharUpgradeXP* dbCharUpgradeXp 
    )
: m_dbCharProto( dbCharProto)
, m_dbCharUpgradeXp( dbCharUpgradeXp)
{
    
}

uint32 CharacterProto::CharacterID() const
{
    return m_dbCharProto.index;
}

uint32 CharacterProto::GetRow() const
{
	return m_dbCharProto.char_row;
}

const string& CharacterProto::GetEnableTactics() const
{
	return m_dbCharProto.teshuzhanshu;
}

int32 CharacterProto::GetBirthLevel() const
{
    return static_cast<int32>( m_dbCharProto.char_lv);
}

int32 CharacterProto::GetBirthQuality() const
{
    return static_cast<int32>( m_dbCharProto.char_quality);
}

int32 CharacterProto::GetBirthReinForce() const
{
    return static_cast<int32>( m_dbCharProto.char_reinforce);
}

int32 CharacterProto::GetKeZhiJiaCheng1() const
{
	return m_dbCharProto.kezhijiacheng1;
}

int32 CharacterProto::GetKeZhiJiaCheng2() const
{
	return m_dbCharProto.kezhijiacheng2;
}

int32 CharacterProto::GetKeZhiJiaCheng3() const
{
	return m_dbCharProto.kezhijiacheng3;
}

string CharacterProto::GetTerrTalent() const
{
	return m_dbCharProto.tian_fu;
}
double CharacterProto::GetTerrTalentBuff() const
{
	return m_dbCharProto.tian_fu_buff;
}

bool CharacterProto::TryGetUpgradeXp( int32 curLevel , int32& outUpgradeXp) const
{
    if (m_dbCharUpgradeXp &&  static_cast<uint32>( curLevel - 1 ) < MAX_CHARACTER_LEVEL - 1)
    {
        outUpgradeXp =  static_cast<int32>( m_dbCharUpgradeXp->xp_value[curLevel - 1]);
        return true;
    }
    return false;    
}


bool CharacterProto::TryAddXp( int32& curLv, int32& curXp, int32 xpAdd ,int32 maxLv) const
{
	MMO_ASSERT(curXp >=0);

    if ( curLv == maxLv)
    {
        return false;
    }

    int newLv = curLv;	//当前等级
    int newXp = curXp;	//当前经验
    newXp += xpAdd;		//总经验

	int nowMaxHP = 0;
	if(TryGetUpgradeXp( curLv, nowMaxHP))	//当前等级最大经验值
	{
		if(curLv == maxLv && (nowMaxHP-1) == curXp)		//当前等级与经验均到达最大，则不可提升经验
			return false;
	}

    while(true)
	{
		int upgradeXP = 0;	//当前等级升级所需要的经验
		if ( !TryGetUpgradeXp( newLv, upgradeXP))	//没有获取到当前等级所需经验值
		{
			curLv = newLv;
			curXp = (newXp>xpAdd)? (newXp-xpAdd):0;	//经验有没有提供升级，如果升级了则剩余经验为0，没有则为原经验
			break;
		}

		bool breakRet = false;
        if (newLv >= maxLv)		//等级超过上限
        {
			curLv = newLv;
			curXp = (newXp>=upgradeXP)? (upgradeXP-1):newXp;	//如果超过所需经验，则经验填满
			break;
        }
        
        if (newXp < upgradeXP)	//剩余经验不足以升级
		{
			curLv = newLv;
			curXp = newXp;
			break;
        }

        newXp -= upgradeXP;	//如果成功，则剩余经验扣除升级所需经验，等级提升一级
        newLv ++;
    }
    
    return true;
}

uint32 CharacterProto::CharClass() const
{
    return 1;//m_dbCharProto.class_option;
}

uint32 CharacterProto::CharType() const
{
	return m_dbCharProto.type;
}

int32 CharacterProto::GetCharacterLvUpCoin( int32 curLv ) const
{
    static float xp2coin = GET_BASE_DEF_FLOAT( BD_CHAR_LEVEL_UP_XP_TO_COIN);
    static uint32 roundNumber = GET_BASE_DEF_UINT( BD_ROUNDING_NUMBER);
    int32 xp = 0;
    TryGetUpgradeXp(curLv, xp);
    int32 need_coin = int32( Utility::Rounding( (uint32)(xp * xp2coin) , roundNumber) );
    return need_coin;
}

int32 CharacterProto::GetCharacterLvUpCoin( int32 fromLv, int32 toLv ) const
{
    MMO_ASSERT( toLv >= fromLv);
    int32 costCoin = 0;
    for ( int32 i = fromLv; i != toLv; ++i)
    {
        costCoin += GetCharacterLvUpCoin( i);
    }
    return costCoin;
}

int32 CharacterProto::GetCharacterQualityUpCoin( int32 curQuality ) const
{
    return int32(
        Utility::Rounding(
            (uint32)(powf( (float)curQuality, 2) * GET_BASE_DEF_FLOAT(pb::BD_CHAR_UPGRADE_QUALITY_COIN)),
            (uint32)(GET_BASE_DEF_UINT( (uint32)BD_ROUNDING_NUMBER))
            )
        );
}

int32 CharacterProto::GetCharacterQualityUpCoin( int32 fromQua, int32 toQua ) const
{
    MMO_ASSERT( toQua >= fromQua);
    int32 costCoin = 0;
    for ( int32 i = fromQua; i != toQua; ++i)
    {
        costCoin += GetCharacterQualityUpCoin( i);
    }
    return costCoin;
}

int32 CharacterProto::GetCharacterReinforceUpCoin( int32 from, int32 to ) const
{
    MMO_ASSERT( from <= to);
    int32 costCoin = 0;
    for ( int32 i = from; i != to; ++i)
    {
        costCoin += GetCharacterReinforceUpCoin( i);
    }
    return costCoin;
}

int32 CharacterProto::GetCharacterReinforceUpCoin( int32 curReinforce ) const
{
    return int32(Utility::Rounding( curReinforce * GET_BASE_DEF_UINT( BD_CHAR_UPGRADE_REINFORCE_COIN), GET_BASE_DEF_UINT( BD_ROUNDING_NUMBER)));
}

const std::vector<uint32>* CharacterProto::GetSkillUnLockIds( uint32 quality ) const
{
    LearnSkillMap::const_iterator it = m_learnSkillMap.find(quality);
    if ( it != m_learnSkillMap.end())
    {
        return &(it->second);
    }
    else{
        return NULL;    
    }
}

const DB_CharSetSkills* CharacterProto::GetSkillInfo( uint32 index ) const
{
    Index2Skill::const_iterator it = m_index2skill.find(index);
    if ( it != m_index2skill.end())
    {
        return it->second;
    }
    return NULL;
}


const DB_CharProto&  CharacterProto::GetCharProto( void ) const
{
	return m_dbCharProto ;
}

bool CharacterProto::IsCharTypeHero() const
{
    return true;// m_dbCharProto.type == DB_CharProto::CHAR_TYPE_HERO;
}


int32 CharacterProto::GetZhaoMuCost() const
{
	return static_cast<int32>( m_dbCharProto.choose_cost);
}