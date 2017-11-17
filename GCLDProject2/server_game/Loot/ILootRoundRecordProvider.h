#pragma once

namespace pb{
    class GS2C_LootRecord;
    class GS2C_AllLootRecord;
}
class Player;
class ILootRoundRecord
{
public:
    virtual ~ILootRoundRecord(){}
    virtual int VictimType() const = 0;
    virtual int VictimId() const = 0; 
    virtual int FilterId() const = 0;
    virtual int RoundRollSuccessCount() const = 0;
    virtual int RoundRollCount() const = 0;
    virtual void AddRoundRecord( bool success, int mod, int rounder) = 0;
    virtual void SaveTo( pb::GS2C_LootRecord& msg) const = 0;
    virtual void LoadFrom( const pb::GS2C_LootRecord& msg) = 0;
    virtual void SaveToClientNeed( pb::GS2C_LootRecord& msg) const = 0;

};


class ILootRecordProvider
{
public:
    virtual ~ILootRecordProvider(){}
    virtual ILootRoundRecord* GetRoundRecord( int victim_type,int victim_id, int filterId ) const = 0;

    virtual void AddRoundRecord(Player* player,
        int victim_type,int victim_id, int filterId,
        bool success, int mod, int rounder
        ) = 0;

    virtual void SaveTo( pb::GS2C_AllLootRecord& msg ) const = 0;
    virtual void LoadFrom( const pb::GS2C_AllLootRecord& msg ) = 0;
    virtual void SaveToClientNeed( pb::GS2C_AllLootRecord& msg) const= 0;


};