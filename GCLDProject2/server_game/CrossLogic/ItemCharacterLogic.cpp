#include <boost/typeof/typeof.hpp>

#include "ItemCharacterLogic.h"
#include "PlayerItemLogic.h"

#include "server_client/DbProxyServerClient.h"

#include "character/CharacterStorage.h"
#include "character/Character.h"
#include "character/CharacterProto.h"

#include "item/ItemArray.h"
#include "item/Item.h"
#include "item/ItemEquip.h"

#include "object/Player.h"

#include "quest/behavior/PlayerBehaviorCounter.h"
//#include "Social/SocialHandler.h"
#include "Opcode.pb.h"
#include "ServerOpcode.pb.h"
#include "quest/QuestLogger.h"
#ifdef _MMO_SERVER_
#include "fuben/FubenLog.h"
#endif



using namespace pb;


bool ItemCharacterLogic::SetBattleArray( Player* player, const CharacterIds& newBattleArray )
{
    CharacterStorage* charStorage = player->m_characterStorage.get();
    if ( charStorage == NULL )
        return false;

    std::set< CharacterId > newIds, oldIds;
    vector< CharacterId> addId;
    vector< CharacterId> removeId;

    const CharacterIds& oldBattleArray = charStorage->GetBattleArray();

    for (CharacterIds::const_iterator it = newBattleArray.begin(); it != newBattleArray.end(); ++it)
    {
        uint32 charaId = *it;

        const Character* ch = charStorage->GetCharacter( charaId);

        if ( charaId != 0 ) 
        {
            if ( ch == NULL)
            {
                NLOG( "CharacterStorage::SetBattleArray, do not have character, set battle arry fail" );
                return false;
            }

            if ( newIds.find(charaId) != newIds.end())
            {
                return false;
            }

            if ( !ch->Proto().IsCharTypeHero())
            {
                return false;
            }
			newIds.insert(charaId); 
        }
    }

    for (BOOST_AUTO(it, oldBattleArray.begin()); it != oldBattleArray.end(); ++it)
    {
        if ( *it != 0)  oldIds.insert(*it);  
    }

	//删除的武将需要移除装备
    Get_InA_NotInB( oldIds, newIds, removeId);
	//新招募的武将 宝器添加的血量不能加上去
	Get_InA_NotInB( newIds, oldIds, addId);

	int nRemove = (int)removeId.size();
	int nadd = (int)addId.size();
	if (nRemove >= nadd)
	{
		for (int i=0; i< nRemove; i++)
		{
			if (i < nadd)
			{
				QuikChangeEquipsOwnerToNewCharacter(player,removeId[i],addId[i]);
			}
			else
			{
				ClearEquipFromCharacter(player,removeId[i]);
			}
		}
	}
	else
	{
		for ( BOOST_AUTO(it, removeId.begin()); it != removeId.end(); ++it)
		{
			ClearEquipFromCharacter(player, *it);
		} 
	}
	
    //AutomaticEquipToCharacters(player,addId);

    player->m_questLogger->UpdateQuestCategory( player,QC1_CHAR);


    charStorage->SetBattleArray( player,newBattleArray );

    ItemCharacterLogic::ReCalculateBattleAttr( player);

#ifdef _MMO_SERVER_
	if(!player->m_questLogger->GetQuestFinished(GET_BASE_DEF_INT( pb::BD_AUTO_DRAFT_FOR_END_MAIN_QUEST))
		&& (int)player->m_fubenLog->m_curStageID <= GET_BASE_DEF_INT( pb::BD_AUTO_DRAFT_FOR_END_STAGE_LEVEL))
	{
		for (CharacterIds::const_iterator it = newBattleArray.begin(); it != newBattleArray.end(); ++it)
		{
			uint32 charaId = *it;

			Character* ch = charStorage->MutableCharacter( charaId);

			if ( ch ) 
			{
				ch->RecoverFullSoldier();
				player->m_characterStorage->SetModifiedID(ch->GetID());
			}
		}
	}
#endif
    if (player)
    {
        pb::GS2C_BattleCharacter msg;
        charStorage->SaveBattleArrayToMsg( msg);
        sDbProxyClient.SendPlayerPack( *player, pb::SG2D_UPDATE_BATTLE_CHARS, msg );
        player->Send(pb::SMSG_SET_BATTLE_CHARACTER, msg);

        //sSocialHandler.OnPlayerAttrChange(pb::PLAYER_VALUE_TYPE_32_BIT_FIELD,player->GetGuid(),player->GetLeaderProtoId(),pb::PLAYER_FIELD_HEAD_ICON_ID);
    }

    return true;
}

