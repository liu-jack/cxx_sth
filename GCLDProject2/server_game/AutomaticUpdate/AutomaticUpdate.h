/*******************************************************
* @ 执行update，在固定的时间点进行初始化以及重置操作
* @ brief

* @ author liup
* @ date 2017-3-23
********************************************************/
#pragma once

#include "def/TypeDef.h"
#include "common/utility/TimeParser.h"
#include "UpdateEvent.h"

#define SECOND_LONG			1
#define HALF_MINUTE_LONG	(SECOND_LONG * 30)
#define MINUTE_LONG			(SECOND_LONG * 60)
#define HALF_HOUR_LONG		(MINUTE_LONG * 30)
#define HOUR_LONG			(MINUTE_LONG * 60)
#define TWO_HOUR_LONG		(HOUR_LONG * 2)
#define THREE_HOUR_LONG		(HOUR_LONG * 3)
#define FOUR_HOUR_LONG		(HOUR_LONG * 4)
#define SIX_HOUR_LONG		(HOUR_LONG * 6)
#define EIGHT_HOUR_LONG		(HOUR_LONG * 8)
#define TWELVE_HOUR_LONG	(HOUR_LONG * 12)
#define DAY_LONG			(HOUR_LONG * 24)
#define WEEK_LONG			(DAY_LONG * 7)
#define MONTH_LONG			(DAY_LONG * 30)

class AutomaticUpdate
{
	typedef std::map<detail_enum::timeCheckPeriod, int> PeriodMap;

	typedef std::vector<boost::shared_ptr<UpdateEvent>> UpdateMap;
public:
	//typedef bool(AutomaticUpdate::*UpdateFunc)();
	//静态单例
	static AutomaticUpdate& GetInstance(){ static AutomaticUpdate T; return T; }

	void Add(boost::function<void()> func, detail_enum::timeCheckPoint checkPoint, detail_enum::timeCheckPeriod checkPeriod);
	void Init();

	//接口，实时刷新
	void Update();

	bool doUpdateZero(time_t nowtime, boost::shared_ptr<UpdateEvent> upe);
	bool doUpdateFour(time_t nowtime, boost::shared_ptr<UpdateEvent> upe);
	bool doUpdateTwentyTwo(time_t nowtime, boost::shared_ptr<UpdateEvent> upe);
	bool doUpdateTwentyThreeAndTen(time_t nowtime, boost::shared_ptr<UpdateEvent> upe);

	detail_enum::timeCheckPeriod GetTimePeriod(uint64 time);

	void DoUpdatePer_Day_Zero();				//更新  每日		0点
	void DoUpdatePer_Day_TwentyThreeAndTen();	//更新	每日		23点10分

	void DoUpdatePer_FourHour_Zero();			//更新	每四小时	0点

	void DoUpdatePer_Hour_Zero();				//更新	每小时		0点

	void DoUpdatePer_Second_Zero();				//更新	每秒		0点
private:
	UpdateMap		m_updateMap;		//需要定时刷新的事件列表
	PeriodMap		m_periodMap;		//参数列表
};

#define sAutoUpdate AutomaticUpdate::GetInstance()