#include "CombatObj.h"
#include "System.h"
#include "Combat.pb.h"
#include "def/MmoAssert.h"
#include "Base.h"
#include "datastruct/struct_buff.h"
#include "../Service/ServiceMgr.h"
#include "OS.h"
#include "SkillDataMgr.h"
//#include "CombatBuff.h"

using namespace Combat;

uint32 _Service_Char_Buff(void* p){
    CombatObj* ptr = (CombatObj*)p;
    ptr->RefreshBuff();
    return 500;
}

SoldierRaw::SoldierRaw()
{
	hp = 0;	//剩余血量
	isSoldierSelectTactic = false;
	isSoldierUseSkill = false;
	killerCharId = 0;
	killerPlayerId = 0;
}

SoldierRaw::~SoldierRaw()
{

}
CombatObj::CombatObj()
{
	//memset(&soldierLv, 0, sizeof(*this) - MMO_OFFSET(CombatObj, soldierLv));

	soldierLv = 1;
	isAutoFight = false;
	m_pMap = NULL;

	skillPercent = 0; //战法提升百分之几
	fightPercent = 0; //战力提升百分之几
	expPercent = 0;   //经验提升百分之几
	reduceCostHP = 0;
	doubleKill = 0;

	terrainPercent = 0;//地形加成
	guardAddPercent = 0;//守卫军加成

	crossDoubleKill = 0;

	//每次战斗开始时清零的数据--------------------------------------------------------
	isUsedSkill = false;
	isSkillSuccess = false;
	isInCombat = false;
	isFighting = false; //已上阵，前6排上阵
	has_fighted = false;
	isSelectTactic = false;
	tactic = Attack;      //选择的战术
	deadCntOneLoop = 0; //本轮死亡数
	killCntOneLoop = 0; //本轮击杀数
	statisticExp = 0;   //统计：战斗获得的总经验
	statisticExpPlayer = 0;//战斗获得的玩家经验
	statisticMoney = 0; //统计：战斗获得的总银币
	statisticDead = 0;
	statisticKill = 0;
	oldCombatGroupUniqueID = 0; //被诱敌时，旧战斗组id
	nowCombatGroupID = 0;
	combatType = 0;
	killOnceAHero = 0;

	killerPID = 0;
	killerCID = 0;

	leftSkillTimes = 0;
	isInTraining = false;
	isLifeGuard = false;
	isExpedition = false;

	keji_deskillPercent = 0;
	keji_fightPercent = 0;
	keji_expPercent = 0;
	keji_skillPercent = 0;

	total_fightPercent = 0;
	total_skillPercent = 0;
	total_expPercent = 0;
	total_reduceCostHP = 0;
	total_doubleKill = 0;
	total_deSkillPercent = 0;
	m_extraAwards.clear();
}
bool CombatObj::IsDead()
{
    for (std::vector<SoldierRaw>::iterator it = soldiers.begin(); it != soldiers.end(); ++it)		//每排士兵数量
    {
        if (it->hp > 0) return false;
    }
    return true;
}
double CombatObj::ForeHp()		//前排的血量
{
    if (soldiers.empty()) // 空STL容器调front()、pop()直接宕机
        return 0;
    return soldiers.front().hp;
}

int CombatObj::CurHp()		//总生命值
{
	int sum = 0;
	for (std::vector<SoldierRaw>::iterator it = soldiers.begin(); it != soldiers.end(); ++it)
	{
		if (it->hp > 0)
		{
			sum += it->hp;
		}
	}
	return sum;
}

int CombatObj::HurtHp(int hurt)		//真实伤害值，没有溢出无效 前排扣血
{
    if (soldiers.empty()) return 0;

    int trueHurt = (soldiers.front().hp >= hurt) ? hurt : soldiers.front().hp;

    soldiers.front().hp -= hurt;

    return trueHurt;
}

void CombatObj::setRestHP(int restHp)		//设置剩余血量，在迷雾战中使用
{
	if(soldiers.empty()) return;

	while((CurHp()-soldiers.front().hp) > restHp)
	{
		soldiers.erase(soldiers.begin());
	}
	soldiers.front().hp -= (CurHp()-restHp);
}