void ItemCharacterLogic::AutomaticEquipToCharacters( Player* player, std::vector< uint32>& charIdsToEquip )
{
    std::vector<ItemEquip*> equipVec;
    std::list<ItemEquip*> equipList;

    player->m_bag->CollectFreeEquipAndSort(equipVec);
    std::copy(equipVec.begin(), equipVec.end(), std::back_inserter(equipList));

    for ( BOOST_AUTO(iter, charIdsToEquip.begin()); iter != charIdsToEquip.end(); ++iter)
    {
        Character* character = player->m_characterStorage->MutableCharacter( *iter);
        if (character)
        {
            AutomaticEquipToCharacter(player, character, equipList);
        }
    }
}

void ItemCharacterLogic::AutomaticEquipToCharacter( Player* player, Character* character, std::list<ItemEquip*>& equip_group )
{
    BOOST_AUTO(iter, equip_group.begin());
    while( true)
    {
        if ( character->IsAllSlotEquip())
            break;
        if ( iter == equip_group.end())
            break;
        
        if ( EquipItemToCharacter(player, *iter, character) == pb::ErrCommonSuccess)
        {
            iter =  equip_group.erase(iter); 
        }
        else
        {
            ++iter;
        }
    }
}

ItemCharacterLogic::ErrCode ItemCharacterLogic::ClearEquipFromCharacter( Player* player, int charId )
{
    ItemArray* bag = player->m_bag.get();
    if ( bag == NULL)
        return pb::ErrCommonFail;

    CharacterStorage* charStorage = player->m_characterStorage.get();
    if ( bag == NULL)
        return pb::ErrCommonFail;

    Character* character = charStorage->MutableCharacter( charId);
    if ( character == NULL)
        return pb::ErrCommonFail;

	for ( int i = pb::EQUIP_SLOT_WEAPON; i < pb::EQUIP_SLOT_COUNT; ++i)
    {
        int equipId = character->GetSlotEquip(i);
        if (equipId > 0)
        {
            ItemEquip* rawEquip = NULL;
            if ( PlayerItemLogic::TryGetItemEquip(bag, equipId, rawEquip))
            {
				character->SetSlotEquip( i, 0); 
				rawEquip->DettachFromCharacter( character->GetID()); 
				bag->SetModifiedID( rawEquip->BagSlot());
            }
        }
    }

    return pb::ErrCommonFail;
}

