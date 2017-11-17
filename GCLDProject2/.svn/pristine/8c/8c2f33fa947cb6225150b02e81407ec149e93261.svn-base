#pragma once

#include<list>
#include<vector>

#include "def/MmoAssert.h"
#include "def/TypeDef.h"
#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"
#include "lock_free/LockFreeQueue.h"

#define WheelGroupNumber 2// 只能是2修改成更大的值没有意义, 因为为2是就已经保证了，当其中一条时间轴满一半时，另外一条已经重新开始了
#define WheelNumber 3
#define WheelUnitNumber 1000

enum TimerEventState
{
	TIMER_EVENT_STATE_WAIT,
	TIMER_EVENT_STATE_CANCEL,
	TIMER_EVENT_STATE_EXPIRED,
};

struct ITimerEvent
{
	virtual ~ITimerEvent() {}

	virtual int State() const =0;
	virtual void Cancel() =0;
    virtual int Repeat() =0;
    virtual U32 DelayStartTime() const =0;
    virtual U32 IntervalTime() const =0;
	virtual boost::function<void()>& Handler() =0;
	virtual time_t GetBeginTime() const = 0;
	virtual bool IsExpier() const = 0;
	virtual bool IsCancel() const = 0;
};

struct TimerEvent : public ITimerEvent
{
public: 
	TimerEvent(boost::function<void()>& f, U32 start, int repeat, U32 interval)
	:m_handler(f)
	,m_expireCallback(0)
	,m_startTime(start)
	,m_intervalTime(interval)
	,m_delayStartTime(start)
	,m_repeat(repeat)
	,m_state(TIMER_EVENT_STATE_WAIT)
	,m_beginTime(0)
	{
	}

	void SetExpireCallbackFun(boost::function<void()> fb)
	{
		m_expireCallback = fb;
	}

	void Execute()
	{
		m_handler();
		UpdateForNextExec();
	}

	time_t GetBeginTime() const
	{
		return m_beginTime;
	}

	void SetBeginTime(time_t time)
	{
		m_beginTime = time;
	}

	U32 StartTime() const
	{
		return m_startTime;
	}

	void SetStartTime(U32 time)
	{
		m_startTime = time;
	}

	bool CanExecute() const
	{
		return m_state == TIMER_EVENT_STATE_WAIT;
	}
	
	virtual int State() const // ITimerEvent
	{
		return m_state;
	}

    virtual int Repeat()
    {
        return m_repeat;
    }

    virtual U32 DelayStartTime() const
    {
        return m_delayStartTime;
    }
    virtual U32 IntervalTime() const
    {
        return m_intervalTime;
    }
	boost::function<void()>& Handler() 
    {
        return m_handler;
    }

	bool IsExpier() const
	{
		return m_state == TIMER_EVENT_STATE_EXPIRED;
	}

	bool IsCancel() const
	{
		return m_state == TIMER_EVENT_STATE_CANCEL;
	}

	virtual void Cancel()
	{
		m_state = TIMER_EVENT_STATE_CANCEL; // ITimerEvent
	}

private:
	void UpdateForNextExec()
	{
		m_startTime = m_intervalTime;

		if (m_repeat == 0)
		{
			return;
		}

		if ( (--m_repeat) == 0 )
		{
			if (CanExecute())
			{
				if (!m_expireCallback.empty())
				{
					m_expireCallback();
				}
			}
			m_state = TIMER_EVENT_STATE_EXPIRED;
		}
	}

private:
	boost::function<void()>		m_handler;
	boost::function<void()>		m_expireCallback;
	U32							m_startTime;
	U32							m_intervalTime;
    U32                         m_delayStartTime;
	int							m_repeat;
	TimerEventState				m_state;
	time_t                      m_beginTime;
};


class TimingWheel;

class TimingWheelUnit
{
private:
	friend class TimingWheel ;

	//typedef std::vector<boost::shared_ptr<TimerEvent> >	EventList;
	typedef std::list<boost::shared_ptr<TimerEvent> >	EventList;      // 改为list在出错的时候不会崩掉，找不到问题，这里不用随机迭代，不损性能。
	typedef std::vector<EventList>						EventListArray;

public:
	TimingWheelUnit(U32 units, U32 unitLen)
	:m_EvtListArray(units)
	,m_curSlot(0)
	,m_UnitLen(unitLen)
	{
	}

