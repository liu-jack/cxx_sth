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

	void DoUpdatePer_Day_Zero();				//更新  每日		0点
	void DoUpdatePer_Day_TwentyThreeAndTen();	//更新	每日		23点10分
	void DoUpdatePer_Day_TwentyThree();			//更新	每日		23点
	void DoUpdatePer_FourHour_Zero();			//更新	每四小时	0点

	void DoUpdatePer_Hour_Zero();				//更新	每小时		0点

	void DoUpdatePer_Second_Zero();				//更新	每秒		0点
	void DoUpdatePer_Minute_Zero();


private:
	TimingWheel m_eventTimer;
};

#define sAutoUpdateGame AutoUpdateGame::GetInstance()