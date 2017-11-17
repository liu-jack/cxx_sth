#ifndef __SYSTEM_H_INCLUDED__
#define __SYSTEM_H_INCLUDED__

#include <time.h>
//#include "def\MmoAssert.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
/// @brief Structure representing a system time.
/// @see System::GetSystemTime(SystemTime& st).
typedef struct
{
	/// year (i.e.: 2006)
	short year;
	/// month (1-12)
	short month;
	/// day of week (WEEK_SUNDAY, WEEK_MONDAY, ...)
	short dayOfWeek;
	/// day of month (1-31)
	short day;
	/// local hour (0-23)
	short hour;
	/// minutes (0-59)
	short minute;
	/// seconds (0-59)
	short second;

	/// @brief Enumeration for days of week
   enum
   {
		WEEK_SUNDAY = 0,
		WEEK_MONDAY,
		WEEK_TUESDAY,
		WEEK_WEDNESDAY,
		WEEK_THURSDAY,
		WEEK_FRIDAY,
		WEEK_SATURDAY,
		WEEK_MAX
   };

} SystemTime;

typedef struct
{
	char name[20];
	unsigned long long user; 
	unsigned long long nice; 
	unsigned long long system;
	unsigned long long idle; 
	unsigned long long iowait;
	unsigned long long irq;
	unsigned long long softirq;
	unsigned long long steal;
	unsigned long long guest;
}CPU_STATE;

typedef struct
{
	char	name[20];
	unsigned int	total;
	unsigned int	buffer;
	unsigned int	free;  
	unsigned int	cached;  
}MEM_STATE;

typedef struct statstruct_proc {
  int           pid;                      	/** The process id. **/
  char          exName [1024]; 				/** The filename of the executable **/
  char          state; /** 1 **/          	/** R is running, S is sleeping, 
												   D is sleeping in an uninterruptible wait,
													Z is zombie, T is traced or stopped **/
  unsigned      euid,                     	/** effective user id **/
                egid;                     	/** effective group id */					     
  int           ppid;                     	/** The pid of the parent. **/
  int           pgrp;                     	/** The pgrp of the process. **/
  int           session;                  	/** The session id of the process. **/
  int           tty;                      	/** The tty the process uses **/
  int           tpgid;                    	/** (too long) **/
  unsigned int	flags;                    	/** The flags of the process. **/
  unsigned int	minflt;                   	/** The number of minor faults **/
  unsigned int	cminflt;                  	/** The number of minor faults with childs **/
  unsigned int	majflt;                   	/** The number of major faults **/
  unsigned int  cmajflt;                  	/** The number of major faults with childs **/
  int           utime;                    	/** user mode jiffies **/
  int           stime;                    	/** kernel mode jiffies **/
  int			cutime;                   	/** user mode jiffies with childs **/
  int           cstime;               		/** kernel mode jiffies with childs **/
  int           counter;              		/** process's next timeslice **/
  int           priority;             		/** the standard nice value, plus fifteen **/
  unsigned int  timeout;              		/** The time in jiffies of the next timeout **/
  unsigned int  itrealvalue;          		/** The time before the next SIGALRM is sent to the process **/
  int           starttime; /** 20 **/ 		/** Time the process started after system boot **/
  unsigned int  vsize;                		/** Virtual memory size **/
  unsigned int  rss;                  		/** Resident Set Size **/
  unsigned int  rlim;                 		/** Current limit in bytes on the rss **/
  unsigned int  startcode;            		/** The address above which program text can run **/
  unsigned int	endcode;              		/** The address below which program text can run **/
  unsigned int  startstack;           		/** The address of the start of the stack **/
  unsigned int  kstkesp;              		/** The current value of ESP **/
  unsigned int  kstkeip;              		/** The current value of EIP **/
  int			signal;                   	/** The bitmap of pending signals **/
  int           blocked; /** 30 **/   		/** The bitmap of blocked signals **/
  int           sigignore;            		/** The bitmap of ignored signals **/
  int           sigcatch;             		/** The bitmap of catched signals **/
  unsigned int  wchan;  /** 33 **/    		/** (too long) **/
  int			sched, 						/** scheduler **/
                sched_priority;		  		/** scheduler priority **/		
} AppInfo;

