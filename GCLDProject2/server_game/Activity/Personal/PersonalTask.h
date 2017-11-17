#pragma once
#include "def/TypeDef.h"
#include "def/ConstDef.h"
#include "Map/Table/MapCountry.h"
#include "boost/smart_ptr/shared_ptr.hpp"
#include "Base.h"
#include "boost/function.hpp"

class Player;
namespace pb
{
	class OneTaskInfo;
	class GS2C_PersonalTask_Info;
	class GS2C_PersonalTask_Take_Award;
}

enum TaskEnum{
	TASK_NULL,
	KILL_IN_COUNTRY,	//��սɱ��
	ATTRACT_IN_COUNTRY,	//��ս�յ�
	RUSH_IN_COUNTRY,	//��սͻ��
	ADD_SOLIDER_IN_COUNTRY,	//��ս����
	TOURNAMENT_IN_COUNTRY,//�������
	CASTING_TIMES,			//�����������
	INVEST_TIMES,			//Ͷ�ʴ���
	OCCUPY_TIMES,			//��ռ�ǳ���
};

class PersonTask
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
		uint8 nowStage;	//��ǰ���еĽ׶�
		uint32 nowSubStage;	//�ڶ��׶ε�С�׶�
		TaskSub task[3];		//һ�׶�С���� ����
		TaskSub task_second;//���׶�����
	};
	std::map<uint64, TaskPlayer>    m_playerLst;		//����б�

	bool	m_isBegined;		
	uint64	m_startTime;
	bool	m_isCountryOpen[3];		//���������Ƿ��ܿ�ʼ��������
	std::vector<int> tasklst;
	std::vector<int> taskSecond;

private:
	typedef std::vector<boost::function<void()> >CallBackEnd;
	CallBackEnd m_ActEnd;

public:
	PersonTask();
	static PersonTask& Instance(){ static PersonTask T; return T; }
	
	bool Begin(int lastSecond, MapCountryQuest* table);
	void End();

	void TakeAwardsBeforeStart();					//��һ������ʼ֮ǰ����������ȡ��

	void GetPersonalTask();
	TaskPlayer& GetPlayerData(Player& player);
	bool isPlayerInList(uint64 playerid);

	void addProgress(TaskEnum tasktype, uint32 addnum, Player& player);	//�����������

	void _TakeRewards(Player&player, uint8 taskid, pb::GS2C_PersonalTask_Take_Award& msg);//���ͻ�ȡ����
	
	void AddCallBack_OnActEnd(const boost::function<void()>& func);

public:
	void SaveTo(pb::OneTaskInfo& msg, Player& player, uint32 taskid);
	void SaveTo(pb::GS2C_PersonalTask_Info& msg, Player& player);

};

#define sPersonTask PersonTask::Instance()