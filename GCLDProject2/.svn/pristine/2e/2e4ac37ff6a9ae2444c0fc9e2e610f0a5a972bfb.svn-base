#ifndef _SERVER_FRAMEWORK_LOGGER_H_
#define _SERVER_FRAMEWORK_LOGGER_H_

#include <memory>
#include "SimpleSingleton.h"

class LoggerImpl;

struct ILogWriter
{
	virtual ~ILogWriter() {}

	virtual void Write(const char* log) = 0;
	virtual void Flush() {}
	virtual void ChekState() {}
};

class Logger : public SimpleSingleton<Logger>
{
	friend class SimpleSingleton<Logger>;

public:
	void AddFileLog(const char* logFilePrefix);
	void Log(const char *cur_file, const char *cur_fun, int cur_line, char type, const char *fmt, ...);
	void LogNotTypeAndTime(const char *cur_file, const char *cur_fun, int cur_line, const char *fmt, ...);

private:
	Logger();

public:
	~Logger();

private:
	std::auto_ptr<LoggerImpl>	m_loggerImpl;
	std::auto_ptr<ILogWriter>	m_fileWriter;
};

#define sLogger (Logger::Instance())

#define _LPLOG

#ifndef DISABLE_LOG
	#define ELOG(...)   sLogger.Log(__FILE__, __FUNCTION__, __LINE__, 'E', __VA_ARGS__) // error conditions
	#define WLOG(...)   sLogger.Log(__FILE__, __FUNCTION__, __LINE__, 'W', __VA_ARGS__) // warning conditions
	#define NLOG(...)   sLogger.Log(__FILE__, __FUNCTION__, __LINE__, 'N', __VA_ARGS__) // normal but significant condition
	#define SLOG(...)   sLogger.LogNotTypeAndTime(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__) // normal but significant condition
	#ifdef _LPLOG
		#define LLOG(...)   sLogger.Log(__FILE__, __FUNCTION__, __LINE__, 'L', __VA_ARGS__) // detail LP log with L
		#define PLOG(...)	sLogger.Log(__FILE__, __FUNCTION__, __LINE__, 'P', __VA_ARGS__) // simple LP log with P
	#else
		#define LLOG(...)   (__VA_ARGS__)// detail LP log with L
		#define PLOG(...)	sLogger.Log(__FILE__, __FUNCTION__, __LINE__, 'P', __VA_ARGS__) // simple LP log with P
	#endif

	#ifdef _DEBUG
		#define DLOG(...)   sLogger.Log(__FILE__, __FUNCTION__, __LINE__, 'D', __VA_ARGS__) // debug-level messages
		#define ILOG(...)   sLogger.Log(__FILE__, __FUNCTION__, __LINE__, 'I', __VA_ARGS__) // informational
	#else
		#define DLOG(...)
		#define ILOG(...)   
	#endif
#else
	#define ELOG(...) 
	#define WLOG(...)
	#define NLOG(...)
	#define DLOG(...)
	#define ILOG(...)   
#endif

#endif // _SERVER_FRAMEWORK_LOGGER_H_