#include "CombatGroup.h"
#include "../object/Player.h"
#include "CombatCalc.h"
#include "../Service/ServiceMgr.h"
#include "datastruct/struct_soldier_food.h"
#include "../reward/reward.h"
#include "System.h"
#include "../Map/Table/MapTableMgr.h"
#include "../Map/Table/MapCity.h"
#include "../Map/city/city.h"
#include "../character/Character.h"
#include "../character/CharacterStorage.h"
#include "Combat.pb.h"
#include "Opcode.pb.h"
#include "CommomDef.pb.h"
#include "../Map/country/task/CountryTaskCity.h"
#include "World.pb.h"
#include "../session/PlayerPool.h"
#include "../Map/country/country.h"
#include "../Activity/OccupyCity/OccupyCity.h"
#include "../Map/WorldMap.h"
#ifdef _MMO_SERVER_
#include "../AutomaticUpdate/AutoUpdateGame.h"
#endif
#ifdef _SERVER_CROSS_
#include "server_cross/cross_wall/CrossActivity/CrossDestroy/CrossDestroy.h"
#include "server_cross/cross_wall/CrossWarManager/CrossWarMgr.h"
#include "server_cross/cross_wall/CrossActivity/CrossKill/CrossKill.h"
#endif
#include "BaseDefineMgr.h"
#include "BaseDefine.pb.h"
#include "Technology/PlayerTechnology.h"
#include "Technology/Table/TechnologyTableMgr.h"
#include "Activity/Personal/PersonalTask.h"
#include "../Map/country/country.h"
#include "Activity/Defence/DefenceBorderland.h"
#include "AutoCountryWar/PlayerAutoCountryWarLogic.h"
#include "AutoCountryWar.pb.h"
#include "Reinforcement/Reinforce.h"
#include "Social.pb.h"
#include "chat/ChatSpeaker.h"
#include "Npc/Npc.h"

using namespace pb;

using namespace Combat;

const uint32 Combat_Operate_Time = 10; //选择战术的时长
const char Combat_Client_Flash_Time = 20; //客户端战斗动画时间
const char How_Many_Line_Fighting = 6; //前几排算上阵
const char Combat_Delete_Time_Last = 3;	//3秒后如果观看玩家为0并且没有开展，则删除这个战斗组
const uint32 Combat_Delete_Prepare_Time = 3600;	//一小时后删除僵尸战斗组 有玩家观战，没点准备
const uint32 Combat_Delete_Short_time	= 600;	//十分钟后关闭僵尸战斗组		有玩家观战，没点准备		官职战时间
uint32 _Service_Combat(void* p)
{
    CombatGroup* pGroup = (CombatGroup*)p;

#ifdef _SERVER_CROSS_
	if(sCrossWarMgr.GetCrossState() != State_Start)
	{
		pGroup->ShutDown();
		return 50;
	}
#endif

    pGroup->_NewUpdateOperateTime();	//秒表

    return 1000;//50; // 50ms
}
CombatGroup::CombatGroup(CombatType typ)
    : m_unique_id(0)		/*战斗组唯一的id*/
	//,m_seige_army_count_(0)
    , m_leftOperateSec(Combat_Operate_Time)/*剩余操作时间*/		//初始时间9秒
	, m_leftFlashSec(0)	/*剩余动画时间*/
	, m_eCombat(typ)	/*战斗类型*/
	, m_isCombatEnd(false)
	, m_combatEndLast(3)
	, m_isAttractCombat(false)
	, m_battleDeleteTime(Combat_Delete_Time_Last)
	, m_CurrentRound(0)
	, m_isQuickCombat(false)
	, m_FubenPlayer(0)
	, m_fight_calc(false)
	, m_isInnerCir(false)
	, m_idxInLst(0)
	, m_isStartNextRound(false)
	//, m_battleDeletePrepareTime(Combat_Delete_Prepare_Time)
{
	_CreateUniqueId();	//创建唯一id
	if(typ == Combat::CT_Country || typ == Combat::CT_Teach_Map)			//如果是国战，自动准备完毕 教学关卡
		m_isReady = true;
	else
		m_isReady = false;

	if(typ == Combat::CT_Official)
		m_battleDeletePrepareTime = Combat_Delete_Short_time;
	else
		m_battleDeletePrepareTime = Combat_Delete_Prepare_Time;

	m_nextOperTime = sOS.TimeSeconds() + m_leftOperateSec + m_leftFlashSec;

	m_timeClockMS = sOS.TimeMS();
	m_combatGainsMap.clear();
}

void CombatGroup::ShutDown()
{
	for (GroupLst::iterator it = m_group_attack.begin(); it != m_group_attack.end(); ++it)
	{
		if((*it)->nowCombatGroupID == m_unique_id)
		{
			(*it)->ClearCombatStatus();		//清理武将状态
			_NotifyClientHeroStatus(*it);

			if((*it)->IsPlayer() && m_eCombat == CT_Teach_Map)
			{
				if( ::Character* hero = dynamic_cast<::Character*>(*it))
				{
					hero->RecoverFullSoldier();
					hero->m_player->m_characterStorage->SetModifiedID(hero->GetID());
				}
			}
		}
	}
	for (GroupLst::iterator it = m_group_defence.begin(); it != m_group_defence.end(); ++it)
	{
		if((*it)->nowCombatGroupID == m_unique_id)
		{
			(*it)->ClearCombatStatus();
			_NotifyClientHeroStatus(*it);
		}
	}
	ServiceMgr::UnRegister(SERVICE_COMBAT, this);
	delete this;
}

CombatGroup::~CombatGroup()		//析构，清理战斗组状态
{
	if(m_isAttractCombat) return;
	for (GroupLst::iterator it = m_group_attack.begin(); it != m_group_attack.end(); ++it)
	{
		if((*it)->nowCombatGroupID == m_unique_id)
		{
			(*it)->ClearCombatStatus();		//清理武将状态
			_NotifyClientHeroStatus(*it);
		}
// 		if((*it))
// 			delete (*it);
	}
	for (GroupLst::iterator it = m_group_defence.begin(); it != m_group_defence.end(); ++it)
	{
		if((*it)->nowCombatGroupID == m_unique_id)
		{
			(*it)->ClearCombatStatus();
			_NotifyClientHeroStatus(*it);
		}
// 		if((*it))
// 			delete (*it);
	}
}
void StatisticData::add(const CombatObj& obj)	//战斗数据增长
{
	statisticExp += obj.statisticExp;
	statisticExpPlayer += obj.statisticExpPlayer;
	statisticMoney += obj.statisticMoney;
	statisticDead += obj.statisticDead;
	statisticKill += (int)(obj.statisticKill * (1 + obj.total_doubleKill/100.0));
}

CombatGroup* CombatGroup::CreateGroup(CombatObj& attack, CombatObj& defence, CombatType typ, bool isInAttract, uint32 terrainType)	//创建战斗组（攻击方，防守方，战斗类型）
{
    CombatGroup* pRet = new CombatGroup(typ);

	// printf("CreateGroup1 -- 1 \n");
    pRet->_AddObj(attack, pRet->m_group_attack,true);		//初始化进攻方
    pRet->_AddObj(defence, pRet->m_group_defence,true);		//初始化防守方

	pRet->m_isAttractCombat = isInAttract;
	pRet->m_terrainType = terrainType;

	// printf("CreateGroup1 -- 2 \n");

	if (pRet->m_group_attack.empty() || pRet->m_group_defence.empty())
	{
		if(pRet->m_group_attack.empty())
		{
			LLOG("{%d} IMPORTANT BUG!!!!!! attack group is empty",pRet->m_unique_id);
			//MMO_ASSERT(0);
		}
		if(pRet->m_group_defence.empty())
		{
			LLOG("{%d} IMPORTANT BUG!!!!!! defence group is empty",pRet->m_unique_id);
			//MMO_ASSERT(0);
		}
		//MMO_ASSERT(0);
		delete pRet;
		return NULL;
	}
	//单挑自动开战
	pRet->m_isReady = true;
	pRet->UpdateHerosInCombat();

	// printf("CreateGroup1 -- 3 \n");

    ServiceMgr::Register(SERVICE_COMBAT, pRet);		//？
	return pRet;
}

//创建战斗组 防守方有多方
CombatGroup* CombatGroup::CreateGroup(CombatObj& attack, std::set<CombatObj*>& defence, CombatType typ, uint32 terrainType)
{
    MMO_ASSERT(!defence.empty());

    CombatGroup* pRet = new CombatGroup(typ);
	pRet->m_terrainType = terrainType;
	// printf("CreateGroup2 -- 1 \n");

    bool addret = pRet->_AddObj(attack, pRet->m_group_attack);

	PLOG("{%d}CombatGroup::CreateGroup one&list one countryid=%d, isSuccess=%d",pRet->m_unique_id,attack.GetCountryId(),addret);
	// printf("CreateGroup2 -- 2  defence.size(%d) \n", defence.size());

	LLOG("{%d} defencesize=%d",pRet->m_unique_id,defence.size());

    std::set<CombatObj*>::iterator it = defence.begin();
    for (; it != defence.end(); ++it)
    {
		// printf("CreateGroup2 -- 3  defence:obj(%d)\n", *it);

        addret = pRet->_AddObj(*(*it), pRet->m_group_defence);

		LLOG("{%d}CombatGroup::CreateGroup one&list list countryid=%d, isSuccess=%d",pRet->m_unique_id,(*it)->GetCountryId(),addret);
    }

	// printf("CreateGroup2 -- 4 \n");

	if (pRet->m_group_attack.empty() || pRet->m_group_defence.empty())
	{
		if(pRet->m_group_attack.empty())
		{
			LLOG("{%d} IMPORTANT BUG!!!!!! attack group is empty",pRet->m_unique_id);
			//MMO_ASSERT(0);
		}
		if(pRet->m_group_defence.empty())
		{
			LLOG("{%d} IMPORTANT BUG!!!!!! defence group is empty",pRet->m_unique_id);
			//MMO_ASSERT(0);
		}
		delete pRet;
		return NULL;
	}

	// printf("CreateGroup2 -- 5 \n");

    ServiceMgr::Register(SERVICE_COMBAT, pRet);
	return pRet;
}

//创建战斗组 进攻方，防守方均为多位
CombatGroup* CombatGroup::CreateGroup(std::vector<CombatObj*>& attack, std::set<CombatObj*>& defence, CombatType typ, uint32 terrainType)
{
    MMO_ASSERT(!attack.empty());
    MMO_ASSERT(!defence.empty());


    CombatGroup* pRet = new CombatGroup(typ);
	pRet->m_terrainType = terrainType;

    for (std::vector<CombatObj*>::iterator it = attack.begin(); it != attack.end(); ++it)
    {
		pRet->_AddObj(*(*it), pRet->m_group_attack);		//向攻击序列中添加攻击武将
    }
    for (std::set<CombatObj*>::iterator it = defence.begin(); it != defence.end(); ++it)
    {
		pRet->_AddObj(*(*it), pRet->m_group_defence);		//向防守序列中添加防守武将
    }

	// printf("CreateGroup3 -- 4 \n");

	////有可能是0血，如果是玩家就不处理，等待加血，如果是npc则删除
	GroupLst groupTmp;
	for (GroupLst::iterator it = pRet->m_group_attack.begin(); it!=pRet->m_group_attack.end(); ++it)
	{
		if(!(*it)->IsNpc()) //不是npc则保留
		{
			groupTmp.push_back((*it));	
		}
		else if(!(*it)->soldiers.empty())	//是npc，并且有兵力，则保留
		{
			groupTmp.push_back((*it));
		}
	}
	pRet->m_group_attack.clear();
	for(GroupLst::iterator ite = groupTmp.begin();ite != groupTmp.end(); ++ite)
	{
		pRet->m_group_attack.push_back(*ite);
	}

	int idx = pRet->m_group_attack.size()>5? 5:pRet->m_group_attack.size();
	for (int i = 0; i < idx; ++i)
	{
		int id = pRet->m_group_attack[i]->GetTableId();
	}

	groupTmp.clear();
	for (GroupLst::iterator it = pRet->m_group_defence.begin(); it!=pRet->m_group_defence.end(); ++it)
	{
		if(!(*it)->IsNpc()) //不是npc则保留
		{
			groupTmp.push_back((*it));	
		}
		else if(!(*it)->soldiers.empty())	//是npc，并且有兵力，则保留
		{
			groupTmp.push_back((*it));
		}
	}
	pRet->m_group_defence.clear();
	for(GroupLst::iterator ite = groupTmp.begin();ite != groupTmp.end(); ++ite)
	{
		pRet->m_group_defence.push_back(*ite);
	}

	if (pRet->m_group_attack.empty() || pRet->m_group_defence.empty())	//不允许空兵力
	{
		if(pRet->m_group_attack.empty())
		{
			LLOG("{%d} IMPORTANT BUG!!!!!! attack group is empty",pRet->m_unique_id);
			//MMO_ASSERT(0);
		}
		if(pRet->m_group_defence.empty())
		{
			LLOG("{%d} IMPORTANT BUG!!!!!! defence group is empty",pRet->m_unique_id);
			//MMO_ASSERT(0);
		}
		//MMO_ASSERT(0);
		delete pRet;
		return NULL;
	}

	// printf("CreateGroup3 -- 5 \n");

    ServiceMgr::Register(SERVICE_COMBAT, pRet);
	return pRet;
}

CombatGroup* CombatGroup::CreateGroup(std::set<CombatObj*>& defence, CombatType typ, uint32 terrainType)
{
	MMO_ASSERT(!defence.empty());

	CombatGroup* pRet = new CombatGroup(typ);
	pRet->m_terrainType = terrainType;

	for(std::set<CombatObj*>::iterator it = defence.begin(); it != defence.end(); ++it)
	{
		pRet->_AddObj(*(*it), pRet->m_group_defence);		//向防守序列中添加防守武将
	}

	ServiceMgr::Register(SERVICE_COMBAT, pRet);
	return pRet;
}

//在战斗结束后添加回调函数
void CombatGroup::AddCallBack_OnEnd(const boost::function<void(CombatGroup*, bool)>& func)
{
    m_CallBack.push_back(func);
}

//void CombatGroup::AddCallBack_OnAddSoldier(const boost::function<void(CombatGroup*, uint32)>& func)
//{
//	m_AddHerosCallBack.push_back(func);
//}

void CombatGroup::AddCallBack_OnNpcDead(const boost::function<void(CombatGroup*, CombatObj*)>& func)
{
	m_NpcDeadCallBack.push_back(func);
}

void CombatGroup::AddCallBack_OnCharacterDead(const boost::function<void (CombatGroup*, CombatObj*)>& func)
{
	m_CharDeadCallBack.push_back(func);
}

void CombatGroup::AddCallBack_OnBegin(const boost::function<void()>& func)
{
	m_CombatBegin.push_back(func);
}

void CombatGroup::AddCallBack_OnGroupRelease(const boost::function<void(CombatGroup*)>& func)
{
	m_ReleaseCallBack.push_back(func);
}

//创建唯一id
void CombatGroup::_CreateUniqueId()
{
    static uint16 s_auto_id = 0;

    m_unique_id = ((++s_auto_id) << 16) + m_index;

	// printf("CreateGroup(%d) -- \n", m_unique_id);
}

//通过唯一id查找战斗组
CombatGroup* CombatGroup::FindGroup(uint32 uniqueId)
{
    int idx = uniqueId & 0xFFFF;

    if (CombatGroup* ret = FindByIdx(idx))
        if (ret->m_unique_id == uniqueId)
            return ret;

    return NULL;
}

void CombatGroup::SendNewRoundMsg(uint64 oprEndTime, CombatObj* objAtt, CombatObj* objDef)
{
	pb::GS2C_Combat_New_Round msgNewRound;
	msgNewRound.set_choose_time(oprEndTime);		//选择战术截止时间
	Tactic_Param* tacPars = msgNewRound.mutable_tactic_param();

	tacPars->set_atttacticenablestr(m_group_attack.front()->GetTacticAndSkills());
	tacPars->set_deftacticenablestr(m_group_defence.front()->GetTacticAndSkills());

	tacPars->set_atttacticrealstr(m_group_attack.front()->GetTacticsAndSkillsEnable());
	tacPars->set_deftacticrealstr(m_group_defence.front()->GetTacticsAndSkillsEnable());

	msgNewRound.set_roundcount(m_CurrentRound);		//当前回合数
	msgNewRound.set_group_id(m_unique_id);			//战斗组id
	Broadcast(pb::SMSG_COMBAT_NEW_ROUND, msgNewRound);
}

