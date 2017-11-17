#pragma once

#include <map>
#include "def/TypeDef.h"
#include "GameServer.pb.h"
#include "memory_buffer/MemoryPool.h"
#include "Enum.pb.h"
typedef std::map<uint32, uint32> MoneyMap;
typedef std::map<uint32, uint32> ItemMap;
class LootList
{
    DECLARE_OBJ_POOL(LootList)
	
public:
	LootList();
	~LootList();

	void Append(const LootList& other);

	bool IsEmpty() const;
	bool HasItem(uint32 itemId) const;
	uint32 CountItem(uint32 itemId) const;
	//bool IsFull() const;
	void Clear();
	void AddMoney( uint32 moneyType, uint32 money);
    void AddMoney( const CurrType2Count& count);

	void AddItem( uint32 itemId, uint32 count);
    void AddItem( const ItemID2Count& count);
	void SetItem( uint32 itemId, uint32 count);
	
	MoneyMap& MutableMoneys();
	ItemMap& MutableItem();
	const MoneyMap& Moneys() const;
	const ItemMap& Items() const;
	uint32 GetPlayerExp();
	void SetPlayerExp(const uint32 player_exp);
	uint32 GetCharExp();
	void AddPlayerExp(uint32 exp);
	void AddCharExp(uint32 exp);
	
    template< typename T>
    bool PickAllMoneyAndItems(T picker, pb::GS2C_LootList& msg, pb::ITEM_REASON reason);

	void SaveTo(pb::GS2C_LootList& msg) const;
	void SaveTo( pb::GS2C_Bag& msg ) const;
	void SaveTo( pb::GxDB_EquipInfo& msg ) const;
	void SaveTo( pb::GS2C_ItemLootList& msg ) const;
	void LoadFrom (const pb::GS2C_LootList& msg);

    // unuse
	bool PickOneItem(int& itemId, int& count);
    void PickItem(uint32 itemId);
//private:
	int Size() const;

private:
	ItemMap		m_items;
	MoneyMap    m_moneys;
	uint32      m_playerExp;
	uint32      m_charExp;
};