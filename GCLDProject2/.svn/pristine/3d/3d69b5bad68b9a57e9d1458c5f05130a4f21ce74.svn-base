#include "TimerThread.h"
#include "OS.h"
#include "def/MmoAssert.h"
//#include "Debugger.h"

TimerThread::TimerThread()
:m_isRunning(false)
{
}

TimerThread::~TimerThread()
{
	Stop();
}

void TimerThread::Start()
{
	Stop();

	m_isRunning = true;
	m_timerWheel.reset(new TimingWheel(3, 1000));
	m_workThread.reset(new boost::thread(boost::bind(&TimerThread::Run, this)));
}

void TimerThread::Stop()
{
	m_isRunning = false;
	if (m_workThread)
	{
		m_workThread->join();
		m_workThread.reset();
		m_timerWheel.reset();
	}
}

void TimerThread::Run()
{
	MMO_ASSERT(m_timerWheel.get() != NULL);

	uint64 lastTime = sOS.TimeMS() /1000;

	while (m_isRunning)
	{
		uint64 currentTime = sOS.TimeMS() /1000;
		uint64 diffTime = currentTime - lastTime;
		lastTime = currentTime;

		{
			boost::mutex::scoped_lock lock(m_mutex);
			m_timerWheel->Update(diffTime);
		}

		boost::this_thread::sleep(boost::posix_time::milliseconds(500));
	}
}

void TimerThread::AddEvent( boost::function<void()> f, int interval )
{
	MMO_ASSERT(m_timerWheel.get() != NULL);
	MMO_ASSERT(interval > 0);

	boost::mutex::scoped_lock lock(m_mutex);

	m_timerWheel->AddEventRef(f, interval, 0, interval);
}

boost::shared_ptr<TimerEvent> TimerThread::AddEventRef( boost::function<void()>& f, U32 start, int repeat /*= 1*/, U32 interval /*=0 */ )
{
	MMO_ASSERT(m_timerWheel.get() != NULL);
	MMO_ASSERT(interval > 0);
	boost::mutex::scoped_lock lock(m_mutex);
	return	m_timerWheel->AddEventRef(f, interval, repeat, interval);
}