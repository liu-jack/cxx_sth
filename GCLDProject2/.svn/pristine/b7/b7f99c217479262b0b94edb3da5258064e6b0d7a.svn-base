#include "TimerEventGroup.h"
#include "def/MmoAssert.h"

TimerEventGroup::TimerEventGroup(void)
:m_pTimingWheel(NULL)
{
}

TimerEventGroup::~TimerEventGroup(void)
{
	ClearAll();
}

void TimerEventGroup::Attach(TimingWheel* pTimingWheel)
{
	//MMO_ASSERT (pTimingWheel != NULL);
	//MMO_ASSERT (m_pTimingWheel == NULL);

	m_pTimingWheel = pTimingWheel;
}

void TimerEventGroup::Detach()
{
	m_pTimingWheel = NULL;
	ClearAll();
}

time_t TimerEventGroup::Add(int type, int id,boost::function<void()>& f, U32 start, int repeat, U32 interval)
{
	MMO_ASSERT(type >= 0);
	if (m_pTimingWheel == NULL)
	{
		return 0;
	}
	TypeEventMap::iterator iter = m_events.find(type);
	if (iter == m_events.end())
	{
		boost::shared_ptr<TimerEvent> ptrEvent = m_pTimingWheel->AddEventRef(f, start, repeat, interval);
		m_events[type][id] = ptrEvent;
		ptrEvent->SetExpireCallbackFun(boost::bind(&TimerEventGroup::RemoveEvent, this, type,id));
		return ptrEvent->GetBeginTime();
	}
	else
	{
		EventMap::iterator it = iter->second.find(id);
		if (it == iter->second.end())
		{
			boost::shared_ptr<TimerEvent> ptrEvent = m_pTimingWheel->AddEventRef(f, start, repeat, interval);
			m_events[type][id] = ptrEvent;
			ptrEvent->SetExpireCallbackFun(boost::bind(&TimerEventGroup::RemoveEvent, this, type,id));
			return ptrEvent->GetBeginTime();
		}
		else
		{
			if (it->second)
			{
				it->second->Cancel();
			}
			iter->second.erase(it);
			boost::shared_ptr<TimerEvent> ptrEvent = m_pTimingWheel->AddEventRef(f, start, repeat, interval);
			m_events[type][id] = ptrEvent;
			ptrEvent->SetExpireCallbackFun(boost::bind(&TimerEventGroup::RemoveEvent, this, type,id));
			return ptrEvent->GetBeginTime();
			
		}
	}

	return 0;
}

bool TimerEventGroup::HasTypeTimer(int type)
{
	EventMap::iterator it = m_typeEvent.find(type);
	if (it != m_typeEvent.end())
	{
		return true;
	}
	return false;
}

void TimerEventGroup::AddTimer(int type, boost::function<void()>& f, U32 start, int repeat, U32 interval )
{
	RemoveEventType(type);
	boost::shared_ptr<TimerEvent> ptrEvent = m_pTimingWheel->AddEventRef(f, start, repeat, interval);
	m_typeEvent[type] = ptrEvent;
	ptrEvent->SetExpireCallbackFun(boost::bind(&TimerEventGroup::RemoveEventType, this, type));
}

void TimerEventGroup::RemoveEvent(int type, int id)
{
	MMO_ASSERT (id > 0);
	MMO_ASSERT(type >= 0);

	TypeEventMap::iterator iter = m_events.find(type);
	if (iter != m_events.end())
	{
		EventMap::iterator it = iter->second.find(id);
		if (it != iter->second.end())
		{
			if (it->second)
			{
				it->second->Cancel();
			}
			iter->second.erase(it);
		}
	}
}

void TimerEventGroup::RemoveEventType(int type)
{
	MMO_ASSERT(type >= 0);

	EventMap::iterator it = m_typeEvent.find(type);
	if (it != m_typeEvent.end())
	{
		if (it->second)
		{
			it->second->Cancel();
		}
		m_typeEvent.erase(it);
	}
}

void TimerEventGroup::ClearAll()
{
	for (TypeEventMap::iterator iter = m_events.begin(); iter != m_events.end(); ++iter)
	{
		EventMap::iterator it = iter->second.begin();
		for (;it != iter->second.end(); ++it)
		{
			if (it->second)
			{
				it->second->Cancel();
			}
		}
		iter->second.clear();
	}
	m_events.clear();

	for (EventMap::iterator it = m_typeEvent.begin(); it != m_typeEvent.end(); ++it)
	{
		if (it->second)
		{
			it->second->Cancel();
		}
	}
	m_typeEvent.clear();
}

time_t TimerEventGroup::GetTimerStartTime( int type,int id ) const
{
	MMO_ASSERT (id > 0);
	MMO_ASSERT(type >= 0);

	TypeEventMap::const_iterator iter = m_events.find(type);
	if (iter != m_events.end())
	{
		EventMap::const_iterator it = iter->second.find(id);
		if (it == iter->second.end())
		{
			return 0;
		}
		if (it->second)
		{
			return it->second->GetBeginTime();
		}
	}
	return 0;
}