void CombatObj::HurtPerHp(float percent) //当前总血量百分比扣血 前排扣血
{
    int sum = 0;
    for (std::vector<SoldierRaw>::iterator it = soldiers.begin(); it != soldiers.end(); ++it)
    {
        sum += it->hp;
    }
    int hurt = (int)(sum * percent);

    for (std::vector<SoldierRaw>::iterator it = soldiers.begin(); it != soldiers.end(); ++it)
    {
        int& refHp = it->hp;
        if (refHp >= hurt) {
            refHp -= hurt;
            return;
        } else {
            hurt -= refHp;
            refHp = 0;
        }
    }
}
bool CombatObj::AutoTactic()		//自动选择战术
{
    if (false == GetIsSelectTactic())	//没有选择战术
    {
        if (leftSkillTimes <= 0) // 没有剩余技能次数
            SelectTactic(RandTactic());
        else
            SelectTactic(Skill_Tactic);	//选择使用技能
		return true;
    }
    else if (tactic == Skill_Tactic)	//选择了技能
    {
        if (isUsedSkill) //释放完技能之后，本排士兵未死，下一轮随机战术
        {
            SelectTactic(RandTactic());
			return true;
		}
    }
	return false;
}

//int CombatObj::GetSkillOrTactic()
//{
//	int ret = 0;
//	if(false == isSelectTactic)		//没有选择战术
//	{
//	}
//}

TacticEnum CombatObj::RandTactic()
{
	int randnum = System::Rand(0, 3);	//0,1,2
	uint32 tactics = GetTacticAndSkills();
	int checknum = 0;
	int failret=0;
	for(int i=0; i<Tactic_Max_Cnt; i++)	//0,1,2,3,4,5
	{
		if( (tactics & (1<<i)) != 0)	//该位有数字
		{
			if(randnum == checknum)
			{
				return (TacticEnum)i;
			}
			checknum++;
			failret = i;
		}
	}
	//ASSERT(0);
	LLOG("can't get true tactic");
	return (TacticEnum)failret;
}

TacticEnum CombatObj::RandEnableTactic()
{
	uint32 enableTactics = GetTacticsAndSkillsEnable();	//可以点击的技能或战术
	uint32 totalTactics = GetTacticAndSkills();		//面板显示的技能或战术

	//随机一个普通战术
	int randTactic = System::Rand(0, 3);	//0,1,2
	uint32 retTacticNum = enableTactics & (((1 << randTactic) << 3) | (1 << randTactic));//示例：1 101 010 & (((1<<0)<<3) | (1<<0)) = (1 101 010 & 0 001 001) = 0 001 000 强化攻击
	
	if(retTacticNum == 0)	//如果没有选出来，则使用随机的战术
		retTacticNum = totalTactics & (((1 << randTactic) << 3) | (1 << randTactic));

	TacticEnum retTactic = TransIntToTactic(retTacticNum);
	return retTactic;
}

TacticEnum CombatObj::GetFinalTacticOrSkill(bool canUseSkill, TacticEnum otherTac, uint32 aiLevel)
{
	TacticEnum retTactic = Attack;
	if(canUseSkill)	//允许释放技能
	{
		if(HaveBuffWork(confuse) || HaveBuffWork(silence))	//如果被混乱或者沉默，则技能无法释放
		{
			retTactic = GetFinalTacticOrSkill(false,otherTac,aiLevel);	//递归调用
		}
		else	//如果可以释放技能，则直接释放
		{
			retTactic = Skill_Tactic;
		}
	}
	else	//不能施放技能，则在战术里面选择
	{
		if(HaveBuffWork(confuse) || HaveBuffWork(disturb))	//如果被混乱或者扰乱，则不能正常选战术，只能选择被克制的战术
		{
			retTactic = GetBeiKeZhiTactic(otherTac);
		}
		else	//选择克制战术或者随机战术
		{
			if(aiLevel)	//如果是高级ai,选择克制战术
			{
				retTactic = GetKeZhiTactic(otherTac);
			}
			else	//选择随机战术
			{
				retTactic = RandEnableTactic();
			}
		}
	}
	return retTactic;
}

void CombatObj::SelectTactic(TacticEnum typ)		//选择技能
{
    if (leftSkillTimes <= 0 && typ == Skill_Tactic)	//没有选择次数不能选择技能
    {
        MMO_ASSERT(0);
        return;
    }
    tactic = typ;
    SetIsSelectTactic();
	LLOG("CombatObj::SelectTactic tac:%d",(int)typ);
}

