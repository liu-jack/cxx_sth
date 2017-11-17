#pragma once
#include "def/TypeDef.h"

class ILootRecordProvider;

struct LootParam
{
    LootParam() 
        : victim_type(0), victim_id(0)
        , victim_lv(0), player_vip_lv(0)
        , loot_correct_count(0)
		, is_count_loot(false)
        , provider(NULL){}

    int victim_type;
    int victim_id;
    int victim_lv;

    int player_vip_lv;
    int loot_correct_count;
	bool is_count_loot;//是否将物品Id和Count分开掉落，此时物品掉落数量为1，Count为其它意义，如当level掉落

    ILootRecordProvider * provider;

};



union LootIdType
{
    friend bool inline operator < ( const LootIdType &lhs, const LootIdType &rhs );
    friend bool inline operator == ( const LootIdType &lhs, const LootIdType &rhs );
public:
    ~LootIdType( void ) {}
    LootIdType( void ) {}
    LootIdType( const uint64 &type ) : m_lootId(type) {}
    explicit LootIdType( uint32 victimType, uint32 victimId, uint32 filterId = 0)
        : m_victimType( victimType), m_filterId( filterId), m_victimId( victimId){}

    LootIdType( const LootIdType& other) : m_lootId( other.m_lootId){}

public:
    inline void SetUint64( uint64 u)    {   m_lootId = u;           }
    inline uint64 GetUint64() const     {   return m_lootId ;       }
    inline uint32 VictimType() const       {   return m_victimType;    }
    inline uint32 VictimId() const         {   return m_victimId;      } 
    inline uint32 FilterId() const         {   return m_filterId;      }

private:
    uint64 m_lootId ;
    struct
    {
        uint16 m_victimType  :16; // 低
        uint16 m_filterId    :16;  // 
        uint32 m_victimId    :32;  // 

    };
};

bool inline operator < ( const LootIdType &lhs, const LootIdType &rhs )
{
    return lhs.m_lootId < rhs.m_lootId;
}
bool operator == ( const LootIdType &lhs, const LootIdType &rhs )
{
    return lhs.m_lootId == rhs.m_lootId;
}
