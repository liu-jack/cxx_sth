#pragma once
#include "def/TypeDef.h"

class Player;
//namespace pb
//{
//	class GS2C_ActMgr_Begin;
//}

//跨服任务，主要是个人任务
class CrossActMgr {
public:
	enum CrossActivityEnum
	{
		Act_None			= 0,
		CrossKill			= 1,	//杀敌任务
		CrossDestroy		= 2,	//三个灭国个人任务
		_MAX_TYPE
	};

	std::map<uint64, uint32> m_PlayerTaskLevel;
public:
	static CrossActMgr& Instance(){ static CrossActMgr T; return T; }

	void Init();
	void StartActivity();
	void EndActivity();
	//void _Update();
	//void OnEnterNexHour(const int hour);

	//void GetActivityList(Player* player,pb::GS2C_ActMgr_Begin& msg);
	void TakeAwardsOnEnd();

	uint32 GetPlayerTaskLevel(Player& player);

private:
	void UpdateActivity(int hour);
};
#define sCrossActMgr CrossActMgr::Instance()
