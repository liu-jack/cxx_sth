#pragma once

#include <list>
#include <vector>
#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"
#include "def/TypeDef.h"

namespace detail_enum{
enum timeCheckPeriod		//定时的时间段	以24小时为标准，不允许出现非24的约数
{
	PER_SECOND,			//每秒
	PER_HALF_MINUTE,	//每30秒
	PER_MINUTE,			//每分钟
	PER_HALF_HOUR,		//每半小时
	PER_HOUR,			//每小时
	PER_TWO_HOUR,		//两小时
	PER_THREE_HOUR,		//三小时
	PER_FOUR_HOUR,		//四小时
	PER_SIX_HOUR,		//六小时
	PER_EIGHT_HOUR,		//八小时
	PER_TWELVE_HOUR,	//十二小时
	PER_DAY,			//一天 24小时
	PER_WEEK,			//一周 7天
	PER_MONTH,			//一个月 30天
	TIME_NULL,			//无效时间
};

enum timeCheckPoint		//定时基准时间点
{
	TIME_ZERO,			//0点
	TIME_FOUR,			//4点
	TIME_TWENTYTWO,		//22点
	TIME_TWENTYTHREE_AND_TEN,	//23点10分
};
}

struct IUpdateEvent
{
	virtual ~IUpdateEvent() {}

	virtual boost::function<void()>& Handler() = 0;
	virtual void Cancel() = 0;
	virtual detail_enum::timeCheckPeriod GetCheckPeriod() const = 0;
	virtual detail_enum::timeCheckPoint GetCheckPoint() const = 0;
};

struct UpdateEvent : public IUpdateEvent
{
public:
	UpdateEvent(boost::function<void()>& func, detail_enum::timeCheckPeriod checkPeriod, detail_enum::timeCheckPoint checkPoint)
		:m_handler(func)
		,m_checkPeriod(checkPeriod)
		,m_checkPoint(checkPoint)
		,m_lastUpdateTime(0)
	{
	}

	void Execute()
	{
		m_handler();
	}

	virtual void Cancel()
	{
		
	}

	virtual detail_enum::timeCheckPeriod GetCheckPeriod() const
	{
		return m_checkPeriod;
	}

	virtual detail_enum::timeCheckPoint GetCheckPoint() const
	{
		return m_checkPoint;
	}

	time_t GetLastUpdateTime()
	{
		return m_lastUpdateTime;
	}

	void SetLastUpdateTime(uint64 timenow)
	{
		m_lastUpdateTime = timenow;
	}

	virtual boost::function<void()>& Handler() 
	{
		return m_handler;
	}

private:
	boost::function<void()>		m_handler;
	detail_enum::timeCheckPeriod				m_checkPeriod;
	detail_enum::timeCheckPoint				m_checkPoint;
	time_t						m_lastUpdateTime;
};