void CombatGroup::AniFinished()
{
	bool buffret = false;

	if(m_group_attack.empty() || m_group_defence.empty())
	{
		PLOG("{%d}AniFinished Important Bug！！！！！！！！！！！",m_unique_id);
		//ASSERT(0);
		//保护措施,结束战斗
		m_isCombatEnd = true;
		OnGroupCombatEnd(!m_group_attack.empty());
		return;
	}

	if(m_leftFlashSec > 0)
	{
		m_leftFlashSec = 0;
		m_leftOperateSec = Combat_Operate_Time;
		m_CurrentRound++;
		LLOG("{%d}m_CurrentRound=%d",m_unique_id,m_CurrentRound);
		uint64 choosetime = sOS.TimeSeconds() + m_leftOperateSec;

		SendNewRoundMsg(choosetime,m_group_attack.front(),m_group_defence.front());	//发送新回合消息

	}
}
//倒计时（自动战斗）
void CombatGroup::_UpdateOperateTime(bool innerCir /* = false */)
{

	//if(m_isCombatEnd)		//战斗结束
	//{
	//	return;
	//}

	//if(!m_isQuickCombat)			//如果不是快速战斗，只允许整一秒的循环进来，其他循环抛弃
	//{
	//	////LLOG("{%d} not quick combat time check", m_unique_id);
	//	//if(sOS.TimeMS() - m_timeClockMS >= 1000)		//1000毫秒即一秒进来一次
	//	//	m_timeClockMS = sOS.TimeMS();
	//	//else
	//	//	return;
	//	//if(!m_fight_calc)
	//	//{
	//	//	m_fight_calc = true;
	//	//}
	//	//else
	//	//{
	//	//	return;
	//	//}
	//	if(m_fight_calc)	//在战斗计算中，不进入循环
	//		return;
	//}
	//else		//快速战斗中，如果计算空闲状态，则进入计算状态，否则返回
	//{
	//	if(!innerCir)
	//	{
	//		if(!m_isInnerCir)		//如果不在内循环	仅第一次赋值
	//			m_isInnerCir = true;
	//		else
	//			return;
	//	}

	//	if(!m_fight_calc)
	//	{
	//		//LLOG("{%d} is quick combat time check free to busy", m_unique_id);
	//		m_fight_calc = true;
	//		m_leftFlashSec = 0;		//动画显示时间归零
	//		m_leftOperateSec = 0;	//操作时间归零
	//	}
	//	else
	//	{
	//		//LLOG("{%d} is quick combat time check busy return", m_unique_id);
	//		return;
	//	}
	//}

	////检测玩家是否掉线，掉线则踢掉
	//int i = 0;
	//for (vector<uint64>::iterator it = m_watchPlayer.begin(); it != m_watchPlayer.end();)
	//{
	//	if(i++ > 200) break;

	//	if (Player* player = sPlayerPool.GetByPlayerId(*it))
	//	{
	//		if(player->GetDataIsLost())
	//		{
	//			it = m_watchPlayer.erase(it);
	//			continue;
	//		}
	//	}
	//	it++;
	//}

	//if(!m_isReady)	//玩家没有点准备， 有玩家但是不开始或者没玩家，删除战斗组
	//{
	//	LLOG("{%d}not ready proupid=%d",m_unique_id,m_unique_id);
	//	if(m_watchPlayer.size() == 0 && m_battleDeleteTime-- <= 0)		//待准备阶段没有玩家观战，则解散该战斗组
	//	{
	//		PLOG("{%d}release combat no watcher",m_unique_id);
	//		ReleaseGroup();
	//		ServiceMgr::UnRegister(SERVICE_COMBAT, this);
	//		delete this;
	//	}

	//	if(m_battleDeletePrepareTime-- <= 0)	//战斗组在规定时间内没有开启战斗，则删除战斗组
	//	{
	//		PLOG("{%d}release combat out of time",m_unique_id);
	//		ReleaseGroup();
	//		ServiceMgr::UnRegister(SERVICE_COMBAT, this);
	//		delete this;
	//	}

	//	return;			
	//}

	//if(m_group_attack.empty() || m_group_defence.empty())
	//{
	//	PLOG("{%d}Update Important Bug！！！！！！！！！！！",m_unique_id);
	//	//ASSERT(0);
	//	//保护措施,结束战斗
	//	m_isCombatEnd = true;
	//	OnGroupCombatEnd(!m_group_attack.empty());
	//	return;
	//}

	//CombatObj* pAttack = m_group_attack.front();
	//CombatObj* pDefence = m_group_defence.front();

	//LLOG("{%d}####### time minite circle #######",m_unique_id);
	//LLOG("{%d}unique id = %ld",m_unique_id,m_unique_id);
	//LLOG("{%d}m_group_attack.size=%d,m_group_defence.size=%d",m_unique_id,m_group_attack.size(),m_group_defence.size());
	////LLOG("{%d}pAttack.hp=%d, pDefence.hp=%d, aid=%d,did=%d",m_unique_id,pAttack->CurHp(),pDefence->CurHp(),pAttack->GetTableId(),pDefence->GetTableId());

	//if(pAttack->CurHp() == 0 || pDefence->CurHp() == 0)
	//{
	//	PLOG("{%d}Update Second Important Bug！！！！！！！！！！！ ",m_unique_id);
	//	//ASSERT(0);
	//	//保护措施，令战斗正常打完
	//	_CheckDead();
	//	if (m_group_attack.empty() || m_group_defence.empty())
	//	{
	//		m_isCombatEnd = true;
	//		OnGroupCombatEnd(!m_group_attack.empty());
	//		return;
	//	}
	//	if(m_isQuickCombat)
	//		_UpdateOperateTime(true);
	//	return;
	//}


	////战斗计时流程！！
 //   if (m_leftFlashSec > 0 && !m_isQuickCombat)	//动画时间大于0		不可能出现在快速战斗的情况中！！
	//{
	//	if(m_watchPlayer.size() == 0)			//如果没有玩家观看，剩余播放动画时间减半
	//		m_leftFlashSec = m_leftFlashSec/2;
	//	else
	//		m_leftFlashSec--;		//有玩家，则计时正常减

	//	LLOG("{%d}rest anition time = %d",m_unique_id,m_leftFlashSec);
	//	
	//	if(m_leftFlashSec == 0)		//如果倒计时结束仍然没有收到动画结束消息，则开始下一回合
	//	{
	//		//bool buffret = false;
	//		m_leftOperateSec = GetInitOperateSeconds();
	//		m_CurrentRound++;
	//		LLOG("{%d}m_CurrentRound=%d",m_unique_id,m_CurrentRound);
	//		uint64 choosetime = sOS.TimeSeconds() + m_leftOperateSec;
	//		pb::GS2C_Combat_New_Round msgNewRound;
	//		msgNewRound.set_choose_time(choosetime);

	//		
	//		//攻守双方的可释放技能
	//		if(m_group_attack.size()>0)
	//			msgNewRound.set_atttacticenablestr(m_group_attack.front()->GetTacticAndSkills());
	//		if(m_group_defence.size()>0)
	//			msgNewRound.set_deftacticenablestr(m_group_defence.front()->GetTacticAndSkills());


	//		if(m_group_attack.front()->HaveBuffWork(confuse))
	//			msgNewRound.set_canattselecttactic(false);
	//		else
	//			msgNewRound.set_canattselecttactic(true);

	//		if(m_group_defence.front()->HaveBuffWork(confuse))
	//			msgNewRound.set_candefselecttactic(false);
	//		else
	//			msgNewRound.set_candefselecttactic(true);
	//		msgNewRound.set_roundcount(m_CurrentRound);
	//		msgNewRound.set_group_id(m_unique_id);

	//		Broadcast(pb::SMSG_COMBAT_NEW_ROUND, msgNewRound);

	//		if (m_group_attack.empty() || m_group_defence.empty())
	//		{
	//			PLOG("{%d}################# battle end #############",m_unique_id);

	//			m_isCombatEnd = true;
	//			OnGroupCombatEnd(!m_group_attack.empty());
	//			return;
	//		}
	//		LLOG("{%d}################# battle not end #############  ",m_unique_id);

	//		// 更新上阵情况
	//		_RefreshFightingObj(m_group_attack);
	//		_RefreshFightingObj(m_group_defence);

	//		//if(buffret)
	//		//{
	//		//	m_leftFlashSec = Combat_Client_Flash_Time;
	//		//	return;
	//		//}

	//	}
	//	else	//动画倒计时没结束就不进行下面的操作
	//		return;
	//}

	//if(m_leftFlashSec == 0 && m_isQuickCombat)		//如果是快速战斗，则先计算buff伤害
	//{
	//	//bool buffret = false;
	//	m_leftOperateSec = GetInitOperateSeconds();
	//	m_CurrentRound++;
	//	LLOG("{%d}m_CurrentRound=%d",m_unique_id,m_CurrentRound);
	//	//uint64 choosetime = sOS.TimeSeconds() + m_leftOperateSec;
	//	pb::GS2C_Combat_New_Round msgNewRound;
	//	//msgNewRound.set_choose_time(choosetime);
	//	if (m_group_attack.empty() || m_group_defence.empty())
	//	{
	//		PLOG("{%d}################# battle end #############",m_unique_id);

	//		m_isCombatEnd = true;
	//		OnGroupCombatEnd(!m_group_attack.empty());
	//		return;
	//	}
	//	LLOG("{%d}################# battle not end #############  ",m_unique_id);

	//	// 更新上阵情况
	//	_RefreshFightingObj(m_group_attack);
	//	_RefreshFightingObj(m_group_defence);

	//}

	////AI选择
	//if(m_leftFlashSec == 0)
	//{
	//	LLOG("{%d}help select AI",m_unique_id);
	//	//是npc则帮忙选择  先防守方选择技能
	//	pb::GS2C_Combat_Select_Tactic_Single msg;
	//	if ((pDefence->IsNpc() || pDefence->IsReinforce()) && pDefence->NewAutoTactic(m_eCombat,pAttack->tactic)){
	//		LLOG("{%d}Defence npc select",m_unique_id);
	//		msg.set_type((pb::Msg_ECombatTactic)pDefence->tactic);
	//		msg.set_is_attacker(false);
	//		msg.set_attackernum(m_group_attack.size());
	//		msg.set_defendernum(m_group_defence.size());
	//		msg.set_group_id(m_unique_id);
	//		if(!m_isQuickCombat)
	//			Broadcast(pb::SMSG_COMBAT_SELECT_TACTIC_SINGLE, msg);
	//	}
	//	if ((pAttack->IsNpc() || pAttack->IsReinforce()) && pAttack->NewAutoTactic(m_eCombat,pDefence->tactic)){
	//		LLOG("{%d}Attack npc select",m_unique_id);
	//		msg.set_type((pb::Msg_ECombatTactic)pAttack->tactic);
	//		msg.set_is_attacker(true);
	//		msg.set_attackernum(m_group_attack.size());
	//		msg.set_defendernum(m_group_defence.size());
	//		msg.set_group_id(m_unique_id);
	//		if(!m_isQuickCombat)
	//			Broadcast(pb::SMSG_COMBAT_SELECT_TACTIC_SINGLE, msg);
	//	}

	//	if (Character* hero = dynamic_cast<Character*>(pDefence))
	//	{
	//		LLOG("{%d}help defence hero select AI",m_unique_id);
	//		//是玩家，并且上一轮已经选择了技能，则帮助随机选择战术
	//		if(!hero->IsShadow())
	//		{
	//			if(pDefence->isSelectTactic == true && pDefence->tactic == Skill_Tactic)
	//			{
	//				LLOG("{%d}defence hero had used skill then change",m_unique_id);
	//				if(pDefence->NewAutoTactic(m_eCombat,pAttack->tactic))
	//				{
	//					LLOG("{%d}defence hero change",m_unique_id);
	//					msg.set_type((pb::Msg_ECombatTactic)pDefence->tactic);
	//					msg.set_is_attacker(false);
	//					msg.set_attackernum(m_group_attack.size());
	//					msg.set_defendernum(m_group_defence.size());
	//					msg.set_group_id(m_unique_id);
	//					if(!m_isQuickCombat)
	//						Broadcast(pb::SMSG_COMBAT_SELECT_TACTIC_SINGLE, msg);
	//				}
	//			}
	//			else 
	//			{
	//				//如果自动战斗，则选择战术
	//				if (Player* player = sPlayerPool.GetByPlayerId(hero->GetPlayerId()))
	//				{
	//					if(player->IsAutoCombat())
	//					{
	//						if(pDefence->NewAutoTactic(m_eCombat,pAttack->tactic))
	//						{
	//							LLOG("{%d}defence hero is auto select tactic",m_unique_id);
	//							msg.set_type((pb::Msg_ECombatTactic)pDefence->tactic);
	//							msg.set_is_attacker(false);
	//							msg.set_attackernum(m_group_attack.size());
	//							msg.set_defendernum(m_group_defence.size());
	//							msg.set_group_id(m_unique_id);
	//							if(!m_isQuickCombat)
	//								Broadcast(pb::SMSG_COMBAT_SELECT_TACTIC_SINGLE, msg);
	//						}
	//					}
	//				}
	//			}
	//		}
	//		
	//		//是幻影
	//		if(hero->IsShadow() && hero->NewAutoTactic(m_eCombat,pAttack->tactic))
	//		{
	//			LLOG("{%d}defence shadow select AI",m_unique_id);
	//			msg.set_type((pb::Msg_ECombatTactic)pDefence->tactic);
	//			msg.set_is_attacker(false);
	//			msg.set_attackernum(m_group_attack.size());
	//			msg.set_defendernum(m_group_defence.size());
	//			msg.set_group_id(m_unique_id);
	//			if(!m_isQuickCombat)
	//				Broadcast(pb::SMSG_COMBAT_SELECT_TACTIC_SINGLE, msg);
	//		}
	//	}

	//	if (Character* hero = dynamic_cast<Character*>(pAttack))
	//	{
	//		LLOG("{%d}help attack hero select AI",m_unique_id);
	//		//不是幻影
	//		if(!hero->IsShadow())
	//		{
	//			if(pAttack->isSelectTactic == true && pAttack->tactic == Skill_Tactic)
	//			{
	//				LLOG("{%d}attack hero had used skill then change",m_unique_id);
	//				if(pAttack->NewAutoTactic(m_eCombat,pDefence->tactic))
	//				{
	//					LLOG("{%d}attack hero change",m_unique_id);
	//					msg.set_type((pb::Msg_ECombatTactic)pAttack->tactic);
	//					msg.set_is_attacker(true);
	//					msg.set_attackernum(m_group_attack.size());
	//					msg.set_defendernum(m_group_defence.size());
	//					msg.set_group_id(m_unique_id);
	//					if(!m_isQuickCombat)
	//						Broadcast(pb::SMSG_COMBAT_SELECT_TACTIC_SINGLE, msg);
	//				}
	//			}
	//			else 
	//			{
	//				//如果自动战斗，则选择战术
	//				if (Player* player = sPlayerPool.GetByPlayerId(hero->GetPlayerId()))
	//				{
	//					if(player->IsAutoCombat())
	//					{
	//						if(pAttack->NewAutoTactic(m_eCombat,pDefence->tactic))
	//						{
	//							LLOG("{%d}attack hero is auto select tactic",m_unique_id);
	//							msg.set_type((pb::Msg_ECombatTactic)pAttack->tactic);
	//							msg.set_is_attacker(true);
	//							msg.set_attackernum(m_group_attack.size());
	//							msg.set_defendernum(m_group_defence.size());
	//							msg.set_group_id(m_unique_id);
	//							if(!m_isQuickCombat)
	//								Broadcast(pb::SMSG_COMBAT_SELECT_TACTIC_SINGLE, msg);
	//						}
	//					}
	//				}
	//			}
	//		}

	//		//是幻影
	//		if(hero->IsShadow() && hero->NewAutoTactic(m_eCombat,pDefence->tactic))
	//		{
	//			LLOG("{%d}attack shadow select AI",m_unique_id);
	//			msg.set_type((pb::Msg_ECombatTactic)pAttack->tactic);
	//			msg.set_is_attacker(true);
	//			msg.set_attackernum(m_group_attack.size());
	//			msg.set_defendernum(m_group_defence.size());
	//			msg.set_group_id(m_unique_id);
	//			if(!m_isQuickCombat)
	//				Broadcast(pb::SMSG_COMBAT_SELECT_TACTIC_SINGLE, msg);
	//		}
	//	}
	//	
	//	m_leftFlashSec = -1;
	//}

	//if(m_leftOperateSec>0)
	//	m_leftOperateSec--;
	//
	//LLOG("{%d}pAttack->isSelectTactic:%d,pDefence->isSelectTactic:%d",m_unique_id,pAttack->isSelectTactic,pDefence->isSelectTactic);

 //   if (pAttack->isSelectTactic && pDefence->isSelectTactic)	//都选择了战术
	//{
	//	LLOG("{%d}################# let's fight ###############",m_unique_id);
	//	m_leftOperateSec = 0;
	//	if(m_isQuickCombat)
	//		m_leftFlashSec = 0;
	//	else
	//		m_leftFlashSec = Combat_Client_Flash_Time;
	//	DoFight1(pAttack, pDefence);

	//}
	//else if (m_leftOperateSec <= 0)		//战术选择时间结束了
 //   {
	//	LLOG("{%d}################# let's auto fight ###############",m_unique_id);
	//	if(!pDefence->isSelectTactic)
	//		pDefence->NewAutoTactic(m_eCombat,pAttack->tactic);
	//	if(!pAttack->isSelectTactic)
	//		pAttack->NewAutoTactic(m_eCombat,pDefence->tactic);		//自动选择战术

	//	m_leftOperateSec = 0;
	//	if(m_isQuickCombat)
	//		m_leftFlashSec = 0;
	//	else
	//		m_leftFlashSec = Combat_Client_Flash_Time;
	//	DoFight1(pAttack, pDefence);
	//}
	//else if(m_leftOperateSec  == 7)
	//{
	//	if(!pDefence->isSelectTactic && pDefence->HaveBuffWork(confuse))
	//		pDefence->NewAutoTactic(m_eCombat,pAttack->tactic);

	//	if(!pAttack->isSelectTactic && pAttack->HaveBuffWork(confuse))
	//		pAttack->NewAutoTactic(m_eCombat,pDefence->tactic);
	//}
	//else if(m_leftOperateSec > 0)
	//{
	//	pb::GS2C_Combat_Select_Tactic_Single msg;
	//	if (Character* hero = dynamic_cast<Character*>(pDefence))
	//	{
	//		if(!hero->IsShadow())
	//		{
	//			//如果自动战斗，则选择战术
	//			if (Player* player = sPlayerPool.GetByPlayerId(hero->GetPlayerId()))
	//			{
	//				if(player->IsAutoCombat())
	//				{
	//					if(pDefence->NewAutoTactic(m_eCombat,pAttack->tactic))
	//					{
	//						LLOG("{%d}defence auto combat",m_unique_id);
	//						msg.set_type((pb::Msg_ECombatTactic)pDefence->tactic);
	//						msg.set_is_attacker(false);
	//						msg.set_attackernum(m_group_attack.size());
	//						msg.set_defendernum(m_group_defence.size());
	//						msg.set_group_id(m_unique_id);
	//						if(!m_isQuickCombat)
	//							Broadcast(pb::SMSG_COMBAT_SELECT_TACTIC_SINGLE, msg);
	//					}
	//				}
	//			}
	//		}
	//	}

	//	if (Character* hero = dynamic_cast<Character*>(pAttack))
	//	{
	//		if(!hero->IsShadow())
	//		{
	//			//如果自动战斗，则选择战术
	//			if (Player* player = sPlayerPool.GetByPlayerId(hero->GetPlayerId()))
	//			{
	//				if(player->IsAutoCombat())
	//				{
	//					if(pAttack->NewAutoTactic(m_eCombat,pDefence->tactic))
	//					{
	//						LLOG("{%d}attack auto combat",m_unique_id);
	//						msg.set_type((pb::Msg_ECombatTactic)pAttack->tactic);
	//						msg.set_is_attacker(true);
	//						msg.set_attackernum(m_group_attack.size());
	//						msg.set_defendernum(m_group_defence.size());
	//						msg.set_group_id(m_unique_id);
	//						if(!m_isQuickCombat)
	//							Broadcast(pb::SMSG_COMBAT_SELECT_TACTIC_SINGLE, msg);
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	//if(m_isQuickCombat)
	//	_UpdateOperateTime(true);
	//	
}

