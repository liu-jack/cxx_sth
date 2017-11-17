/*******************************************************
* @ 执行update，在固定的时间点进行初始化以及重置操作
* @ brief

* @ author liup
* @ date 2017-3-23
********************************************************/
#pragma once

#include "def/TypeDef.h"
#include "common/utility/TimeParser.h"
#include "UpdateEventBase.h"

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

class AutoUpdateBase
{
public:
	typedef std::map<auto_update_enum::timeCheckPeriod, int> PeriodMap;

	typedef std::vector<boost::shared_ptr<UpdateEventBase>> UpdateMap;
public:
	//typedef bool(AutomaticUpdate::*UpdateFunc)();
	//静态单例
	//static AutoUpdateBase& GetInstance(){ static AutoUpdateBase T; return T; }

	void Add(boost::function<void()> func, auto_update_enum::timeCheckPoint checkPoint, auto_update_enum::timeCheckPeriod checkPeriod);
	void Init();
	virtual void InitAddFunctions() = 0;	//添加需要计时监控的函数
	virtual void Update(uint64 nowTimeMS,int64 diffMS) = 0;				//接口，实时刷新

	auto_update_enum::timeCheckPeriod GetTimePeriod(uint64 time);

public:
	UpdateMap		m_updateMap;		//需要定时刷新的事件列表
	PeriodMap		m_periodMap;		//参数列表
};