#include "PlayerDungeonLog.h"
#include "Table/DungeonTableMgr.h"
#include "datastruct/struct_dungeon.h"
#include "OS.h"
#include "../reward/reward.h"
#include "../object/Player.h"
#include "Dungeon.pb.h"
#include "Opcode.pb.h"
#include "../character/Character.h"
#include "System.h"
#include <math.h>
#include "../CrossLogic/PlayerDungeonLogDB.h"
#include "../character/CharacterStorage.h"
#include "../session/PlayerPool.h"
#include "Technology/PlayerTechnology.h"
#include "Technology/Table/TechnologyTableMgr.h"
#include "Palace/PalaceMgr.h"
#ifdef _MMO_SERVER_
//#include "../GameServer.h"
#include "../AutomaticUpdate/AutoUpdateGame.h"
#endif
#include "Combat/CombatObj.h"
#include "BaseDefine.pb.h"

static const int Escape_Base_Time = 30;
static const int Free_TempCaneLv_Last_Time = 3600 * 24 * 3;
static const int Gold_TempCaneLv_Last_Time = 3600 * 24;

PlayerDungeonLog::PlayerDungeonLog()
{
    //memset(&m_DungeonLv, 0, sizeof(*this) - MMO_OFFSET(PlayerDungeonLog, m_DungeonLv));
	m_DungeonLv = 0;
	m_SilverCoinForBuild = 0; //建造时累计的银币
	m_CaneLv = 1;
	m_CaneExp = 0;
	m_freeTryCane = 0; //免费试用笞杖
	m_TempCaneLv = 0;
	m_TempCaneLvEndTime = 0;
	m_CatchHeroTime = 0;
	m_TodayCatchHeroCnt = 0;  //今天抓了几个
	m_CatchHeroCntForExp = 0; //连续抓了几个人

	//统计数据，无需入库，仅界面显示用
	m_TodayCaneCnt = 0;
	m_TodayCaneExp = 0;
	m_CaneTime = 0;
}
void PlayerDungeonLog::SaveTo(pb::GS2C_Dungeon_Base& msg)
{
    if (!sOS.IsToday(m_CaneTime)) {
        m_TodayCaneCnt = 0;
        m_TodayCaneExp = 0;
    }

    msg.set_dngeon_levle(m_DungeonLv);
    msg.set_silver_for_build(m_SilverCoinForBuild);
    msg.set_cane_levle(m_CaneLv);
    msg.set_cane_exp(m_CaneExp);
    msg.set_free_try_cane(m_freeTryCane);
    msg.set_cane_temp_levle(m_TempCaneLv);
    msg.set_cane_temp_time(m_TempCaneLvEndTime);
    msg.set_today_catch_hero_cnt(m_TodayCatchHeroCnt);
    msg.set_catch_hero_cnt_for_exp(m_CatchHeroCntForExp);
    msg.set_catch_hero_time(m_CatchHeroTime);
    msg.set_today_cane_cnt(m_TodayCaneCnt);
    msg.set_today_cane_get_exp(m_TodayCaneExp);
}
void PlayerDungeonLog::SaveTo(pb::GS2C_Dungeon_All_Info& msg)
{
	//PLOG("GS2C_Dungeon_All_Info");
    SaveTo(*msg.mutable_base_info());

    for (vector<CatchHero>::iterator it = m_HeroLst.begin(); it != m_HeroLst.end(); ++it)
    {
        pb::CatchHero* hero = msg.add_catch_hero();
        hero->set_player_id(it->playerId);
        hero->set_hero_tid(it->heroTid);
        hero->set_is_caned(it->isCaned);
		hero->set_isshadow(it->isShadow);
		hero->set_hero_lv(it->level);

        if (Player* player = sPlayerPool.GetByPlayerId(it->playerId))
        {
            if (Character* ptr = player->m_characterStorage->GetCharacterByTableId(it->heroTid))
            {
				hero->set_hero_lv(ptr->GetLevel());
				hero->set_escape_end_time(ptr->_GetEscapeTime());
            }
            hero->set_player_name(player->CharName());
            hero->set_country(player->GetCountryId());
        }
		//PLOG("send all info playerid=%lld, heroid=%d, isshadow=%d",it->playerId,it->heroTid,it->isShadow);
    }
}
void PlayerDungeonLog::LoadFrom(const pb::GS2C_Dungeon_All_Info& msg)
{
    const pb::GS2C_Dungeon_Base& baseInfo = msg.base_info();
    m_DungeonLv = baseInfo.dngeon_levle();
    m_SilverCoinForBuild = baseInfo.silver_for_build();
    m_CaneLv = baseInfo.cane_levle();
	if(m_CaneLv<1)
		m_CaneLv = 1;
    m_CaneExp = baseInfo.cane_exp();
    m_TempCaneLv = baseInfo.cane_temp_levle();
	m_TempCaneLvEndTime = baseInfo.cane_temp_time();
    m_TodayCatchHeroCnt = baseInfo.today_catch_hero_cnt();
    m_CatchHeroCntForExp = baseInfo.catch_hero_cnt_for_exp();
    m_CatchHeroTime = baseInfo.catch_hero_time();

    CatchHero tmp;
    for (int i = 0; i < (int)msg.catch_hero_size(); ++i)
    {
        tmp.playerId = msg.catch_hero(i).player_id();
        tmp.heroTid = msg.catch_hero(i).hero_tid();
		tmp.isCaned = msg.catch_hero(i).is_caned();
		tmp.isShadow = false;
        m_HeroLst.push_back(tmp);
    }
}

