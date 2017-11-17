#pragma once
#include "def/TypeDef.h"
#include "Enum.pb.h"
class Player;
namespace pb
{
	class GS2C_ActMgr_Begin;
}

#define NULL_ACTIVE_HOUR 25

class ActMgr {
public:
    enum ActivityEnum
    {
		Act_None			= 0,
        Attack_Defence_City = 1,//���سǳ�
        Tournament,             //����
        Occupy_City,            //��������
        Suppress_Monster,       //��ѹ����
        Investment,             //Ͷ������
        Defence_Borderland,     //���ر߽�
        _MAX_TYPE
    };
public:
    static ActMgr& Instance(){ static ActMgr T; return T; }

	void Init();
    void _Update();
    void OnEnterNexHour(const int hour);

	bool isCrossWarCover;	//�Ƿ������ǣ���������򲻿�����ս����

	ActivityEnum currentAct;	//��ǰ���ڽ��еĻ
	ActivityEnum lastAct;		//��һ���
	ActivityEnum NextAct;		//��һ�������Ļ
	int NextActStartHour;//��һ�������ʱ��

	void GetActivityList(Player* player,pb::GS2C_ActMgr_Begin& msg);
	void TakeAwardsBeforeStart(ActivityEnum tpy);

	void AddPlayerInActivity(uint64 playerid);
	void AddEventLogFMT(pb::EVENT_ID eventID, int paramCount, ...);
	size_t GetPlayerSetSize() { return player_set.size(); }

	bool IsNowActivity();
private:
    void UpdateActivity(int hour);

private:
	std::set<uint64> player_set;
};
#define sActMgr ActMgr::Instance()