//玩家，装备，武将，槽位..
ItemCharacterLogic::ErrCode ItemCharacterLogic::_EquipItemToCharacter( Player* player, ItemEquip* equip, Character* character, uint32 equipSlot, bool replaceRaw /*= true*/)
{
    ItemArray* bag = player->m_bag.get();
    if ( bag == NULL)
        return pb::ErrCommonFail;

	if (equip->IsSold())
	{
		return pb::ErrInvalidItemId;
	}

    CharacterStorage *charStore = player->m_characterStorage.get();
    if ( charStore == NULL)
        return pb::ErrCommonFail;
    //////////////////////////////////////////////////////////////////////////
    // 脱掉 原有装备
    int equipBagSolt = character->GetSlotEquip(equipSlot);

    // 如果此操作不要脱掉原有装备， 且已经装备。 返回错误。
    if ( replaceRaw == false && equipBagSolt > 0)
        return pb::ErrCommonFail;

	//bool removeItemIsSuit =  false;
    ItemEquip* rawEquip = NULL;
    if ( PlayerItemLogic::TryGetItemEquip(bag, equipBagSolt, rawEquip))
    {
		_RemoveEquipFromCharacter( character, bag, rawEquip, equipSlot);
    }
    //////////////////////////////////////////////////////////////////////////

    if ( equip == NULL)
        return pb::ErrCommonSuccess;    // 没有新装备要穿, 即只需脱掉装备


	const LogicItemProto* logic_item_proto = equip->Proto();
	if (logic_item_proto == NULL)
		return pb::ErrCommonFail;

	if (logic_item_proto->IsSuit())		//是套装
	{
		if ( equip->EquiperId() != 0)	//已经装备到武将身上，则脱下来
		{
			Character* rawEquiper = charStore->MutableCharacter( equip->EquiperId());
			MMO_ASSERT( rawEquiper) ;
			if ( rawEquiper)
			{
				int curEquipSlot = rawEquiper->GetEquipSlot(equip->BagSlot());
				if (curEquipSlot >= 0 )
				{
					_RemoveEquipFromCharacter( rawEquiper, bag, equip, curEquipSlot);
				}
				else
				{
					equip->DettachFromCharacter( equip->EquiperId());
					bag->SetModifiedID( equip->BagSlot());
				}
			}
		}
		for (uint32  curSolt = 0; curSolt < (uint32)pb::EQUIP_SLOT_HOLY; curSolt++)		//对于每一个槽位
		{
			int equipBagSolt = character->GetSlotEquip(curSolt);
			if (equipBagSolt > 0)	//当前槽位有装备，先脱下
			{
				ItemEquip* curEquip = NULL;
				if ( PlayerItemLogic::TryGetItemEquip(bag, equipBagSolt, curEquip))
				{
					_RemoveEquipFromCharacter( character, bag, curEquip, curSolt);
				}
			}
			else if (equipBagSolt < 0)		//设置当前槽位装备为0
			{
				character->SetSlotEquip(curSolt,0);
			}
		}
		for (uint32  curSolt = 0; curSolt < (uint32)pb::EQUIP_SLOT_HOLY; curSolt++)
		{
			MMO_ASSERT( character->GetSlotEquip(curSolt) == 0);
			if ( character->GetSlotEquip(curSolt) != 0)
				return pb::ErrCommonFail;
			if( !logic_item_proto->CanEquipToSlot(curSolt) )
				return pb::ErrCommonFail;
			character->SetSlotEquip( curSolt, equip->BagSlot());		//槽位装备上该套装
		}
		equip->AttachToCharacter( character->GetID());		//将该装备的从属设置为该武将
		bag->SetModifiedID( equip->BagSlot());
	}
	else
	{
		MMO_ASSERT( character->GetSlotEquip(equipSlot) == 0);
		if ( character->GetSlotEquip(equipSlot) != 0)
			return pb::ErrCommonFail;  
		if( !logic_item_proto->CanEquipToSlot(equipSlot) )
			return pb::ErrCommonFail;
		/*if ( !logic_item_proto->MatchCharClassRequired( character->Proto().CharClass() ) )  
			return pb::ErrCommonFail;*/
	    
		/*if ( !logic_item_proto->MatchCharLevelRequired( character->GetLevel()) )
			return pb::ErrCommonFail;*/

		if ( equip->EquiperId() > 0)
		{
			Character* rawEquiper = charStore->MutableCharacter( equip->EquiperId());
			MMO_ASSERT( rawEquiper) ;
			if ( rawEquiper)
			{
				int curEquipSlot = rawEquiper->GetEquipSlot(equip->BagSlot());
				if (curEquipSlot > 0 )
				{
					_RemoveEquipFromCharacter( rawEquiper, bag, equip, curEquipSlot);
				}
				else
				{
					equip->DettachFromCharacter( equip->EquiperId());
					bag->SetModifiedID( equip->BagSlot());
				}
			}
			else
			{
				equip->DettachFromCharacter( equip->EquiperId());
				bag->SetModifiedID( equip->BagSlot());
			}
		}
		equip->AttachToCharacter( character->GetID());
		bag->SetModifiedID( equip->BagSlot());

		character->SetSlotEquip( equipSlot, equip->BagSlot());
	}
    return pb::ErrCommonSuccess;
}

