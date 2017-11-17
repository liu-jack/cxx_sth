#include "SellLog.h"

#include "OS.h"
#include "utility/TimePeriod.h"
#include "utility/MsgTool.h"
#include "def/MmoAssert.h"

#include "SellManager.h"
#include "LogicSellItem.h"


#include "BaseDefineMgr.h"
#include "BaseDefine.pb.h"
using namespace pb;



RandomProgram::RandomProgram(  VictimType victim)
: m_victimType ( victim)
, m_randomProgramId( 0)
, m_todayManualRefreshCount( 0)
, m_autoRefreshTime( 0)
, m_lastManualRefreshTime( 0)
{

}

void RandomProgram::InitNewData()
{
    GenRandomProgram();
}


bool RandomProgram::LazyUpdate()
{
    if ( NeedGetNewRandomProgram())
    {
        GenRandomProgram();
        return true;
    }
    else
    {
        return false;
    }
}

ProgramID RandomProgram::GetProgramId( ) const
{
    return m_randomProgramId;
}

bool RandomProgram::TryManualRefresh( uint32 vipLv)
{
    uint32 TodayManualRefreshCount = GetTodayManualRefreshCount();
    uint32 DailyManualRefreshMax = 0;//sSellMgr.GetDailyManualRefreshMaxCount(m_victimType, vipLv);
    if ( TodayManualRefreshCount < DailyManualRefreshMax)
    {
        m_randomProgramId = sSellMgr.GetRandomProgramId( m_victimType, m_randomProgramId);
        
        m_todayManualRefreshCount = TodayManualRefreshCount + 1;
        m_lastManualRefreshTime = sOS.GetRealTime();

        return true; 
    }
    return false;
}

void RandomProgram::SaveTo( pb::GS2C_SellRandomProgram& msg ) const
{
    msg.set_victim_type( m_victimType);
    msg.set_program_id( m_randomProgramId);
    msg.set_today_manual_refresh_count( m_todayManualRefreshCount);
    msg.set_auto_refresh_time( m_autoRefreshTime);
    msg.set_last_manual_refresh_time( m_lastManualRefreshTime);
}

void RandomProgram::LoadFrom( const pb::GS2C_SellRandomProgram& msg )
{
    m_victimType = msg.victim_type();
    m_randomProgramId = msg.program_id();
    m_autoRefreshTime = msg.auto_refresh_time();
    m_todayManualRefreshCount = msg.today_manual_refresh_count();
    m_lastManualRefreshTime = msg.last_manual_refresh_time();
}

bool RandomProgram::NeedGetNewRandomProgram() const
{
    if ( m_autoRefreshTime == 0 )
    {
        return true;
    }
    // Ê±¼ä 
    if ( m_autoRefreshTime < sPeriodSystem.GetNowPeriodEndTime( GetAutoRefeshSecond()))
    {
        return true;
    }

    return  false;
}

uint32 RandomProgram::GetTodayManualRefreshCount() const
{
    if ( sPeriodSystem.GetNowPeriodBeginTime( SECONDS_PER_DAY) < m_lastManualRefreshTime )
    {
        return m_todayManualRefreshCount;
    }
    else
    {
        return 0;
    }
}

uint32 RandomProgram::GetAutoRefeshSecond() const
{
    switch( m_victimType)
    {
    case SELL_VICTIM_HERO: 
    case SELL_VICTIM_AWAKE:
        return sSellMgr.GetVictimAvalidPeriod(m_victimType);
    default:
        ASSERT(false);
        return 0;
    }
}

void RandomProgram::GenRandomProgram() const
{
    m_randomProgramId = sSellMgr.GetRandomProgramId( m_victimType, m_randomProgramId);  
    m_autoRefreshTime = sPeriodSystem.GetNowPeriodEndTime( GetAutoRefeshSecond());
}


//////////////////////////////////////////////////////////////////////////


SellProgramRecord::SellProgramRecord( ProgramID  programId)
: m_programId( programId)
, m_recordValidTime( 0)
, m_sellId2Count()
{
}

SellProgramRecord::~SellProgramRecord( void )
{
}

void SellProgramRecord::SaveTo( pb::GS2C_SellProgramInfo& msg, ProgramID sell_id /*= 0*/ , bool isSendToClient/* = true*/) const
{
    msg.set_program_id( m_programId);
    msg.set_record_valid_time( m_recordValidTime);

    // 
    if ( (isSendToClient && IsRecordAvalid()) 
        || !isSendToClient)
    {
        ::pb::GS2C_SellRecordList* recordsMsg = msg.mutable_records();
        if (sell_id == 0)
        {
            for (SellId2Count::const_iterator it = m_sellId2Count.begin(); it != m_sellId2Count.end(); ++it)
            {
                SaveRecordToMsg(recordsMsg, it);
            }
        }
        else
        {
            SellId2Count::const_iterator it = m_sellId2Count.find( sell_id);
            if ( it != m_sellId2Count.end())
            {
                SaveRecordToMsg( recordsMsg, it);
            }
        }
    }  
}