void PlayerDungeonLog::LevelUp(Player& player)
{
    if (m_DungeonLv >= (int)sDungeonTableMgr.GetMaxDungeonLv())
    {
        return;
    }
    if (const DB_Dungeon* table = sDungeonTableMgr.GetDungeon(m_DungeonLv))
    {
        if (player.GetLevel() < (int32)table->need_level)
        {
            return;
        }
        if (sReward.Change(player, Reward::Coupon, -(int)table->up_cost))
        {
            ++m_DungeonLv;
            PlayerDungeonLogDB::SendBaseInfoToDb(player, *this);
        }
    }
}
void PlayerDungeonLog::Build_Token(Player& player)
{
    if (m_DungeonLv == 0)
    {
        const DB_Dungeon* table = sDungeonTableMgr.GetDungeon(m_DungeonLv);
        if (player.GetLevel() < (int32)table->need_level)
        {
            return;
        }
        if (sReward.Change(player, Reward::Coupon, -(int)table->up_cost))
        {
            ++m_DungeonLv;
            PlayerDungeonLogDB::SendBaseInfoToDb(player, *this);
        }
    }
}
void PlayerDungeonLog::Build_Silver(Player& player, bool isFast)
{
    if (m_DungeonLv == 0)
    {
        const DB_Dungeon* table = sDungeonTableMgr.GetDungeon(m_DungeonLv);
		if (player.GetLevel() < (int32)table->need_level)
		{
			return;
		}
        int costNum = 0;
        if (isFast)
            costNum = table->second_cost_num - m_SilverCoinForBuild;
        else
            costNum = table->second_cost_num / 10;

        if (sReward.Change(player, table->second_cost_id, -costNum))
        {
            m_SilverCoinForBuild += costNum;

            if (m_SilverCoinForBuild >= (int32)table->second_cost_num) {
                m_SilverCoinForBuild = 0;
                ++m_DungeonLv;
            }
            PlayerDungeonLogDB::SendBaseInfoToDb(player, *this);
        }
    }
}
int PlayerDungeonLog::CalcCaneExp(bool isOffical)
{
    const DB_Cane* table = sDungeonTableMgr.GetCane(GetCaneLv());
    
	if(isOffical)
		return table->base_exp + table->exp_add + IncCaneBaseExp() + IncCaneOfficialExp();
	else
		return table->base_exp + table->exp_add + IncCaneBaseExp();
}
int PlayerDungeonLog::IncCaneBaseExp()
{
    const int Special_Level = 4;
    if (m_DungeonLv >= Special_Level)
    {
        return sDungeonTableMgr.GetDungeon(Special_Level)->special_ability;
    }
    return 0;
}
int PlayerDungeonLog::IncCaneOfficialExp()
{
    const int Special_Level = 3;
    if (m_DungeonLv >= Special_Level)
    {
        return sDungeonTableMgr.GetDungeon(Special_Level)->special_ability;
    }
    return 0;
}
float PlayerDungeonLog::CalcCatchRate()
{
    const int Special_Level = 5;		//5级的概率计算不一样，概率跟高

    if (!sOS.IsToday(m_CatchHeroTime))	//上次抓捕时间
    {
        m_TodayCatchHeroCnt = 0;	//当天抓捕人数
    }

    float x = (float)m_TodayCatchHeroCnt;

    if (m_DungeonLv >= Special_Level)
    {
        return 1 / (::pow(x / 10, 3) + 1);
    }
    return 1 / (::pow(x / 5, 2) + 1);
}
int PlayerDungeonLog::GetCaneLv()
{
    if (sOS.TimeSeconds() >= m_TempCaneLvEndTime)
    {
        return m_CaneLv;
    }
    else
    {
        return MAX(m_CaneLv, m_TempCaneLv);
    }
}
bool PlayerDungeonLog::Cane(Player& player,uint32 &player_exp)
{
    //最近抓的最先揍
    if (m_HeroLst.size() == 0) return false;

	//if(m_HeroLst.back().escapeEndTime > 0)	//正在逃跑
	//{
		Player* owner_player = sPlayerPool.GetByPlayerId(m_HeroLst.back().playerId);	//被捕获武将所属玩家
		if(owner_player)
		{
			Character* charone = owner_player->m_characterStorage->MutableCharacter(m_HeroLst.back().heroTid);
			if(charone)
			{
				if(charone->_GetEscapeTime() > 0)	//武将正在逃跑
				{
					if(charone->_GetEscapeTime() <= sOS.TimeSeconds())	//已经过了逃跑时间,则判断为逃跑成功
					{
						PlayerDungeonLog::Escape(&player, charone, charone->_GetEscapeTime());
						return false;
					}
					else			//没有过逃跑时间则增加时间
					{
						uint32 lasttime = (uint32)(charone->_GetEscapeTime()-sOS.TimeSeconds()) + GetEscapeSecond() + 1;
						charone->_SetEscapeTime(charone->_GetEscapeTime() + GetEscapeSecond());
						sAutoUpdateGame.AddEvent(
							boost::bind(&PlayerDungeonLog::Escape, this, &player, charone, charone->_GetEscapeTime()),
							lasttime
							);
					}
				}
			}
		}
	//}

    player_exp = CalcCaneExp(m_HeroLst.back().isOffical);

   // sReward.Change(player, Reward::PlayerExp, player_exp);

	AddCaneExpOnCane(player);

    m_HeroLst.back().isCaned = true;

    PlayerDungeonLogDB::SendDelHeroToDb(player, m_HeroLst.back());

	m_HeroLst.pop_back();

    //统计显示数据
    ++m_TodayCaneCnt;
    m_TodayCaneExp += player_exp;
    m_CaneTime = sOS.TimeSeconds();
	PlayerDungeonLogDB::SendBaseInfoToDb(player, *this);
	return true;
}
void PlayerDungeonLog::TryCane(Player& player, int dstLv)
{
    if (dstLv <= GetCaneLv()) return;

    if (const DB_Cane* table = sDungeonTableMgr.GetCane(dstLv))
    {
        if (m_freeTryCane && dstLv == GetCaneLv() + 1)
        {
            m_freeTryCane = false;
            m_TempCaneLv = dstLv;
            m_TempCaneLvEndTime = sOS.TimeSeconds() + Free_TempCaneLv_Last_Time;
        }
        else if (sReward.Change(player, Reward::Coin_Gold, -(int)table->try_gold))
        {
            m_freeTryCane = false;
            m_TempCaneLv = dstLv;
            m_TempCaneLvEndTime = sOS.TimeSeconds() + Gold_TempCaneLv_Last_Time;
        }
		PlayerDungeonLogDB::SendBaseInfoToDb(player, *this);
    }
}
void PlayerDungeonLog::CatchObj(Player& player, Combat::CombatObj& obj)
{
    if (!CanCatchObj(obj)) return;	//是否可抓

    if (System::Randf(0, 1) > CalcCatchRate()) return;	//计算概率

    CatchHero data;
    data.playerId = obj.GetPlayerId();	//如果是npc则为npcID;
    data.heroTid = obj.GetTableId();	//protoid
    data.isCaned = false;
    data.escapeEndTime = 0;
	data.isOffical = false;
	data.isShadow = false;
	data.level = obj.GetLevel();

	//发送捕获信息
	pb::GS2C_Dungeon_Add_Hero sendmsg;
	pb::CatchHero* heroinfo = sendmsg.mutable_hero();
	heroinfo->set_player_id(data.playerId);
	heroinfo->set_hero_tid(data.heroTid);

    if (Character* hero = dynamic_cast<Character*>(&obj))
    {
		if(!hero->IsShadow())	//不是幻影，则需要锁住武将
		{
			hero->SetDungeonOwner(player.GetGuid());	//设置被抓捕的武将被谁抓住
			data.isOffical = sPalaceMgr(hero->GetCountryId())->IsPlayerOffical(player.GetGuid());
		}
		data.isShadow = hero->IsShadow();
		heroinfo->set_player_name(player.CharName());
		heroinfo->set_isshadow(data.isShadow);
    }

	//PLOG("CatchObj playerid = %lld, heroid = %d, isoffical = %d, isShadow = %d",data.playerId,data.heroTid,data.isOffical,data.isShadow);
    m_HeroLst.push_back(data);
    AddCaneExpOnCatchHero(player);		//抓到武将后获得鞭笞等级经验
    m_CatchHeroTime = sOS.TimeSeconds();
	uint32 player_exp = 0;
    if (m_DungeonLv >= 5)
	{
		Cane(player,player_exp); //自动笞杖
	}
    PlayerDungeonLogDB::SendAddHeroToDb(player, data);

	m_TodayCatchHeroCnt++;
	PlayerDungeonLogDB::SendBaseInfoToDb(player, *this);

	player.Send(pb::SMSG_DUNGEON_ADD_HERO,sendmsg);
	player.AddXP(player_exp);
	player.UpdateAllDailyQuestInfo(DUNGEON_ZHUABU,1,0);//每日任务
}