ItemCharacterLogic::ErrCode ItemCharacterLogic::QuikExchangeCharacterEquips( Player* player, uint32 charId ,uint32 toCharId )
{
	if (charId == toCharId)
	{
		return pb::ErrCommonFail;
	}
	if ( player == NULL)
		return pb::ErrCommonFail;
	ItemArray* bag = player->m_bag.get();
	if ( bag == NULL)
		return pb::ErrCommonFail;
	CharacterStorage* charStorage = player->m_characterStorage.get();
	if ( charStorage == NULL )
		return pb::ErrCommonFail;
	Character* charact = charStorage->MutableCharacter(charId);
	Character* toCharact = charStorage->MutableCharacter(toCharId);
	bool hasExchange = false;
	if (charact && toCharact)
	{

		if(charact->isInPrison() || toCharact->isInPrison())
			return pb::ErrHeroInDungeon;

		//如果有套装，则先脱下
		int tempequipid = charact->GetSlotEquip(0);
		int tmptoEquipId = toCharact->GetSlotEquip(0);
		ItemEquip* tmprawEquip = NULL;
		ItemEquip* tmptoRawEquip = NULL;
		int tmpsuitSlot = 0;
		int tmptosuitSlot = 0;
		bool tmphassuit = false;
		bool tmptohassuit = false;
		if(tempequipid>0)
		{
			if(PlayerItemLogic::TryGetItemEquip(bag, tempequipid, tmprawEquip))
			{
				if(tmprawEquip->Proto()->IsSuit())
				{
					tmpsuitSlot = charact->GetEquipSlot(tmprawEquip->BagSlot());
					if (tmpsuitSlot >= 0 )
					{
						_RemoveEquipFromCharacter( charact, bag, tmprawEquip, tmpsuitSlot);
						hasExchange = true;
						tmphassuit = true;
					}
				}
			}
		}

		if(tmptoEquipId>0)
		{
			if(PlayerItemLogic::TryGetItemEquip(bag, tmptoEquipId, tmptoRawEquip))
			{
				if(tmptoRawEquip->Proto()->IsSuit())
				{
					tmptosuitSlot = toCharact->GetEquipSlot(tmptoRawEquip->BagSlot());
					if (tmptosuitSlot >= 0 )
					{
						_RemoveEquipFromCharacter( toCharact, bag, tmptoRawEquip, tmptosuitSlot);
						hasExchange = true;
						tmptohassuit = true;
					}
				}
			}
		}
		

		for ( int i = pb::EQUIP_SLOT_WEAPON; i < pb::EQUIP_SLOT_COUNT; ++i)
		{
			int equipId = charact->GetSlotEquip(i);
			int toEquipId = toCharact->GetSlotEquip(i);
			if (equipId > 0  && toEquipId > 0)		//双方都有装备
			{
				ItemEquip* rawEquip = NULL;
				ItemEquip* toRawEquip = NULL;
				if ( PlayerItemLogic::TryGetItemEquip(bag, equipId, rawEquip) &&  PlayerItemLogic::TryGetItemEquip(bag, toEquipId, toRawEquip))
				{
					ASSERT(rawEquip->EquiperId() == charact->GetID());
					ASSERT(toRawEquip->EquiperId() == toCharact->GetID());
					charact->SetSlotEquip( i, toRawEquip->BagSlot()); 
					toCharact->SetSlotEquip(i, rawEquip->BagSlot());
					rawEquip->DettachFromCharacter( charact->GetID()); 
					rawEquip->AttachToCharacter(toCharact->GetID());
					toRawEquip->DettachFromCharacter( toCharact->GetID()); 
					toRawEquip->AttachToCharacter(charact->GetID());
					bag->SetModifiedID( rawEquip->BagSlot());
					bag->SetModifiedID( toRawEquip->BagSlot());
					hasExchange = true;
				}
				else if (PlayerItemLogic::TryGetItemEquip(bag, equipId, rawEquip))
				{
					ASSERT(rawEquip->EquiperId() == charact->GetID());
					charact->SetSlotEquip( i, 0); 
					toCharact->SetSlotEquip(i, rawEquip->BagSlot());
					rawEquip->DettachFromCharacter( charact->GetID()); 
					rawEquip->AttachToCharacter(toCharact->GetID());
					bag->SetModifiedID( rawEquip->BagSlot());
					hasExchange = true;
				}
				else if (PlayerItemLogic::TryGetItemEquip(bag, toEquipId, toRawEquip))
				{
					ASSERT(toRawEquip->EquiperId() == toCharact->GetID());
					charact->SetSlotEquip( i, toRawEquip->BagSlot()); 
					toCharact->SetSlotEquip(i, 0);
					toRawEquip->DettachFromCharacter( toCharact->GetID()); 
					toRawEquip->AttachToCharacter(charact->GetID());
					bag->SetModifiedID( toRawEquip->BagSlot());
					hasExchange = true;
				}
			}
			else if (equipId > 0)		//发起方有装备
			{
				ItemEquip* rawEquip = NULL;
				if ( PlayerItemLogic::TryGetItemEquip(bag, equipId, rawEquip))
				{
					ASSERT(rawEquip->EquiperId() == charact->GetID());
					charact->SetSlotEquip( i, 0); 
					toCharact->SetSlotEquip(i, rawEquip->BagSlot());
					rawEquip->DettachFromCharacter( charact->GetID()); 
					rawEquip->AttachToCharacter(toCharact->GetID());
					bag->SetModifiedID( rawEquip->BagSlot());
					hasExchange = true;
				}
			}
			else if (toEquipId > 0)		//接收方有装备
			{
				ItemEquip* toRawEquip = NULL;
				if ( PlayerItemLogic::TryGetItemEquip(bag, toEquipId, toRawEquip))
				{
					ASSERT(toRawEquip->EquiperId() == toCharact->GetID());
					charact->SetSlotEquip( i, toRawEquip->BagSlot()); 
					toCharact->SetSlotEquip(i, 0);
					toRawEquip->DettachFromCharacter( toCharact->GetID()); 
					toRawEquip->AttachToCharacter(charact->GetID());
					bag->SetModifiedID( toRawEquip->BagSlot());
					hasExchange = true;
				}
			}
		}

		//穿上套装
		if(tmphassuit)
		{
			_EquipItemToCharacter(player, tmprawEquip, toCharact, 0);
		}
		if(tmptohassuit)
		{
			_EquipItemToCharacter(player, tmptoRawEquip, charact, 0);
		}

		if (hasExchange)
		{
			charact->ReFreshCombatData();
			toCharact->ReFreshCombatData();
		}
		return pb::ErrCommonSuccess;
	}
	return pb::ErrCommonFail;
}


