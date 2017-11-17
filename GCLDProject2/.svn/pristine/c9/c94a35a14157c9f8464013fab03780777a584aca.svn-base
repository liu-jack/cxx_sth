#pragma once
#include "SellDefine.h"
#include "SimpleSingleton.h"
#include "LogicSellItem.h"
#include "ptr_container/PtrHashMap.h"
#include "boost/unordered/unordered_map.hpp"

class SellManager : public SimpleSingleton< SellManager>
{
    friend class SimpleSingleton< SellManager>;
public:
    typedef std::set< ProgramID> ProgramIdSet;
    typedef PtrHashMap< SellId, LogicSellItem> LogicSellItemMap;
    typedef std::vector< const LogicSellItem*> SellItems;
    typedef std::map< ProgramID, SellItems> SellItemsInProgram;
    typedef boost::unordered_map< ProgramID, VictimType> Program2Victim;
public:
    void Init();
    static bool IsVictimRandomProgram( VictimType type);
   // static uint32 GetDailyManualRefreshMaxCount( VictimType type, uint32 vipLv);
    static uint32 GetVictimAvalidPeriod( VictimType type); 
    uint32 GetVictimTypeOfProgram( ProgramID program) const;


    const LogicSellItem* GetLogicSellItem( SellId sell_id) const;
    
    ProgramID GetRandomProgramId( VictimType type, ProgramID exceptId = 0) const;

    const SellItems* GetSellItemsInProgram( ProgramID programid) const;
private:
    SellManager();
    ~SellManager();
    ProgramIdSet        m_programIdSet[SELL_VICTIM_COUNT];
    LogicSellItemMap    m_logicSellItemMap;
    SellItemsInProgram  m_sellItemsInProgram;
    Program2Victim      m_program2Victim;

};


#define sSellMgr SellManager::Instance()