//立即逃脱掉
void PlayerDungeonLog::Escape(Player* dungeonOwner, Character* escapeHero, uint64 endtime)
{
    for (vector<CatchHero>::iterator it = m_HeroLst.begin(); it != m_HeroLst.end(); ++it)
    {
        if (it->playerId == escapeHero->GetPlayerId() && it->heroTid == escapeHero->GetTableId())
        {
			//if(it->escapeEndTime <= endtime)
			//{
				PlayerDungeonLogDB::SendDelHeroToDb(*dungeonOwner, *it);
				m_HeroLst.erase(it);
				break;
			//}
        }
	}
	escapeHero->SetDungeonOwner(0);
	escapeHero->_SetEscapeTime(0);
	PlayerDungeonLogDB::SendBaseInfoToDb(*dungeonOwner, *this);
}

//尝试逃脱
void PlayerDungeonLog::TryPrisonerEscape(Player* dungeonOwner, Character* obj)
{
#ifdef  _MMO_SERVER_
	//CatchHero* prisoner = NULL;

	//for (vector<CatchHero>::iterator it = m_HeroLst.begin(); it != m_HeroLst.end(); ++it)
	//{
	//	if (it->playerId == obj->GetPlayerId() && it->heroTid == obj->GetTableId())
	//	{
	//		prisoner = &(*it);
	//	}
	//}

	//prisoner->escapeEndTime = sOS.TimeSeconds() + GET_BASE_DEF_INT(pb::BD_PRISON_BREAK_BASE_TIME);

	obj->_SetEscapeTime(sOS.TimeSeconds() + GET_BASE_DEF_INT(pb::BD_PRISON_BREAK_BASE_TIME));

	sAutoUpdateGame.AddEvent(
	    boost::bind(&PlayerDungeonLog::Escape, this, dungeonOwner, obj, obj->_GetEscapeTime()),
	    GET_BASE_DEF_INT(pb::BD_PRISON_BREAK_BASE_TIME)
		);
	PlayerDungeonLogDB::SendBaseInfoToDb(*dungeonOwner, *this);
#endif

}