	inline bool IncreasePoint()
	{
		m_curSlot ++;
		size_t uints = m_EvtListArray.size();

		if( m_curSlot < uints )
		{
			return false;
		}

		m_curSlot -= uints;
		return true;
	}

	inline U32 CurSlot() const
	{
		return m_curSlot;
	}
	
	inline void PushEvent(U32 slot, boost::shared_ptr<TimerEvent>& ev, U32 absTime)
	{
		ev->SetStartTime(absTime % m_UnitLen);
		m_EvtListArray.at(slot).push_back(ev);
	}

	void ExecCurSlot(TimingWheel& wheel, int level);


private:
	inline void SetCurSlot( U32 slot )
	{
		MMO_ASSERT( slot < m_EvtListArray.size() ) ;
		m_curSlot =slot ;
	}

	void ExecEvent(TimingWheel& wheel, int level, boost::shared_ptr<TimerEvent>& ev);

	EventListArray	m_EvtListArray;
	U32				m_curSlot;
	U32				m_UnitLen;
};


// 使用建议，添加的事件，开始时间与间隔时间都不应该超过时间条的一半大小，不然第二次可能出现加入失败的情况
class TimingWheel
{
	friend class TimingWheelUnit;
	typedef vector<TimingWheelUnit> WheelGroup;

public:
	TimingWheel(U32 wheels = WheelNumber, U32 units = WheelUnitNumber, U32 resolution = 1);

	boost::shared_ptr<TimerEvent> AddEvent(
		boost::function<void()> f, 
		U32 start, 
		int repeat = 1, 
		U32 interval =0
		);

	boost::shared_ptr<TimerEvent> AddEventRef(
		boost::function<void()>& f, 
		U32 start, 
		int repeat = 1, 
		U32 interval =0
		);

	/// 一段时间内, 每隔多久执行一次的方法
	///
	/// @param start       : 多少时间单位后开始
	/// @param sustainTime : 持续多少时间, 以Update时间单位为准 ( 必须 > 0 否者添加失败 )
	/// @param interval    ：执行的间隔时间                     ( 必须 > 0 否者添加失败 )
	boost::shared_ptr<TimerEvent> AddIntervalEvent(
		boost::function<void()> f, 
		U32 start, 
		S32 sustainTime, 
		S32 interval
		);

	/// 一段时间内均匀执行N次添加方法
	///
	/// @param start       : 多少时间单位后开始
	/// @param sustainTime : 持续多少时间, 以Update时间单位为准 ( 必须 > 0 否者添加失败 )
	/// @param repeat      ：持续时间内重复次数                 ( 必须 > 0 否者添加失败 )
	boost::shared_ptr<TimerEvent> AddRepeatEvent(
		boost::function<void()> f, 
		U32 start, 
		S32 sustainTime, 
		S32 repeat
		);


	void Update(int64 dtMs);

	void UseTimeWheel( int32 index ) ;

private:
	void AddEvent(boost::shared_ptr<TimerEvent>& ev);
	void ForwardOneSlot();
	void CalcAbsSlot(U32 absTime, U32* absSlot);
	void CalcAbsSlot(U32 absTime, std::vector<U32>& absSlot);

	// 现在 WheelGroupNumber 个时间轴的大小是一样的，所以取当前的就行了, 如果不一样时需要传进下标，返回对应的size.
	int  WheelCount() const { return m_Wheels[m_curUseIndex].size(); }
	U32 GetStartTime(U32 start, U32 resolution);
	int GetRepeat(S32 sustainTime, S32 interval);
	int GetRepeat2(S32 sustainTime, S32 repeat);
	uint32          m_maxTime ;

	U32             m_Units;
	U32             m_resolution;
	U32             m_onceMs;
	int64           m_subTicks;
	bool            m_isDefaultConfig;

	int32           m_curUseIndex ;
	U32             m_curTime[WheelGroupNumber];
	WheelGroup      m_Wheels[WheelGroupNumber];
};