typedef struct statstruct_thread {
  int           pid;                      	/** The process id. **/
  char          comm[1024]; 				/** The filename of the executable **/
  char          state; /** 1 **/          	/** R is running, S is sleeping, 
												   D is sleeping in an uninterruptible wait,
												   Z is zombie, T is traced or stopped **/					     
  int           ppid;                     	/** The pid of the parent. **/
  int           pgrp;                     	/** The pgrp of the process. **/
  int           session;                  	/** The session id of the process. **/
  int           tty_nr;                    	/** The controlling terminal of the process **/
  int           tpgid;                    	/** the id of the foreground process group **/
  unsigned int	flags;                    	/** The flags of the process. **/
  unsigned long	minflt;                   	/** The number of minor faults **/
  unsigned long	cminflt;                  	/** The number of minor faults with childs **/
  unsigned long	majflt;                   	/** The number of major faults **/
  unsigned long cmajflt;                  	/** The number of major faults with childs **/
  unsigned long utime;                    	/** user mode jiffies **/
  unsigned long stime;                    	/** kernel mode jiffies **/
  long			cutime;                   	/** user mode jiffies with childs **/
  long          cstime;               		/** kernel mode jiffies with childs **/
  long          prioprity;             		/** the standard nice value, plus fifteen **/
  long			nice;						/** the nice value **/
  long			num_threads;				/** number of threads in this process **/
  long          itrealvalue;				/** the time in jiffies before the next SIGALRM is sent to the process**/
  unsigned long long starttime;				/** Time the process started after system boot **/
  unsigned long vsize;						/** Virtual memory size **/
  long			rss;						/** Resident Set Size **/
  unsigned long rsslim;						/** Current limit in bytes on the rss **/
  unsigned long startcode;					/** The address above which program text can run **/
  unsigned long endcode;					/** The address below which program text can run **/
  unsigned long startstack;					/** The address of the start of the stack **/
  unsigned long kstkesp;					/** The current value of ESP **/
  unsigned long kstkeip;					/** The current value of EIP **/
  unsigned long signal;						/** The bitmap of pending signals **/
  unsigned long blocked;					/** The bitmap of blocked signals **/
  unsigned long sigignore;					/** The bitmap of ignored signals **/
  unsigned long sigcatch;					/** The bitmap of catched signals **/
  unsigned long wchan;						/** channel in which the process is waiting **/
  unsigned long nswap;						/** number of pages swapped **/
  unsigned long cnswap;						/** cumulative nswap for child processes **/
  int			exit_signal;				/** signal to be sent to parent when we die **/
  int			processor;					/** cpu number last executed on **/
  unsigned      rt_priority;				/** real-time scheduling priority **/
  unsigned		policy;						/** scheduling policy **/
  unsigned long long delayacct_blkio_ticks; /** aggregated block I/O delays, measred in clock ticks **/
  unsigned long guest_time;					/** guest time of the process **/
  long			cguest_time;				/** guest time of the process's children **/		
} ThreadInfo;


/// @brief System class, similar to Java System class.
class System
{
public:	
	static void Init();
	static void Free();

	//random number generator - really fast
	static void SRand();
	static void SRand(int seed);
	static int Rand();
	static int Rand(int lowerBound, int upperBound);
	static float Randf();
	static float Randf(float lowerBound, float upperBound);
    template <typename T>  static typename std::vector<T*>::iterator RandWeight( std::vector<T*>& vec)
    {
        int sumWeight = 0;
        for (typename std::vector<T*>::iterator it = vec.begin(); it != vec.end(); ++it)
        {
            sumWeight += (*it)->weight;
        }
        int rendNum = Rand(0, sumWeight);
        for (typename std::vector<T*>::iterator it = vec.begin(); it != vec.end(); ++it)
        {
            if (rendNum < (int)(*it)->weight)
                return it;
            else
                rendNum -= (*it)->weight;
        }
        //MMO_ASSERT(0);
        return vec.end();
    }
    static int RandWeight(const IntPairVec& vec) //权重数值对<val, weight>
    {
        int sumWeight = 0;
        for (IntPairVec::const_iterator it = vec.begin(); it != vec.end(); ++it)
        {
            sumWeight += it->second;
        }
        int rendNum = Rand(0, sumWeight);
        for (IntPairVec::const_iterator it = vec.begin(); it != vec.end(); ++it)
        {
            if (rendNum < it->second)
                return it->first;
            else
                rendNum -= it->second;
        }
        //MMO_ASSERT(0);
        return -1;
    }

	static time_t GetRealTime();
	static tm GetRealDateTime();
	static long CurrentTimeMillis();
	static unsigned int GetAppTime();

	static void Update();
	static void GetCPUState(CPU_STATE& cpust);
	static void GetMemState(MEM_STATE& mem);
	static int	CalcCpuOccupy (CPU_STATE& o, CPU_STATE& n);
	static int	GetAppInfo(AppInfo* pInfo);

#if  defined(_MMO_SERVER_) || defined(_SERVER_CROSS_)

	static int  GetThreadsStat(ThreadInfo info[], int size);
	static void GetSubCpusStat(CPU_STATE stat[], int size);
	static unsigned long long GetProcessUseTimeMs();
	static unsigned long long TimeMs2Tick(unsigned long long timeMs);
	static unsigned long long GetCpuTimeMs();
	static int GetCpuNum();
	
private:
	static bool IsDigit(const char* s);

#endif

private:
	static long long s_appStartTime;
	static time_t	s_realTime;
	//static tm		s_realTM;
};	

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

inline bool IsEven(int number)
{
	return (number & 0x1) == 0;
}

inline bool IsOdd(int number)
{
	return !IsEven(number);
}

#endif
