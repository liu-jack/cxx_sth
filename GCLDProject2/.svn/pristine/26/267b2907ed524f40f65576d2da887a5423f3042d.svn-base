#pragma once
#include "LootDefine.h"
#include "ptr_container/PtrMap.h"
#include "ILootRoundRecordProvider.h"

class Player;

class LootRoundRecord 
    : public ILootRoundRecord
{
    friend class LootRoundRecordGroup;
public:
    LootRoundRecord( const LootIdType& lootIdType);
    LootRoundRecord();
    int VictimType() const   { return m_lootProto.VictimType();}
    int VictimId() const     { return m_lootProto.VictimId();} 
    int FilterId() const     { return m_lootProto.FilterId();}

    int RoundRollSuccessCount() const    { return m_rollSuccessCount;}
    int RoundRollCount() const           { return m_rollCount;}
    void AddRoundRecord( bool success, int mod, int rounder);

    void SaveToClientNeed( pb::GS2C_LootRecord& msg) const;
    void SaveTo( pb::GS2C_LootRecord& msg) const;
    void LoadFrom( const pb::GS2C_LootRecord& msg);

private:
    
    LootIdType  m_lootProto;
    int         m_rollSuccessCount;
    int         m_rollCount;

};


class LootRoundRecordGroup 
    : public ILootRecordProvider
{
    typedef PtrMap<uint64, LootRoundRecord> LootRecords; 
public:
    LootRoundRecordGroup();
    ILootRoundRecord* GetRoundRecord( int victim_type, int victim_id, int filterId ) const;

    void AddRoundRecord(Player*  player,
        int victim_type,int victim_id, int filterId,
        bool success, int mod, int rounder
        );

    void SaveToClientNeed( pb::GS2C_AllLootRecord& msg) const;
    void SaveTo( pb::GS2C_AllLootRecord& msg) const;
    void LoadFrom( const pb::GS2C_AllLootRecord& msg );
private:
    ILootRoundRecord* GetRoundRecord( const LootIdType& lootId) const;

    mutable LootRecords     m_records;

};