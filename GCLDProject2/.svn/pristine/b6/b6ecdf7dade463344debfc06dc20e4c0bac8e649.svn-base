#pragma once
#include "common/auto_update_base/AutoUpdateBase.h"
#include "common/auto_update_base/UpdateEventBase.h"
#include "TimerEvent/TimingWheel.h"

class AutoUpdateCross:public AutoUpdateBase
{
private:
	AutoUpdateCross();
public:
	static AutoUpdateCross& GetInstance(){ static AutoUpdateCross T; return T; }

	virtual void InitAddFunctions();
	virtual void Update(uint64 nowTimeMS,int64 diffMS);

	//bool doUpdateZero(time_t nowtime, boost::shared_ptr<UpdateEventBase> upe);
	//bool doUpdateFour(time_t nowtime, boost::shared_ptr<UpdateEventBase> upe);
	//bool doUpdateTwentyTwo(time_t nowtime, boost::shared_ptr<UpdateEventBase> upe);
	//bool doUpdateTwentyThreeAndTen(time_t nowtime, boost::shared_ptr<UpdateEventBase> upe);
	void AddEvent(boost::function<void()> f, U32 start, int repeat = 1, U32 interval =0);
	TimingWheel& EventTimer() { return m_eventTimer; }

	bool doUpdateZero(time_t nowtime, boost::shared_ptr<UpdateEventBase> upe);

	void DoUpdatePer_Day_Zero();				//更新  每日		0点
	void DoUpdatePer_Hour_Zero();				//更新  每时		0点

private:
	TimingWheel m_eventTimer;
};

#define sAutoUpdateCross AutoUpdateCross::GetInstance()