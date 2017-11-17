#ifndef _BUFF_FIGHT_
#define _BUFF_FIGHT_

#include "def/TypeDef.h"
struct DB_Buff_Fight;
struct DB_Buff_Relation;

class BuffFight
{
	const DB_Buff_Fight& m_dbBuffFight;
public:
	BuffFight(const DB_Buff_Fight& db);
	//uint32 Id() const;

	int GetBuffType();
	int GetLastRoundsCount();		//buff持续回合数
	int GetBuffTarget();			//获取buff释放对象，1敌方，2己方
	int GetRows();					//获取效果行数
	float GetValue1();
	float GetValue2();
	float GetValue3();
	int	GetWeight();	//获取权值
	int GetEffectStage();//获取起效时机
};

class BuffRelation
{
	const DB_Buff_Relation& m_dbBuffRelation;
public:
	BuffRelation(const DB_Buff_Relation& db);

	int GetRelations(uint32 id);	//返回列表第几个的关系
	uint32 GetTypeID();	//获得bufftype
	std::vector<int>& GetRelationLst();

private:
	std::vector<int>	relationLst;
};

#endif