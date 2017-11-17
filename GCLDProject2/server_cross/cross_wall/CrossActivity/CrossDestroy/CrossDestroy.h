#pragma once
#include "def/TypeDef.h"
#include "def/ConstDef.h"
#include "Activity/Personal/PersonalTask.h"
class Player;
//enum TaskEnum;
namespace pb
{
	class GS2C_Cross_Destroy_Task_Infos;
	class GS2C_Take_Destroy_Task_Award_Ret;
}

class CrossDestroy
{
public:
	struct TaskSub{
		uint32		taskid;		//������
		TaskEnum	taskType;	//��������
		uint32		target;		//����Ŀ��
		uint32		curCompletion;//������ɶ�
		bool		canTakeAward;//�����ܷ���ȡ
		bool		isTakeAward;//�����Ƿ�����ȡ
	};

	struct TaskPlayer { //�������
		TaskPlayer();
		TaskSub task[3];		//����С����
	};

	std::map<uint64, TaskPlayer>    m_playerLst;		//����б�
	bool	m_isBegined;

public:
	CrossDestroy();
	static CrossDestroy& Instance(){ static CrossDestroy T; return T; }

	bool Begin();
	void End();

	void SaveTo(Player& player, pb::GS2C_Cross_Destroy_Task_Infos& msg);

	bool IsPlayerInLst(uint64 playerid);
	TaskPlayer& GetPlayerData(Player& player);
	void addProgress(TaskEnum tasktype, uint32 addnum, Player& player);	//�����������

	void TakeAwards(Player&player, uint8 taskid, pb::GS2C_Take_Destroy_Task_Award_Ret& msg);
	void TakeAwardsOnEnd();
};

#define sCrossDestroy CrossDestroy::Instance()