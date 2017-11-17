
#include <sys/timeb.h>
#include <time.h>

#include <string>
#include <fstream>
#include <boost/pool/pool.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
//#include <boost/filesystem.hpp>

#include "Logger.h"
#include "preHeader.h"
#include "lock_free/LockFreeQueue.h"
#include "ptr_container/PtrVector.h"

#define LOG_LINE_BUFF_LEN 510
#define LOG_LINE_ITEM_LEN 512
#define LOG_FILE_MAX_SIZE 260*1048576 //Bytes


//=========================================================================================================
class LoggerImpl
{
private:
	boost::mutex					m_logLinePoolMutex;
	boost::pool<>					m_logLinePool;
	ServerQueue<char*>				m_logQueue;
	std::auto_ptr<boost::thread>	m_workThread;
	bool							m_isWorking;
	ILogWriter*						m_logWriter;

private:
	void FreeLineBuff(char* buff)
	{
		boost::mutex::scoped_lock l(m_logLinePoolMutex);
		m_logLinePool.free(buff);
	}

	void FlushLogQueue()
	{
		char* log = NULL;
		while(m_logQueue.Dequeue(log))
		{
			m_logWriter->Write(log);
			FreeLineBuff(log);
		}

		if (log)
		{
			m_logWriter->Flush();
			m_logWriter->ChekState();
		}
	}

public:
	LoggerImpl(ILogWriter* logWriter)
	:m_logLinePool(LOG_LINE_ITEM_LEN)
	,m_isWorking(true)
	,m_logWriter(logWriter)
	{
		m_workThread.reset(new boost::thread(boost::ref(*this)));
	}

	~LoggerImpl()
	{
		m_isWorking = false;
		m_workThread->join();
	}

	char* AllocLineBuff()
	{
		boost::mutex::scoped_lock l(m_logLinePoolMutex);
		return (char*)m_logLinePool.malloc();
	}

	void Enqueue(char* log)
	{
		m_logQueue.Enqueue(log);
	}

	void operator()()
	{
		while(m_isWorking)
		{
			FlushLogQueue();
			boost::this_thread::sleep(boost::posix_time::milliseconds(1));
		}

		FlushLogQueue();
	}
};

//=========================================================================================================
class LogWriterGroup : public ILogWriter
{
public:
	void Add(ILogWriter* writer)
	{
		m_writers.push_back(writer);
	}

	virtual void Write(const char* log)
	{
		std::for_each(
			m_writers.begin(), 
			m_writers.end(), 
			boost::bind(&ILogWriter::Write, _1, log)
		);
	}

	virtual void Flush() 
	{
		std::for_each(
			m_writers.begin(), 
			m_writers.end(), 
			boost::bind(&ILogWriter::Flush, _1)
		);
	}

	virtual void ChekState()
	{
		std::for_each(
			m_writers.begin(), 
			m_writers.end(), 
			boost::bind(&ILogWriter::ChekState, _1)
		);
	}

private:
	PtrVector<ILogWriter>	m_writers;
};

//=========================================================================================================
class ConsoleLogWriter : public ILogWriter
{
public:
	virtual void Write(const char* log)
	{
		printf ( "%s", log);
	}
};

//=========================================================================================================
class FileLogWriter : public ILogWriter
{
public:
	FileLogWriter(const char* prefix)
	{
		m_restBytes = LOG_FILE_MAX_SIZE;
		//m_logPath = prefix;
		m_prefix.assign( prefix ) ;
		OpenFile( m_prefix.c_str() ) ;
	}

	~FileLogWriter()
	{
		CloseFile() ;
	}

	virtual void Write(const char* log)
	{
		m_stream << log;
		uint64 curLen = strlen(log) + 1;
		m_restBytes = m_restBytes > curLen ? (m_restBytes - curLen) :0;
	}

	virtual void Flush() 
	{
		m_stream.flush();
	}

	virtual void ChekState()
	{
		if( m_restBytes == 0 )
		{
			OpenFile( m_prefix.c_str() ) ;
		}
	}

public:
	void OpenFile( const char* prefix )
	{
		CloseFile() ;
		m_restBytes = LOG_FILE_MAX_SIZE;
		std::string fname = GenerateFileName(prefix);
		m_stream.open(fname.c_str(), std::ios::app);
	/*	if (boost::filesystem::exists(m_logPath))
		{
			boost::filesystem::remove(m_logPath);
		}
		m_logPath = fname;*/
	}