void CombatGroup::_NewUpdateOperateTime(bool innerCir /* = false */)
{
	LLOG("[FightLog]{%d} _NewUpdateOperateTime",m_unique_id);
	bool isFightRoundOver = false;	//是否计算完一回合战斗 dofight返回
	LLOG("[FightLog]{%d} check point A",m_unique_id);

	/****************** 战斗循环前准备 **********************/
	if(m_isCombatEnd)		//战斗结束 不进入循环
	{
		LLOG("[FightLog]{%d} check point B",m_unique_id);
		return;
	}

	if(!m_isQuickCombat)			//如果不是快速战斗
	{
		LLOG("[FightLog]{%d} check point C",m_unique_id);
		if(m_fight_calc)	//在战斗计算中，舍弃此次循环
			return;
	}
	else		//如果是快速战斗
	{
		LLOG("[FightLog]{%d} check point D",m_unique_id);
		if(!innerCir)	//如果是快速战斗的递归调用，则其他update进来的舍弃掉
		{
			if(!m_isInnerCir)		//如果不在内循环	仅第一次赋值
				m_isInnerCir = true;
			else
				return;
		}

		if(!m_fight_calc)	//如果不在计算中，则倒计时归零，直接计算
		{
			m_fight_calc = true;
			m_leftFlashSec = 0;		//动画显示时间归零
			m_leftOperateSec = 0;	//操作时间归零
		}
		else		//如果在计算中，则舍弃此次循环（自动战斗肯定不会在计算中，因为只有在计算完之后才会调用递归）
		{
			return;
		}
	}

	LLOG("[FightLog]{%d} check point E",m_unique_id);

	//检测玩家是否掉线，掉线则踢掉
	int i = 0;
	for (vector<uint64>::iterator it = m_watchPlayer.begin(); it != m_watchPlayer.end();)
	{
		if(i++ > 200) break;

		if (Player* player = sPlayerPool.GetByPlayerId(*it))
		{
			if(player->GetDataIsLost())
			{
				it = m_watchPlayer.erase(it);
				continue;
			}
		}
		it++;
	}

	LLOG("[FightLog]{%d} check point F",m_unique_id);

	if(!m_isReady)	//玩家没有点准备， 有玩家但是不开始或者没玩家，删除战斗组
	{
		LLOG("[FightLog]{%d} check point G",m_unique_id);

		LLOG("[FightLog]{%d}not ready proupid=%d",m_unique_id,m_unique_id);
		if(m_watchPlayer.size() == 0 && m_battleDeleteTime-- <= 0)		//待准备阶段没有玩家观战，则解散该战斗组
		{
			PLOG("[FightLog]{%d}release combat no watcher",m_unique_id);
			ReleaseGroup();
			ServiceMgr::UnRegister(SERVICE_COMBAT, this);
			delete this;
		}

		if(m_battleDeletePrepareTime-- <= 0)	//战斗组在规定时间内没有开启战斗，则删除战斗组
		{
			PLOG("[FightLog]{%d}release combat out of time",m_unique_id);
			ReleaseGroup();
			ServiceMgr::UnRegister(SERVICE_COMBAT, this);
			delete this;
		}
		return;			
	}

	LLOG("[FightLog]{%d} check point H",m_unique_id);

	if(m_group_attack.empty() || m_group_defence.empty())
	{
		PLOG("[FightLog]{%d}Update Important Bug！！！！！！！！！！！",m_unique_id);
		//ASSERT(0);
		//保护措施,结束战斗
		m_isCombatEnd = true;
		OnGroupCombatEnd(!m_group_attack.empty());
		return;
	}

	LLOG("[FightLog]{%d} check point I",m_unique_id);

	CombatObj* pAttack = m_group_attack.front();
	CombatObj* pDefence = m_group_defence.front();

	LLOG("{%d}####### time minite circle #######",m_unique_id);
	LLOG("{%d}unique id = %ld",m_unique_id,m_unique_id);
	LLOG("{%d}m_group_attack.size=%d,m_group_defence.size=%d",m_unique_id,m_group_attack.size(),m_group_defence.size());
	//LLOG("{%d}pAttack.hp=%d, pDefence.hp=%d, aid=%d,did=%d",m_unique_id,pAttack->CurHp(),pDefence->CurHp(),pAttack->GetTableId(),pDefence->GetTableId());

	if(pAttack->CurHp() == 0 || pDefence->CurHp() == 0)
	{
		PLOG("{%d}Update Second Important Bug！！！！！！！！！！！ ",m_unique_id);
		//ASSERT(0);
		//保护措施，令战斗正常打完
		CleanFightList();
		if (m_group_attack.empty() || m_group_defence.empty())
		{
			m_isCombatEnd = true;
			OnGroupCombatEnd(!m_group_attack.empty());
			return;
		}
		if(m_isQuickCombat)	//如果是快速战斗，则进入下一个内循环
			_NewUpdateOperateTime(true);
		return;
	}
	/****************** 战斗循环前准备 结束 **********************/

	LLOG("[FightLog]{%d} check point J",m_unique_id);

	/****************** 战斗循环流程 **********************/
	if (m_leftFlashSec > 0 && !m_isQuickCombat)	//如果不是快速战斗，且动画表现时间没有结束
	{
		LLOG("[FightLog]{%d} check point K",m_unique_id);

		if(m_watchPlayer.size() == 0)			//如果没有玩家观看，剩余播放动画时间减半
			m_leftFlashSec = m_leftFlashSec/2;
		else
			m_leftFlashSec--;		//有玩家，则计时正常减

		LLOG("{%d}rest anition time = %d",m_unique_id,m_leftFlashSec);

		if(m_leftFlashSec == 0)		//如果倒计时结束仍然没有收到动画结束消息，则开始下一回合
		{
			//bool buffret = false;
			m_leftOperateSec = GetInitOperateSeconds();					//等待操作时间 快速战斗0，其他10
			m_CurrentRound++;											//当前回合数
			LLOG("{%d}m_CurrentRound=%d",m_unique_id,m_CurrentRound);
			uint64 choosetime = sOS.TimeSeconds() + m_leftOperateSec;	//操作截止时间
			
			SendNewRoundMsg(choosetime,m_group_attack.front(),m_group_defence.front());	//发送新回合消息
		}
		else	//动画倒计时没结束就不进行下面的操作
			return;
	}

	if(m_leftFlashSec == 0 && m_isQuickCombat)		//如果是快速战斗
	{
		LLOG("[FightLog]{%d} check point L",m_unique_id);
		//bool buffret = false;
		m_leftOperateSec = GetInitOperateSeconds();	//等待操作时间 快速战斗0，其他10
		m_CurrentRound++;							//当前回合数
		LLOG("{%d}m_CurrentRound=%d",m_unique_id,m_CurrentRound);
	}

	//AI选择
	if(m_leftFlashSec == 0)		//动画播放时间为0（进入了新的回合）
	{
		LLOG("[FightLog]{%d} check point M",m_unique_id);
		LLOG("{%d}help select AI",m_unique_id);
		pb::GS2C_Combat_Select_Tactic_Single msg;

		//如果防守方是npc或者攻城部队或者幻影，则自动选择战术
		if ((pDefence->IsNpc() || pDefence->IsReinforce() || pDefence->IsShadow()) && pDefence->NewAutoTactic(m_eCombat,pAttack->tactic)){
			LLOG("{%d}Defence npc select",m_unique_id);
			msg.set_type((pb::Msg_ECombatTactic)pDefence->tactic);
			msg.set_is_attacker(false);
			msg.set_attackernum(m_group_attack.size());
			msg.set_defendernum(m_group_defence.size());
			msg.set_group_id(m_unique_id);
			if(!m_isQuickCombat)
				Broadcast(pb::SMSG_COMBAT_SELECT_TACTIC_SINGLE, msg);
		}
		
		//如果防守方是真、假武将
		if (pDefence->IsPlayer())
		{
			LLOG("{%d}help defence hero select AI",m_unique_id);
			//如果上一轮已经选择了技能，则帮助随机选择战术
			if(pDefence->GetIsUseSkill() == true && pDefence->tactic == Skill_Tactic)
			{
				LLOG("{%d}defence hero had used skill then change",m_unique_id);
				if(pDefence->NewAutoTactic(m_eCombat,pAttack->tactic))
				{
					LLOG("{%d}defence hero change",m_unique_id);
					msg.set_type((pb::Msg_ECombatTactic)pDefence->tactic);
					msg.set_is_attacker(false);
					msg.set_attackernum(m_group_attack.size());
					msg.set_defendernum(m_group_defence.size());
					msg.set_group_id(m_unique_id);
					if(!m_isQuickCombat)
						Broadcast(pb::SMSG_COMBAT_SELECT_TACTIC_SINGLE, msg);
				}
			}
			else 
			{
				//如果自动战斗，则选择战术
				if (Player* player = sPlayerPool.GetByPlayerId(pDefence->GetPlayerId()))
				{
					if(player->IsAutoCombat())
					{
						if(pDefence->NewAutoTactic(m_eCombat,pAttack->tactic))
						{
							LLOG("{%d}defence hero is auto select tactic",m_unique_id);
							msg.set_type((pb::Msg_ECombatTactic)pDefence->tactic);
							msg.set_is_attacker(false);
							msg.set_attackernum(m_group_attack.size());
							msg.set_defendernum(m_group_defence.size());
							msg.set_group_id(m_unique_id);
							if(!m_isQuickCombat)
								Broadcast(pb::SMSG_COMBAT_SELECT_TACTIC_SINGLE, msg);
						}
					}
				}
			}
		}

		//如果攻击方是npc或者攻城部队或者幻影，则自动选择战术
		if ((pAttack->IsNpc() || pAttack->IsReinforce() || pAttack->IsShadow()) && pAttack->NewAutoTactic(m_eCombat,pDefence->tactic)){
			LLOG("{%d}Attack npc select",m_unique_id);
			msg.set_type((pb::Msg_ECombatTactic)pAttack->tactic);
			msg.set_is_attacker(true);
			msg.set_attackernum(m_group_attack.size());
			msg.set_defendernum(m_group_defence.size());
			msg.set_group_id(m_unique_id);
			if(!m_isQuickCombat)
				Broadcast(pb::SMSG_COMBAT_SELECT_TACTIC_SINGLE, msg);
		}

		if (pAttack->IsPlayer())
		{
			LLOG("{%d}help attack hero select AI",m_unique_id);
			//如果上一轮已经选择了技能，则帮助随机选择战术
			if(pAttack->GetIsUseSkill() == true && pAttack->tactic == Skill_Tactic)
			{
				LLOG("{%d}attack hero had used skill then change",m_unique_id);
				if(pAttack->NewAutoTactic(m_eCombat,pDefence->tactic))
				{
					LLOG("{%d}attack hero change",m_unique_id);
					msg.set_type((pb::Msg_ECombatTactic)pAttack->tactic);
					msg.set_is_attacker(true);
					msg.set_attackernum(m_group_attack.size());
					msg.set_defendernum(m_group_defence.size());
					msg.set_group_id(m_unique_id);
					if(!m_isQuickCombat)
						Broadcast(pb::SMSG_COMBAT_SELECT_TACTIC_SINGLE, msg);
				}
			}
			else 
			{
				//如果自动战斗，则选择战术
				if (Player* player = sPlayerPool.GetByPlayerId(pAttack->GetPlayerId()))
				{
					if(player->IsAutoCombat())
					{
						if(pAttack->NewAutoTactic(m_eCombat,pDefence->tactic))
						{
							LLOG("{%d}attack hero is auto select tactic",m_unique_id);
							msg.set_type((pb::Msg_ECombatTactic)pAttack->tactic);
							msg.set_is_attacker(true);
							msg.set_attackernum(m_group_attack.size());
							msg.set_defendernum(m_group_defence.size());
							msg.set_group_id(m_unique_id);
							if(!m_isQuickCombat)
								Broadcast(pb::SMSG_COMBAT_SELECT_TACTIC_SINGLE, msg);
						}
					}
				}
			}
		}

		m_leftFlashSec = -1;
	}

	if(m_leftOperateSec>0)		//操作时间倒计时
		m_leftOperateSec--;

	LLOG("{%d}pAttack->isSelectTactic:%d,pDefence->isSelectTactic:%d",m_unique_id,pAttack->GetIsSelectTactic(),pDefence->GetIsSelectTactic());

	if (pAttack->GetIsSelectTactic() && pDefence->GetIsSelectTactic())	//都选择了战术，则进入回合计算
	{
		LLOG("{%d}################# let's fight ###############",m_unique_id);

		isFightRoundOver = CalcNewFight(pAttack, pDefence);

	}
	else if (m_leftOperateSec <= 0)		//选择战术时间超时，则自动选择战术
	{
		LLOG("{%d}################# let's auto fight ###############",m_unique_id);
		if(!pDefence->GetIsSelectTactic())
			pDefence->NewAutoTactic(m_eCombat,pAttack->tactic);
		if(!pAttack->GetIsSelectTactic())
			pAttack->NewAutoTactic(m_eCombat,pDefence->tactic);		//自动选择战术

		isFightRoundOver = CalcNewFight(pAttack, pDefence);
	}
	else if(m_leftOperateSec  <= GetInitOperateSeconds()/2)		//对于混乱效果的玩家，过几秒后才帮助选择技能
	{
		if(!pDefence->GetIsSelectTactic() && pDefence->HaveBuffWork(confuse))
			pDefence->NewAutoTactic(m_eCombat,pAttack->tactic);

		if(!pAttack->GetIsSelectTactic() && pAttack->HaveBuffWork(confuse))
			pAttack->NewAutoTactic(m_eCombat,pDefence->tactic);
	}

	if(isFightRoundOver)	//计算结果
	{
		m_leftOperateSec = 0;	//操作时间归零
		if(m_isQuickCombat)
			m_leftFlashSec = 0;	//如果快速战斗，则动画时间归零
		else
			m_leftFlashSec = Combat_Client_Flash_Time;	//不是快速战斗，则20秒动画时间
	}

	if(m_isQuickCombat)
		_NewUpdateOperateTime(true);

}

//开始新战斗流程
bool CombatGroup::CalcNewFight(CombatObj* attack, CombatObj* defence)
{
	LLOG("[FightLog]{%d} CalcNewFight",m_unique_id);

	if(!m_fight_calc)
		m_fight_calc = true;
	//else
	//	return false;

	if( attack->soldiers.size() == 0 || defence->soldiers.size() == 0 )
	{
		m_isCombatEnd = true;
		OnGroupCombatEnd(!m_group_attack.empty());
		return false;
	}

	//使用过技能则随机选一个战术
	if(defence->tactic == Skill_Tactic && (defence->GetIsUseSkill() || defence->leftSkillTimes <= 0))
	{
		LLOG("[4001] is there a bug?");
		defence->NewAutoTactic(m_eCombat,attack->tactic);
	}
	if(attack->tactic == Skill_Tactic && (attack->GetIsUseSkill() || attack->leftSkillTimes <= 0))		//选择了技能且进入第二回合,本排兵没死，则随机给一个战术
	{
		LLOG("[4001] is there a bug?");
		attack->NewAutoTactic(m_eCombat,defence->tactic);
	}

	pb::GS2C_Combat_Select_Tactic msg;	//填充战斗信息
	msg.set_attack_type((pb::Msg_ECombatTactic)attack->tactic);
	msg.set_defence_type((pb::Msg_ECombatTactic)defence->tactic);

	//进入流程化计算
	FightInOrder(msg,attack,defence);

	uint64 overtime = sOS.TimeSeconds() + m_leftFlashSec;
	msg.set_over_time(overtime);
	msg.set_attackernum(m_group_attack.size());
	msg.set_defendernum(m_group_defence.size());
	msg.set_roundcount(m_CurrentRound);
	msg.set_group_id(m_unique_id);
	msg.set_attshadownum(GetShadowNum(true));
	msg.set_defshadownum(GetShadowNum(false));

	if(!m_isQuickCombat)
	{
		Broadcast(pb::SMSG_COMBAT_SELECT_TACTIC, msg);
	}


	// 有一方打光了，结束战斗
	if (m_group_attack.empty() || m_group_defence.empty())
	{
		m_isCombatEnd = true;
		OnGroupCombatEnd(!m_group_attack.empty());
		return false;
	}

	//自动国战
	AfterFightAutoCountryWar();

	m_fight_calc = false;
	return true;
}

void CombatGroup::AfterFightAutoCountryWar()
{
	if(m_eCombat == Combat::CT_Country)
	{
		//---------先突进----------//
		uint32 countAtt = m_group_attack.size();
		uint32 countDef = m_group_defence.size();
		uint32 countDis = 0;
		uint32 countTmp = 0;
		uint64 timeNow = sOS.TimeSeconds();
		Combat::GroupLst lstRush;
		if(countAtt > countDef * 2)	//进攻队列是防守队列的两倍多
		{
			countDis = countAtt - countDef * 2;
			for(Combat::GroupLst::iterator it = m_group_attack.begin(); it != m_group_attack.end(); it++)
			{
				if(countTmp >= countDis) break;

				if(Character* hero = dynamic_cast<Character*>(*it))
				{
					if(!hero->isFighting && !hero->IsShadow())	//没有战斗 不是幻影
					{
						if(hero->GetIsInAutoCountryWar())	//武将是自动国战,添加到突进列表
						{
							lstRush.push_back(*it);
							countTmp++;
						}
					}
				}
			}
		}
		else if(countDef > countAtt * 2)
		{
			countDis = countDef - countAtt * 2;
			for(Combat::GroupLst::iterator it = m_group_defence.begin(); it != m_group_defence.end(); it++)
			{
				if(countTmp >= countDis) break;

				if(Character* hero = dynamic_cast<Character*>(*it))
				{
					if(!hero->isFighting && !hero->IsShadow())	//没有战斗 不是幻影
					{
						if(hero->GetIsInAutoCountryWar())	//武将是自动国战,添加到突进列表
						{
							lstRush.push_back(*it);
							countTmp++;
						}
					}
				}
			}
		}

		//突进队列
		for(Combat::GroupLst::iterator it = lstRush.begin(); it != lstRush.end(); it++)
		{
			uint32 nextCityId = 0;
			Character* hero = dynamic_cast<Character*>(*it);
			hero->GetNextCityId(nextCityId);
			if (City* pDestCity = sWorldMap.GetCity(nextCityId))
			{
				if (pDestCity->IsFighting() || hero->GetCountryId() != pDestCity->GetCountryId())	//是否处于国战状态
				{
					if(hero->m_player->rush_cd_endtime > timeNow)	//玩家突进cd中
					{
						continue;
					}

					hero->_ExitCity(*sWorldMap.GetCity(hero->GetCurCity()));
					hero->_EnterCity(*pDestCity); 

					sPersonTask.addProgress(RUSH_IN_COUNTRY,1,*(hero->m_player));
#ifdef _SERVER_CROSS_
					sCrossDestroy.addProgress(RUSH_IN_COUNTRY,1,*(hero->m_player));
#endif

					if(::Country* country = sWorldMap.GetCountry(hero->m_player->GetCountryId()))
					{
						if(country->GetHasShield())
							hero->m_player->rush_cd_endtime = timeNow + GET_BASE_DEF_UINT(pb::BD_CROSS_DEFENSE_SHIELD_CD_TIME);
					}

					hero->m_player->UpdateAllDailyQuestInfo(TUJING,1,0);//每日任务  突进
				}
			}
		}

		//---------再诱敌----------//
		Combat::GroupLst lstAttract;
		//攻方先诱
		for(Combat::GroupLst::iterator it = m_group_attack.begin(); it != m_group_attack.end(); it++)
		{
			if(Character* hero = dynamic_cast<Character*>(*it))
			{
				if(!hero->isFighting && !hero->IsShadow())	//没有战斗 不是幻影
				{
					if(hero->GetIsInAutoCountryWar())	//武将是自动国战,添加到诱敌列表
					{
						lstAttract.push_back(*it);
					}
				}
			}
		}
		for(Combat::GroupLst::iterator it = lstAttract.begin(); it != lstAttract.end(); it++)
		{
			Character* hero = dynamic_cast<Character*>(*it);
			pb::GS2C_Combat_Attract_Enemy msg;
			bool attret = AutoAttractEnemy(hero,true,msg);
			//TODO: 发送给观战的本人（考虑到价值不大，暂时不占用资源）
			//if(attret)	//成功
			//{
			//}
		}

		//守方后诱
		lstAttract.clear();
		for(Combat::GroupLst::iterator it = m_group_defence.begin(); it != m_group_defence.end(); it++)
		{
			if(Character* hero = dynamic_cast<Character*>(*it))
			{
				if(!hero->isFighting && !hero->IsShadow())	//没有战斗 不是幻影
				{
					if(hero->GetIsInAutoCountryWar())	//武将是自动国战,添加到诱敌列表
					{
						lstAttract.push_back(*it);
					}
				}
			}
		}
		for(Combat::GroupLst::iterator it = lstAttract.begin(); it != lstAttract.end(); it++)
		{
			Character* hero = dynamic_cast<Character*>(*it);
			pb::GS2C_Combat_Attract_Enemy msg;
			bool attret = AutoAttractEnemy(hero,false,msg);
			//TODO: 发送给观战的本人（考虑到价值不大，暂时不占用资源）
			//if(attret)	//成功
			//{
			//}
		}
	}
}

void CombatGroup::FightInOrder(pb::GS2C_Combat_Select_Tactic& msg, CombatObj* attack, CombatObj* defence)
{
	LLOG("[FightLog]{%d}CombatGroup::FightInOrder attack soldiers:%d  defence soldiers:%d",m_unique_id,attack->soldiers.size(), defence->soldiers.size());

	std::vector<E_Combat_Serialize> combatSes;
	if (attack->tactic == Skill_Tactic && defence->tactic == Skill_Tactic)
	{
		combatSes.push_back(CoSe_SkillHurt_Att);	//2
		combatSes.push_back(CoSe_BuffBefore);		//3
		combatSes.push_back(CoSe_SkillHurt_Def);	//4
		combatSes.push_back(CoSe_BuffAfter);		//5
		combatSes.push_back(CoSe_CutToDie);			//7
		combatSes.push_back(CoSe_BuffMove);			//20
	}
	else if(attack->tactic == Skill_Tactic)
	{
		combatSes.push_back(CoSe_SkillHurt_Att);	//2
		combatSes.push_back(CoSe_BuffBefore);		//3
		combatSes.push_back(CoSe_BuffAfter);		//5
		combatSes.push_back(CoSe_CutToDie);			//7
		combatSes.push_back(CoSe_BuffMove);			//20
	}
	else if(defence->tactic == Skill_Tactic)
	{
		combatSes.push_back(CoSe_SkillHurt_Def);	//4
		combatSes.push_back(CoSe_BuffAfter);		//5
		combatSes.push_back(CoSe_BuffSpare);		//6
		combatSes.push_back(CoSe_CutToDie);			//7
		combatSes.push_back(CoSe_BuffMove);			//20
	}
	else
	{
		combatSes.push_back(CoSe_TacticCrash);		//1
		combatSes.push_back(CoSe_BuffBefore);		//3
		combatSes.push_back(CoSe_BuffAfter);		//5
		combatSes.push_back(CoSe_CutToDie);			//7
		combatSes.push_back(CoSe_BuffMove);			//20
	}


	E_Combat_Serialize next_step = CoSe_Null;
	E_Combat_Serialize combat_step = combatSes.front();
	combatSes.erase(combatSes.begin());	//获取一个，移除一个

	while(combat_step != CoSe_Null)
	{
		LLOG("[FightLog]{%d}combat_step = %d",m_unique_id,(int)combat_step);

		if(combatSes.size() > 0)	//如果接下来还有步走
			next_step = combatSes.front();
		LLOG("[FightLog]{%d}next_step = %d",m_unique_id,(int)next_step);

		switch(combat_step)
		{
		case CoSe_TacticCrash:			//攻守方互冲
			{
				Combat_Seque* sequeOne = msg.add_sequelst();
				sequeOne->set_sequetype(combat_step);

				sCombatCalc.DoHurt_Crash(*sequeOne, this);

				pb::Combat_All_Buff* buffinfo = sequeOne->mutable_soldierbufflst();
				SaveTo(*buffinfo);
			}
			break;
		case CoSe_SkillHurt_Att:		//攻方使用技能
			{
				Combat_Seque* sequeOne = msg.add_sequelst();
				sequeOne->set_sequetype(combat_step);

				sCombatCalc.DoHurt_Skill(*sequeOne, this,combat_step);

				pb::Combat_All_Buff* buffinfo = sequeOne->mutable_soldierbufflst();
				SaveTo(*buffinfo);

				msg.set_is_attack_skill_success(attack->isSkillSuccess);
				msg.set_attskillid(attack->SkillID());
			}
			break;
		case CoSe_BuffBefore:			//攻方使用技能后结算buff
			{
				Combat_Seque* sequeOne = msg.add_sequelst();
				sequeOne->set_sequetype(combat_step);

				sCombatCalc.DoHurt_Buff(*sequeOne,this,combat_step);

				pb::Combat_All_Buff* buffinfo = sequeOne->mutable_soldierbufflst();
				SaveTo(*buffinfo);

				if(attack->ForeHp() == 0 || defence->ForeHp() == 0)	//攻方或守方的第一行死亡，则跳到5
					next_step = CoSe_BuffAfter;
			}
			break;
		case CoSe_SkillHurt_Def:		//守方使用技能
			{
				Combat_Seque* sequeOne = msg.add_sequelst();
				sequeOne->set_sequetype(combat_step);

				sCombatCalc.DoHurt_Skill(*sequeOne,this,combat_step);

				pb::Combat_All_Buff* buffinfo = sequeOne->mutable_soldierbufflst();
				SaveTo(*buffinfo);

				msg.set_is_defence_skill_success(defence->isSkillSuccess);
				msg.set_defskillid(defence->SkillID());
			}
			break;
		case CoSe_BuffAfter:			//守方使用技能后结算buff
			{
				Combat_Seque* sequeOne = msg.add_sequelst();
				sequeOne->set_sequetype(combat_step);

				sCombatCalc.DoHurt_Buff(*sequeOne,this,combat_step);

				pb::Combat_All_Buff* buffinfo = sequeOne->mutable_soldierbufflst();
				SaveTo(*buffinfo);

				if(attack->ForeHp() == 0 || defence->ForeHp() == 0)	//攻方或守方的第一行死亡，则跳到20
					next_step = CoSe_BuffMove;
			}
			break;
		case CoSe_BuffSpare:			//守方释放技能，攻方不放技能，结算buff
			{
				Combat_Seque* sequeOne = msg.add_sequelst();
				sequeOne->set_sequetype(combat_step);

				sCombatCalc.DoHurt_Buff(*sequeOne,this,CoSe_BuffBefore);	//特殊的buff处理阶段

				pb::Combat_All_Buff* buffinfo = sequeOne->mutable_soldierbufflst();
				SaveTo(*buffinfo);

				if(attack->ForeHp() == 0 || defence->ForeHp() == 0)	//攻方或守方的第一行死亡，则跳到20
					next_step = CoSe_BuffMove;
			}
			break;
		case CoSe_CutToDie:				//互砍
			{
				Combat_Seque* sequeOne = msg.add_sequelst();
				sequeOne->set_sequetype(combat_step);

				sCombatCalc.DoHurt_Cut(*sequeOne,this);

				pb::Combat_All_Buff* buffinfo = sequeOne->mutable_soldierbufflst();
				SaveTo(*buffinfo);
			}
			break;
		case CoSe_BuffMove:				//移动后结算buff
			{
				//TODO:首先清理战场，将死亡的删除
				CleanFightList();

				//更新武将上阵状态
				_RefreshFightingObj(m_group_attack);
				_RefreshFightingObj(m_group_defence);

				//如果其中一方全部死亡，则跳出序列化
				if(m_group_attack.size() == 0 || m_group_defence.size() == 0)
				{
					next_step = CoSe_Null;
					break;
				}

				Combat_Seque* sequeOne = msg.add_sequelst();
				sequeOne->set_sequetype(combat_step);

				sCombatCalc.DoHurt_Buff(*sequeOne, this,combat_step);

				pb::Combat_All_Buff* buffinfo = sequeOne->mutable_soldierbufflst();
				SaveTo(*buffinfo);

				if(m_group_attack.front()->ForeHp() == 0 || m_group_defence.front()->ForeHp() == 0)	//攻方或守方的第一行死亡，则跳到20
					next_step = CoSe_BuffMove;
				else
					next_step = CoSe_Null;		//结束
			}
			break;
		}
		
		while(combatSes.size() > 0)
		{
			if(next_step != combatSes.front())
				combatSes.erase(combatSes.begin());
			else
			{
				combatSes.erase(combatSes.begin());
				break;
			}
		}
		combat_step = next_step;
	}

}

