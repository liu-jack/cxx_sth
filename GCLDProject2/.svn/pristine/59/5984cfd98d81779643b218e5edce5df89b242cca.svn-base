#pragma once

#include <boost/thread.hpp>
#include "boost/function.hpp"
#include <boost/smart_ptr.hpp>
#include "TimingWheel.h"
class TimerThread
{
public:
	TimerThread();
	~TimerThread();

	void Start();
	void Stop();

	void AddEvent(boost::function<void()> f, int interval);
	boost::shared_ptr<TimerEvent> AddEventRef(
		boost::function<void()>& f, 
		U32 start, 
		int repeat = 1, 
		U32 interval =0
		);

private:
	void Run();

private:
	bool								m_isRunning;
	boost::scoped_ptr<boost::thread>	m_workThread;
	boost::scoped_ptr<TimingWheel>		m_timerWheel;
	boost::mutex						m_mutex;

};