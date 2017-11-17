#include "SellManager.h"

#include "System.h"
#include "mysqldata.h"
#include "DbTool.h"
#include "LogicSellItem.h"
#include "object/PlayerDataManager.h"
#include "BaseDefineMgr.h"
#include "BaseDefine.pb.h"

#include "boost/typeof/typeof.hpp"
using namespace pb;


SellManager::SellManager()
{
    
}

SellManager::~SellManager()
{

}

void SellManager::Init()
{
    FOREACH_DB_ITEM( proto, DB_SellProto)
    {
        if ( proto->victim_type < SELL_VICTIM_COUNT)
        {
            m_programIdSet[ proto->victim_type].insert( proto->programID);
        }
        m_program2Victim[ proto->programID] = proto->victim_type;
    }

    FOREACH_DB_ITEM( sell_item, DB_SellProgItems)
    {
        const DB_SellProgram* sell_program = DbTool::Get<DB_SellProgram>( sell_item->programID );
        if ( sell_program)
        {
            Program2Victim::iterator it = m_program2Victim.find( sell_item->programID);
            if (it != m_program2Victim.end())
            {
                VictimType victim = it->second;
                LogicSellItem* logicSellItem = new LogicSellItem(*sell_item,*sell_program, victim);
                if ( logicSellItem)
                {
                    m_logicSellItemMap[ sell_item->id] = logicSellItem;
                    m_sellItemsInProgram[ sell_item->programID].push_back( logicSellItem);

                }
            }
        }
    }
}

ProgramID SellManager::GetRandomProgramId( VictimType type, ProgramID exceptId /*= 0*/ )const
{
    if ( type >= SELL_VICTIM_COUNT)
    {
        return 0;
    }

    std::vector< ProgramID> vec;
    const ProgramIdSet& programIds = m_programIdSet[type];
    vec.reserve( programIds.size());
    for ( ProgramIdSet::const_iterator it = programIds.begin(); it !=  programIds.end(); ++it)
    {
        if ( *it != exceptId)
        {
            vec.push_back( *it);
        }
    }
    uint32 index = (uint32)System::Rand(0, vec.size());
    if ( index < vec.size())
    {
        return vec[index]; 
    }else
    {
        return 0;
    }
       
}

bool SellManager::IsVictimRandomProgram( VictimType type )
{
    switch( type)
    {
    case SELL_VICTIM_HERO   :
    case SELL_VICTIM_AWAKE  :
        return true;
    default:
        return false;
    }
}

// TODO : !!!!!!! DBBaseDefine
//uint32 SellManager::GetDailyManualRefreshMaxCount( VictimType type , uint32 vipLv )
//{
//    switch( type)
//    {
//    case SELL_VICTIM_HERO   :
//        return sPlayerDataMgr.GetVipResetShopHeroLimit(vipLv);        
//    case SELL_VICTIM_AWAKE  :
//        return sPlayerDataMgr.GetVipResetShopAwakeLimit(vipLv);
//    default:
//        return 0;
//    }
//}

const LogicSellItem* SellManager::GetLogicSellItem( SellId sell_id ) const
{
    return m_logicSellItemMap.pfind( sell_id);
}

const SellManager::SellItems* SellManager::GetSellItemsInProgram( ProgramID programid ) const
{
    BOOST_AUTO( it , m_sellItemsInProgram.find(programid));
    if ( it != m_sellItemsInProgram.end())
    {
        return &(it->second);
    }
    return NULL;
}

uint32 SellManager::GetVictimAvalidPeriod( VictimType type )
{
    static uint32 seconds[] = 
    {
        GET_BASE_DEF_UINT( pb::BD_SELL_VICTIM_BASIC_AUTO_REFRESH_MINUTE ) * 60,            // SELL_VICTIM_NORMAL 
        GET_BASE_DEF_UINT( pb::BD_SELL_VICTIM_HERO_AUTO_REFRESH_MINUTE) * 60,              // SELL_VICTIM_HERO   
        GET_BASE_DEF_UINT( pb::BD_SELL_VICTIM_EQUIP_AUTO_REFRESH_MINUTE ) * 60,            // SELL_VICTIM_EQUIP  
        GET_BASE_DEF_UINT( pb::BD_SELL_VICTIM_HORNOR_AUTO_REFRESH_MINUTE ) * 60,           // SELL_VICTIM_HONOR    
        GET_BASE_DEF_UINT( pb::BD_SELL_VICTIM_BOUNTY_AUTO_REFRESH_MINUTE ) * 60,           // SELL_VICTIM_BOUNTY   
        GET_BASE_DEF_UINT( pb::BD_SELL_VICTIM_GUILD_AUTO_REFRESH_MINUTE ) * 60,            // SELL_VICTIM_GUILD  
        GET_BASE_DEF_UINT( pb::BD_SELL_VICTIM_AWAKE_AUTO_REFRESH_MINUTE) * 60,             // SELL_VICTIM_AWAKE  
        GET_BASE_DEF_UINT( pb::BD_SELL_VICTIM_PROMO_AUTO_REFRESH_MINUTE ) * 60,            // SELL_VICTIM_PROMO 
        GET_BASE_DEF_UINT( pb::BD_SELL_VICTIM_VIP_GIFT_ONCE_AUTO_REFRESH_MINUTE ) * 60,    // SELL_VICTIM_VIP_GIFT_ONCE  
        GET_BASE_DEF_UINT( pb::BD_SELL_VICTIM_VIP_GIFT_CYCLE_AUTO_REFRESH_MINUTE) * 60     // SELL_VICTIM_VIP_GIFT_CYCLE 
    };

    if ( type < sizeof( seconds) / sizeof(uint32))
    {
        return seconds[ type];
    }
    else
    {
        ASSERT( false);
        return 0;
    }
}

uint32 SellManager::GetVictimTypeOfProgram( ProgramID program ) const
{
    Program2Victim::const_iterator it = m_program2Victim.find( program);
    if ( it != m_program2Victim.end())
    {
        return it->second;
    }
    else
    {
        ASSERT( false);
        return 0;
    }
}



