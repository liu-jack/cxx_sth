#include "LogicSellItem.h"
#include "datastruct/struct_npc.h"
#include "utility/Utility.h"

#include "utility/TimeParser.h"
#include "object/PlayerDataManager.h"
#include "System.h"

#include "BaseDefineMgr.h"
#include "BaseDefine.pb.h"

LogicSellItem::LogicSellItem( const DB_SellProgItems& sell_item, const DB_SellProgram& sell_program, VictimType victim)
: m_dbSellItem( sell_item)
, m_dbSellProgram( sell_program)
, m_victimType( victim)
{

}


void LogicSellItem::GetCurr2Count( uint32 todayBuyCount, uint32 count, float vipScale, CurrType2Count& type2count ) const
{
    for ( uint32 i = 1; i <= count; ++i)
    {
        m_dbSellItem.GetCurrType2Count(type2count, todayBuyCount + i);   
    }

    float scale = m_dbSellProgram.priceScale;
    if ( IsAffectPriceScale())
    {
        scale *= vipScale;
    }

    Utility::MultiplyValue( type2count, scale, ::floorf);
}


VictimType LogicSellItem::GetVictimType() const
{
    return m_victimType;
}

bool LogicSellItem::IsBuyCountLimited( uint32 toBuyCount, uint32 vipLv) const
{
    using namespace pb;

    uint32 limitCount = m_dbSellItem.buy_limit;
    /*static const uint32 AP_SELL_ID = GET_BASE_DEF_UINT( BD_VIP_SELL_AP_ID);
    static const uint32 SP_SELL_ID = GET_BASE_DEF_UINT( BD_VIP_SELL_SP_ID);
    static const uint32 COIN_SELL_ID = GET_BASE_DEF_UINT( BD_VIP_SELL_COIN_ID);
    static const uint32 XP_SELL_ID = GET_BASE_DEF_UINT( BD_VIP_SELL_XP_ID);
    static const uint32 EQUIP_SELL_ID = GET_BASE_DEF_UINT( BD_VIP_SELL_EQUIP_ID);
    static const uint32 ACC_SELL_ID = GET_BASE_DEF_UINT( BD_VIP_SELL_ACC_ID);

    if ( SellId() == AP_SELL_ID){
        limitCount = sPlayerDataMgr.GetVipBuyApMax( vipLv);
    }else if ( SellId() == SP_SELL_ID ){
        limitCount = sPlayerDataMgr.GetVipBuySpMax( vipLv);
    }else if ( SellId() == COIN_SELL_ID){
        limitCount = sPlayerDataMgr.GetVipBuyCoinLimit(vipLv);   
    }else if ( SellId() == XP_SELL_ID){
        limitCount = sPlayerDataMgr.GetVipBuyXpLimit(vipLv);   
    }else if ( SellId() == EQUIP_SELL_ID){
        limitCount = sPlayerDataMgr.GetVipBuyEquipLimit(vipLv);   
    }else if ( SellId() == ACC_SELL_ID){
        limitCount = sPlayerDataMgr.GetVipBuyAccLimit(vipLv);   
    }*/


    return limitCount != 0 && limitCount < toBuyCount;
}

uint32 LogicSellItem::BulkCount() const
{
    return m_dbSellItem.bulk_count;
}

uint32 LogicSellItem::ProgramId() const
{
    return m_dbSellItem.programID;
}

uint32 LogicSellItem::SellId() const
{
    return m_dbSellItem.id;
}

uint32 LogicSellItem::ItemProtoId() const
{
    return m_dbSellItem.itemID;
}

uint32 LogicSellItem::CurrRaiseSellCount() const
{
    return m_dbSellItem.curr_raise_count;
}

float LogicSellItem::CurrRaiseRatio() const
{
    return m_dbSellItem.curr_raise_ratio;
}

bool LogicSellItem::IsAffectPriceScale() const
{
    return Utility::TestBit( m_dbSellItem.vip_option, VIP_SCALE_AFFECT);
}

bool LogicSellItem::IsAffectCritical() const
{
    return Utility::TestBit( m_dbSellItem.vip_option, VIP_CRITICAL_AFFECT);
}

uint32 LogicSellItem::VipCritial( uint32 count, float vipCriticalRate ) const
{
    uint32 hitCritical = 0;
    if ( IsAffectCritical())
    {
        for ( uint32 i = 0; i < count; ++i )
        {
            if ( System::Randf(0.0,1.0) < vipCriticalRate)
            {
                ++hitCritical;
            }
        }
    } 
    return hitCritical;
}

bool LogicSellItem::IsProgramActiveWhen( time_t t ) const
{
    if ( m_dbSellProgram.beginTime == 0 && m_dbSellProgram.endTime == 0)
        return true;

    if ( m_dbSellProgram.beginTime - get_gmt_offset() <= t
        && m_dbSellProgram.endTime - get_gmt_offset() > t)
    {
        return true;
    }
    
    return false;        
}

