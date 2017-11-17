#pragma once

#include "def/TypeDef.h"


class StageLevel;

namespace pb{
    class GS2C_StageLevel;
}

class EndlessLevelLog
{
public:
    EndlessLevelLog( const StageLevel& Proto);
    void LoadFrom(const pb::GS2C_StageLevel& levelInfo);
    void SaveTo(pb::GS2C_StageLevel& levelInfo) const;

    void ResetPeriodlyData(  ) ;

    bool CanFullSweep( uint32 vip_lv) const ;
    bool CanHalfSweep() const;

    uint32 FullSweepFloorCount() const;
    uint32 HalfSweepFloorCount() const;

    uint32 GetResetCountCurPeriod( void ) const;
    void AddResetCountCurPeriod( uint32 count = 1 );

    uint32 GetSweepCountCurPeriod() const;
    void AddSweepCountCurPeriod( uint32 count = 1);

    void GMResetRemain(  ) ;
    void LazyUpdate( ) ;

    uint32 GetCurFloor( void ) {   return m_curFloor ;   }
    void SetCurFloor( uint32 num ) {   m_curFloor =num ;   }

    uint32 GetHistoryMaxFloor() const { return m_historyMaxFloor;}
    void SetHistoryMaxFloor( uint32 num) { m_historyMaxFloor = num;} 

    const StageLevel& Proto() const { return m_LevelProto; }

private:
    const StageLevel& m_LevelProto;

    uint32        m_curFloor ;  
    uint32        m_historyMaxFloor ;   

    uint32        m_sweepCount;
    uint32        m_resetCount ;
    time_t        m_dataValidTime ;
};