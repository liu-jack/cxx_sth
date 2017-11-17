#pragma once
#include "def/TypeDef.h"

class Player;
//namespace pb
//{
//	class GS2C_ActMgr_Begin;
//}

//���������Ҫ�Ǹ�������
class CrossActMgr {
public:
	enum CrossActivityEnum
	{
		Act_None			= 0,
		CrossKill			= 1,	//ɱ������
		CrossDestroy		= 2,	//���������������
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