bool CombatObj::isTacticLegal(TacticEnum typ)
{
	LLOG("CombatObj::isTacticLegal leftSkillTimes=%d",leftSkillTimes);
	if(typ == Skill_Tactic)
	{
		if(leftSkillTimes > 0)//选择是技能
			return true;
		else
			return false;
	}

	if((GetTacticsAndSkillsEnable() & (1<<(int)typ)) != 0)
		return true;

	return false;
}

//创建时候
void CombatObj::InitCombatData()//全部属性重置，用于初始化
{
	//printf("CombatObj::InitCombatData -- 1   \n");

	//memset(&isUsedSkill, 0, sizeof(*this) - MMO_OFFSET(CombatObj, isUsedSkill));
	//每次战斗开始时清零的数据--------------------------------------------------------
	isUsedSkill = false;
	isSkillSuccess = false;
	isInCombat = false;
	isFighting = false; //已上阵，前6排上阵
	isSelectTactic = false;
	tactic = Attack;      //选择的战术
	deadCntOneLoop = 0; //本轮死亡数
	killCntOneLoop = 0; //本轮击杀数
	statisticExp = 0;   //统计：战斗获得的总经验
	statisticExpPlayer = 0;//战斗获得的玩家经验
	statisticMoney = 0; //统计：战斗获得的总银币
	statisticDead = 0;
	statisticKill = 0;
	killOnceAHero = 0;
	oldCombatGroupUniqueID = 0; //被诱敌时，旧战斗组id
	m_fightBuff.ClearAllBuff();
	m_extraAwards.clear();
	total_deSkillPercent = 0;
	total_doubleKill = 0;
	total_expPercent = 0;
	total_fightPercent = 0;
	total_reduceCostHP = 0;
	total_skillPercent = 0;
	killerPID = 0;
	killerCID = 0;

	for(std::vector<SoldierRaw>::iterator it=soldiers.begin(); it!=soldiers.end(); it++)
	{
		it->killerCharId = 0;
		it->killerPlayerId = 0;
		it->isSoldierSelectTactic = false;
		it->isSoldierUseSkill = false;
	}
}

//初次进入战斗
void CombatObj::InitCombatDataSecond(uint32 geography)//战斗开始属性重置，用于重置本次战斗会用到的局部属性
{
	LLOG("CombatObj::InitCombatDataSecond");
	isUsedSkill = false;
	isSkillSuccess = false;
	isInCombat = false;
	isFighting = false; //已上阵，前6排上阵
	isSelectTactic = false;
	tactic = RandTactic();      //随机一个战术
	deadCntOneLoop = 0; //本轮死亡数
	killCntOneLoop = 0; //本轮击杀数
	statisticExp = 0;   //统计：战斗获得的总经验
	statisticExpPlayer = 0;//战斗获得的玩家经验
	statisticMoney = 0; //统计：战斗获得的总银币
	statisticDead = 0;
	statisticKill = 0;
	killOnceAHero = 0;
	killerPID = 0;
	killerCID = 0;
	//战法范围0，表示无技能，默认使用了
	//isUsedSkill = (SkillRange() == 0);

	
	m_maxHP = (stPro.rowSoldierMaxCnt) * (int)GetMaxRow();
	if(m_maxHP == CurHp())
		leftSkillTimes = sSkillDataMgr.GetObjSkillMaxTimes(SkillID());
	else
		leftSkillTimes = 0;

	LLOG("skill times=%d",leftSkillTimes);

	m_fightBuff.ClearAllBuff();

	terrainType = geography;
	indexInCombat = 0;
	isAttactInCombat = true;
	m_extraAwards.clear();

	for(std::vector<SoldierRaw>::iterator it=soldiers.begin(); it!=soldiers.end(); it++)
	{
		it->killerCharId = 0;
		it->killerPlayerId = 0;
		it->isSoldierSelectTactic = false;
		it->isSoldierUseSkill = false;
	}

}

