#pragma once
#include <vector>

namespace Combat{

enum TacticEnum {
    Attack,     //攻击
    Defence,    //防御
    Charge,     //冲锋
    AttackEx,   //强化攻击
    DefenceEx,  //强化防御
    ChargeEx,   //强化冲锋

    Tactic_Max_Cnt,

    Skill_Tactic = Tactic_Max_Cnt	//技能
};

// 战术克制加成表
const char Tactic_Common[Tactic_Max_Cnt][Tactic_Max_Cnt] = {
    2, 5, 0, 0, 8, 0,
    0, 2, 5, 0, 0, 8,
    5, 0, 2, 8, 0, 0,
    8, 10, 0, 2, 10, 0,
    0, 8, 10, 0, 2, 10,
    10, 0, 8, 10, 0, 2,
};

// 对撞克制表(1完美克制、2不完美克制、3正常克制、4同战术克制、5被克制)
const char Tactic_Crash[Tactic_Max_Cnt][Tactic_Max_Cnt] = {
    4, 3, 5, 5, 2, 5,
    5, 4, 3, 5, 5, 2,
    3, 5, 4, 2, 5, 5,
    2, 1, 5, 4, 1, 5,
    5, 2, 1, 5, 4, 1,
    1, 5, 2, 1, 5, 4,
};
// 对撞克制伤害比
const char Tactic_CrashRatio[5] = { 30, 15, 20, 5, 2 };

enum CombatType {
    CT_Country,    //国战
    CT_Fuben,      //副本战
    CT_Fog,        //迷雾战
    CT_Ore,        //矿战
    CT_EventStory,	//事件剧本，计算同国战
	CT_Crusade,		//远征，战斗不得经验，征兵不耗粮食
	CT_Official,   //官战
    CT_Monster,    //蛮族战斗
    CT_Activity_Tournament,//比武活动
	CT_Teach_Map,	//教学关卡

    _MAX_COMBAT_TYPE
};
const float FOOD_TO_EXP[_MAX_COMBAT_TYPE] = { 0.285f, 0.195f, 0.195f, 0.195f, 0.195f, 0.195f, 0.112f, 0.195f, 0.195f, 0.285f };

class CombatObj;
typedef std::vector<CombatObj*> GroupLst;


#define Battle_Line_Cnt 6

enum CombatObjType
{
	Obj_Npc,
	Obj_Char,
	Obj_Shadow,
	Obj_Reinforce,
	Obj_FakeChar,
};

//战斗地形
enum CombatTerr
{
	Terr_None,
	Terr_Mountain,	//山地
	Terr_Plain,		//平原
	Terr_City,		//城池
	Terr_Water,		//水域
	Terr_Forest,	//密林
};

}