ItemCharacterLogic::ErrCode ItemCharacterLogic::QuikChangeEquipsOwnerToNewCharacter( Player* player, uint32 charId ,uint32 toCharId )
{
	if (charId == toCharId)
	{
		return pb::ErrCommonFail;
	}
	if ( player == NULL)
		return pb::ErrCommonFail;
	ItemArray* bag = player->m_bag.get();
	if ( bag == NULL)
		return pb::ErrCommonFail;
	CharacterStorage* charStorage = player->m_characterStorage.get();
	if ( charStorage == NULL )
		return pb::ErrCommonFail;
	Character* charact = charStorage->MutableCharacter(charId);
	Character* toCharact = charStorage->MutableCharacter(toCharId);
	bool hasExchange = false;
	if (charact && toCharact)
	{

		if(charact->isInPrison() || toCharact->isInPrison())
			return pb::ErrHeroInDungeon;
		ClearEquipFromCharacter(player, toCharId);
		//如果有套装，则先脱下
		int tempequipid = charact->GetSlotEquip(0);
		ItemEquip* tmprawEquip = NULL;
		int tmpsuitSlot = 0;
		bool tmphassuit = false;
		if(tempequipid>0)
		{
			if(PlayerItemLogic::TryGetItemEquip(bag, tempequipid, tmprawEquip))
			{
				if(tmprawEquip->Proto()->IsSuit())
				{
					tmpsuitSlot = charact->GetEquipSlot(tmprawEquip->BagSlot());
					if (tmpsuitSlot >= 0 )
					{
						_RemoveEquipFromCharacter( charact, bag, tmprawEquip, tmpsuitSlot);
						hasExchange = true;
						tmphassuit = true;
					}
				}
			}
		}
		for ( int i = pb::EQUIP_SLOT_WEAPON; i < pb::EQUIP_SLOT_HOLY; ++i)
		{
			int equipId = charact->GetSlotEquip(i);
			if (equipId > 0)		//发起方有装备
			{
				ItemEquip* rawEquip = NULL;
				if ( PlayerItemLogic::TryGetItemEquip(bag, equipId, rawEquip))
				{
					ASSERT(rawEquip->EquiperId() == charact->GetID());
					charact->SetSlotEquip( i, 0); 
					toCharact->SetSlotEquip(i, rawEquip->BagSlot());
					rawEquip->DettachFromCharacter( charact->GetID()); 
					rawEquip->AttachToCharacter(toCharact->GetID());
					bag->SetModifiedID( rawEquip->BagSlot());
					hasExchange = true;
				}
			}
		}
		ClearEquipFromCharacter(player, charId);
		//穿上套装
		if(tmphassuit)
		{
			_EquipItemToCharacter(player, tmprawEquip, toCharact, 0);
		}
		if (hasExchange)
		{
			toCharact->ReFreshCombatData();
		}
		return pb::ErrCommonSuccess;
	}
	return pb::ErrCommonFail;
}