void CombatGroup::CleanFightList()
{
	int herocount = 0;	//作用武将个数纪录
	GroupLst::iterator it = m_group_attack.begin();	//攻方武将队列
	for(; it != m_group_attack.end();)
	{
		if(herocount++  > 6)	//最多6个武将，避免国战太多武将
			break;

		CombatObj* comobj = *it;

		for(std::vector<SoldierRaw>::iterator ite = comobj->soldiers.begin(); ite != comobj->soldiers.end();)
		{
			if(ite->hp <= 0)	//如果该排士兵死亡，则删除
			{
				ite = comobj->soldiers.erase(ite);
			}
			else
				ite++;
		}

		if(comobj->soldiers.size() == 0)	//如果武将死亡
		{
			it = m_group_attack.erase(it);
			MakeObjDead(comobj);
		}
		else
			it++;
	}

	herocount = 0;
	GroupLst::iterator itr = m_group_defence.begin();	//攻方武将队列
	for(; itr != m_group_defence.end();)
	{
		if(herocount++  > 6)	//最多6个武将，避免国战太多武将
			break;

		CombatObj* comobj = *itr;

		for(std::vector<SoldierRaw>::iterator ite = comobj->soldiers.begin(); ite != comobj->soldiers.end();)
		{
			if(ite->hp <= 0)	//如果该排士兵死亡，则删除
			{
				ite = comobj->soldiers.erase(ite);
			}
			else
				ite++;
		}

		if(comobj->soldiers.size() == 0)	//如果武将死亡
		{
			itr = m_group_defence.erase(itr);
			MakeObjDead(comobj);
		}
		else
			itr++;
	}
}

void CombatGroup::MakeObjDead(CombatObj* comobj)
{
	CombatObj* charkiller = NULL;	//杀手武将
	if(comobj->killerPID > 0)	//如果有杀手
	{
		if(Player* player = sPlayerPool.GetByPlayerId(comobj->killerPID))	//获得杀手
		{
			if(comobj->killerCID > 0)
			{
				if(Character* charone = player->m_characterStorage->GetCharacterByTableId(comobj->killerCID))	//杀手武将
				{
					charkiller = charone;
					if(charone->GetIsInAutoCountryWar())	//武将是自动国战
					{
						pb::GS2C_AC_Army_Info ac_msg;
						ac_msg.set_heroid(charone->GetID());
						ac_msg.set_killorbekilled(true);
						if(::Character* atthero = dynamic_cast<::Character*>(comobj))	//死亡的武将
						{
							ac_msg.set_killname(atthero->m_player->CharName().c_str());
						}
						ac_msg.set_protoid(comobj->GetTableId());
						ac_msg.set_damage(charone->killOnceAHero);
						player->Send(pb::SMSG_AC_ARMY_INFO, ac_msg);
						charone->killOnceAHero = 0;
					}
				}
			}
		}
	}

	comobj->ClearCombatStatus();		//清理武将状态

	_NotifyClientHeroStatus(comobj);		//更新武将战斗状态

	if(comobj->IsPlayer() && m_eCombat == CT_Teach_Map)
	{
		if( ::Character* hero = dynamic_cast<::Character*>(comobj))
		{
			hero->RecoverFullSoldier();
			hero->m_player->m_characterStorage->SetModifiedID(hero->GetID());
		}
	}

	if(comobj->IsNpc() && m_eCombat == Combat::CT_Fog)
	{
		for (CallBackDead::iterator it = m_NpcDeadCallBack.begin(); it != m_NpcDeadCallBack.end(); ++it)
		{
			(*it)(this, comobj);
		}
	}

	comobj->OnDead(charkiller); //TODO：通知client，死亡情况

}

//进入战斗计算流程
bool CombatGroup::DoFight1(CombatObj* attack, CombatObj* defence)		//选择了战术	通知双方操作码
{
	uint64 time1 = sOS.TimeMS();
	LLOG("[1005] attcount=%d, defcount=%d",m_group_attack.size(),m_group_defence.size());
	if(!m_fight_calc)
	{
		m_fight_calc = true;
	}

	//下发所有结果
    pb::GS2C_Combat_Select_Tactic msg;

	LLOG("{%d}#### before battle ####  attack soldiers:%d  defence soldiers:%d",m_unique_id,attack->soldiers.size(), defence->soldiers.size());
	if( attack->soldiers.size() == 0 || defence->soldiers.size() == 0 )
	{
		//ASSERT(0);
		m_isCombatEnd = true;
		OnGroupCombatEnd(!m_group_attack.empty());
		return false;
	}

	LLOG("{%d}#### before battle ####  attack soldiers first:%d  defence soldiers first:%d",m_unique_id,attack->soldiers.front().hp, defence->soldiers.front().hp);

	DoFight2(attack, defence, msg);

	LLOG("{%d}#### after battle ####  attack soldiers num:%d  defence soldiers num:%d",m_unique_id,attack->soldiers.size(), defence->soldiers.size());
	if(attack->soldiers.size()>0)
		LLOG("{%d}attack.soldiers.hp = %d",m_unique_id,attack->soldiers.front().hp);
	if(defence->soldiers.size()>0)
		LLOG("{%d}defence.soldiers.hp = %d",m_unique_id,defence->soldiers.front().hp);
	//LLOG("#### after battle ####  attack soldiers:%d  defence soldiers:%d",attack.soldiers.front(), defence.soldiers.front());

	msg.set_attack_type((pb::Msg_ECombatTactic)attack->tactic);
	msg.set_defence_type((pb::Msg_ECombatTactic)defence->tactic);
    msg.set_is_attack_skill_success(attack->isSkillSuccess);
    msg.set_is_defence_skill_success(defence->isSkillSuccess);
	uint64 overtime = sOS.TimeSeconds() + m_leftFlashSec;
	msg.set_over_time(overtime);
	msg.set_attackernum(m_group_attack.size());
	msg.set_defendernum(m_group_defence.size());
	msg.set_roundcount(m_CurrentRound);
	msg.set_group_id(m_unique_id);
	LLOG("{%d}opertime = %d, flashtime = %d",m_unique_id,m_leftOperateSec,m_leftFlashSec);
	LLOG("{%d}Now Time = %lld  Over Time = %lld",m_unique_id,sOS.TimeSeconds(), msg.over_time());

	uint32 countnum = 0;
	if(m_eCombat == Combat::CT_Country)
	{
		for(uint32 i = 0; i < m_group_attack.size(); i++)
		{
			CombatObj* pAttack = m_group_attack[i];
			if(pAttack)
			{
				if(pAttack->IsShadow())
					countnum++;
			}
		}
		msg.set_attshadownum(countnum);

		countnum = 0;
		for(uint32 i = 0; i < m_group_defence.size(); i++)
		{
			CombatObj* pDef = m_group_defence[i];
			if(pDef)
			{
				if(pDef->IsShadow())
					countnum++;
			}
		}
		msg.set_defshadownum(countnum);
	}

	if(!m_isQuickCombat)
	{
		Broadcast(pb::SMSG_COMBAT_SELECT_TACTIC, msg);
	}

	_CheckDead();	//清理武将

	// 有一方打光了，结束战斗
	if (m_group_attack.empty() || m_group_defence.empty())
	{
		LLOG("{%d}################# battle end #############  attack soldiers:%d  defence soldiers:%d",m_unique_id,attack->soldiers.size(), defence->soldiers.size());
		//LLOG("################# battle end #############  attack soldiers:%d  defence soldiers:%d",attack.soldiers.front(), defence.soldiers.front());
		// printf("CombatEnd(%d) -- attack soldiers:%d  defence soldiers:%d \n\n", m_unique_id, attack.soldiers.size(), defence.soldiers.size());
		
		m_isCombatEnd = true;
		OnGroupCombatEnd(!m_group_attack.empty());
		return false;
	}
	LLOG("{%d}################# battle not end #############  ",m_unique_id);

	// 更新上阵情况		移动
	_RefreshFightingObj(m_group_attack);
	_RefreshFightingObj(m_group_defence);

	//自动国战的判断
	if(m_eCombat == Combat::CT_Country)
	{
		//---------先突进----------//
		uint32 countAtt = m_group_attack.size();
		uint32 countDef = m_group_defence.size();
		uint32 countDis = 0;
		uint32 countTmp = 0;
		uint64 timeNow = sOS.TimeSeconds();
		Combat::GroupLst lstRush;
		if(countAtt > countDef * 2)	//进攻队列是防守队列的两倍多
		{
			countDis = countAtt - countDef * 2;
			for(Combat::GroupLst::iterator it = m_group_attack.begin(); it != m_group_attack.end(); it++)
			{
				if(countTmp >= countDis) break;

				if(Character* hero = dynamic_cast<Character*>(*it))
				{
					if(!hero->isFighting && !hero->IsShadow())	//没有战斗 不是幻影
					{
						if(hero->GetIsInAutoCountryWar())	//武将是自动国战,添加到突进列表
						{
							lstRush.push_back(*it);
							countTmp++;
						}
					}
				}
			}
		}
		else if(countDef > countAtt * 2)
		{
			countDis = countDef - countAtt * 2;
			for(Combat::GroupLst::iterator it = m_group_defence.begin(); it != m_group_defence.end(); it++)
			{
				if(countTmp >= countDis) break;

				if(Character* hero = dynamic_cast<Character*>(*it))
				{
					if(!hero->isFighting && !hero->IsShadow())	//没有战斗 不是幻影
					{
						if(hero->GetIsInAutoCountryWar())	//武将是自动国战,添加到突进列表
						{
							lstRush.push_back(*it);
							countTmp++;
						}
					}
				}
			}
		}

		//突进队列
		for(Combat::GroupLst::iterator it = lstRush.begin(); it != lstRush.end(); it++)
		{
			uint32 nextCityId = 0;
			Character* hero = dynamic_cast<Character*>(*it);
			hero->GetNextCityId(nextCityId);
			if (City* pDestCity = sWorldMap.GetCity(nextCityId))
			{
				if (pDestCity->IsFighting() || hero->GetCountryId() != pDestCity->GetCountryId())	//是否处于国战状态
				{
					if(hero->m_player->rush_cd_endtime > timeNow)	//玩家突进cd中
					{
						continue;
					}

					hero->_ExitCity(*sWorldMap.GetCity(hero->GetCurCity()));
					hero->_EnterCity(*pDestCity); 

					sPersonTask.addProgress(RUSH_IN_COUNTRY,1,*(hero->m_player));
#ifdef _SERVER_CROSS_
					sCrossDestroy.addProgress(RUSH_IN_COUNTRY,1,*(hero->m_player));
#endif

					if(::Country* country = sWorldMap.GetCountry(hero->m_player->GetCountryId()))
					{
						if(country->GetHasShield())
							hero->m_player->rush_cd_endtime = timeNow + GET_BASE_DEF_UINT(pb::BD_CROSS_DEFENSE_SHIELD_CD_TIME);
					}

					hero->m_player->UpdateAllDailyQuestInfo(TUJING,1,0);//每日任务  突进
				}
			}
		}

		//---------再诱敌----------//
		Combat::GroupLst lstAttract;
		//攻方先诱
		for(Combat::GroupLst::iterator it = m_group_attack.begin(); it != m_group_attack.end(); it++)
		{
			if(Character* hero = dynamic_cast<Character*>(*it))
			{
				if(!hero->isFighting && !hero->IsShadow())	//没有战斗 不是幻影
				{
					if(hero->GetIsInAutoCountryWar())	//武将是自动国战,添加到诱敌列表
					{
						lstAttract.push_back(*it);
					}
				}
			}
		}
		for(Combat::GroupLst::iterator it = lstAttract.begin(); it != lstAttract.end(); it++)
		{
			Character* hero = dynamic_cast<Character*>(*it);
			pb::GS2C_Combat_Attract_Enemy msg;
			bool attret = AutoAttractEnemy(hero,true,msg);
			//TODO: 发送给观战的本人（考虑到价值不大，暂时不占用资源）
			//if(attret)	//成功
			//{
			//}
		}
		
		//守方后诱
		lstAttract.clear();
		for(Combat::GroupLst::iterator it = m_group_defence.begin(); it != m_group_defence.end(); it++)
		{
			if(Character* hero = dynamic_cast<Character*>(*it))
			{
				if(!hero->isFighting && !hero->IsShadow())	//没有战斗 不是幻影
				{
					if(hero->GetIsInAutoCountryWar())	//武将是自动国战,添加到诱敌列表
					{
						lstAttract.push_back(*it);
					}
				}
			}
		}
		for(Combat::GroupLst::iterator it = lstAttract.begin(); it != lstAttract.end(); it++)
		{
			Character* hero = dynamic_cast<Character*>(*it);
			pb::GS2C_Combat_Attract_Enemy msg;
			bool attret = AutoAttractEnemy(hero,false,msg);
			//TODO: 发送给观战的本人（考虑到价值不大，暂时不占用资源）
			//if(attret)	//成功
			//{
			//}
		}
	}

	uint64 time2 = sOS.TimeMS();
	uint64 diff = time2 - time1;
	LLOG("[1005] diff time = %llu ",diff);
	//if(m_isQuickCombat)
	m_fight_calc = false;
	return true;
}
void CombatGroup::DoFight2(CombatObj* attack, CombatObj* defence, pb::GS2C_Combat_Select_Tactic& msg)	//战斗开始
{
	LLOG("{%d}CombatGroup::DoFight2 attack soldiers:%d  defence soldiers:%d",m_unique_id,attack->soldiers.size(), defence->soldiers.size());
	//使用过技能则随机选一个战术
	if(defence->tactic == Skill_Tactic && defence->isUsedSkill)
	{
		defence->NewAutoTactic(m_eCombat,attack->tactic);
	}
	if(attack->tactic == Skill_Tactic && attack->isUsedSkill)		//选择了技能且进入第二回合,本排兵没死，则随机给一个战术
	{
		attack->NewAutoTactic(m_eCombat,defence->tactic);
	}

	//检验技能是否合法   防止出现错乱
	if(attack->isTacticLegal(attack->tactic) && !attack->IsReinforce())
		attack->RandTactic();
	if(defence->isTacticLegal(defence->tactic) && !defence->IsReinforce())
		defence->RandTactic();

	if (attack->tactic == Skill_Tactic && defence->tactic == Skill_Tactic)
	{
		sCombatCalc.UseSkill(this, m_group_attack, m_group_defence, true, true, msg, m_eCombat);	//双方都使用技能	传入战斗类型
	}
	else if (attack->tactic == Skill_Tactic)
	{
		sCombatCalc.UseSkill(this, m_group_attack, m_group_defence, true, false, msg, m_eCombat);
	}
	else if (defence->tactic == Skill_Tactic)
	{
		sCombatCalc.UseSkill(this, m_group_attack, m_group_defence, false, true, msg, m_eCombat);
	}
	else
	{
		sCombatCalc._GetBuffs(m_group_attack, m_group_defence, msg);
		sCombatCalc.Attack(this, attack, defence, msg, m_eCombat);		//直接对砍
	}	
	sCombatCalc.RefreshSoldiers(m_group_attack,m_group_defence.front(),msg,false);	//清理士兵，不清理武将
	sCombatCalc.RefreshSoldiers(m_group_defence,m_group_attack.front(),msg,true);
	LLOG("{%d}CombatGroup::DoFight2 over attack soldiers:%d  defence soldiers:%d",m_unique_id,attack->soldiers.size(), defence->soldiers.size());
	
}

//检查是否死亡
void CombatGroup::_CheckDead()
{
    CombatObj* pAttack = m_group_attack.front();
    CombatObj* pDefence = m_group_defence.front();

	CombatObj* attKiller;
	CombatObj* defKiller;

	if(pAttack->IsNpc() || pAttack->IsShadow())
		attKiller = NULL;
	else
		attKiller = pAttack;

	if(pDefence->IsNpc() || pDefence->IsShadow())
		defKiller = NULL;
	else
		defKiller = pDefence;

	if(m_eCombat != Combat::CT_Country)
	{
		attKiller = NULL;
		defKiller = NULL;
	}

	// printf("pAttack(%d), pDefence(%d) \n", pAttack, pDefence);

    CombatObj* pTmp1 = pAttack;
	if(::Character* hero = dynamic_cast<::Character*>(pTmp1))
	{
		_OtherKillStatisticData(*hero, m_eCombat);
		pTmp1->deadCntOneLoop = 0;
		pTmp1->killCntOneLoop = 0;
	}
    while (pTmp1->soldiers.empty())
    {
		CalcRewardOneLoop(pTmp1, m_eCombat);
		//CalcRewardOneLoop(pDefence, m_eCombat);

		LLOG("{%d}pTmp1 is dead(%d)", m_unique_id,pTmp1);

		if(defKiller)
		{
			if(::Character* defHero = dynamic_cast<::Character*>(defKiller))
			{
				if(defHero->GetIsInAutoCountryWar())	//武将是自动国战
				{
					pb::GS2C_AC_Army_Info ac_msg;
					ac_msg.set_heroid(defHero->GetID());
					ac_msg.set_killorbekilled(true);
					if(::Character* atthero = dynamic_cast<::Character*>(pTmp1))	//死亡的武将
					{
						ac_msg.set_killname(atthero->m_player->CharName().c_str());
					}
					ac_msg.set_protoid(pTmp1->GetTableId());
					ac_msg.set_damage(defHero->killOnceAHero);
					defHero->m_player->Send(pb::SMSG_AC_ARMY_INFO, ac_msg);
					defHero->killOnceAHero = 0;
				}
			}
		}

		//if(::Character* hero = dynamic_cast<::Character*>(pTmp1))
		//{
		//	if(hero->GetIsInAutoCountryWar())	//武将在自动国战
		//	{
		//		hero->m_player->m_AutoCountryWarLog->AddExpGet(pTmp1->statisticExpPlayer);

		//		pb::GS2C_AC_Army_Info ac_msg;
		//		ac_msg.set_heroid(hero->GetID());
		//		ac_msg.set_killorbekilled(false);
		//		hero->m_player->Send(pb::SMSG_AC_ARMY_INFO, ac_msg);
		//		hero->killOnceAHero = 0;
		//	}
		//}

		_DelObj(*pTmp1, m_group_attack);
		pTmp1->statisticExp = 0;
		pTmp1->statisticExpPlayer = 0;
		pTmp1->statisticMoney = 0;
		pTmp1->deadCntOneLoop = 0;
		pTmp1->killCntOneLoop = 0;

		//LLOG("【%d】pTmp1(%d) _DelObj\n",m_unique_id, pTmp1);

		if((*pTmp1).IsNpc() && m_eCombat == Combat::CT_Fog)
		{
			for (CallBackDead::iterator it = m_NpcDeadCallBack.begin(); it != m_NpcDeadCallBack.end(); ++it)
			{
				(*it)(this, pTmp1);
			}
		}
		
		pTmp1->OnDead(defKiller); //TODO：通知client，死亡情况
		//LLOG("【%d】pTmp1(%d) OnDead\n", m_unique_id,pTmp1);

		if (m_group_attack.empty()) break;
        pTmp1 = m_group_attack.front();
    }
    CombatObj* pTmp2 = pDefence;
	if(::Character* hero = dynamic_cast<::Character*>(pTmp2))
	{
		_OtherKillStatisticData(*hero, m_eCombat);
		pTmp2->deadCntOneLoop = 0;
		pTmp2->killCntOneLoop = 0;
	}
    while (pTmp2->soldiers.empty())
    {
		CalcRewardOneLoop(pTmp2, m_eCombat);
		//CalcRewardOneLoop(pAttack, m_eCombat);

		LLOG("{%d}pTmp2 is dead(%d)", m_unique_id,pTmp2);

		if(attKiller)
		{
			if(::Character* attHero = dynamic_cast<::Character*>(attKiller))
			{
				if(attHero->GetIsInAutoCountryWar())	//武将是自动国战
				{
					pb::GS2C_AC_Army_Info ac_msg;
					ac_msg.set_heroid(attHero->GetID());
					ac_msg.set_killorbekilled(true);
					if(::Character* atthero = dynamic_cast<::Character*>(pTmp2))	//死亡的武将
					{
						ac_msg.set_killname(atthero->m_player->CharName().c_str());
					}
					ac_msg.set_protoid(pTmp2->GetTableId());
					ac_msg.set_damage(attHero->killOnceAHero);
					attHero->m_player->Send(pb::SMSG_AC_ARMY_INFO, ac_msg);
					attHero->killOnceAHero = 0;
				}
			}
		}

		//if(::Character* hero = dynamic_cast<::Character*>(pTmp2))
		//{
		//	if(hero->GetIsInAutoCountryWar())	//武将在自动国战
		//	{
		//		hero->m_player->m_AutoCountryWarLog->AddExpGet(pTmp2->statisticExpPlayer);

		//		pb::GS2C_AC_Army_Info ac_msg;
		//		ac_msg.set_heroid(hero->GetID());
		//		ac_msg.set_killorbekilled(false);
		//		hero->m_player->Send(pb::SMSG_AC_ARMY_INFO, ac_msg);
		//		hero->killOnceAHero = 0;
		//	}
		//}

		_DelObj(*pTmp2, m_group_defence);

		pTmp2->statisticExp = 0;
		pTmp2->statisticExpPlayer = 0;
		pTmp2->statisticMoney = 0;
		pTmp2->deadCntOneLoop = 0;
		pTmp2->killCntOneLoop = 0;

		//LLOG("【%d】pTmp2(%d) _DelObj\n",m_unique_id, pTmp2);

		if((*pTmp2).IsNpc() && m_eCombat == Combat::CT_Fog)
		{
			for (CallBackDead::iterator it = m_NpcDeadCallBack.begin(); it != m_NpcDeadCallBack.end(); ++it)
			{
				(*it)(this, pTmp2);
			}
		}

        pTmp2->OnDead(attKiller);
		//LLOG("【%d】pTmp2(%d) OnDead\n",m_unique_id, pTmp2);

        if (m_group_defence.empty()) break;
        pTmp2 = m_group_defence.front();

		// printf("pTmp2(%d) front\n", pTmp2);
	}
}

