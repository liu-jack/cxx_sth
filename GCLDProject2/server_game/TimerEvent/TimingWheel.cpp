#include "preHeader.h"
#include "TimingWheel.h"
#include "Logger.h"
#include "OS.h"
#include <string>

void TimingWheelUnit::ExecCurSlot(TimingWheel& wheel, int level)
{
	EventList& eventList = m_EvtListArray.at(m_curSlot);
	EventList::iterator iter = eventList.begin();

	const int beforeSize =eventList.size() ;
	for( ; iter != eventList.end(); ++iter)
	{
		ExecEvent(wheel, level, *iter);
		const int afgerSize =eventList.size() ;

		if( afgerSize != beforeSize )
		{
			ELOG( "TimingWheelUnit::ExecCurSlot() appear event while for add self!" ) ;
		}
		MMO_ASSERT( afgerSize == beforeSize ) ;
	}

	eventList.clear();
}

void TimingWheelUnit::ExecEvent(TimingWheel& wheel, int level, boost::shared_ptr<TimerEvent>& ev)
{
	if (level ==0)
	{
		if (!ev->CanExecute())
		{
			return;
		}
		ev->Execute();
	}

	if (ev->CanExecute())
	{
		wheel.AddEvent(ev);
	}
}

TimingWheel::TimingWheel(U32 wheels, U32 units, U32 resolution)
:m_Units(units)
,m_resolution(resolution)
,m_subTicks(0)
,m_onceMs(resolution*1000)
,m_isDefaultConfig(false)
{
	MMO_ASSERT(wheels >0);
	MMO_ASSERT(units >0);
	MMO_ASSERT(resolution >0);

	if ((wheels == WheelNumber) && (units == WheelUnitNumber) && (resolution == 1))
	{
		m_isDefaultConfig = true;
	}

	memset( m_curTime, 0, sizeof( m_curTime ) ) ;
	
	for( int32 w =0; w < WheelGroupNumber; ++w )
	{
		U32 unitLen = 1;
		for(U32 i=0; i < wheels; i++)
		{
			m_Wheels[w].push_back(TimingWheelUnit(units, unitLen));
			unitLen *= units;
		}
		
		// 保持时间轴成一半一半的状态
		if( w > 0 )
		{
			m_curTime[w] = unitLen / 2 * w ;

			// 计算出加在不同时间段上的位置
			if (m_isDefaultConfig)
			{
				U32  absSlot[WheelNumber];
				CalcAbsSlot( m_curTime[w], absSlot ) ;
				for( uint32 i =0; i < WheelNumber; ++i )
				{
					m_Wheels[w][i].SetCurSlot( absSlot[i] ) ;
				}
			}
			else
			{
				std::vector<U32>  absSlot(WheelCount());
				CalcAbsSlot( m_curTime[w], absSlot ) ;
				for( uint32 i =0; i < wheels; ++i )
				{
					m_Wheels[w][i].SetCurSlot( absSlot[i] ) ;
				}
			}
		}
		m_maxTime =unitLen - 1 ;
	}


	// 先使用第一个时间条
	UseTimeWheel( 0 ) ;
}

void TimingWheel::UseTimeWheel( int32 index )
{
	MMO_ASSERT( index < WheelGroupNumber ) ;
	m_curUseIndex =index ;
}




boost::shared_ptr<TimerEvent> TimingWheel::AddEvent(boost::function<void()> f, U32 start, int repeat, U32 interval)
{
	return AddEventRef(f, start, repeat, interval);
}

boost::shared_ptr<TimerEvent> TimingWheel::AddEventRef(boost::function<void()>& f, U32 start, int repeat, U32 interval)
{
	uint32 s = start/m_resolution;
	if (s ==0)
	{
		s = 1;
	}

	boost::shared_ptr<TimerEvent> ev(new TimerEvent(f, s, repeat, interval/m_resolution));
	ev->SetBeginTime(sOS.TimeSeconds());
	AddEvent(ev);

	return ev;
}

boost::shared_ptr<TimerEvent> TimingWheel::AddIntervalEvent( boost::function<void()> f, U32 start,  S32 sustainTime, S32 interval )
{
	MMO_ASSERT( start > 0 || sustainTime > 0 ) ;
	if( start <= 0 || sustainTime <= 0 )
	{
		return boost::shared_ptr<TimerEvent>() ;
	}

	uint32 repeat =sustainTime / interval ;
	if( repeat == 0 )
	{
		repeat =1 ;
	}

	return AddEventRef( f, start, repeat, interval );
}

