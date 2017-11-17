#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>

class WorkingMsgQueue
{
public:
	WorkingMsgQueue() : m_ioWork(m_ioSvc)
	{
	}

	~WorkingMsgQueue()
	{
		Stop();
	}

	void Start()
	{
		Stop();
		m_workThread.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &m_ioSvc)));
	}

	void Stop()
	{
		if (m_workThread)
		{
			m_ioSvc.stop();
			m_workThread->join();
			m_ioSvc.reset();
			m_workThread.reset();
		}
	}

	template <typename CompletionHandler>
	void post(CompletionHandler handler)
	{
		m_ioSvc.post(handler);
	}

private:
	boost::asio::io_service				m_ioSvc;
	boost::asio::io_service::work		m_ioWork; // keep m_ioSvc running
	boost::scoped_ptr<boost::thread>	m_workThread;

};