void CombatGroup::_CheckBuffDead()
{
	CombatObj* pAttack = m_group_attack.front();
	CombatObj* pDefence = m_group_defence.front();
	
	//CombatObj* attKiller;
	CombatObj* defKiller;

	//if(pAttack->IsNpc() || pAttack->IsShadow())
	//	attKiller = NULL;
	//else
	//	attKiller = pAttack;

	if(pDefence->IsNpc() || pDefence->IsShadow())
		defKiller = NULL;
	else
		defKiller = pDefence;

	CombatObj* pTmp1 = pAttack;
	while (pTmp1->soldiers.empty())
	{
		CalcRewardOneLoop(pTmp1, m_eCombat);
		//CalcRewardOneLoop(pDefence, m_eCombat);

		LLOG("{%d}pTmp1(%d) \n", m_unique_id,pTmp1);


		if(defKiller)
		{
			if(::Character* defHero = dynamic_cast<::Character*>(defKiller))
			{
				if(defHero->GetIsInAutoCountryWar())	//武将是自动国战
				{
					pb::GS2C_AC_Army_Info ac_msg;
					ac_msg.set_heroid(defHero->GetID());
					ac_msg.set_killorbekilled(true);
					if(::Character* atthero = dynamic_cast<::Character*>(pTmp1))	//死亡的武将
					{
						ac_msg.set_killname(atthero->m_player->CharName().c_str());
					}
					ac_msg.set_protoid(pTmp1->GetTableId());
					ac_msg.set_damage(defHero->killOnceAHero);
					defHero->m_player->Send(pb::SMSG_AC_ARMY_INFO, ac_msg);
					defHero->killOnceAHero = 0;
				}
			}
		}

		//if(::Character* hero = dynamic_cast<::Character*>(pTmp1))
		//{
		//	if(hero->GetIsInAutoCountryWar())	//武将在自动国战
		//	{
		//		hero->m_player->m_AutoCountryWarLog->AddExpGet(pTmp1->statisticExpPlayer);

		//		pb::GS2C_AC_Army_Info ac_msg;
		//		ac_msg.set_heroid(hero->GetID());
		//		ac_msg.set_killorbekilled(false);
		//		hero->m_player->Send(pb::SMSG_AC_ARMY_INFO, ac_msg);
		//		hero->killOnceAHero = 0;
		//	}
		//}

		_DelObj(*pTmp1, m_group_attack);

		pTmp1->statisticExp = 0;
		pTmp1->statisticExpPlayer = 0;
		pTmp1->statisticMoney = 0;
		pTmp1->deadCntOneLoop = 0;
		pTmp1->killCntOneLoop = 0;

		//LLOG("【%d】pTmp1(%d) _DelObj\n",m_unique_id, pTmp1);

		if((*pTmp1).IsNpc() && m_eCombat == Combat::CT_Fog)
		{
			for (CallBackDead::iterator it = m_NpcDeadCallBack.begin(); it != m_NpcDeadCallBack.end(); ++it)
			{
				(*it)(this, pTmp1);
			}
		}

		pTmp1->OnDead(defKiller); //TODO：通知client，死亡情况
		//LLOG("【%d】pTmp1(%d) OnDead\n", m_unique_id,pTmp1);

		if (m_group_attack.empty()) break;
		pTmp1 = m_group_attack.front();
	}

}

//战斗结束的逻辑
void CombatGroup::OnGroupCombatEnd(bool isAttackWin)
{
	PLOG("{%d} [1001] OnGroupCombatEnd",m_unique_id);
	//活下来的解除战斗状态
	for (GroupLst::iterator it = m_group_attack.begin(); it != m_group_attack.end(); ++it)
	{
		(*it)->ClearCombatStatus();
        _NotifyClientHeroStatus(*it);

		if((*it)->IsPlayer() && m_eCombat == CT_Teach_Map)
		{
			if( ::Character* hero = dynamic_cast<::Character*>(*it))
			{
				hero->RecoverFullSoldier();
				hero->m_player->m_characterStorage->SetModifiedID(hero->GetID());
			}
		}
	}
	for (GroupLst::iterator it = m_group_defence.begin(); it != m_group_defence.end(); ++it)
	{
		(*it)->ClearCombatStatus();
        _NotifyClientHeroStatus(*it);
    }

    _AttractEnemyEnd(isAttackWin); //诱敌结束，剩下的回到队尾

	if(m_eCombat == Combat::CT_Fog)		//对于迷雾战，如果防守方空血则攻击方获胜 （迷雾战没有诱敌）
		if(m_group_defence.empty())
			isAttackWin = true;

    //根据战斗类型，处理结束回调，如：城池是否易主、副本通关奖励
    for (CallBackVec::iterator it = m_CallBack.begin(); it != m_CallBack.end(); ++it)
    {
		(*it)(this, isAttackWin);
    }

    ServiceMgr::UnRegister(SERVICE_COMBAT, this);
    delete this;
}

//向攻击组添加武将
bool CombatGroup::AddObjToAttack(CombatObj& obj,bool isSkillIgnore/* = false*/)
{
	LLOG("{%d} CombatGroup::AddObjToAttack",m_unique_id);
    if (!_AddObj(obj, m_group_attack, isSkillIgnore))
    {
		LLOG("{%d}add failde",m_unique_id);
        return false;
    }

    //广播给 m_viewPlayer
    pb::GS2C_Combat_Add_Obj msg;
    msg.set_group_id(m_unique_id);
    msg.set_tableid(obj.GetTableId());
    msg.set_player_id(obj.GetPlayerId());
    msg.set_player_name(obj.GetPlayerName());
    msg.set_is_attacker(true);
	msg.set_country_id(obj.GetCountryId());
	msg.set_indexincombat(obj.indexInCombat);
	msg.set_listlengh(m_group_attack.size());
	if (Character* hero = dynamic_cast<Character*>(&obj))
	{
		msg.set_is_shadow(hero->IsShadow());
	}
    Broadcast(pb::SMSG_COMBAT_ADD_OBJ, msg);
    return true;
}

//向防御组添加武将
bool CombatGroup::AddObjToDefencek(CombatObj& obj,bool isSkillIgnore/* = false*/)
{
    if (!_AddObj(obj, m_group_defence, isSkillIgnore))
    {
        return false;
    }

    //广播给 m_viewPlayer
    pb::GS2C_Combat_Add_Obj msg;
    msg.set_group_id(m_unique_id);
    msg.set_tableid(obj.GetTableId());
    msg.set_player_id(obj.GetPlayerId());
    msg.set_player_name(obj.GetPlayerName());
	msg.set_is_attacker(false);
	msg.set_country_id(obj.GetCountryId());
	msg.set_indexincombat(obj.indexInCombat);
	msg.set_listlengh(m_group_defence.size());
	if (Character* hero = dynamic_cast<Character*>(&obj))
	{
		msg.set_is_shadow(hero->IsShadow());
	}
    Broadcast(pb::SMSG_COMBAT_ADD_OBJ, msg);
    return true;
}