void SellProgramRecord::LoadFrom( const pb::GS2C_SellProgramInfo& msg )
{
    m_programId    = msg.program_id();

    if ( m_recordValidTime != msg.record_valid_time())
    {
        m_sellId2Count.clear();
    }
    m_recordValidTime = msg.record_valid_time();  

    const pb::GS2C_SellRecordList& recordList = msg.records();

    for ( int i = 0; i < recordList.sell_records_size(); ++i)
    {
        const pb::GS2C_ItemSellRecord& record = recordList.sell_records(i);
		LoadOneRecord( record ) ;
    }
}


void SellProgramRecord::LoadOneRecord( const pb::GS2C_ItemSellRecord& msg )
{
    m_sellId2Count[ msg.sell_id()] = msg.sell_count();
}



uint32 SellProgramRecord::GetTodaySellItemCount( SellId sell_id ) const
{
    if ( IsRecordAvalid() )
    {
        SellId2Count::const_iterator it = m_sellId2Count.find( sell_id);
        if ( it != m_sellId2Count.end())
        {
            return it->second;
        }           
    }
    return 0;
}


bool SellProgramRecord::IsBuyCountLimited( const LogicSellItem& logicSellItem , uint32 count, uint32 vipLv) const
{
    uint32 toBuyCount = GetTodaySellItemCount(logicSellItem.SellId()) + count;
    if ( logicSellItem.IsBuyCountLimited( toBuyCount, vipLv))
    {
        return true;
    }
    return false;
}


SellProgramRecord::OP_RESULT SellProgramRecord::TryAddSellItemCount( SellId sell_id, uint32 count, uint32 vipLv)
{
    const LogicSellItem* logicSellItem = sSellMgr.GetLogicSellItem( sell_id);
    if ( logicSellItem == NULL)
    {
        return OP_FALSE;
    }
    else{
        return TryAddSellItemCount( *logicSellItem , count, vipLv);
    }

}

SellProgramRecord::OP_RESULT SellProgramRecord::TryAddSellItemCount( const LogicSellItem& logicSellItem , uint32 count, uint32 vipLv)
{

    OP_RESULT result = OP_FALSE;
    if ( !IsRecordAvalid() )
    {
        ResetSellRecord();
        result = OP_AFFECT_ALL;
    }

    if ( IsBuyCountLimited(logicSellItem, count, vipLv) )
    {
        return result;
    }

    UpdateRecordValidTime();
    m_sellId2Count[logicSellItem.SellId()] += count;
    result = (std::max)(OP_AFFECT_ONE , result);
    return result;
}

void SellProgramRecord::ResetSellRecord()
{
    for(SellId2Count::iterator it = m_sellId2Count.begin(); it != m_sellId2Count.end(); ++it)
    {
        it->second = 0;
    }
}

bool SellProgramRecord::IsRecordAvalid() const
{
    return m_recordValidTime >= sOS.GetRealTime() || m_recordValidTime == 0;
}

void SellProgramRecord::UpdateRecordValidTime()
{
    uint32 period = sSellMgr.GetVictimAvalidPeriod( sSellMgr.GetVictimTypeOfProgram( m_programId));
    if (period == 0)
    {
        m_recordValidTime = 0;
    }
    else
    {
        m_recordValidTime = sPeriodSystem.GetNowPeriodEndTime(period);
    }
}

void SellProgramRecord::SaveRecordToMsg( ::pb::GS2C_SellRecordList* recordsMsg, SellId2Count::const_iterator it ) const
{
    pb::GS2C_ItemSellRecord* oneRecord = recordsMsg->add_sell_records();
    if ( oneRecord)
    {
        oneRecord->set_sell_id( it->first);
        oneRecord->set_sell_count( it->second);
    }
}



//////////////////////////////////////////////////////////////////////////
SellLog::SellLog()
{
}

SellLog::~SellLog()
{
}

bool SellLog::InitVictimRDProgram( VictimType victim_type )
{
    if ( victim_type < SELL_VICTIM_COUNT 
        && m_victim2RDProgram[victim_type] == NULL )
    {
        RandomProgram * rdProgram = new RandomProgram( victim_type);
        if ( rdProgram)
        {
            rdProgram->InitNewData();
            m_victim2RDProgram[victim_type] = rdProgram;
            return true;
        }
    }
    return false;
}


