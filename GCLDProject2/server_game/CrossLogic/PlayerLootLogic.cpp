#include "PlayerLootLogic.h"

#include "Loot/LootList.h"
#include "Loot/LootManager.h"
#include "Loot/LootFilterGroup.h"
#include "item/LogicItemProto.h"
#include "item/ItemManager.h"

#include "object/Player.h"

#include "CommomDef.pb.h"
#include "character/CharacterStorage.h"
#include "character/Character.h"

template<>
bool LootList::PickAllMoneyAndItems( Player* player, pb::GS2C_LootList& msg , pb::ITEM_REASON reason)
{
    for (MoneyMap::const_iterator it = m_moneys.begin(); it != m_moneys.end(); ++it)
    {
        player->AddCurrency( reason, it->first, it->second);
        pb::PairValue* msgItem = msg.add_curency_type_count();
        msgItem->set_first(it->first);
        msgItem->set_second(it->second);
    }
    m_moneys.clear();

    for (ItemMap::iterator it = m_items.begin(); it != m_items.end(); ++it )
    {
        int itemId = it->first;
		int itemCount = it->second;
		const LogicItemProto* itemproto = sItemMgr.Find( itemId );
		if (itemproto && (itemproto->IsEquip() || itemproto->IsBoxImm()))//对装备不做处理
		{
			continue;
		}
		else
		{
			if ( player->CreateItem(  reason, itemId, itemCount ) != pb::ErrCommonSuccess) 
			{
				WLOG( "Player Pick item Error ;item id : %d", itemId);
				continue;
			}
		}
        pb::PairValue* itemMsg = msg.add_item_id_count();
        itemMsg->set_first(itemId);
        itemMsg->set_second(itemCount);
    }
    m_items.clear();

	player->AddXP(m_playerExp);
	msg.set_player_exp(m_playerExp);
	m_playerExp = 0;

	struct _Temp_Hero_Func //辅助函数对象
	{
		uint32 _exp;

		_Temp_Hero_Func(uint32 n) : _exp(n) {}
		bool operator()(Character* p)
		{
			p->TryAddXP(_exp);
			return false;
		}
	};
	_Temp_Hero_Func objFunc(m_charExp);
	player->m_characterStorage->ForEachBattleCharacter(objFunc);
	msg.set_char_exp(m_charExp);
	m_charExp = 0;

    return msg.curency_type_count_size() > 0 
		|| msg.item_id_count_size() > 0 
		|| msg.player_exp() > 0 
		|| msg.char_exp() > 0;
}



bool PlayerLootLogic::PlayerLoot( Player& player, pb::GS2C_LootList& lootListMsg, LootParam& lootParam , pb::ITEM_REASON reason)
{
    LootFilterGroup* pGroup = sLootMgr.FindLootFilterGroup( lootParam.victim_type, lootParam.victim_id);
    if (pGroup)
    {
        std::auto_ptr<LootList> lootList(pGroup->GenerateLoot(&player,lootParam));

        if (lootList.get())
        {
            lootList->PickAllMoneyAndItems( &player,lootListMsg, reason);
            return true;
        }
    }
    return false;
}
