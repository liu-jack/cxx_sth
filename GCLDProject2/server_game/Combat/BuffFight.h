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
	int GetLastRoundsCount();		//buff�����غ���
	int GetBuffTarget();			//��ȡbuff�ͷŶ���1�з���2����
	int GetRows();					//��ȡЧ������
	float GetValue1();
	float GetValue2();
	float GetValue3();
	int	GetWeight();	//��ȡȨֵ
	int GetEffectStage();//��ȡ��Чʱ��
};

class BuffRelation
{
	const DB_Buff_Relation& m_dbBuffRelation;
public:
	BuffRelation(const DB_Buff_Relation& db);

	int GetRelations(uint32 id);	//�����б�ڼ����Ĺ�ϵ
	uint32 GetTypeID();	//���bufftype
	std::vector<int>& GetRelationLst();

private:
	std::vector<int>	relationLst;
};

#endif