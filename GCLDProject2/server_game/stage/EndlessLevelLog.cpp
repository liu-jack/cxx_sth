#include "EndlessLevelLog.h"
#include "StageLevel.h"

#include "OS.h"
#include "def/MmoAssert.h"
#include "utility/TimePeriod.h"

#include "Enum.pb.h"
#include "InterServer.pb.h"
#include "ServerOpcode.pb.h"


static const unsigned HALF_SWEEP_FLOOR_REMAIN = 10;
static const unsigned FULL_SWEEP_VIP_REQUIRED = 2;


EndlessLevelLog::EndlessLevelLog( const StageLevel& Proto )
: m_LevelProto(Proto)
, m_curFloor( 0)
, m_historyMaxFloor( 0)
, m_sweepCount( 0)
, m_resetCount( 0)
, m_dataValidTime( 0)
{

}

void EndlessLevelLog::ResetPeriodlyData()
{
    m_resetCount = 0 ;
    m_sweepCount = 0 ;
    m_dataValidTime = sPeriodSystem.GetNowPeriodEndTime( m_LevelProto.GetResetPeriodLong() ) ;
}

void EndlessLevelLog::GMResetRemain()
{
    ResetPeriodlyData() ;
}

void EndlessLevelLog::LazyUpdate()
{
    time_t now = sOS.GetRealTime();
    if( now >= m_dataValidTime )
    {
        ResetPeriodlyData() ;
    }
}

void EndlessLevelLog::LoadFrom( const pb::GS2C_StageLevel& levelInfo )
{
    m_curFloor              =levelInfo.cur_floor();
    m_historyMaxFloor       =levelInfo.history_floor();
    m_resetCount            =levelInfo.reset_count();
    m_sweepCount            =levelInfo.sweep_count_today();
    m_dataValidTime         =levelInfo.data_valid_time();

}

void EndlessLevelLog::SaveTo( pb::GS2C_StageLevel& levelInfo ) const
{
    levelInfo.set_level_id( m_LevelProto.id() );

    levelInfo.set_cur_floor( m_curFloor );
    levelInfo.set_history_floor( m_historyMaxFloor );
    levelInfo.set_reset_count( m_resetCount );
    levelInfo.set_sweep_count_today( m_sweepCount);
    levelInfo.set_data_valid_time( m_dataValidTime );
}


bool EndlessLevelLog::CanFullSweep( uint32 vip_lv ) const
{
    if (  vip_lv < FULL_SWEEP_VIP_REQUIRED )
    {
        return false;
    }
    if ( m_curFloor == m_historyMaxFloor)
    {
        return false;
    }
    return true;
}

bool EndlessLevelLog::CanHalfSweep() const
{
    if ( m_historyMaxFloor < HALF_SWEEP_FLOOR_REMAIN)
    {
        return false;
    }
    if ( m_curFloor >= m_historyMaxFloor - HALF_SWEEP_FLOOR_REMAIN)
    {
        return false;
    }
    return true;
}

uint32 EndlessLevelLog::GetResetCountCurPeriod( void ) const
{
    if ( m_dataValidTime > sOS.GetRealTime())
    {
        return m_resetCount;
    }
    else
    {
        return 0;
    }
}

void EndlessLevelLog::AddResetCountCurPeriod( uint32 count )
{
    LazyUpdate();
    m_resetCount += count ;
}


uint32 EndlessLevelLog::GetSweepCountCurPeriod() const
{
    if ( m_dataValidTime > sOS.GetRealTime() )
    {
        return m_sweepCount;
    }
    else
    {
        return 0;
    }
}

void EndlessLevelLog::AddSweepCountCurPeriod( uint32 count )
{
    LazyUpdate();
    m_sweepCount += count;
}

uint32 EndlessLevelLog::FullSweepFloorCount() const
{
    return m_historyMaxFloor - m_curFloor; 
}

uint32 EndlessLevelLog::HalfSweepFloorCount() const
{
    return m_historyMaxFloor - HALF_SWEEP_FLOOR_REMAIN - m_curFloor;
}
