#ifndef _SKILLDATAMGR_
#define _SKILLDATAMGR_
#include "def/TypeDef.h"
#include "ptr_container/PtrMap.h"
#include "BuffFight.h"
#include "SkillSlg.h"

class SkillDataMgr
{
	
	typedef PtrMap<uint32, BuffFight> BuffFightMap;
	typedef PtrMap<uint32, SkillSlg> SkillSlgMap;
	typedef PtrMap<uint32, BuffRelation> BuffRelationMap;

private:
	BuffFightMap	m_buffFightMap;
	SkillSlgMap		m_skillSlgMap;
	BuffRelationMap m_buffRelationMapDetail;	//buff关系详细表
	std::map<uint32, uint32> m_buffRelationTypeToId;	//buffId到Type的转换
public:
	static SkillDataMgr& GetInstance(){ static SkillDataMgr T; return T; }

	void Init();

	int GetObjSkillMaxTimes(uint32 skillID);	//获取武将技能最大施放次数

	int GetObjSkillRange(uint32 skillID);		//获取技能战法范围

	float GetDamageFactor(uint32 skillID);		//获取技能伤害系数

	int GetBuffIDBySkill(uint32 skillID);		//通过技能id获得buff

	int GetBuffType(uint32 buffid);		//获取bufftype

	int GetBuffLastRoundsCount(uint32 buffid);	//获取buff持续回合数

	int GetBuffTarget(uint32 buffid);	//获取buff作用目标

	int GetBuffRows(uint32 buffid);	//获取效果行数

	float GetBuffValue1(uint32 buffid);
	float GetBuffValue2(uint32 buffid);
	float GetBuffValue3(uint32 buffid);

	int GetBuffRelationship(uint32 typeA, uint32 typeB);

	int GetWeight(uint32 buffid);
	int GetEffectStage(uint32 buffid);
};

#define sSkillDataMgr SkillDataMgr::GetInstance()




#endif