bool CombatGroup::AddReinforceToAttack(CombatObj& obj)
{
	int idx = 0;

	if(m_isCombatEnd) return false;
	if(obj.IsDead()) return false;	//参与国战不允许0血量

	if(Reinforce* reinhero = dynamic_cast<Reinforce*>(&obj))
	{
		for (GroupLst::iterator it = m_group_attack.begin(); it != m_group_attack.end(); ++it, ++idx)
		{
			CombatObj* attobj = (*it);
			if(Character* hero = dynamic_cast<Character*>(attobj))
			{
				if(!hero->IsShadow() && !hero->isFighting && (hero->GetPlayerId() == reinhero->GetPlayerId()))	//如果是武将，且没有上阵，且武将与增援部队同属一人
				{
					if(idx>0)
					{
						CombatObj* foreobj = m_group_attack[idx-1];
						if(Reinforce* forehero = dynamic_cast<Reinforce*>(foreobj))
						{
							if(forehero->GetPlayerId() != reinhero->GetPlayerId())	//前一个不是自己的增援部队，则塞入增援部队
							{
								reinhero->InitCombatDataSecond(m_terrainType);
								reinhero->isInCombat = true;
								reinhero->nowCombatGroupID = m_unique_id;
								reinhero->combatType = (uint32)m_eCombat; 
								obj.indexInCombat = m_idxInLst++;
								obj.isAttactInCombat = true;
								m_group_attack.insert(m_group_attack.begin()+idx,&obj);
								PLOG("{%d} [1001] addobj",m_unique_id);

								GS2C_Add_Reinforce sendmsg;
								sendmsg.set_isatt(true);
								sendmsg.set_afteridx(hero->indexInCombat);
								reinhero->SaveToGeneral(*sendmsg.mutable_obj());
								sendmsg.set_group_id(m_unique_id);
								Broadcast(SMSG_ADD_REINFORCE,sendmsg);

								return true;
							}
						}
						else
						{
							reinhero->InitCombatDataSecond(m_terrainType);
							reinhero->isInCombat = true;
							reinhero->nowCombatGroupID = m_unique_id;
							reinhero->combatType = (uint32)m_eCombat; 
							obj.indexInCombat = m_idxInLst++;
							obj.isAttactInCombat = true;
							m_group_attack.insert(m_group_attack.begin()+idx,&obj);
							PLOG("{%d} [1001] addobj",m_unique_id);

							GS2C_Add_Reinforce sendmsg;
							sendmsg.set_isatt(true);
							sendmsg.set_afteridx(hero->indexInCombat);
							reinhero->SaveToGeneral(*sendmsg.mutable_obj());
							sendmsg.set_group_id(m_unique_id);
							Broadcast(SMSG_ADD_REINFORCE,sendmsg);

							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

bool CombatGroup::AddReinforceToDefence(CombatObj& obj)
{
	int idx = 0;

	if(m_isCombatEnd) return false;
	if(obj.IsDead()) return false;	//参与国战不允许0血量

	if(Reinforce* reinhero = dynamic_cast<Reinforce*>(&obj))
	{
		for (GroupLst::iterator it = m_group_defence.begin(); it != m_group_defence.end(); ++it, ++idx)
		{
			CombatObj* attobj = (*it);
			if(Character* hero = dynamic_cast<Character*>(attobj))
			{
				if(!hero->IsShadow() && !hero->isFighting && (hero->GetPlayerId() == reinhero->GetPlayerId()))	//如果是武将，且没有上阵，且武将与增援部队同属一人
				{
					if(idx>0)
					{
						CombatObj* foreobj = m_group_defence[idx-1];
						if(Reinforce* forehero = dynamic_cast<Reinforce*>(foreobj))
						{
							if(forehero->GetPlayerId() != reinhero->GetPlayerId())	//前一个不是自己的增援部队，则塞入增援部队
							{
								reinhero->InitCombatDataSecond(m_terrainType);
								reinhero->isInCombat = true;
								reinhero->nowCombatGroupID = m_unique_id;
								reinhero->combatType = (uint32)m_eCombat; 
								obj.indexInCombat = m_idxInLst++;
								obj.isAttactInCombat = false;
								m_group_defence.insert(m_group_defence.begin()+idx,&obj);
								PLOG("{%d} [1001] addobj",m_unique_id);

								GS2C_Add_Reinforce sendmsg;
								sendmsg.set_isatt(false);
								sendmsg.set_afteridx(hero->indexInCombat);
								reinhero->SaveToGeneral(*sendmsg.mutable_obj());
								sendmsg.set_group_id(m_unique_id);
								Broadcast(SMSG_ADD_REINFORCE,sendmsg);

								return true;
							}
						}
						else
						{
							reinhero->InitCombatDataSecond(m_terrainType);
							reinhero->isInCombat = true;
							reinhero->nowCombatGroupID = m_unique_id;
							reinhero->combatType = (uint32)m_eCombat; 
							obj.indexInCombat = m_idxInLst++;
							obj.isAttactInCombat = false;
							m_group_defence.insert(m_group_defence.begin()+idx,&obj);
							PLOG("{%d} [1001] addobj",m_unique_id);

							GS2C_Add_Reinforce sendmsg;
							sendmsg.set_isatt(false);
							sendmsg.set_afteridx(hero->indexInCombat);
							reinhero->SaveToGeneral(*sendmsg.mutable_obj());
							sendmsg.set_group_id(m_unique_id);
							Broadcast(SMSG_ADD_REINFORCE,sendmsg);

							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

//删除攻击组武将
void CombatGroup::DelObjToAttack(CombatObj& obj)
{
    _DelObj(obj, m_group_attack);

	if(m_isQuickCombat) return;

    pb::GS2C_Combat_Del_Obj msg;
    msg.set_group_id(m_unique_id);
    pb::PairValueUint64* pPair = msg.add_attack();
    pPair->set_first(obj.GetPlayerId());
    pPair->set_second((uint64)obj.IsShadow());
	msg.set_listlengh(m_group_attack.size());
    Broadcast(pb::SMSG_COMBAT_DEL_OBJ, msg);
}

//删除防御组武将
void CombatGroup::DelObjToDefencek(CombatObj& obj)
{
    _DelObj(obj, m_group_defence);

	if(m_isQuickCombat) return;

    pb::GS2C_Combat_Del_Obj msg;
    msg.set_group_id(m_unique_id);
    pb::PairValueUint64* pPair = msg.add_defence();
    pPair->set_first(obj.GetPlayerId());
	pPair->set_second((uint64)obj.IsShadow());
	msg.set_listlengh(m_group_defence.size());
    Broadcast(pb::SMSG_COMBAT_DEL_OBJ, msg);
}

bool CombatGroup::_AddObj(CombatObj& obj, GroupLst& lst, bool isSkillIgnore/* = false*/)
{
	LLOG("{%d} addobj,id=%d,incombat=%d,m_isCombatEnd=%d",m_unique_id,obj.GetTableId(),obj.isInCombat,m_isCombatEnd);
	// printf("AddObj -- 1  lst.size(%d) \n", lst.size());
	//即便是武将血量为0也需要创建战斗组，需要玩家补充血量
	if(obj.isInCombat) return false;

	if(m_isCombatEnd) return false;		//战斗已经结束，不允许添加武将

	//除国战外，其他状态在收到准备消息的时候修改
	if(m_eCombat == Combat::CT_Country || (m_isReady && !m_isCombatEnd))
	{
		LLOG("{%d} is country",m_unique_id);
		if(obj.IsDead()) return false;	//参与国战不允许0血量
		LLOG("{%d} is not dead",m_unique_id);
		int oldSkillTimes = obj.leftSkillTimes;
		obj.InitCombatDataSecond(m_terrainType);
		if(isSkillIgnore)
			obj.leftSkillTimes = oldSkillTimes;
		obj.isInCombat = true;
		obj.nowCombatGroupID = m_unique_id;
		obj.combatType = (uint32)m_eCombat; 

		obj.indexInCombat = m_idxInLst++;
		obj.isAttactInCombat = (&lst == &m_group_attack)? true:false;
		
		int oldSumLine = 0;
		for (GroupLst::iterator it = lst.begin(); it != lst.end(); ++it)
		{
			oldSumLine += (*it)->soldiers.size();
		}
		if (oldSumLine < How_Many_Line_Fighting)	//如果之前的所有部队小于6行则这个武将必定上阵
		{
			obj.isFighting = true;

			//通知client新上阵武将
			if(!m_isQuickCombat)
			{
				pb::GS2C_Combat_Next_Obj msg;
				msg.set_group_id(m_unique_id);
				if (&lst == &m_group_attack)
					obj.SaveTo(*msg.mutable_attack());
				else
					obj.SaveTo(*msg.mutable_defence());

				Broadcast(pb::SMSG_COMBAT_NEXT_OBJ, msg);
			}
		}
	}
	else
	{
		LLOG("{%d} is else",m_unique_id);
		obj.InitCombatDataSecond(m_terrainType);
		obj.indexInCombat = m_idxInLst++;
		obj.isAttactInCombat = (&lst == &m_group_attack)? true:false;
	}

	//准备界面阶段不会重置武将的属性以及状态？？
    //obj.InitCombatData(); //TODO:zhoumf：测试代码，每次加入都重算战斗数据，兵力满员


	lst.push_back(&obj);
	
	/*combat_players_.push_back(&obj);*/
// 	::Character* hero = dynamic_cast<::Character*>(&obj);
// 	if(hero)
// 	{
// 		if(obj.isInCombat )
// 		{
// 			if(hero->m_player && hero->m_player->HaveTechnology(GONGCHENGGONGXUN1)) //科技。助阵、助攻、占领城池可以获得相应军功
// 			{
// 				const TecTable * tec = sTechnologyTableMgr.GetTable(GONGCHENGGONGXUN1);
// 				if(tec){
// 					hero->m_player->AddExploitValue(tec->Value1());
// 				}
// 			}
// 		}
// 		if(obj.isFighting)
// 		{
// 			if(hero->m_player && hero->m_player->HaveTechnology(GONGCHENGGONGXUN1)) //科技。助阵、助攻、占领城池可以获得相应军功
// 			{
// 				const TecTable * tec = sTechnologyTableMgr.GetTable(GONGCHENGGONGXUN1);
// 				if(tec){
// 					hero->m_player->AddExploitValue(tec->Value2());
// 				}
// 			}
// 		}
// 	}
	PLOG("{%d} [1001] addobj",m_unique_id);
    _NotifyClientHeroStatus(&obj);
    return true;
}

void CombatGroup::UpdateHerosInCombat()
{
	PLOG("{%d} [1001] UpdateHerosInCombat",m_unique_id);
	//为了预防遭受打副本的时候遭受攻城，首先把武将拖回主城
	if(m_eCombat != Combat::CT_Country)
	{
		for (GroupLst::iterator it = m_group_attack.begin(); it != m_group_attack.end(); ++it)
		{
			if (Character* hero = dynamic_cast<Character*>(*it))
			{
				if(!hero->IsShadow())
				{
					// 先把武将全拖回主城
					hero->initPath();	//停止寻路
					hero->FlyTo(sMapTableMgr.GetCapitalCity((*it)->GetCountryId()));
					//hero->SetMap(&_map);
				}
			}
		}
	}

	int oldSumLine = 0;
	for (GroupLst::iterator it = m_group_attack.begin(); it != m_group_attack.end(); ++it)
	{
		(*it)->InitCombatDataSecond(m_terrainType);
		(*it)->isInCombat = true;
		(*it)->nowCombatGroupID = m_unique_id;
		(*it)->combatType = (uint32)m_eCombat; 
		(*it)->indexInCombat = m_idxInLst++;
		if(oldSumLine < How_Many_Line_Fighting)
			(*it)->isFighting = true;
		oldSumLine += (*it)->soldiers.size();
		_NotifyClientHeroStatus(*it);
		//(*it)->isFighting = true;
	}

	oldSumLine = 0;
	for (GroupLst::iterator ite = m_group_defence.begin(); ite != m_group_defence.end(); ++ite)
	{
		(*ite)->InitCombatDataSecond(m_terrainType);
		(*ite)->isInCombat = true;
		(*ite)->nowCombatGroupID = m_unique_id;
		(*ite)->combatType = (uint32)m_eCombat; 
		(*ite)->indexInCombat = m_idxInLst++;
		if(oldSumLine < How_Many_Line_Fighting)
			(*ite)->isFighting = true;
		oldSumLine += (*ite)->soldiers.size();
		_NotifyClientHeroStatus(*ite);
		//(*ite)->isFighting = true;
	}

	if(m_eCombat == CT_Crusade)
	{
		for (CallBackBegin::iterator it = m_CombatBegin.begin(); it != m_CombatBegin.end(); ++it)
		{
			(*it)();
		}
	}
}

void CombatGroup::ReleaseHeros()
{
	GroupLst groupTmp;

	for (GroupLst::iterator it = m_group_attack.begin(); it!=m_group_attack.end(); ++it)
	{
		if(!(*it)->soldiers.empty() && !(*it)->isInCombat)
		{
			groupTmp.push_back((*it));
		}
	}
	m_group_attack.clear();
	for(GroupLst::iterator ite = groupTmp.begin();ite != groupTmp.end(); ++ite)
	{
		m_group_attack.push_back(*ite);
	}

	groupTmp.clear();
	for (GroupLst::iterator it = m_group_defence.begin(); it!=m_group_defence.end(); ++it)
	{
		if(!(*it)->soldiers.empty() && !(*it)->isInCombat)
		{
			groupTmp.push_back((*it));
		}
	}
	m_group_defence.clear();
	for(GroupLst::iterator ite = groupTmp.begin();ite != groupTmp.end(); ++ite)
	{
		m_group_defence.push_back(*ite);
	}


}

void CombatGroup::_DelObj(CombatObj& obj, GroupLst& lst)
{
    GroupLst::iterator it = std::find(
        lst.begin(),
        lst.end(),
        &obj);
    if (it == lst.end()) return;
	lst.erase(it);

	m_statisticMap[obj.GetPlayerId()].add(obj);

	if(::Character* hero = dynamic_cast<::Character*>(&obj))
	{
		IntPairVec& recordOne = m_extraAwards[hero->GetPlayerId()];
		for(IntPairVec::iterator it = hero->m_extraAwards.begin(); it != hero->m_extraAwards.end(); it++)
		{
			bool isgot = false;
			for(IntPairVec::iterator ite = recordOne.begin(); ite != recordOne.end(); ite++)
			{
				if(it->first == ite->first)
				{
					isgot = true;
					ite->second += it->second;
					break;
				}
			}
			if(!isgot)
				recordOne.push_back(*it);
		}
	}

	obj.ClearCombatStatus();
	PLOG("{%d} [1001] _DelObj",m_unique_id);
    _NotifyClientHeroStatus(&obj);		//更新武将战斗状态

	if(obj.IsPlayer() && m_eCombat == CT_Teach_Map)
	{
		if( ::Character* hero = dynamic_cast<::Character*>(&obj))
		{
			hero->RecoverFullSoldier();
			hero->m_player->m_characterStorage->SetModifiedID(hero->GetID());
		}
	}
}
CombatObj* CombatGroup::FindObj(uint64 playerId, uint32 heroId, bool isAttacker)
{
    GroupLst& lst = isAttacker ? m_group_attack : m_group_defence;
    for (GroupLst::iterator it = lst.begin(); it != lst.end(); ++it)
    {
        if (playerId == (*it)->GetPlayerId() && heroId == (*it)->GetTableId())
        {
            return *it;
        }
    }
    return NULL;
}

//旁观
void CombatGroup::Watch(uint64 playerId)
{
	for (vector<uint64>::iterator it = m_watchPlayer.begin(); it != m_watchPlayer.end(); ++it)
	{
		if(*it == playerId)
			return;
	}
    m_watchPlayer.push_back(playerId);

    //TODO：下发战斗数据
}
void CombatGroup::UnWatch(uint64 playerId)
{
    std::vector<uint64>::iterator it = std::find(
        m_watchPlayer.begin(),
        m_watchPlayer.end(),
        playerId);
    if (it == m_watchPlayer.end()) return;

    m_watchPlayer.erase(it);

	if(m_watchPlayer.size() == 0 && !m_isReady)
	{
		ReleaseGroup();
		ServiceMgr::UnRegister(SERVICE_COMBAT, this);
		delete this;
	}
}

//广播opcode
void CombatGroup::Broadcast(int opCode, google::protobuf::Message& msg, uint64 except /* = 0 */)
{
	if(m_isQuickCombat) return;

    for (vector<uint64>::iterator it = m_watchPlayer.begin(); it != m_watchPlayer.end(); ++it)
    {
        if (except == *it) continue;
        if (Player* player = sPlayerPool.GetByPlayerId(*it))
        {
            player->Send(opCode, msg);
        }
    }
}

//广播结果
void CombatGroup::BroadcastStatisticData(bool isAttackWin)
{
    pb::GS2C_Combat_End msg;
    msg.set_is_attack_win(isAttackWin);
	msg.set_group_id(m_unique_id);
    for (vector<uint64>::iterator it = m_watchPlayer.begin(); it != m_watchPlayer.end(); ++it)
    {
        if (Player* player = sPlayerPool.GetByPlayerId(*it))
        {
            SaveCombatInfosOfGains(msg,(*it));

            player->Send(pb::SMSG_COMBAT_COMBAT_END, msg);
        }
    }
}

//计算每回合奖励
void CombatGroup::CalcRewardOneLoop(CombatObj* obj, CombatType typ)
{
    ::Character* hero = dynamic_cast<::Character*>(obj);
    if (hero == NULL) return;
    
    const SoldierFoodTable* pTable = SoldierFoodTable::GetTableByLv(hero->soldierLv);
    MMO_ASSERT(pTable);
    if (pTable == NULL) return;


    // 远征，不获得经验、奖励
    if (CT_Crusade == typ) {
        //exp = 0;
        //money = 0;
    } else {
		if (hero->m_player)
		{
			sReward.Change(*hero->m_player, Reward::PlayerExp, hero->statisticExpPlayer);
			sReward.Change(*hero->m_player, Reward::Coin_Silver, hero->statisticMoney);
		}

		if(hero->IsShadow())
		{
			Character* charone = hero->m_player->m_characterStorage->MutableCharacter(hero->GetID());
			if(charone)
				charone->TryAddXP(hero->statisticExp);
		}
		else
			hero->TryAddXP(hero->statisticExp);
    }
	//LLOG("【%d】one loop calc %d,%f,%f,%d,%d,%u", obj->nowCombatGroupID, hero->deadCntOneLoop,needFood,FOOD_TO_EXP[typ],killRatio,hero->expPercent,hero->m_player.GetHerosExpUpPercent());
	LLOG("{%d} one loop award exp = %d, coin = %d",obj->nowCombatGroupID, hero->statisticExpPlayer, hero->statisticMoney);

	LLOG("{%d} CombatGroup::CalcRewardOneLoop killcnt = %d",obj->nowCombatGroupID, hero->killCntOneLoop);
    _OtherKillStatisticData(*hero, typ);

	////结算一次奖励，清空击杀比，收集统计数据

	//StatisticData& data = m_statisticMap[hero->GetPlayerId()];
	//data.statisticExp += hero->statisticExp;
	//data.statisticMoney += hero->statisticMoney;
	//data.statisticDead += hero->deadCntOneLoop;
	//data.statisticKill += hero->killCntOneLoop;

	//hero->statisticExp = 0;
	//hero->statisticMoney = 0;
 //   hero->deadCntOneLoop = 0;
 //   hero->killCntOneLoop = 0;
}

void CombatGroup::CalcRewardEnd(CombatObj* obj, CombatType typ)
{
	::Character* hero = dynamic_cast<::Character*>(obj);
	if (hero == NULL) return;

	const SoldierFoodTable* pTable = SoldierFoodTable::GetTableByLv(hero->soldierLv);
	MMO_ASSERT(pTable);
	if (pTable == NULL) return;

	// 远征，不获得经验、奖励
	if (CT_Crusade == typ) {
		//exp = 0;
		//money = 0;
	} else {
		if (hero->m_player)
		{
			sReward.Change(*hero->m_player, Reward::PlayerExp, hero->statisticExpPlayer);
			sReward.Change(*hero->m_player, Reward::Coin_Silver, hero->statisticMoney);
		}
		if(hero->IsShadow())
		{
			Character* charone = hero->m_player->m_characterStorage->MutableCharacter(hero->GetID());
			if(charone)
				charone->TryAddXP(hero->statisticExp);
		}
		else
			hero->TryAddXP(hero->statisticExp);


		if(hero->GetIsInAutoCountryWar())	//武将在自动国战
			hero->m_player->m_AutoCountryWarLog->AddExpGet(hero->statisticExpPlayer);

		hero->statisticExp = 0;
		hero->statisticExpPlayer = 0;
		hero->statisticMoney = 0;
		hero->deadCntOneLoop = 0;
		hero->killCntOneLoop = 0;
	}
}

void CombatGroup::_SaveStaticKillOnce(::Character& hero, CombatType typ, uint32 killonce)
{
	const uint64 playerId = hero.GetPlayerId();
	switch (typ) {
	case Combat::CT_Country:
		{
			LLOG("{%d} CombatGroup::_OtherKillStatisticData playerId=%lld, killCntOneLoop=%d",hero.nowCombatGroupID,playerId, killonce);
			sCountryTaskCity.AddKillCnt(playerId, killonce);//国战杀敌统计
			sOccupyCity.AddKillCnt(playerId, killonce);
			sDefenceBorderland.AddKillCnt(playerId, killonce);

			sPersonTask.addProgress(KILL_IN_COUNTRY,killonce,*(hero.m_player));
#ifdef _SERVER_CROSS_
			sCrossKill.addProgress(KILL_IN_COUNTRY,killonce,*(hero.m_player));
#endif

			if(hero.m_player->m_AutoCountryWarLog->GetAutoCountryWar())	//玩家在自动战斗
			{
				if(hero.GetIsInAutoCountryWar())	//武将也是自动战斗
					hero.m_player->m_AutoCountryWarLog->AddKillCount(killonce);
			}
		}
		break;
	case Combat::CT_Fuben:
		break;
	case Combat::CT_Fog:
		break;
	case Combat::CT_Ore:
		break;
	case Combat::CT_Crusade:
		break;
	case Combat::CT_Official:
		break;
	case Combat::CT_Monster:
		{
			if (::Country* pCountry = sWorldMap.GetCountry(hero.GetCountryId()))
			{
				pCountry->AddKillMonsterNum(playerId, killonce);
			}
		}
		break;
	default:
		break;
	}

#ifdef _SERVER_CROSS_
	if (::Country* pCountry = sWorldMap.GetCountry(hero.GetCountryId()))
	{
		pCountry->AddCrossKill(playerId,killonce);
	}
#endif
}

void CombatGroup::_OtherKillStatisticData(::Character& hero, CombatType typ)
{
    const uint64 playerId = hero.GetPlayerId();
    switch (typ) {
    case Combat::CT_Country:
        {
			LLOG("{%d} CombatGroup::_OtherKillStatisticData playerId=%lld, killCntOneLoop=%d",hero.nowCombatGroupID,playerId, hero.killCntOneLoop);
            sCountryTaskCity.AddKillCnt(playerId, hero.killCntOneLoop);//国战杀敌统计
			sOccupyCity.AddKillCnt(playerId, hero.killCntOneLoop);
			sDefenceBorderland.AddKillCnt(playerId, hero.killCntOneLoop);

			if(hero.m_player->m_AutoCountryWarLog->GetAutoCountryWar())	//玩家在自动战斗
			{
				if(hero.GetIsInAutoCountryWar())	//武将也是自动战斗
					hero.m_player->m_AutoCountryWarLog->AddKillCount(hero.killCntOneLoop);
			}
        }
        break;
    case Combat::CT_Fuben:
        break;
    case Combat::CT_Fog:
        break;
    case Combat::CT_Ore:
        break;
    case Combat::CT_Crusade:
        break;
    case Combat::CT_Official:
        break;
    case Combat::CT_Monster:
        {
            if (::Country* pCountry = sWorldMap.GetCountry(hero.GetCountryId()))
			{
                pCountry->AddKillMonsterNum(playerId, hero.killCntOneLoop);
			}
        }
        break;
    default:
        break;
    }

#ifdef _SERVER_CROSS_
	if (::Country* pCountry = sWorldMap.GetCountry(hero.GetCountryId()))
	{
		pCountry->AddCrossKill(playerId,hero.killCntOneLoop);
	}
#endif
}

//诱敌
bool CombatGroup::AttractEnemy(::Player& playe, bool isAttacker, pb::GS2C_Combat_Attract_Enemy& msg) 
{
    GroupLst& refLst1 = isAttacker ? m_group_attack : m_group_defence;
    GroupLst& refLst2 = isAttacker ? m_group_defence : m_group_attack;

	CombatObj* selfObj = NULL;
    const uint64 playerId = playe.GetGuid();
    //按序找自己未上阵的
	GroupLst::iterator it = refLst1.begin();
	for (; it != refLst1.end(); ++it)
	{
		if(Character* hero = dynamic_cast<Character*>(*it))
		{
			if(!hero->IsShadow())
			{
				if ((*it)->GetPlayerId() == playerId && false == (*it)->isFighting)
				{
					selfObj = *it;
					break;
				}
			}
		}
	}
	if (selfObj == NULL)
	{
		msg.set_is_success(false);
		return false;
	}


	int costs = selfObj->CurHp() * 3 / 1000 * 10;
	costs = (std::max)(costs,500);
	if(!playe.EnoughCurrency(eCoin,costs))
	{
		msg.set_is_success(false);
		return false;
	}

    CombatObj* pObj = NULL;
	//混乱，有几率单挑自己人
	if (isAttacker && !m_group_defence.empty())
	{
		if (City* city = sWorldMap.GetCity( m_group_defence[0]->GetCurCity() ))
		{
			if (city->HaveStatus(City::Confuse) && System::Randf(0, 1) <= 0.4f)
			{
				pObj = RandObj(refLst1, selfObj);
			}
		}
	}
	if (pObj == NULL) //随机一个对面未上阵的
	{
		pObj = RandObj(refLst2, NULL);
	}
	if (pObj == NULL)
	{
		msg.set_is_success(false);
		return false;
	}


    //记录原队伍
    pObj->oldCombatGroupUniqueID = m_unique_id;
    selfObj->oldCombatGroupUniqueID = m_unique_id;

    //从原队伍删除
	//bool selfobjHasUseSkill = selfObj->isUsedSkill;
	//if(selfObj->isUsedSkill && selfObj->CurHp()>1)
	//	selfObj->setRestHP(selfObj->CurHp()-1);
    _DelObj(*selfObj, refLst1);
	//selfObj->isUsedSkill = selfobjHasUseSkill;
	
	if(!m_isQuickCombat)
	{
		pb::GS2C_Combat_Del_Obj msg1;
		msg1.set_group_id(m_unique_id);
		pb::PairValueUint64* pPair1 = msg1.add_attack();
		pPair1->set_first(selfObj->GetPlayerId());
		pPair1->set_second((uint64)selfObj->IsShadow());
		msg1.set_indexincombat(selfObj->indexInCombat);
		msg1.set_listlengh(m_group_attack.size());
		Broadcast(pb::SMSG_COMBAT_DEL_OBJ, msg1);
	}
	AttractCombat_attack_id.insert(selfObj->GetPlayerId());

	//bool pObjHasUseSkill = pObj->isUsedSkill;
	//if(pObj->isUsedSkill && pObj->CurHp()>1)
	//	pObj->setRestHP(pObj->CurHp()-1);
	_DelObj(*pObj, refLst2);
	//pObj->isUsedSkill = pObjHasUseSkill;

	if(!m_isQuickCombat)
	{
		pb::GS2C_Combat_Del_Obj msg2;
		msg2.set_group_id(m_unique_id);
		pb::PairValueUint64* pPair2 = msg2.add_defence();
		pPair2->set_first(pObj->GetPlayerId());
		pPair2->set_second((uint64)pObj->IsShadow());
		msg2.set_indexincombat(pObj->indexInCombat);
		msg2.set_listlengh(m_group_defence.size());
		Broadcast(pb::SMSG_COMBAT_DEL_OBJ, msg2);
	}
	AttractCombat_defence_id.insert(pObj->GetPlayerId());
    //出去单挑
    CombatGroup* pNewGroup = NULL;
	if (isAttacker)
        pNewGroup = CreateGroup(*selfObj, *pObj, m_eCombat,true, m_terrainType);
	else
        pNewGroup = CreateGroup(*pObj, *selfObj, m_eCombat,true, m_terrainType);

	//pNewGroup->m_isAttractCombat = true;

    //pb::GS2C_Combat_Attract_Enemy msg;
    msg.set_group_id(pNewGroup->m_unique_id);
    msg.set_player_id(pObj->GetPlayerId());
    msg.set_hero_table_id(pObj->GetTableId());
    //playe.Send(pb::SMSG_COMBAT_ATTRACT_ENEMY, msg);

	//诱敌成功，扣除银币
	sReward.Change(playe, Reward::Coin_Silver, -costs);

	sPersonTask.addProgress(ATTRACT_IN_COUNTRY,1,playe);
#ifdef _SERVER_CROSS_
	sCrossDestroy.addProgress(ATTRACT_IN_COUNTRY,1,playe);
#endif

	msg.set_costs(costs);
	msg.set_is_success(true);
	msg.set_my_hero_id(selfObj->GetTableId());
	msg.set_my_player_id(playerId);
// 	if(pNewGroup)
// 	{
// 		delete pNewGroup;
// 		pNewGroup = NULL;
// 	}
	
    return true;
}

//国战自动诱敌
bool CombatGroup::AutoAttractEnemy(Character* charone, bool isAttacker, pb::GS2C_Combat_Attract_Enemy& msg)
{
	GroupLst& refLst1 = isAttacker ? m_group_attack : m_group_defence;	//自己方
	GroupLst& refLst2 = isAttacker ? m_group_defence : m_group_attack;	//对方

	int costs = charone->CurHp() * 3 / 1000 * 10;
	costs = (std::max)(costs,500);
	if(!charone->m_player->EnoughCurrency(eCoin,costs))
	{
		return false;
	}

	CombatObj* pObj = NULL;
	if (pObj == NULL) //获得对面第一个没有上阵的武将
	{
		pObj = GetFirstNotFightingObj(refLst2);
	}
	if (pObj == NULL)
	{
		return false;
	}


	//记录原队伍
	pObj->oldCombatGroupUniqueID = m_unique_id;
	charone->oldCombatGroupUniqueID = m_unique_id;

	//从原队伍删除

	_DelObj(*charone, refLst1);

	if(!m_isQuickCombat)
	{
		pb::GS2C_Combat_Del_Obj msg1;
		msg1.set_group_id(m_unique_id);
		pb::PairValueUint64* pPair1 = msg1.add_attack();
		pPair1->set_first(charone->GetPlayerId());
		pPair1->set_second((uint64)charone->IsShadow());
		msg1.set_indexincombat(charone->indexInCombat);
		msg1.set_listlengh(m_group_attack.size());
		Broadcast(pb::SMSG_COMBAT_DEL_OBJ, msg1);
	}
	AttractCombat_attack_id.insert(charone->GetPlayerId());

	_DelObj(*pObj, refLst2);

	if(!m_isQuickCombat)
	{
		pb::GS2C_Combat_Del_Obj msg2;
		msg2.set_group_id(m_unique_id);
		pb::PairValueUint64* pPair2 = msg2.add_defence();
		pPair2->set_first(pObj->GetPlayerId());
		pPair2->set_second((uint64)pObj->IsShadow());
		msg2.set_indexincombat(pObj->indexInCombat);
		msg2.set_listlengh(m_group_defence.size());
		Broadcast(pb::SMSG_COMBAT_DEL_OBJ, msg2);
	}
	AttractCombat_defence_id.insert(pObj->GetPlayerId());
	//出去单挑
	CombatGroup* pNewGroup = NULL;
	if (isAttacker)
		pNewGroup = CreateGroup(*charone, *pObj, m_eCombat,true,m_terrainType);
	else
		pNewGroup = CreateGroup(*pObj, *charone, m_eCombat,true,m_terrainType);

	//pNewGroup->m_isAttractCombat = true;

	//pb::GS2C_Combat_Attract_Enemy msg;
	msg.set_group_id(pNewGroup->m_unique_id);
	msg.set_player_id(pObj->GetPlayerId());
	msg.set_hero_table_id(pObj->GetTableId());
	//playe.Send(pb::SMSG_COMBAT_ATTRACT_ENEMY, msg);

	//诱敌成功，扣除银币
	sReward.Change(*(charone->m_player), Reward::Coin_Silver, -costs);

	sPersonTask.addProgress(ATTRACT_IN_COUNTRY,1,*(charone->m_player));
#ifdef _SERVER_CROSS_
	sCrossDestroy.addProgress(ATTRACT_IN_COUNTRY,1,*(charone->m_player));
#endif

	msg.set_costs(costs);
	msg.set_is_success(true);
	msg.set_my_hero_id(charone->GetTableId());
	msg.set_my_player_id(charone->m_player->GetGuid());
	return true;
}

//诱敌结果
void CombatGroup::_AttractEnemyEnd(bool isAttackWin)
{
	if(!m_isAttractCombat)	//这场战斗不是诱敌战，则不处理
		return;
	LLOG("{%d} is attract end isAttackWin=%d",m_unique_id,isAttackWin);
	BroadcastStatisticData(isAttackWin);
	SaveAllStaticData();

    GroupLst& lst = isAttackWin ? m_group_attack : m_group_defence;
    GroupLst::iterator it = lst.begin();
    while (it != lst.end())
    {
        CombatObj* obj = *(it++);

		if (Character* hero = dynamic_cast<Character*>(obj))
		{
			hero->CalculateAttr();
		}

		LLOG("{%d} oldCombatGroupUniqueID=%d",m_unique_id,obj->oldCombatGroupUniqueID);
		if (obj->oldCombatGroupUniqueID == 0)	//没有原来的战斗组，即不是诱敌战
			continue;

		//如果原来的战斗没有结束就添加进原战斗队列中
        if (CombatGroup* ptr = FindGroup(obj->oldCombatGroupUniqueID))
        {
			LLOG("{%d} add into group",m_unique_id);
            if (isAttackWin)
                ptr->AddObjToAttack(*obj,true);
            else
                ptr->AddObjToDefencek(*obj,true);
			obj->leftSkillTimes = 0;		//诱敌再次进入战斗组时技能释放次数清零
			//_NotifyClientHeroStatus(obj);
        }
        else if (City* pCity = sWorldMap.GetCity(obj->GetCurCity()))	//战斗结束了就加入地图
        {
            pCity->_AddCombatObj(*obj, obj->GetCountryId());
        }
        obj->oldCombatGroupUniqueID = 0;
    }

	//////add by zhoulunhao 加诱敌值
	if(isAttackWin)
	{
		if(Character* hero = dynamic_cast<Character*>(m_group_attack.front()))
		{
			if(hero->m_player)
			{
				hero->m_player->AddTrickValue(1);
			}
		}
	}
	else 
	{

		if(!m_group_defence.empty())

		{
			if(Character *defener = dynamic_cast<Character*>(m_group_defence.front()))
			{
				if(defener->m_player)
				{
					defener->m_player->AddTrickValue(1);
				}
			}
		}
		else
		{
			//TODO:给没有被打下的玩家的诱敌值加1
			if(!AttractCombat_defence_id.empty()) //里面的值要么是0,要么是玩家ID
			{
				Player* player = sPlayerPool.GetByPlayerId(*AttractCombat_defence_id.rbegin());
				if(player)
				{
					player->AddTrickValue(1);//
				}
			}
		}
	}

}
//刷新上阵武将
void CombatGroup::_RefreshFightingObj(GroupLst& lst)
{
    int sumLine = 0;
    for (GroupLst::iterator it = lst.begin(); it != lst.end(); ++it)
    {
        if (sumLine < How_Many_Line_Fighting)
        {
            if ((*it)->isFighting == false) //通知client新上阵武将
            {
				if(!m_isQuickCombat)
				{
					pb::GS2C_Combat_Next_Obj msg;
					msg.set_group_id(m_unique_id);

					if (&lst == &m_group_attack)
						(*it)->SaveTo(*msg.mutable_attack());
					else
						(*it)->SaveTo(*msg.mutable_defence());
					Broadcast(pb::SMSG_COMBAT_NEXT_OBJ, msg);
				}

				(*it)->isFighting = true;
				PLOG("{%d} [1001] _RefreshFightingObj",m_unique_id);
				_NotifyClientHeroStatus(*it);
				LLOG("{%d} add one hero notice id=%d", m_unique_id,(*it)->GetTableId());
            }
		}
		sumLine += (*it)->soldiers.size();
    }
}
int CombatGroup::_GetFirstNotFightingIdx(GroupLst& lst)
{
    int sumLine = 0;
    for (int i = 0; i < (int)lst.size()-1; ++i)
    {
		//LLOG("sumLine=%d,size=%d,i=%d state=%d",sumLine,lst[i]->soldiers.size(),i,lst[i]->isFighting);
        if (sumLine < How_Many_Line_Fighting && 
            sumLine + lst[i]->soldiers.size() >= How_Many_Line_Fighting)
        {
            return i + 1;
        }
        sumLine += lst[i]->soldiers.size();
		//if(lst[i]->isFighting == false)
		//	return i;
    }
    return lst.size()+1;
}
CombatObj* CombatGroup::RandObj(GroupLst& lst, CombatObj* except)
{	
	const uint startIdx = _GetFirstNotFightingIdx(lst);		//第一个非战斗状态的武将下标
	//if(startIdx == lst.size()) return NULL;

	//LLOG("startIdx=%d, size=%d",startIdx,lst.size());
	if(startIdx > lst.size()) return NULL;
	//LLOG("get one randobj");
    uint destIdx = System::Rand(startIdx, lst.size());		//第一个与最后一个之间

	if (lst[destIdx] == except)
	{
		if (destIdx+2 < lst.size()){
			return lst[destIdx-1];
		}
		else if (destIdx >= startIdx){
			return lst[destIdx-1];
		}
		else
			return NULL;
	}
    return lst[destIdx];
}

CombatObj* CombatGroup::GetFirstNotFightingObj(GroupLst& lst)
{
	for(GroupLst::iterator it = lst.begin(); it != lst.end(); it++)
	{
		if(!(*it)->isFighting)
			return (*it);
	}
	return NULL;
}

//突进 选择的突进英雄，目标城池，是否进攻方
bool CombatGroup::Rush(::Character& hero, uint16 destCityId, bool isAttacker, pb::GS2C_Rush_Result& msg) //突进
{
	if (hero.isFighting) 
	{
		msg.set_result(pb::HERO_FIGHTING);
		return false;
	};

    if (const MapCity* table = sMapTableMgr.GetMapCity(hero.GetCurCity()))
    {
		//是否相连城池
        std::vector<int>::const_iterator it = std::find(
            table->m_linkCity.begin(), 
            table->m_linkCity.end(), 
            destCityId);
        if (it == table->m_linkCity.end())
		{
			msg.set_result(pb::CITY_FAR_AWAY);
			return false;
		}

        if (City* pDestCity = sWorldMap.GetCity(destCityId))
        {
            if (pDestCity->IsFighting() || hero.GetCountryId() != pDestCity->GetCountryId())	//是否处于国战状态
            {
				if(sMapTableMgr.GetMapCity(destCityId)->IsCapital())	//不能突进到主城
				{
					msg.set_result(pb::CITY_STATE_ERR);
					return false;
				}

				if(hero.m_player->rush_cd_endtime > sOS.TimeSeconds())	//玩家突进cd中
				{
					msg.set_result(pb::RUSH_IN_CD);
					return false;
				}

				if(hero.m_player->HaveTechnology(TUJING_QIANGHUA))//科技   //玩家兵力满足2倍于对方的状态
				{
					const TecTable* tec = sTechnologyTableMgr.GetTable(TUJING_QIANGHUA);
					if(sWorldMap.GetCity(hero.GetCurCity())->isRushLegal(isAttacker,tec->Value1()))		//玩家兵力满足三倍于对方的状态
					{
						hero._ExitCity(*sWorldMap.GetCity(hero.GetCurCity()));
						hero._EnterCity(*pDestCity);  
						msg.set_result(pb::SUCCESS);

						sPersonTask.addProgress(RUSH_IN_COUNTRY,1,*(hero.m_player));
#ifdef _SERVER_CROSS_
						sCrossDestroy.addProgress(RUSH_IN_COUNTRY,1,*(hero.m_player));
#endif

						if(::Country* country = sWorldMap.GetCountry(pDestCity->GetCountryId()))
						{
							if(country->GetHasShield())
								hero.m_player->rush_cd_endtime = sOS.TimeSeconds() + GET_BASE_DEF_UINT(pb::BD_CROSS_DEFENSE_SHIELD_CD_TIME);
						}

						return true;
					}
					else
					{
						msg.set_result(pb::RUSH_LEGAL);
						return false;
					}
				}
				else
				{
					if(sWorldMap.GetCity(hero.GetCurCity())->isRushLegal(isAttacker,GET_BASE_DEF_UINT(pb::BD_SUDDEN_NEED_TROOPS_MULTIPLE)))		//玩家兵力满足三倍于对方的状态
					{
						hero._ExitCity(*sWorldMap.GetCity(hero.GetCurCity()));
						hero._EnterCity(*pDestCity);  
						msg.set_result(pb::SUCCESS);

						sPersonTask.addProgress(RUSH_IN_COUNTRY,1,*(hero.m_player));
#ifdef _SERVER_CROSS_
						sCrossDestroy.addProgress(RUSH_IN_COUNTRY,1,*(hero.m_player));
#endif

						if(::Country* country = sWorldMap.GetCountry(hero.m_player->GetCountryId()))
						{
							if(country->GetHasShield())
								hero.m_player->rush_cd_endtime = sOS.TimeSeconds() + GET_BASE_DEF_UINT(pb::BD_CROSS_DEFENSE_SHIELD_CD_TIME);
						}

						return true;
					}
					else
					{
						msg.set_result(pb::RUSH_LEGAL);
						return false;
					}
				}
				
            }
			else
			{
				msg.set_result(pb::CITY_STATE_ERR);
				return false;
			}
        }
		else
		{
			msg.set_result(pb::CITY_NOT_FOUND2);
			return false;
		}
    }
	else
	{
		msg.set_result(pb::CITY_NOT_FOUND1);
		return false;
	}
	msg.set_result(pb::OTHER_ERR);
	return false;
}
bool CombatGroup::AddSoldier(::Player& player, bool isAttacker, uint32 hero_id, uint32& addExp, uint32 desCity) //增兵
{
	addExp = 0;
	if(hero_id != 0)
	{
		Character* charone = player.m_characterStorage->MutableCharacter(hero_id);	//获得真武将
		if(charone)
		{
			if(charone->GetCurCity() != desCity) return false;	//真武将的所在城池不符合
		}
	}
	else
	{
		::Character* ptr = player.m_characterStorage->GetMaxLvHeroInCombat(m_unique_id);	//获得最大等级的真武将
		::Character* fakeptr = player.m_characterStorage->GetMaxLvFakeHeroInCombat(m_unique_id);	//获得最大等级假武将
		if (!ptr && !fakeptr)
			return false;
	}

	if(m_eCombat != CT_Teach_Map)
	{
		bool isHeroHere = player.m_characterStorage->HasCharacterInCity((uint16)desCity);
		if(!isHeroHere)
		{
			return false;
		}

		City* city = sWorldMap.GetCity(desCity);
		if(city)
		{
			uint32 countryid = city->GetCountryId();
			if(countryid == player.GetCountryId() && isAttacker)	return false;		//城市国家与玩家相同，则不可能为攻方

			if(countryid != player.GetCountryId() && !isAttacker)	return false;		//城市国家与玩家不同，玩家不可能为守方
		}
		else
			return false;
	}



    if (!sReward.Change(player, Reward::AddSoldierTimes, -1))	//没有有免费的次数则扣金币
    {
        if (!sReward.Change(player, Reward::Coin_Gold, -20))
        {
            return false;
        }
		else
		{//科技  金币增兵时玩家获得经验
			const TecTable* tec = NULL;
			if(player.HaveTechnology(ZENGBINGQIANGHUA8))
			{
				tec = sTechnologyTableMgr.GetTable(ZENGBINGQIANGHUA8);
				player.AddXP(tec->Value1());
				addExp = tec->Value1();
			}
			else if(player.HaveTechnology(ZENGBINGQIANGHUA7))
			{
				tec = sTechnologyTableMgr.GetTable(ZENGBINGQIANGHUA7);
				player.AddXP(tec->Value1());
				addExp = tec->Value1();
			}
			else if(player.HaveTechnology(ZENGBINGQIANGHUA6))
			{
				tec = sTechnologyTableMgr.GetTable(ZENGBINGQIANGHUA6);
				player.AddXP(tec->Value1());
				addExp = tec->Value1();
			}
			else if(player.HaveTechnology(ZENGBINGQIANGHUA5))
			{
				tec = sTechnologyTableMgr.GetTable(ZENGBINGQIANGHUA5);
				player.AddXP(tec->Value1());
				addExp = tec->Value1();
			}
			else if(player.HaveTechnology(ZENGBINGQIANGHUA4))
			{
				tec = sTechnologyTableMgr.GetTable(ZENGBINGQIANGHUA4);
				player.AddXP(tec->Value1());
				addExp = tec->Value1();
			}
			else if(player.HaveTechnology(ZENGBINGQIANGHUA3))
			{
				tec = sTechnologyTableMgr.GetTable(ZENGBINGQIANGHUA3);
				player.AddXP(tec->Value1());
				addExp = tec->Value1();
			}
			else if(player.HaveTechnology(ZENGBINGQIANGHUA2))
			{
				tec = sTechnologyTableMgr.GetTable(ZENGBINGQIANGHUA2);
				player.AddXP(tec->Value1());
				addExp = tec->Value1();
			}
			else if(player.HaveTechnology(ZENGBINGQIANGHUA1))
			{
				tec = sTechnologyTableMgr.GetTable(ZENGBINGQIANGHUA1);
				player.AddXP(tec->Value1());
				addExp = tec->Value1();
			}

		}
    }
	if(hero_id == 0 || (!player.m_characterStorage->MutableCharacter(hero_id) && !player.m_characterStorage->MutableFakeCharacter(hero_id)))	//武将没有找到
	{
		if (::Character* ptr = player.m_characterStorage->GetMaxLvHeroInCombat(m_unique_id))	//按最大等级武将来添加 TODO:按指定武将添加
		{
			if (isAttacker)
				AddObjToAttack(*(ptr->CreateShadow()));
			else
				AddObjToDefencek(*(ptr->CreateShadow()));

			return true;
		}
		if (::Character* ptr = player.m_characterStorage->GetMaxLvFakeHeroInCombat(m_unique_id))	//按最大等级武将来添加 TODO:按指定武将添加
		{
			if (isAttacker)
				AddObjToAttack(*(ptr->CreateShadow()));
			else
				AddObjToDefencek(*(ptr->CreateShadow()));

			return true;
		}

		return false;
	}
	else
	{
		Character* pHero = player.m_characterStorage->MutableCharacter(hero_id);
		if(pHero)
		{
			if (isAttacker)
				AddObjToAttack(*(pHero->CreateShadow()));
			else
				AddObjToDefencek(*(pHero->CreateShadow()));
			return true;
		}
		Character* pFakeHero = player.m_characterStorage->MutableFakeCharacter(hero_id);
		if(pFakeHero)
		{
			if (isAttacker)
				AddObjToAttack(*(pFakeHero->CreateShadow()));
			else
				AddObjToDefencek(*(pFakeHero->CreateShadow()));
			return true;
		}
		return false;
	}
    return false;
}
bool CombatGroup::RunAway(::Player& player, ::Character& hero, uint16 destCityId, bool isAttacker, pb::GS2C_Combat_Runaway& msg) //撤退
{
    if (hero.isFighting)
	{
		msg.set_result(pb::HERO_FIGHTING);
		return false;
	};

    if (const MapCity* table = sMapTableMgr.GetMapCity(hero.GetCurCity()))
    {
        std::vector<int>::const_iterator it = std::find(
            table->m_linkCity.begin(),
            table->m_linkCity.end(),
            destCityId);
        if (it == table->m_linkCity.end())
		{
			msg.set_result(pb::CITY_FAR_AWAY);
			return false;
		}

		if (City* pDestCity = sWorldMap.GetCity(destCityId))
		{
			if (!pDestCity->IsFighting() && pDestCity->GetCountryId() == hero.GetCountryId())
			{
				hero.HurtPerHp(0.1f); //从第一排算起，减10%兵力
				hero._ExitCity(*sWorldMap.GetCity(hero.GetCurCity()));
				hero._EnterCity(*pDestCity); 
				player.m_characterStorage->SetModifiedID( hero.GetID());
				return true;
			}
			else
			{
				msg.set_result(pb::ESCAPE_LEGEL);
				return false;
			}
		}
		else
		{
			msg.set_result(pb::CITY_NOT_FOUND2);
			return false;
		}
    }
	else
	{
		msg.set_result(pb::CITY_NOT_FOUND1);
		return false;
	}
	return false;
}

void CombatGroup::CallUp(::Player& player, uint16 destCityId, pb::GS2C_Combat_Call_Up& msg) //征召
{
#ifdef _MMO_SERVER_
	//TODO: 判断征召条件
	//if(!sTrigger.Check(player,37)) return;

	LLOG("CombatGroup::CallUp destCityId=%d",destCityId);
	bool isHeroHere = player.m_characterStorage->HasCharacterInCity(destCityId);
	if(!isHeroHere)
	{
		msg.set_result(pb::NO_HERO_HERE);
		return;
	}

	if (City* pCity = sWorldMap.GetCity(destCityId))
	{
		if(!pCity->HaveStatus(City::CallUp))
		{
			if (sReward.Change(player, Reward::Coin_Gold, -200))		//扣200金币先
			{
				sReward.Change(player, Reward::PlayerExp, 300000);
				uint64 lasttime = sOS.TimeSeconds();

				uint64 callendtime = lasttime + GET_BASE_DEF_INT(pb::BD_DRAFT_MAX_LAST_TIME) * 60;
				uint64 uniquecallid = lasttime << 16 | destCityId;
				pCity->AddCallUpCountry(player.GetCountryId(),callendtime,uniquecallid);
				pCity->SetStatus(City::CallUp, true);
				LLOG("call up success !!!!");
				// 5*60
				sAutoUpdateGame.AddEvent(
					boost::bind(&City::CallUpEndCallBack, pCity, uniquecallid),
					GET_BASE_DEF_INT(pb::BD_DRAFT_MAX_LAST_TIME) * 60
					);
				msg.set_result(pb::SUCCESS);
			}
			else
				msg.set_result(pb::NO_CALL_COSTS);
		}
		else
			msg.set_result(pb::CITY_STATE_ERR);	
    }
	else
		msg.set_result(pb::CITY_NOT_FOUND1);

#endif
	return;
}

void CombatGroup::CallRequest(::Player& player, pb::C2GS_Combat_Call_Request& callmsg, uint16 destCityId,pb::GS2C_Combat_Call_Request& msg)
{
	LLOG("CombatGroup::CallRequest destCityId=%d",destCityId);
	City* pCity = sWorldMap.GetCity(destCityId);
	::Country* country = sWorldMap.GetCountry(player.GetCountryId());
	
	if(pCity && country)
	{}
	else
	{
		msg.set_result(pb::OTHER_ERR);
		return;
	}

	for(std::vector<PlayerCallRec>::iterator it = country->m_player_call_rec.begin();
		it != country->m_player_call_rec.end(); it++)
	{
		if(it->playerid == player.GetGuid() && pCity->IsCallUpTokenInLst(it->uniqueid))	//已经使用过的ID在列表
		{
			msg.set_result(pb::CALL_DOWN);
			return;
		}
	}

	int herosize = callmsg.hero_id_size();
	uint32 heroid;
	bool hasHeroSuc = false;

	uint64 callupuniqueid = pCity->GetCallUpUniqueid(player.GetCountryId());

	for(int i=0; i<herosize; i++)
	{
		heroid = callmsg.hero_id(i);
		if(Character* pHero = player.m_characterStorage->MutableCharacter(heroid))
		{
			if(pHero->CurHp() != 0)	//空血不许征召
			{
				if (City* pCity = sWorldMap.GetCity(destCityId))
				{
					if(pCity->HaveStatus(City::CallUp) && callupuniqueid)	//当前城市有征召状态并且有该国家的征召
					{
						if(pHero->GetCurCity() != (uint32)destCityId)	//武将不在目标城池
						{
							if(!pHero->isInCombat)	//武将不在战斗队列中
							{
								if(CombatGroup* combatg = pCity->GetCombatGroup())
								{
									if(combatg->m_fight_calc)	//在计算中
									{
										msg.set_result(pb::SERVER_BUSY);
										return;
									}
								}
								pHero->_ExitCity(*sWorldMap.GetCity(pHero->GetCurCity()));
								pHero->_EnterCity(*pCity);  
								//if(pHero->FlyTo(destCityId))
								//{
								msg.set_result(pb::SUCCESS);
								msg.set_call_up_down_id(callupuniqueid);
								pHero->initPath();

								//}
								//else
								//	msg.set_result(pb::REQUEST_LEGAL);

								//TODO:给武将奖励
								hasHeroSuc = true;
								sReward.Change(player, Reward::Food, GET_BASE_DEF_INT(pb::BD_ACCEPT_DRAFT_REWARD_FOOD));
								player.AddExploitValue(GET_BASE_DEF_INT(pb::BD_ACCEPT_DRAFT_REWARD_EXPLOIT));
							}
							else
								msg.set_result(pb::HERO_BUSY);
						}
						else
							msg.set_result(pb::HERO_IN_CITY);
					}
					else
						msg.set_result(pb::CITY_STATE_ERR);
				}
				else
					msg.set_result(pb::CITY_NOT_FOUND1);
			}
			else
				msg.set_result(pb::HERO_ERR);
		}
		else
			msg.set_result(pb::HERO_ERR);
	}
	
	if(hasHeroSuc)
	{
		msg.set_result(pb::SUCCESS);
		PlayerCallRec recone;
		recone.playerid = player.GetGuid();
		recone.uniqueid = callupuniqueid;
		country->m_player_call_rec.push_back(recone);
	}

	return;
}

bool CombatGroup::SpeedUp(::Player& player, uint16 destCityId) //帅旗，加速战斗
{
    if (!player.m_characterStorage->CanSpeedUp(destCityId))
    {
        return false;
    }

    for (int i = 0; i < 20; ++i) {
        m_leftOperateSec = 0;
        _NewUpdateOperateTime();
    }
	return true;
}

void CombatGroup::SaveTo(pb::Combat_All_Buff& buffMsg)
{
	LLOG("Save To All Buff Once");
	int heroNum = 0;
	int soldierRow = 0;
	for (GroupLst::iterator it = m_group_attack.begin(); it != m_group_attack.end(); ++it)
	{
		if(heroNum > 6 || soldierRow > 6) break;

		CombatObj* obj = *it;
		for(std::vector<SoldierRaw>::iterator ite = obj->soldiers.begin(); ite != obj->soldiers.end(); ite++)
		{
			SoldierRaw& rowone = *ite;
			if(rowone.hp > 0)
			{
				LLOG("Att heroNum=%d,soldierRow=%d",heroNum,soldierRow);
				rowone.buff_fight.FillBuffInfoMsg(buffMsg,true,soldierRow);
			}
			soldierRow++;
		}
		heroNum++;
	}
	soldierRow = 0;
	heroNum = 0;
	for (GroupLst::iterator it = m_group_defence.begin(); it != m_group_defence.end(); ++it)
	{
		if(heroNum > 6 || soldierRow > 6) break;

		CombatObj* obj = *it;
		for(std::vector<SoldierRaw>::iterator ite = obj->soldiers.begin(); ite != obj->soldiers.end(); ite++)
		{
			SoldierRaw& rowone = *ite;
			if(rowone.hp > 0)
			{
				LLOG("Def heroNum=%d,soldierRow=%d",heroNum,soldierRow);
				rowone.buff_fight.FillBuffInfoMsg(buffMsg,false,soldierRow);
			}
			soldierRow++;
		}
		heroNum++;
	}
}

void CombatGroup::SaveTo(pb::GS2C_Combat_Begin& msg)
{
	LLOG("{%d} CombatGroup::SaveTo(pb::GS2C_Combat_Begin& msg)",m_unique_id);
	if(m_group_attack.size()<=0 || m_group_defence.size()<=0)
	{
		//ASSERT(0);
		return;
	}

	m_nextOperTime = sOS.TimeSeconds() + m_leftOperateSec + m_leftFlashSec;
	
	uint64 overtime =  sOS.TimeSeconds() + m_leftOperateSec + m_leftFlashSec;
	uint64 choosetime = sOS.TimeSeconds() + m_leftOperateSec;
    
	msg.set_group_id(m_unique_id);

	//应该只发排头的几个武将，剩下的边打边发，否则一次下发数据过多——可能有千人对战啊
	int idx = _GetFirstNotFightingIdx(m_group_attack);
	if(idx > (int)m_group_attack.size())
		idx = (int)m_group_attack.size();
	for (int i = 0; i < idx; ++i)
	{
		m_group_attack[i]->SaveTo(*msg.add_attack_lst());
	}

	idx = _GetFirstNotFightingIdx(m_group_defence);
	if(idx > (int)m_group_defence.size())
		idx = (int)m_group_defence.size();
	for (int i = 0; i < idx; ++i)
	{
		m_group_defence[i]->SaveTo(*msg.add_defence_lst());
	}

    msg.set_combat_type(GetType());
    msg.set_attack_cnt(m_group_attack.size());
    msg.set_defence_cnt(m_group_defence.size());
	msg.set_choose_time(choosetime);
	msg.set_over_time(overtime);

	//发送简易战斗队列列表
	int defsendsize = MIN((int)m_group_defence.size(),(int)GET_BASE_DEF_UINT(BD_INITIAL_MAX_SHOW_TROOPS));
	int attsendsize = MIN((int)m_group_attack.size(),(int)GET_BASE_DEF_UINT(BD_INITIAL_MAX_SHOW_TROOPS));
	for(int i = 0; i < attsendsize; i++)
	{
		m_group_attack[i]->SaveToGeneral(*msg.add_attack_all_list());
	}
	for(int i = 0; i < defsendsize; i++)
	{
		m_group_defence[i]->SaveToGeneral(*msg.add_defence_all_list());
	}

	//攻守双方的可释放技能
	if(m_eCombat != CT_Crusade)
	{
		Tactic_Param* tacPars = msg.mutable_tatic_param();
		
		tacPars->set_atttacticenablestr(m_group_attack.front()->GetTacticAndSkills());
		tacPars->set_deftacticenablestr(m_group_defence.front()->GetTacticAndSkills());

		tacPars->set_atttacticrealstr(m_group_attack.front()->GetTacticsAndSkillsEnable());
		tacPars->set_deftacticrealstr(m_group_defence.front()->GetTacticsAndSkillsEnable());
	}

	msg.set_terraintyp(m_terrainType);
	msg.set_roundcount(m_CurrentRound);

	uint32 countnum = 0;
	if(m_eCombat == Combat::CT_Country)
	{
		for(uint32 i = 0; i < m_group_attack.size(); i++)
		{
			CombatObj* pAttack = m_group_attack[i];
			if(pAttack)
			{
				if(pAttack->IsShadow())
					countnum++;
			}
		}
		msg.set_attshadownum(countnum);

		countnum = 0;
		for(uint32 i = 0; i < m_group_defence.size(); i++)
		{
			CombatObj* pDef = m_group_defence[i];
			if(pDef)
			{
				if(pDef->IsShadow())
					countnum++;
			}
		}
		msg.set_defshadownum(countnum);
	}

	//填充场上buff信息
	SaveTo(*msg.mutable_soldierbufflst());
}

void CombatGroup::SaveTo(pb::GS2C_Combat_Next_Lst& msg, uint32 beginindex, bool isAttack)
{
	if(isAttack)
	{
		int num = 0;
		for (GroupLst::iterator it = m_group_attack.begin(); it != m_group_attack.end(); ++it)
		{
			if((*it)->indexInCombat == beginindex)
			{
				break;
			}
			num++;
		}
		int sendnum = m_group_attack.size() - num - 1;
		if(sendnum<=0) return;

		sendnum = MIN(sendnum, (int)GET_BASE_DEF_UINT(BD_INITIAL_MAX_SHOW_TROOPS));	//发送个数

		for(int i = 0; i < sendnum; i++)
		{
			m_group_attack[++num]->SaveToGeneral(*msg.add_attack_all_list());
		}
	}
	else
	{
		int num = 0;
		for (GroupLst::iterator it = m_group_defence.begin(); it != m_group_defence.end(); ++it)
		{
			if((*it)->indexInCombat == beginindex)
			{
				break;
			}
			num++;
		}
		int sendnum = m_group_defence.size() - num - 1;
		if(sendnum<=0) return;

		sendnum = MIN(sendnum, (int)GET_BASE_DEF_UINT(BD_INITIAL_MAX_SHOW_TROOPS));	//发送个数

		for(int i = 0; i < sendnum; i++)
		{
			m_group_defence[++num]->SaveToGeneral(*msg.add_defence_all_list());
		}
	}
}

bool CombatGroup::SaveTo(pb::GS2C_Combat_Select_Tactic_Single& attmsg, pb::GS2C_Combat_Select_Tactic_Single& defmsg, int& count)
{
	if(m_group_attack.empty() || m_group_defence.empty())
		return false;

	CombatObj* pAttack = m_group_attack.front();
	CombatObj* pDefence = m_group_defence.front();
	
	if(pAttack->GetIsSelectTactic())
		count += 1;

	attmsg.set_type((pb::Msg_ECombatTactic)pAttack->tactic);
	attmsg.set_is_attacker(true);
	attmsg.set_attackernum(m_group_attack.size());
	attmsg.set_defendernum(m_group_defence.size());

	if(pDefence->GetIsSelectTactic())
		count += 2;

	defmsg.set_type((pb::Msg_ECombatTactic)pDefence->tactic);
	defmsg.set_is_attacker(false);
	defmsg.set_attackernum(m_group_attack.size());
	defmsg.set_defendernum(m_group_defence.size());

	return true;
}

void CombatGroup::SaveTo(pb::GS2C_Combat_Pre_Begin& msg)
{
	msg.set_group_id(m_unique_id);
	msg.set_combat_type(GetType());
	msg.set_attack_cnt(m_group_attack.size());
	msg.set_defence_cnt(m_group_defence.size());

	//最大发5个武将，只在准备界面显示用
	int idx = m_group_attack.size()>5? 5:m_group_attack.size();
	for (int i = 0; i < idx; ++i)
	{
		int id = m_group_attack[i]->GetTableId();
		m_group_attack[i]->SaveTo(*msg.add_attack_lst());
	}
	//idx = _GetFirstNotFightingIdx(m_group_defence);
	int Didx = m_group_defence.size()>5? 5:m_group_defence.size();
	//Didx = m_group_defence.size()>5? 5:m_group_defence.size();
	for (int i = 0; i < Didx; ++i)
	{
		m_group_defence[i]->SaveTo(*msg.add_defence_lst());
	}
	msg.set_terraintyp(m_terrainType);
}

void CombatGroup::SaveTo(pb::GS2C_City_Fight_Info& msg)
{
    std::map<int, int> tempMap;
    for (GroupLst::iterator it = m_group_attack.begin(); it != m_group_attack.end(); ++it)
    {
        ++tempMap[(*it)->GetCountryId()];
    }
    for (std::map<int, int>::iterator it = tempMap.begin(); it != tempMap.end(); ++it)
    {
        pb::Struct_Army_Info* attack = msg.add_attacker_lst();
        attack->set_army_count(it->second);
        attack->set_country(it->first);
    }


    tempMap.clear();
    for (GroupLst::iterator it = m_group_defence.begin(); it != m_group_defence.end(); ++it)
    {
        ++tempMap[(*it)->GetCountryId()];
    }
    for (std::map<int, int>::iterator it = tempMap.begin(); it != tempMap.end(); ++it)
    {
        pb::Struct_Army_Info* defend = msg.add_defender_lst();
        defend->set_army_count(it->second);
        defend->set_country(it->first);
    }
}

void CombatGroup::StartCombatAndChange(pb::C2GS_Combat_Begin& msg)
{
	if(msg.attack_lst_size() != static_cast<int>(m_group_attack.size())) return;
	std::vector<uint32> unique_Id;
	for(int i=0; i <msg.attack_lst_size(); i++)
	{
		const pb::Msg_CombatObj& obj = msg.attack_lst(i);
		uint32 id = obj.tableid();
		std::vector<uint32>::iterator iter = std::find(unique_Id.begin(),unique_Id.end(),id);
		if(iter == unique_Id.end())
		{
			unique_Id.push_back(id);
		}
	}
	GroupLst groupTmp;
	for(std::vector<uint32>::iterator iter = unique_Id.begin();iter != unique_Id.end();++iter)
	{
		for (GroupLst::iterator it = m_group_attack.begin(); it!=m_group_attack.end(); ++it)
		{
			if((*it)->GetTableId() == *iter)
			{
				groupTmp.push_back((*it));
			}
		}
	}
	m_group_attack.swap(groupTmp);
}

void CombatGroup::SaveStatisticData(pb::GS2C_Combat_End& msg, uint64 playerId)
{
	StatisticData& data = m_statisticMap[playerId];

    for (GroupLst::iterator it = m_group_attack.begin();
        it != m_group_attack.end(); ++it)
    {
        if ((*it)->GetPlayerId() == playerId)
        {
			data.add(*(*it));

			//CalcRewardEnd((*it), m_eCombat);

			if(::Character* hero = dynamic_cast<::Character*>(*it))
			{
				IntPairVec& recordOne = m_extraAwards[playerId];
				for(IntPairVec::iterator it = hero->m_extraAwards.begin(); it != hero->m_extraAwards.end(); it++)
				{
					bool isgot = false;
					for(IntPairVec::iterator ite = recordOne.begin(); ite != recordOne.end(); ite++)
					{
						if(it->first == ite->first)
						{
							isgot = true;
							ite->second += it->second;
							break;
						}
					}
					if(!isgot)
						recordOne.push_back(*it);
				}
			}
        }
    }
    for (GroupLst::iterator it = m_group_defence.begin();
        it != m_group_defence.end(); ++it)
    {
        if ((*it)->GetPlayerId() == playerId)
        {
			data.add(*(*it));

			//CalcRewardEnd((*it), m_eCombat);

			if(::Character* hero = dynamic_cast<::Character*>(*it))
			{
				IntPairVec& recordOne = m_extraAwards[playerId];
				for(IntPairVec::iterator it = hero->m_extraAwards.begin(); it != hero->m_extraAwards.end(); it++)
				{
					bool isgot = false;
					for(IntPairVec::iterator ite = recordOne.begin(); ite != recordOne.end(); ite++)
					{
						if(it->first == ite->first)
						{
							isgot = true;
							ite->second += it->second;
							break;
						}
					}
					if(!isgot)
						recordOne.push_back(*it);
				}
			}
        }
    }

	LLOG("{%d} award end exp=%d, money=%d",m_unique_id,data.statisticExpPlayer,data.statisticMoney);

    msg.set_statisticexp(data.statisticExpPlayer);
    msg.set_statisticmoney(data.statisticMoney);
    msg.set_statisticdead(data.statisticDead);
    msg.set_statistickill(data.statisticKill);

	IntPairVec& recordOne = m_extraAwards[playerId];
	for(IntPairVec::iterator it = recordOne.begin(); it != recordOne.end(); ++it)
	{
		pb::StReward* p = msg.add_extrewards();
		p->set_type(it->first);
		p->set_value(it->second);
	}
}

void CombatGroup::SaveCombatInfosOfGains(pb::GS2C_Combat_End& msg, uint64 playerId)
{
	CombatGainsRec& data = m_combatGainsMap[playerId];

	msg.set_statisticexp(data.combatPExp);
	msg.set_statisticmoney(data.combatCoin);
	msg.set_statisticdead(data.combatDeads);
	msg.set_statistickill(data.combatKills);

	IntPairVec& recordOne = data.combatExGains;
	for(IntPairVec::iterator it = recordOne.begin(); it != recordOne.end(); ++it)
	{
		pb::StReward* p = msg.add_extrewards();
		p->set_type(it->first);
		p->set_value(it->second);
	}
}

void CombatGroup::SaveAllStaticData()
{
	for (GroupLst::iterator it = m_group_attack.begin();
		it != m_group_attack.end(); ++it)
	{
		CalcRewardEnd((*it), m_eCombat);
	}
	for (GroupLst::iterator it = m_group_defence.begin();
		it != m_group_defence.end(); ++it)
	{
		CalcRewardEnd((*it), m_eCombat);
	}
}

void CombatGroup::_NotifyClientHeroStatus(CombatObj* obj)
{
	// printf("NotifyClientHeroStatus -- 1  obj(%d) \n", obj);

    ::Character* hero = dynamic_cast<::Character*>(obj);

	// printf("NotifyClientHeroStatus -- 2  hero(%d) \n", hero);

    if (hero == NULL) return;	//不是武将不发送

	if(hero->IsShadow()) return;	//幻影不发送

	PLOG("{%d} [1001] change hero:%d status,isfighting=%d",m_unique_id,obj->GetTableId(),obj->isFighting);

    pb::GS2C_Combat_Hero_Status msg;
	msg.set_hero_tid(hero->GetID());
    msg.set_is_in_combat(obj->isInCombat);
    msg.set_is_fighting(obj->isFighting);
	msg.set_is_dead(obj->IsDead());
	msg.set_combat_type(m_eCombat);
	msg.set_group_id(m_unique_id);
	msg.set_is_in_attract(m_isAttractCombat);
	msg.set_isfakechar(obj->IsFakeChar());
	if (hero->m_player)
	{
		hero->m_player->Send(pb::SMSG_COMBAT_HERO_STATUS, msg);
		if(obj->isFighting == false)	//未上阵
		{
			PLOG("{%d} [1001] _NotifyClientHeroStatus false",m_unique_id);
			if(obj->CurHp() > 0 && !m_isCombatEnd && m_isReady)	//当前有血量，战斗在进行中
			{
				PLOG("{%d} [1001] IMPORTANT BUG",m_unique_id);
				//ASSERT(false);
			}

		}
	}

	LLOG("{%d} SMSG_COMBAT_HERO_STATUS  isAttractCombat = %d, heroid = %d, isInCombat = %d, isFighting = %d, IsDead = %d, Type = %d",
						m_unique_id,m_isAttractCombat,obj->GetTableId(),obj->isInCombat,obj->isFighting,obj->IsDead(),m_eCombat);
	// printf("NotifyClientHeroStatus -- 3  hero(%d) \n", hero);
}

void CombatGroup::AddHeros(::Player& player, uint32 heroID)
{
	Character* pHero = player.m_characterStorage->MutableCharacter(heroID);
	AddObjToAttack(*(pHero->CreateShadow()));
}

void CombatGroup::ReleaseGroup()
{
	LLOG("{%d} CombatGroup::ReleaseGroup",m_unique_id);
	IntPairVec intpv;
	for(GroupLst::iterator it = m_group_attack.begin();
		it != m_group_attack.end(); ++it)
	{
		bool findhero = false;
		for(IntPairVec::iterator ite = intpv.begin(); ite != intpv.end(); ite++)
		{
			if(ite->first == (*it)->GetTableId())
			{
				findhero = true;
				ite->second += 1;
			}
		}
		if(!findhero)
			intpv.push_back(std::make_pair((*it)->GetTableId(),1));

		//if((*it)->nowCombatGroupID == m_unique_id)
		//{
		//	(*it)->ClearCombatStatus();		//清理武将状态
		//	_NotifyClientHeroStatus(*it);
		//}
	}


	for (CallBackRelease::iterator it = m_ReleaseCallBack.begin(); it != m_ReleaseCallBack.end(); ++it)
	{
		(*it)(this);
	}
}



bool Combat::CombatGroup::canCallSeigeArmy(const uint32 combat_type,const uint32 country_id,const uint32 city_s_country_id /*= 0*/)
{
	uint32 seige_count = 0,character_count = 0;
	if(combat_type == Combat::CT_Teach_Map)
	{
		for(GroupLst::iterator iter = m_group_attack.begin();iter != m_group_attack.end();++iter)
		{
			if(Reinforce* forehero = dynamic_cast<Reinforce*>(*iter))
			{
				seige_count++;
			}
			if(Character* character = dynamic_cast<Character*>(*iter))
			{
				if(character->isInCombat && !character->isFighting)
					character_count++;
			}
		}
		if(seige_count < character_count) return true;
		else return false;
	}
	else
	{
		if(country_id == city_s_country_id)
		{
			for(GroupLst::iterator iter = m_group_defence.begin();iter != m_group_defence.end();++iter)
			{
				if(Reinforce* forehero = dynamic_cast<Reinforce*>(*iter))
				{
					seige_count++;
				}
				if(Character* character = dynamic_cast<Character*>(*iter))
				{
					if(character->isInCombat && !character->isFighting)
						character_count++;
				}
			}
			if(seige_count < character_count) return true;
			else return false;
		}
		else
		{
			for(GroupLst::iterator iter = m_group_attack.begin();iter != m_group_attack.end();++iter)
			{
				if(Reinforce* forehero = dynamic_cast<Reinforce*>(*iter))
				{
					seige_count++;
				}
				if(Character* character = dynamic_cast<Character*>(*iter))
				{
					if(character->isInCombat && !character->isFighting)
						character_count++;
				}
			}
			if(seige_count < character_count) return true;
			else return false;
		}
	}
}

bool Combat::CombatGroup::IsIdInAttractCombatList( uint64 playerid )
{
	for(std::set<uint64>::iterator iter = AttractCombat_attack_id.begin();iter != AttractCombat_attack_id.end();++iter)
	{
		if(playerid == *iter)
			return true;
	}

	return false;
}

bool Combat::CombatGroup::IsPlayerIdInSet( std::set<uint64>& Set,uint64 id )
{
	if(Set.empty())
		return false;
	for(std::set<uint64>::iterator iter = Set.begin();iter != Set.end();++iter)
	{
		if(id == *iter)
			return true;
	}
	return false;
}

uint32 CombatGroup::GetInitOperateSeconds()
{
	if(m_isQuickCombat)
		return 0;
	else
		return Combat_Operate_Time;
}

int CombatGroup::GetMonsterInAttackLst()
{
	int num = 0;
	for (GroupLst::iterator it = m_group_attack.begin(); it != m_group_attack.end(); ++it)
	{
		if((*it)->GetCountryId() == MONSTER_COUNTRY_ID)
		{
			num++;
		}
	}
	return num;
}

void CombatGroup::RecordCombatGainsInfo(uint64 playerid, pb::Msg_Records_Combat_Gains type, uint32 num, IntPairVec gainsPair)
{
	CombatGainsRec& data = m_combatGainsMap[playerid];
	switch(type)
	{
	case pb::CombatPlayerExp:
		data.combatPExp += num;
		break;
	case pb::CombatDead:
		data.combatDeads += num;
		break;
	case pb::CombatKills:
		data.combatKills += num;
		break;
	case pb::CombatExploit:
		data.combatExploit += num;
		break;
	case pb::CombatCoin:
		data.combatCoin += num;
	default:
		break;
	}
	if(type >= pb::CombatInfoNum)
	{
		for(IntPairVec::iterator it = gainsPair.begin(); it != gainsPair.end(); it++)
		{
			bool isgot = false;
			for(IntPairVec::iterator ite = data.combatExGains.begin(); ite != data.combatExGains.end(); ite++)
			{
				if(ite->first == it->first)
				{
					isgot = true;
					ite->second += it->second;
					break;
				}
			}
			if(!isgot)
				data.combatExGains.push_back(*it);
		}
	}
}

void CombatGroup::BroadcastCombatGainsInfo(uint32 cityid,uint32 wincountry)
{
	if(m_eCombat == Combat::CT_Country)
	{
		for(std::map<uint64, CombatGainsRec>::iterator it = m_combatGainsMap.begin(); it != m_combatGainsMap.end(); it++)
		{
			if(Player* player = sPlayerPool.GetByPlayerId(it->first))
			{
				CombatGainsRec& data = m_combatGainsMap[it->first];
				//发送战报通知
				pb::C2GS_Chat msg;
				char buf[4096] = {0};
				std::stringstream strstm;
				for(IntPairVec::iterator ite = data.combatExGains.begin(); ite != data.combatExGains.end(); ite++)
				{
					strstm << "," << ite->first << "|" << ite->second;
				}
				SPRINTF(buf,"3,%d,%d,%d,%d,%d,%d%s",cityid,(wincountry==player->GetCountryId()),data.combatKills,data.combatDeads,data.combatExploit,data.combatPExp,strstm.str().c_str());
				msg.set_type(pb::CHAT_COMBAT_BC);
				msg.set_chat_info(buf);
				LLOG("[1006] BroadCast Combat3:%s",buf);

				player->m_chatSpeaker->Say(player,msg);
			}
		}
	}
}

uint32 CombatGroup::GetShadowNum(bool isAtt, uint64 playerid/* = 0*/)
{
	uint32 countnum = 0;
	if(isAtt)
	{
		for(uint32 i = 0; i < m_group_attack.size(); i++)
		{
			CombatObj* pAttack = m_group_attack[i];
			if(pAttack)
			{
				if(pAttack->IsShadow() && (playerid == 0 || pAttack->GetPlayerId() == playerid))	//如果武将是幻影并且（筛选玩家id为0或者筛选玩家成功）	
					countnum++;
			}
		}
	}
	else
	{
		for(uint32 i = 0; i < m_group_defence.size(); i++)
		{
			CombatObj* pDef = m_group_defence[i];
			if(pDef)
			{
				if(pDef->IsShadow() && (playerid == 0 || pDef->GetPlayerId() == playerid))
					countnum++;
			}
		}
	}
	return countnum;
}