boost::shared_ptr<TimerEvent> TimingWheel::AddRepeatEvent( boost::function<void()> f, U32 start, S32 sustainTime, S32 repeat )
{
	MMO_ASSERT( repeat > 0 || sustainTime > 0 ) ;
	if( repeat <= 0 || sustainTime <= 0 )
	{
		return boost::shared_ptr<TimerEvent>() ;
	}

	uint32 interval =sustainTime / repeat ;
	if( interval == 0 )
	{
		repeat =1 ;
	}

	return AddEventRef( f, start, repeat, interval );
}


void TimingWheel::CalcAbsSlot(U32 absTime, U32* absSlot)
{
	U32 time = absTime;

	for( U32 i=0; i< WheelNumber; i++)
	{
		absSlot[i] = time % m_Units;
		time /= m_Units;
	}

	if (time >0)
	{
		throw std::runtime_error("Time exceed wheel range\n");
	}
}

void TimingWheel::CalcAbsSlot( U32 absTime, std::vector<U32>& absSlot )
{
	U32 time = absTime;

	for( std::vector<U32>::size_type i=0; i< absSlot.size(); i++)
	{
		absSlot[i] = time % m_Units;
		time /= m_Units;
	}

	if (time >0)
	{
		throw std::runtime_error("Time exceed wheel range\n");
	}

}

void TimingWheel::AddEvent(boost::shared_ptr<TimerEvent>& ev)
{
	// TODO: 调试完删除下面的设定
	if( ev->StartTime() > m_maxTime / 2 )
	{
		ev->SetStartTime( m_maxTime / 2 ) ;
	}
	
	MMO_ASSERT( ev->StartTime() <= m_maxTime / 2 ) ;
	if( ev->StartTime() > m_maxTime / 2 )
	{
		return ;
	}

	U32 absTime =m_curTime[m_curUseIndex] +ev->StartTime() ;
	if( absTime > m_maxTime )
	{
		int i =0 ;
		int32 nextIndex =0 ;
		for( i =0; i < WheelGroupNumber; ++i ) // 最多查找次数
		{
			// 当前第一条时间线已经不足够的时候才开启第二条
			nextIndex =( m_curUseIndex + 1 ) % WheelGroupNumber ;
			absTime =m_curTime[nextIndex] + ev->StartTime() ;
			if( absTime < m_maxTime )
			{
				break ;
			}
		}

		MMO_ASSERT( i < WheelGroupNumber ) ; // 算法出错了，找不到合适的时间轴
		if( i >= WheelGroupNumber )
		{
			ELOG( "TimingWheel::AddEvent(), had error!" ) ;
			return ;
		}

		UseTimeWheel( nextIndex ) ;
	}

	// 计算出加在不同时间段上的位置
	WheelGroup &CurWheel =m_Wheels[ m_curUseIndex ] ;
	if (m_isDefaultConfig)
	{
		U32  absSlot[WheelNumber];
		CalcAbsSlot(absTime, absSlot);
		for( int i = WheelNumber -1; i >= 0; i--)
		{
			if (i ==0 || absSlot[i] > CurWheel[i].CurSlot())
			{
				CurWheel[i].PushEvent(absSlot[i], ev, absTime);
				break;
			}
		}
	}
	else
	{
		int curWhellCount = WheelCount();
		std::vector<U32>  absSlot(curWhellCount);
		CalcAbsSlot(absTime, absSlot);
		for( int i = curWhellCount -1; i >= 0; i--)
		{
			if (i ==0 || absSlot[i] > CurWheel[i].CurSlot())
			{
				CurWheel[i].PushEvent(absSlot[i], ev, absTime);
				break;
			}
		}
	}
}


void  TimingWheel::ForwardOneSlot()
{
	int curWhellCount = m_isDefaultConfig ? WheelNumber:WheelCount();
	for( int w=0; w < WheelGroupNumber; ++w )
	{
		if( ++m_curTime[w] > m_maxTime ) // 时间轴用完了，重置为0
		{
			m_curTime[w] =0 ;
			ILOG( "时间轴 TimingWheel 重置, 如果服务器不挂, Time Event 测试完毕，把构造方法的 2 改回 3 可以删除这个提示！" ) ;
		}

		int maxWheel = 0;

		for(; maxWheel < curWhellCount; ++maxWheel)
		{
			if(!m_Wheels[w].at(maxWheel).IncreasePoint())// 这里增加当前slot.
			{
				break;
			}
		}

		// Add
		if( maxWheel == curWhellCount )
		{
			--maxWheel ;
		}

		for (int i = maxWheel; i >= 0; i--)
		{
			m_Wheels[w][i].ExecCurSlot (*this, i);
		}
	}

}

void TimingWheel::Update(int64 dtMs)
{
	m_subTicks += dtMs;

	for(U32 i =0; i< m_subTicks /m_onceMs; i++ )
	{
		ForwardOneSlot();
	}

	m_subTicks %= m_onceMs;
}