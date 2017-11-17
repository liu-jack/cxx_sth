#pragma once

#include "SellDefine.h"
#include "ptr_container/PtrMap.h"

class RandomProgram;
class SellProgramRecord;
class SellLog;
class LogicSellItem;

namespace pb{
    class GS2C_ItemSellRecord;
    class GS2C_SellProgramInfo;
    class GS2C_SellRandomProgram;
    class GS2C_AllSellData;
    class GS2C_SellRecordList;
} 


class RandomProgram
{
public:
    RandomProgram( VictimType victim);
    ProgramID GetProgramId() const;

    bool LazyUpdate();
    bool TryManualRefresh( uint32 vipLv);

    void SaveTo( pb::GS2C_SellRandomProgram& msg) const;
    void LoadFrom( const pb::GS2C_SellRandomProgram& msg);
    void InitNewData();

public:
    inline bool NeedGetNewRandomProgram() const;
    inline uint32 GetTodayManualRefreshCount() const;
    inline uint32 GetAutoRefeshSecond() const;
    inline void GenRandomProgram() const;

private:
    VictimType          m_victimType;
    mutable ProgramID   m_randomProgramId;
    uint32              m_todayManualRefreshCount;
    mutable time_t      m_autoRefreshTime;
    time_t              m_lastManualRefreshTime;
};

class SellProgramRecord
{
    typedef std::map<SellId, uint32> SellId2Count;
public:
    enum OP_RESULT
    {
        OP_FALSE,
        OP_AFFECT_ONE,
        OP_AFFECT_ALL,
    };

    SellProgramRecord( ProgramID  programId);
	~SellProgramRecord( void );

    uint32    GetTodaySellItemCount( SellId sell_id) const;
    bool      IsBuyCountLimited(const LogicSellItem& logicSellItem, uint32 count, uint32 vipLv) const;

    OP_RESULT TryAddSellItemCount( SellId sell_id , uint32 count, uint32 vipLv); 
    OP_RESULT TryAddSellItemCount( const LogicSellItem& logicSellItem, uint32 count, uint32 vipLv);
    void      ResetSellRecord();
    void      UpdateRecordValidTime();

    void SaveTo( pb::GS2C_SellProgramInfo& msg, ProgramID sell_id = 0, bool isSendToClient = true ) const;


    void LoadFrom( const pb::GS2C_SellProgramInfo& msg);
    void LoadOneRecord( const pb::GS2C_ItemSellRecord& msg);


private:
    bool IsRecordAvalid() const;
    void SaveRecordToMsg( ::pb::GS2C_SellRecordList* recordsMsg, SellId2Count::const_iterator it ) const;

private:
    ProgramID       m_programId;
    time_t          m_recordValidTime;

    SellId2Count    m_sellId2Count;  
};


class SellLog 
{
    typedef PtrMap<VictimType, RandomProgram> Victim2RDProgram;
    typedef PtrMap<ProgramID, SellProgramRecord> ProgramInfoMap;
    typedef int ErrCode;
public:
    SellLog();
    ~SellLog();

    bool InitVictimRDProgram( VictimType victim_type);

    int GetTodaySellCount(const LogicSellItem& logicSellItem) const;

    bool IsItemReachCountLimit( const LogicSellItem& logicSellItem, uint32 count, uint32 vipLv) const;

    void SaveTo( pb::GS2C_AllSellData& msg) const;
    
    void LoadFrom( const pb::GS2C_AllSellData& msg ); 
    void LoadFrom( const pb::GS2C_SellRandomProgram &rdProgramMsg );
    void LoadFrom( const pb::GS2C_ItemSellRecord &recordMsg );    
    void LoadFrom( const pb::GS2C_SellProgramInfo &programInfoMsg );


    RandomProgram* GetRandomProgram( VictimType victim) const; 
    
    SellProgramRecord* AddOrGetSellProgramRecord(ProgramID program);

    const SellProgramRecord* GetSellProgramRecord(ProgramID program) const;
    SellProgramRecord* MutableSellProgramRecord(ProgramID program);


protected:
    Victim2RDProgram    m_victim2RDProgram;
    ProgramInfoMap      m_programInfoMap;

};