bool SellLog::IsItemReachCountLimit( const LogicSellItem& logicSellItem , uint32 count, uint32 vipLv) const
{
    const SellProgramRecord* record = GetSellProgramRecord( logicSellItem.ProgramId());
    if ( record != NULL)
    {
        return record->IsBuyCountLimited(logicSellItem, count, vipLv);
    }
    return false;
}

SellProgramRecord* SellLog::MutableSellProgramRecord( ProgramID program )
{
    return m_programInfoMap.pfind( program );
}

const SellProgramRecord* SellLog::GetSellProgramRecord( ProgramID program ) const
{
    return m_programInfoMap.pfind( program );
}


void SellLog::SaveTo(  pb::GS2C_AllSellData& msg ) const
{
    for ( Victim2RDProgram::const_iterator it = m_victim2RDProgram.begin(); 
        it != m_victim2RDProgram.end(); ++it)
    {
        MsgTool::SaveToRepeatField( it->second, msg.mutable_random_programs());
    }

    for ( ProgramInfoMap::const_iterator it = m_programInfoMap.begin(); 
        it != m_programInfoMap.end(); ++it)
    {
        MsgTool::SaveToRepeatField( it->second, msg.mutable_sell_program_infos());
    }
}


void SellLog::LoadFrom( const pb::GS2C_AllSellData& msg )
{
	for ( int i = 0; i < msg.random_programs_size(); ++i)
    {
        const pb::GS2C_SellRandomProgram& rdProgramMsg = msg.random_programs(i);
        LoadFrom(rdProgramMsg);

    }

    for ( int j = 0; j < msg.sell_program_infos_size(); ++j)
    {
        const pb::GS2C_SellProgramInfo& programInfoMsg = msg.sell_program_infos(j);
        LoadFrom(programInfoMsg);

    }

    for ( int k = 0; k < msg.sell_records_size(); ++k)
    {
        const pb::GS2C_ItemSellRecord& recordMsg = msg.sell_records(k);
        LoadFrom(recordMsg);

    }
    
}

void SellLog::LoadFrom( const pb::GS2C_ItemSellRecord &recordMsg )
{
    const LogicSellItem* logicSellItem = sSellMgr.GetLogicSellItem( recordMsg.sell_id());
    if ( logicSellItem != NULL )
    {
        SellProgramRecord* programInfo = m_programInfoMap.pfind( logicSellItem->ProgramId());
        if ( programInfo != NULL )
        {
            programInfo->LoadOneRecord(recordMsg);
        }
    }
}

void SellLog::LoadFrom( const pb::GS2C_SellRandomProgram &rdProgramMsg )
{
    RandomProgram* rdProgram = m_victim2RDProgram.pfind( rdProgramMsg.victim_type());
    if ( rdProgram == NULL)
    {
        rdProgram = new RandomProgram( rdProgramMsg.victim_type());
    }
    if ( rdProgram != NULL)
    {
        m_victim2RDProgram[rdProgramMsg.victim_type()] = rdProgram;
        rdProgram->LoadFrom(rdProgramMsg);
    }
}

void SellLog::LoadFrom( const pb::GS2C_SellProgramInfo &programInfoMsg )
{
    SellProgramRecord* programInfo = m_programInfoMap.pfind( programInfoMsg.program_id());
    if ( programInfo == NULL)
    {
        programInfo = new SellProgramRecord( programInfoMsg.program_id());
    }
    if ( programInfo != NULL)
    {
        m_programInfoMap[ programInfoMsg.program_id()] = programInfo;  
        programInfo->LoadFrom(programInfoMsg);
    }
}



int SellLog::GetTodaySellCount( const LogicSellItem& logicSellItem ) const
{
    const SellProgramRecord* programRecord = GetSellProgramRecord(logicSellItem.ProgramId());
    if ( programRecord != NULL)
    {
        return programRecord->GetTodaySellItemCount( logicSellItem.SellId());
    }
    else
    {
        return 0;
    }
}

RandomProgram* SellLog::GetRandomProgram( VictimType victim ) const
{
    if ( sSellMgr.IsVictimRandomProgram( victim))
    {
        return m_victim2RDProgram.pfind( victim);
    }
    return NULL;
}

SellProgramRecord* SellLog::AddOrGetSellProgramRecord( ProgramID programId )
{
    SellProgramRecord* programRecord = MutableSellProgramRecord(programId);
    if ( programRecord == NULL)
    {
        programRecord = new SellProgramRecord(programId);
        if (programRecord)
        {
            m_programInfoMap[programId] = programRecord;
        }
    }
    return programRecord;
}