//退出战斗 还未计算统计
void CombatObj::ClearCombatStatus()		////战斗结束后用于清理战斗中的局部属性，统计等属性不清理
{
	//isUsedSkill = false;
	//isSkillSuccess = false;
	isInCombat = false;
	isFighting = false;
	has_fighted = true;
	isSelectTactic = false;
	m_fightBuff.ClearAllBuff();
	//tactic = Attack;      //选择的战术
	//deadCntOneLoop = 0; //本轮死亡数
	//killCntOneLoop = 0; //本轮击杀数
	//statisticExp = 0;   //统计：战斗获得的总经验
	//statisticMoney = 0; //统计：战斗获得的总银币
	//statisticDead = 0;
	//statisticKill = 0;

	//战法范围0，表示无技能，默认使用了
	//isUsedSkill = (SkillRange() == 0);

	nowCombatGroupID = 0;
	combatType = 0;
}
void CombatObj::SaveTo(pb::Msg_CombatObj& msg)
{
    msg.set_tableid(GetTableId());
    msg.set_player_id(GetPlayerId());
    msg.set_player_name(GetPlayerName());
	for (std::vector<SoldierRaw>::iterator it = soldiers.begin(); it != soldiers.end(); ++it)
    {
        msg.add_soldiercnt(it->hp);
    }
	msg.set_rowsoldiermaxcnt(stPro.rowSoldierMaxCnt);
	msg.set_maxhp(GetMaxHp());
    msg.set_level(GetLevel());
    msg.set_country(GetCountryId());
	msg.set_indexincombat(indexInCombat);
	msg.set_isshadow(IsShadow());
	msg.set_isreinforce(IsReinforce());
	msg.set_isfakechar(IsFakeChar());
	LLOG("indexInCombat=%d, tableid=%d",indexInCombat,GetTableId());
}

void CombatObj::SaveToGeneral(pb::Msg_CombatObj& msg)
{
	msg.set_tableid(GetTableId());
	msg.set_player_id(GetPlayerId());
	msg.set_player_name(GetPlayerName());
	msg.set_country(GetCountryId());
	msg.set_indexincombat(indexInCombat);
	msg.set_isshadow(IsShadow());
	msg.set_isreinforce(IsReinforce());

}

void CombatObj::OnDead(CombatObj* killer)
{
}

void CombatObj::RefreshBuff()	//刷新buff
{
    m_Buff.Refresh(OS::TimeMS());

    if (m_Buff.Size() == 0)
    {
        ServiceMgr::UnRegister(SERVICE_BUFF_CHAR, this);
    }
}
void CombatObj::AddBuff(uint32 buffId, uint32 lastMsec /* = 0 */)
{
    if (const PlayerBuffTable* table = PlayerBuffTable::GetTable(buffId))
    {
        switch (table->GetTarget()) {
        case PlayerBuffTable::Char:
        case PlayerBuffTable::All:
            break;
        default:
            return;
        }

        if (m_Buff.Size() == 0)
        {
            ServiceMgr::Register(SERVICE_BUFF_CHAR, this);
        }
        m_Buff.AddBuff(table, lastMsec);
    }
}
void CombatObj::DelBuff(uint32 buffId)
{
    m_Buff.DelBuff(buffId);

    if (m_Buff.Size() == 0)
    {
        ServiceMgr::UnRegister(SERVICE_BUFF_CHAR, this);
    }
}
bool CombatObj::HaveBuff(uint32 buffId) const
{
    return m_Buff.HaveBuff(buffId);
}
bool CombatObj::HaveBuff(CBuff::Type typ) const
{
    return m_Buff.HaveBuff(typ);
}

bool CombatObj::HaveBuffWork(Combat::BuffType tpy)
{
	if(soldiers.size() == 0) return false;

	return soldiers.front().buff_fight.HaveBuffWork(tpy);
}

void CombatObj::DelOneTypeBuff(CBuff::Type typ)
{
    m_Buff.DelOneTypeBuff(typ);

    if (m_Buff.Size() == 0)
    {
        ServiceMgr::UnRegister(SERVICE_BUFF_CHAR, this);
    }
}

uint32 CombatObj::GetTacticAndSkills()
{
	uint32 retnum = GetEnableTactics(terrainType);
	LLOG("CombatObj::GetTacticAndSkills retnum=%d",retnum);
	if(leftSkillTimes <= 0 || GetIsUseSkill())	//如果剩余施放次数为0或者该排已经释放过技能
	{
		return retnum;
	}
	else
	{
		return (retnum |= (1<<6));
	}
}

uint32 CombatObj::GetTacticsAndSkillsEnable()
{
	uint32 tacAndSkis = GetTacticAndSkills();
	if(HaveBuffWork(confuse))	//有混乱效果	0000000
		tacAndSkis &= 0;
	if(HaveBuffWork(disturb))	//有扰乱效果	1000000
		tacAndSkis &= 64;
	if(HaveBuffWork(silence))	//有沉默效果	0111111
		tacAndSkis &= 63;
		
	return tacAndSkis;
}

