#pragma once
#include "TimerEventDefine.h"
#include "TimingWheel.h"

class TimerEventGroup
{
	 typedef std::map<int,boost::shared_ptr<ITimerEvent> > EventMap;

	 typedef std::map<int,EventMap> TypeEventMap;

public:
	TimerEventGroup(void);
	~TimerEventGroup(void);

	void Attach(TimingWheel* pTimingWheel);
	void Detach();

	time_t Add(int type, int id, boost::function<void()>& f, U32 start, int repeat, U32 interval);
	void AddTimer(int type, boost::function<void()>& f, U32 start, int repeat, U32 interval );
	void RemoveEvent(int type,int id);
	void RemoveEventType(int type);
	void ClearAll();
	time_t GetTimerStartTime(int type,int id) const;
	bool HasTypeTimer(int type);

private:
	TimingWheel*		m_pTimingWheel;
	TypeEventMap		m_events;
	EventMap            m_typeEvent;
};