PlayerDungeonLog::CatchHero* PlayerDungeonLog::FindCatchHero(Combat::CombatObj& obj)
{
    for (vector<CatchHero>::iterator it = m_HeroLst.begin(); it != m_HeroLst.end(); ++it)
    {
        if (it->playerId == obj.GetPlayerId() && it->heroTid == obj.GetTableId())
        {
            return &(*it);
        }
    }
    return NULL;
}
int PlayerDungeonLog::GetEscapeSecond()
{
    return Escape_Base_Time + sDungeonTableMgr.GetCane(GetCaneLv())->escape_time;
}
void PlayerDungeonLog::AddCaneExpOnCatchHero(Player& player)
{
    const DB_Cane* table = sDungeonTableMgr.GetCane(m_CaneLv);	//根据鞭笞等级获得表

	if(table->need_people == 0)	return;		//满级

    if (++m_CatchHeroCntForExp >= (int32)table->need_people)	//抓多少人增加一点经验
    {
        m_CatchHeroCntForExp = 0;

        if (++m_CaneExp >= (int32)table->need_exp)
        {
            ++m_CaneLv;
            m_CaneExp = 0;
            m_freeTryCane = true;
        }
    }
}

void PlayerDungeonLog::AddCaneExpOnCane(Player& player)
{
	const DB_Cane* table = sDungeonTableMgr.GetCane(m_CaneLv);	//根据鞭笞等级获得表

	if(table->need_people == 0)	return;		//满级

	if(player.HaveTechnology(NUEDUOSHENGJI))
	{
		const TecTable *tec = NULL;
		tec = sTechnologyTableMgr.GetTable(NUEDUOSHENGJI);
		if(System::Randf(0, 1) < tec->Value1())
		{
			m_CaneExp += tec->Value2();

			if (m_CaneExp >= (int32)table->need_exp)
			{
				++m_CaneLv;
				m_CaneExp = 0;
				m_freeTryCane = true;
			}
		}
	}
}

