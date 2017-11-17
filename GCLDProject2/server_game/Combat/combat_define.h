#pragma once
#include <vector>

namespace Combat{

enum TacticEnum {
    Attack,     //����
    Defence,    //����
    Charge,     //���
    AttackEx,   //ǿ������
    DefenceEx,  //ǿ������
    ChargeEx,   //ǿ�����

    Tactic_Max_Cnt,

    Skill_Tactic = Tactic_Max_Cnt	//����
};

// ս�����Ƽӳɱ�
const char Tactic_Common[Tactic_Max_Cnt][Tactic_Max_Cnt] = {
    2, 5, 0, 0, 8, 0,
    0, 2, 5, 0, 0, 8,
    5, 0, 2, 8, 0, 0,
    8, 10, 0, 2, 10, 0,
    0, 8, 10, 0, 2, 10,
    10, 0, 8, 10, 0, 2,
};

// ��ײ���Ʊ�(1�������ơ�2���������ơ�3�������ơ�4ͬս�����ơ�5������)
const char Tactic_Crash[Tactic_Max_Cnt][Tactic_Max_Cnt] = {
    4, 3, 5, 5, 2, 5,
    5, 4, 3, 5, 5, 2,
    3, 5, 4, 2, 5, 5,
    2, 1, 5, 4, 1, 5,
    5, 2, 1, 5, 4, 1,
    1, 5, 2, 1, 5, 4,
};
// ��ײ�����˺���
const char Tactic_CrashRatio[5] = { 30, 15, 20, 5, 2 };

enum CombatType {
    CT_Country,    //��ս
    CT_Fuben,      //����ս
    CT_Fog,        //����ս
    CT_Ore,        //��ս
    CT_EventStory,	//�¼��籾������ͬ��ս
	CT_Crusade,		//Զ����ս�����þ��飬����������ʳ
	CT_Official,   //��ս
    CT_Monster,    //����ս��
    CT_Activity_Tournament,//����
	CT_Teach_Map,	//��ѧ�ؿ�

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

//ս������
enum CombatTerr
{
	Terr_None,
	Terr_Mountain,	//ɽ��
	Terr_Plain,		//ƽԭ
	Terr_City,		//�ǳ�
	Terr_Water,		//ˮ��
	Terr_Forest,	//����
};

}