	void CloseFile()
	{
		if( m_stream.is_open() )
		{
			m_stream.flush();
			m_stream.close();
			m_stream.clear() ;
		}
	}

private:
	std::string GenerateFileName(const char* prefix)
	{
		char timeTxt[128];
		struct tm pl;
#ifdef WIN32
		struct _timeb tp ;
		::_ftime( &tp ) ;
		localtime_s(&pl,&tp.time);
#else
		struct timeb tp;
		ftime(&tp);
		localtime_r(&tp.time,&pl);
#endif
		strftime(timeTxt, 128, "%Y-%m-%d_%H-%M-%S", &pl);

		std::string logFile = prefix;
		logFile += timeTxt;
		logFile += ".log";

		return logFile;
	}

private:
	//boost::filesystem::path m_logPath;
	uint64              m_restBytes;
	std::string         m_prefix ;
	std::ofstream		m_stream;
};

//=========================================================================================================
Logger::Logger()
{
	LogWriterGroup* writerGroup = new LogWriterGroup();
	writerGroup->Add(new ConsoleLogWriter());
	
	m_fileWriter.reset(writerGroup);
	m_loggerImpl.reset(new LoggerImpl(writerGroup));
}

Logger::~Logger()
{
	m_loggerImpl.reset();
	m_fileWriter.reset();
}

void Logger::AddFileLog( const char* logFilePrefix)
{
	LogWriterGroup* writerGroup = dynamic_cast<LogWriterGroup*>(m_fileWriter.get());
	writerGroup->Add(new FileLogWriter( logFilePrefix));
}

void Logger::Log(const char *cur_file, const char *cur_fun, int cur_line, char type, const char *fmt, ...)
{
	char* content = m_loggerImpl->AllocLineBuff();
	if (!content)
	{
		return;
	}
	int idx =0;
	// prefix
	content[idx ++] = type;
	content[idx ++] = '|';

struct tm pl;
#ifdef WIN32
	struct _timeb tp ;
	::_ftime( &tp ) ;
	localtime_s(&pl,&tp.time);
#else
	struct timeb tp;
	ftime(&tp);
	localtime_r(&tp.time,&pl);
#endif

	// date time
	//time_t now = time(NULL);
	//idx += strftime(content +idx, LOG_LINE_BUFF_LEN -idx, "%m-%d %H:%M:%S", localtime(&now));

	//struct tm lt =*::localtime( & tp.time ) ;
	//idx += sprintf(content +idx, "%m-%d %H:%M:%S:%d", lt.tm_mon, lt.tm_mday, lt.tm_hour, lt.tm_min, lt.tm_sec, tp.millitm );

	idx += strftime(content +idx, LOG_LINE_BUFF_LEN -idx, "%m-%d %H:%M:%S", &pl);
	idx += sprintf( content +idx, ":%d", tp.millitm ) ;
	content[idx ++] = '|';

	// log content
	va_list ap;
	va_start(ap, fmt);
	int len = vsnprintf(content +idx, LOG_LINE_BUFF_LEN -idx, fmt, ap);
	va_end(ap);

	if (len >0 && len < LOG_LINE_BUFF_LEN -idx-1)
	{
		idx += len;
	}
	else
	{
		idx = LOG_LINE_BUFF_LEN-2;
	}

	// line end
	content[idx ++] = '\n';
	content[idx ++] = 0;

	// output
	m_loggerImpl->Enqueue(content);
}


void Logger::LogNotTypeAndTime(const char *cur_file, const char *cur_fun, int cur_line, const char *fmt, ...)
{
	char* content = m_loggerImpl->AllocLineBuff();
	int idx =0;

	// log content
	va_list ap;
	va_start(ap, fmt);
	int len = vsnprintf(content +idx, LOG_LINE_BUFF_LEN -idx, fmt, ap);
	va_end(ap);

	if (len >0 && len < LOG_LINE_BUFF_LEN -idx-1)
	{
		idx += len;
	}
	else
	{
		idx = LOG_LINE_BUFF_LEN-2;
	}

	// line end
	content[idx ++] = '\n';
	content[idx ++] = 0;

	// output
	m_loggerImpl->Enqueue(content);
}

