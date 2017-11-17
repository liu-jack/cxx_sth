#include "LootRoundRecordGroup.h"
#include "LootManager.h"
#include "FixFakeRandom/FixFakeRandomBitSet.h"
#include "LootFilter.h"
#ifdef _MMO_SERVER_
#include "Lottery/LotteryDefine.h"
#endif
#include "object/Player.h"


#include "server_client/DbProxyServerClient.h"
#include "ServerOpcode.pb.h"
#include "Item.pb.h"
#include "Opcode.pb.h"




//////////////////////////////////////////////////////////////////////////

LootRoundRecord::LootRoundRecord( const LootIdType& lootIdType )
: m_lootProto( lootIdType)
, m_rollSuccessCount( 0)
, m_rollCount( 0)
{
    
}

LootRoundRecord::LootRoundRecord()
: m_rollSuccessCount( 0)
, m_rollCount( 0)
{

}

void LootRoundRecord::AddRoundRecord( bool success, int mod, int rounder )
{   
    ++m_rollCount;
    if ( success)
    {
        ++m_rollSuccessCount;
    }

    if ( m_rollCount == rounder)
    {
        m_rollCount = 0;
        m_rollSuccessCount = 0;
    }
}

void LootRoundRecord::SaveTo( pb::GS2C_LootRecord& msg ) const
{
    msg.set_loot_type_id( m_lootProto.GetUint64());
    msg.set_roll_count( m_rollCount);
    msg.set_roll_success_count( m_rollSuccessCount);
}

void LootRoundRecord::LoadFrom( const pb::GS2C_LootRecord& msg )
{
    m_lootProto.SetUint64( msg.loot_type_id());
    m_rollCount = msg.roll_count();
    m_rollSuccessCount = msg.roll_success_count();
}

void LootRoundRecord::SaveToClientNeed( pb::GS2C_LootRecord& msg ) const
{
    const LootFilter* filter = sLootMgr.FindLootFilter( m_lootProto.FilterId());
    if ( filter)
    {
        uint32 remainTime = 0;
        sFixFakeRD.TryGetRemaintoHit(filter->RoundCorrectCount(), filter->RoundCorrectMod(), m_rollCount, remainTime);
        msg.set_loot_type_id( m_lootProto.VictimId());
        msg.set_roll_count( remainTime);
    }
}


//////////////////////////////////////////////////////////////////////////

LootRoundRecordGroup::LootRoundRecordGroup( )
{
    
}


ILootRoundRecord* LootRoundRecordGroup::GetRoundRecord( int victim_type, int victim_id, int filterId ) const
{
    LootIdType id( victim_type, victim_id, filterId);
    return GetRoundRecord(id);   
}

ILootRoundRecord* LootRoundRecordGroup::GetRoundRecord( const LootIdType& lootId ) const
{
    if ( sLootMgr.NeedRollRecord( lootId.FilterId()))
    {
        LootRecords::iterator it = m_records.find(lootId.GetUint64());
        if (it != m_records.end())
        {
            return it->second;
        }
        else
        {
            LootRoundRecord* roundRecord = new LootRoundRecord(lootId);
            if (roundRecord)
            {
                m_records[ lootId.GetUint64()] = roundRecord;
            }
            return roundRecord;
        }
    }
    return NULL;
}

void LootRoundRecordGroup::AddRoundRecord(Player*  player,
    int victim_type,int victim_id, int filterId,
    bool success, int mod, int rounder )
{
#ifdef _MMO_SERVER_
    LootIdType id( victim_type, victim_id, filterId);

    ILootRoundRecord* record = GetRoundRecord(id);
    if ( record == NULL)
        return;

    record->AddRoundRecord( success, mod, rounder);

    if ( player)
    {
        pb::GS2C_LootRecord msg;
        record->SaveTo(msg);
        sDbProxyClient.SendPlayerPack( *player,pb::SG2D_UPDATE_LOOT_RECORD, msg);
        if ( victim_type == LOOTTYPE_LOTTERY && victim_id < LOTTERY_NORMAL_MAX)
        {
            record->SaveToClientNeed(msg);
            player->Send( pb::SMSG_UPDATE_LOOT_ROUND_RECORD, msg);
        }
        
    }
#endif
}

void LootRoundRecordGroup::SaveTo( pb::GS2C_AllLootRecord& msg ) const
{
    for ( LootRecords::const_iterator it = m_records.begin(); it != m_records.end() ; ++it)
    {
        MsgTool::SaveToRepeatField( it->second, msg.mutable_records());
    }
}

void LootRoundRecordGroup::LoadFrom( const pb::GS2C_AllLootRecord& msg )
{
    for ( int i = 0 ; i < msg.records_size(); ++i )
    {
        const pb::GS2C_LootRecord& recordMsg = msg.records(i);
        LootRoundRecord* record = new LootRoundRecord();
        if ( record)
        {
            record->LoadFrom( recordMsg);
            m_records[ recordMsg.loot_type_id()] = record;
        }
    }
}

void LootRoundRecordGroup::SaveToClientNeed( pb::GS2C_AllLootRecord& msg ) const
{
#ifdef _MMO_SERVER_
    for ( uint32 lotteryId = LOTTERY_SILVER; lotteryId < LOTTERY_NORMAL_MAX ; ++lotteryId)
    {
        uint32 filterId = sLootMgr.GetLotteryFixCorrectFilter(lotteryId);
        if ( filterId)
        {
            ILootRoundRecord* lootRecord = GetRoundRecord(LOOTTYPE_LOTTERY, lotteryId, filterId);
            if ( lootRecord)
            {
                pb::GS2C_LootRecord* recordMsg = msg.add_records();
                if ( recordMsg)
                {
                    lootRecord->SaveToClientNeed(*recordMsg);    
                }       
            }
        }
    }
#endif
}