ItemCharacterLogic::ErrCode ItemCharacterLogic::EquipItemToCharacter( Player* player, int itemId, int charId, uint32 equipSlot )
{
    ItemArray* bag = player->m_bag.get();
    if ( bag == NULL)
        return pb::ErrCommonFail;

    CharacterStorage* charStorage = player->m_characterStorage.get();
    if ( charStorage == NULL)
        return pb::ErrCommonFail;

    Character* character = charStorage->MutableCharacter( charId);
    if ( character == NULL)
        return pb::ErrCommonFail;

	if(character->isInPrison())
		return pb::ErrHeroInDungeon;

    ItemEquip * equip = NULL;		//装备
	if (itemId > 0)		//传过来有itemid则为穿该装备
	{
		if (PlayerItemLogic::TryGetItemEquip(bag, itemId, equip))	//在背包中获得对应的装备
		{
			return _EquipItemToCharacter(player, equip, character, equipSlot);
		}
	}
	else			//传过来的没有itemid但是有槽位，则为脱下该槽位的装备
	{
		int equipBagSolt = character->GetSlotEquip(equipSlot);
		if (equipBagSolt > 0)		//该槽位对应的itemid
		{
			ItemEquip* curEquip = NULL;
			if ( PlayerItemLogic::TryGetItemEquip(bag, equipBagSolt, curEquip))
			{
				if(player->IsRechBagCapcity())
				{
					return ErrRechBagCapacity;
				}
				return _RemoveEquipFromCharacter( character, bag, curEquip, equipSlot);
			}
		}
		else if (equipBagSolt < 0)
		{
			character->SetSlotEquip(equipSlot,0);
			return pb::ErrCommonSuccess;
		}
	}

    return pb::ErrCommonFail;
}

ItemCharacterLogic::ErrCode ItemCharacterLogic::EquipItemToCharacter( Player* player, ItemEquip* equip, Character* character )
{
    uint32 equipSlot = equip->Proto()->GetEquipSlot();
    return _EquipItemToCharacter( player, equip, character, equipSlot, false);
}

void ItemCharacterLogic::ReCalculateBattleAttr( Player* player ,const uint32 siege_army_id)
{
//     ItemArray* bag = player->m_bag.get();
//     if ( bag == NULL)
//         return;

    CharacterStorage* charStorage = player->m_characterStorage.get();
    if ( charStorage == NULL)
        return;

    //int power = 0;


    const CharacterIds& ids = charStorage->GetBattleArray();
    for ( uint32 i = 0;
        i < charStorage->GetBattleMemberCount(player) && i < ids.size();
        ++i )
    {
        if ( ids[i] != 0)
        {
            if (Character* character = charStorage->MutableCharacter( ids[i]))
            {
                if (charStorage->GetCharacterAttr(player,i))
                {
					//战斗中不应该改变属性
					character->ReFreshCombatData(siege_army_id);
                }
                continue;
            }
        }
        charStorage->DeleteCharacterAttr(player,i);
    }
   // player->SetCombatPower( power);
}

ItemCharacterLogic::ErrCode ItemCharacterLogic::_RemoveEquipFromCharacter( Character* character, ItemArray* bag, ItemEquip* equip, uint32 equipSlot )
{
    MMO_ASSERT(equip->EquiperId() == 0 || equip->EquiperId() == character->GetID());
    MMO_ASSERT ( character->GetSlotEquip( equipSlot) == static_cast<int32>( equip->BagSlot() ) );

	const LogicItemProto* logic_item_proto = equip->Proto();
	if (logic_item_proto && logic_item_proto->IsSuit())
	{
		uint32 suitSoltCount = (uint32)pb::EQUIP_SLOT_HOLY;
		MMO_ASSERT( equipSlot >= 0 && equipSlot < suitSoltCount);
		for (uint32  curSolt = 0; curSolt < suitSoltCount; curSolt++)
		{
			int curEquipId = character->GetSlotEquip(curSolt);
			if (curEquipId == 0)
				continue;
			character->SetSlotEquip( curSolt, 0); 
		}
		equip->DettachFromCharacter( character->GetID()); 
		bag->SetModifiedID( equip->BagSlot());
	}
	else
	{
		character->SetSlotEquip( equipSlot, 0); 

		equip->DettachFromCharacter( character->GetID()); 
		bag->SetModifiedID( equip->BagSlot());
	}
    return pb::ErrCommonSuccess;
}
