#pragma once
#include "common/auto_update_base/AutoUpdateBase.h"
#include "common/auto_update_base/UpdateEventBase.h"
#include "TimerEvent/TimingWheel.h"
class AutoUpdateGame:public AutoUpdateBase
{
private:
	AutoUpdateGame();
public:
	static AutoUpdateGame& GetInstance(){ static AutoUpdateGame T; return T; }

	virtual void InitAddFunctions();
	virtual void Update(uint64 nowTimeMS,int64 diffMS);


	bool doUpdateZero(time_t nowtime, boost::shared_ptr<UpdateEventBase> upe);
	bool doUpdateFour(time_t nowtime, boost::shared_ptr<UpdateEventBase> upe);
	bool doUpdateTwentyTwo(time_t nowtime, boost::shared_ptr<UpdateEventBase> upe);
	bool doUpdateTwentyThreeAndTen(time_t nowtime, boost::shared_ptr<UpdateEventBase> upe);
	bool doUpdateTwentyThree(time_t nowtime, boost::shared_ptr<UpdateEventBase> upe);
	void AddEvent(boost::function<void()> f, U32 start, int repeat = 1, U32 interval =0);

	TimingWheel& EventTimer() { return m_eventTimer; }

	void DoUpdatePer_Day_Zero();				//����  ÿ��		0��
	void DoUpdatePer_Day_TwentyThreeAndTen();	//����	ÿ��		23��10��
	void DoUpdatePer_Day_TwentyThree();			//����	ÿ��		23��
	void DoUpdatePer_FourHour_Zero();			//����	ÿ��Сʱ	0��

	void DoUpdatePer_Hour_Zero();				//����	ÿСʱ		0��

	void DoUpdatePer_Second_Zero();				//����	ÿ��		0��
	void DoUpdatePer_Minute_Zero();


private:
	TimingWheel m_eventTimer;
};

#define sAutoUpdateGame AutoUpdateGame::GetInstance()