void PlayerDungeonLog::CaneFastLevelUp(Player& player, int dstLv)
{
	int totalneedexp = 0;
	if(dstLv <= (int)sDungeonTableMgr.GetMaxDungeonLv() && dstLv > 1 && m_CaneLv < dstLv)
	{
		for(int i=1; i < dstLv; i++)
		{
			totalneedexp += sDungeonTableMgr.GetCane(i)->need_exp;
		}

		if (const DB_Cane* table = sDungeonTableMgr.GetCane(dstLv))
		{
			int hasExp = 0;
			for (int i = 1; i < m_CaneLv; ++i)
			{
				hasExp += sDungeonTableMgr.GetCane(i)->need_exp;
			}
			int needGold = totalneedexp - hasExp - m_CaneExp;

			if (sReward.Change(player, Reward::Coin_Gold, -needGold))
			{
				m_CaneLv = dstLv;
				m_CaneExp = 0;
				m_freeTryCane = true;
				PlayerDungeonLogDB::SendBaseInfoToDb(player, *this);
			}
		}
	}


}
bool PlayerDungeonLog::CanCatchObj(Combat::CombatObj& obj)
{
    if (Character* hero = dynamic_cast<Character*>(&obj))
    {
        if (hero->IsShadow())
        {
            return m_DungeonLv >= 2;	//幻影两级可抓
        }
    }
    return m_DungeonLv >= 1;	//npc与玩家武将1级可抓
}
