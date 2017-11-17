#pragma once
#include "SellDefine.h"

struct DB_SellProgItems;
struct DB_SellProgram;  

class LogicSellItem
{
    enum{
        VIP_SCALE_AFFECT = 0,
        VIP_CRITICAL_AFFECT = 1,
    };
public:
    LogicSellItem(
        const DB_SellProgItems& sell_item, 
        const DB_SellProgram& sell_program,
        VictimType victim
        );

    const DB_SellProgItems& Proto() const { return m_dbSellItem; }
    bool IsBuyCountLimited( uint32 toBuyCount, uint32 vipLv) const;
    uint32 BulkCount() const;
    uint32 ProgramId() const;
    uint32 SellId() const;
    uint32 ItemProtoId() const;
    uint32 CurrRaiseSellCount() const;
    float CurrRaiseRatio() const;
    bool IsAffectPriceScale() const;
    bool IsAffectCritical() const;
    uint32 VipCritial( uint32 count, float vipCriticalRate) const;

    bool IsProgramActiveWhen( time_t t) const;



    void GetCurr2Count( uint32 todayBuyCount, uint32 count, float vipScale, CurrType2Count& type2count) const;
    VictimType GetVictimType() const;



private:
    const DB_SellProgItems& m_dbSellItem;
    const DB_SellProgram& m_dbSellProgram;
    VictimType  m_victimType;
};