TacticEnum CombatObj::GetKeZhiTactic(TacticEnum typ)
{
	TacticEnum retTactic;
	if(typ == Combat::Tactic_Max_Cnt)
	{
		retTactic = RandEnableTactic();
	}
	else
	{
		uint32 kezhiTac = 0;
		if(typ == Combat::Attack || typ == Combat::AttackEx)		//攻击或强化攻击
			kezhiTac = 36;	// 0 100 100	//冲锋与强化冲锋
		if(typ == Combat::Defence || typ == Combat::DefenceEx)		//防御或强化防御
			kezhiTac = 9;	// 0 001 001	//攻击与强化攻击
		if(typ == Combat::Charge || typ == Combat::ChargeEx)		//冲锋或强化冲锋
			kezhiTac = 18;	// 0 010 010	//防御与强化防御

		uint32 enableTactics = GetTacticsAndSkillsEnable();	//可以点击的技能或战术
		enableTactics &= kezhiTac;
		retTactic = TransIntToTactic(enableTactics);		//示例：传入冲锋 1 001 110 & 0 010 010 = 0 000 010  返回防御  
	}
	return retTactic;
}

TacticEnum CombatObj::GetBeiKeZhiTactic(TacticEnum typ)
{
	TacticEnum retTactic;
	if(typ == Combat::Tactic_Max_Cnt)
	{
		retTactic = RandEnableTactic();
	}
	else
	{
		uint32 beikezhiTac = 0;
		if(typ == Combat::Attack || typ == Combat::AttackEx)		//攻击或强化攻击
			beikezhiTac = 18;	// 0 010 010	//防御与强化防御
		if(typ == Combat::Defence || typ == Combat::DefenceEx)		//防御或强化防御
			beikezhiTac = 36;	// 0 100 100	//冲锋与强化冲锋
		if(typ == Combat::Charge || typ == Combat::ChargeEx)		//冲锋或强化冲锋
			beikezhiTac = 9;	// 0 001 001	//攻击与强化攻击

		uint32 enableTactics = GetTacticsAndSkillsEnable();	//可以点击的技能或战术
		retTactic = TransIntToTactic(enableTactics & beikezhiTac);		//示例：传入冲锋 1 001 110 & 0 010 010 = 0 000 010  返回防御 
	} 
	return retTactic;
}

bool CombatObj::GetIsSelectTactic()
{
	if(soldiers.size() > 0)	//有士兵
		return soldiers.front().isSoldierSelectTactic;

	return false;
}

void CombatObj::SetIsSelectTactic()
{
	if(soldiers.size() > 0)	//有士兵
		soldiers.front().isSoldierSelectTactic = true;
}

bool CombatObj::GetIsUseSkill()
{
	if(soldiers.size() > 0)	//有士兵
		return soldiers.front().isSoldierUseSkill;

	return true;
}

void CombatObj::SetIsUseSkill()
{
	if(soldiers.size() > 0)	//有士兵
		soldiers.front().isSoldierUseSkill = true;
}

//void	 CombatObj::resetExpPercent(int oldexppercent)
//{
//	expPercent = oldexppercent;
//}
//void CombatObj::resetFightPercent(int oldfightpercent)
//{
//	fightPercent = oldfightpercent;
//}
//void CombatObj::resetSkillPercent(int oldskillPercent)
//{
//	skillPercent  = oldskillPercent;
//}

void CombatObj::TrainAddtionUpEnd()
{
	resetTrainAdditionUp();
}

void CombatObj::resetTrainAdditionUp()
{
	expPercent = 0;
	fightPercent = 0;
	doubleKill = 0;
	reduceCostHP = 0;
	trainUpStartTime = 0;
}


void Combat::CombatObj::MoveTo(uint32 cityId)
{

}

uint64 Combat::CombatObj::GetPlayerId()
{
	return 0;
}

int32 Combat::CombatObj::GetLevel() const
{
	return 0;
}

uint32 Combat::CombatObj::GetType() const
{
	return 0;
}

TacticEnum Combat::CombatObj::TransIntToTactic(uint32 tacNum)
{
	TacticEnum retTactic = Attack;
	for(int i=0; i<=Tactic_Max_Cnt; i++)	//0,1,2,3,4,5,6
	{
		if( (tacNum & (1<<i)) != 0)	//该位有数字
		{
			retTactic = (TacticEnum)i;
			break;
		}
	}
	return retTactic;
}