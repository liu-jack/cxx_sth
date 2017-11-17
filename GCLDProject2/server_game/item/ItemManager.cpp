#include "ItemManager.h"

#include <boost/typeof/typeof.hpp>
#include "LogicItemProto.h"

#include "System.h"
#include "mysqldata.h"
#include "DbTool.h"
#include "utility/Utility.h"

#include "Item.h"
#include "ItemEquip.h"
#include "Item.pb.h"

#include "BaseDefineMgr.h"
#include "BaseDefine.pb.h"
//#include "AutomaticUpdate/AutomaticUpdate.h"
//#include "AutomaticUpdate/UpdateEvent.h"
#include "../session/PlayerPool.h"

using namespace pb;


void ItemManager::Init()
{
    InitLogicItemProto();
    InitItemAttr();
	//RecoverItemNums();

}

void ItemManager::InitLogicItemProto()
{
    FOREACH_DB_ITEM(db, DB_ItemProtoType)
    {
        LogicItemProto* itemlogic = new LogicItemProto(*db);
        m_protoMap[db->id] = itemlogic;

        if (db->IsCard())
        {
            m_charproto2itemproto[ db->object_id] = db->id;
        }
        else if ( db->IsPuzzleAcc())
        {
            m_accComposeMaterialMap[ db->object_id].insert( db->id);
        }

        if ( itemlogic->CanBeEquiped() && itemlogic->bonding_option != 1)
        {
            uint32 equipSlot = itemlogic->GetEquipSlot();
            uint32 quality = itemlogic->Quality();
            uint32 require_char = itemlogic->require_char;
            uint32 require_level = itemlogic->require_level;
            if ( itemlogic->require_char == -1)
            {
                for ( require_char = Tank ; require_char <= Healer; ++require_char)
                {
                    m_PQCL2ItemId.insert( std::make_pair( boost::make_tuple( equipSlot, quality, require_char, require_level), db->id)) ;
                }
            }else
            {
                m_PQCL2ItemId.insert( std::make_pair( boost::make_tuple( equipSlot, quality, require_char, require_level), db->id)) ;
            }
        }
        
    }

	FOREACH_DB_ITEM(dbxilian, ItemEquipXiLian)
	{
		m_xiLianLevel2Attr[dbxilian->id] = dbxilian;
	}


	FOREACH_DB_ITEM(dbsuilt, ItemSuitComplex)
	{
		m_suitId2Complex[dbsuilt->item_id] = dbsuilt;
	}

	FOREACH_DB_ITEM(dbbagSolt, ItemBagSoltSell)
	{
		m_bagSoltId2SellInfo[dbbagSolt->id] = dbbagSolt;
	}
}

//void ItemManager::RecoverItemNums()
//{
	////每小时恢复一点免费洗练次数
	//sAutoUpdate.Add(boost::bind(&ItemManager::_recoverPurifyTimes,this),detail_enum::TIME_ZERO,detail_enum::PER_HOUR);
//}

//void ItemManager::_recoverPurifyTimes()
//{
	//NLOG("ItemManager::_recoverPurifyTimes");
	//FOREACH_PLAYERPTR(playerId,playerPtr)
	//{
	//	if(!playerPtr->GetDataIsLost())
	//	{
	//		if(playerPtr->GetCurrency(eFreePurify) < (int32)GET_BASE_DEF_UINT(BD_PLAYER_REGAIN_FREE_PURIFY_NUM_VALUE))
	//		{
	//			NLOG("ItemManager::_recoverPurifyTimes recover once, num=%d",playerPtr->GetCurrency(eFreePurify));
	//			playerPtr->AddCurrency(eFreePurify,1);
	//		}
	//	}
	//}
//}

IntPair ItemManager::TranslateAttr(uint32 itemid,uint32 level)
{
	LogicItemProto* item = m_protoMap.pfind(itemid);
	IntPair newpair;
	if(item)
	{
		if(item->IsAttr())		//是属性
		{
			newpair.first = (itemid%1000000)/10000;
			XiLianLevel2Attr::const_iterator xilianIter = m_xiLianLevel2Attr.find(level);
			if (xilianIter != m_xiLianLevel2Attr.end())
			{
				switch(newpair.first)
				{
				case 0:
					newpair.second = xilianIter->second->troops;
					break;
				case 1:
					newpair.second = xilianIter->second->attack;
					break;
				case 2:
					newpair.second = xilianIter->second->defense;
					break;
				case 3:
					newpair.second = xilianIter->second->chuanci;
					break;
				case 4:
					newpair.second = xilianIter->second->gedang;
					break;
				case 5:
					newpair.second = xilianIter->second->pozhen;
					break;
				case 6:
					newpair.second = xilianIter->second->wuxie;
					break;
				default:
					break;
				}
			}
		}
	}
	return newpair;
}

const ItemSuitComplex* ItemManager::FindSuitComplex( ItemId itemId ) const
{
	SuitId2Complex::const_iterator suitIter = m_suitId2Complex.find(itemId);
	if (suitIter != m_suitId2Complex.end())
	{
		return suitIter->second;
	}
	return NULL;
}

/**********装备属性的计算*********/
//计算一件装备的属性
void ItemManager::GetRealItemAttr(uint32 itemid, float (&attr)[CHARACTER_VALUES_COUNT])
{
	LogicItemProto* item = m_protoMap.pfind(itemid);
	if(item)
	{
		if(item->IsEquip())		//是装备
		{
			if(item->IsSuit())
				GetSuitAttr(itemid,attr);
			else
				GetNormalEquipAttr(itemid,attr);
		}
	}
	return;
}

//计算普通装备
void ItemManager::GetNormalEquipAttr(uint32 itemid, float (&attr)[CHARACTER_VALUES_COUNT])
{
	LogicItemProto* item = m_protoMap.pfind(itemid);
	if(item)
	{
		if(item->IsEquip())		//是装备
		{
			if(!item->IsSuit())		//是普通装备
			{
				const ItemAttrContain* attrContain = GetAttrContain( item->id, item->Quality());
				uint32 baseFlag = item->attr_flag_base;
				float baseValue = 0;
				for( uint32 i = 0 ; i < CHARACTER_VALUES_COUNT; ++i )
				{
					if ( Utility::TestBit( baseFlag, i) )
					{
						baseValue = attrContain->attrBase->value[i];
					}
					else
					{
						baseValue = 0;
					}
					attr[i] +=  baseValue;
				}
			}
		}
	}
	return;
}

//计算套装属性
void ItemManager::GetSuitAttr(uint32 itemid, float (&attr)[CHARACTER_VALUES_COUNT])
{
	LogicItemProto* item = m_protoMap.pfind(itemid);
	if(item)
	{
		if(item->IsSuit())	//是套装
		{
			//计算6件基础装备的属性
			SuitId2Complex::const_iterator suitIter = m_suitId2Complex.begin();		//第一套套装属性
			for(int i=0; i<6; i++)
			{
				GetNormalEquipAttr(suitIter->second->req_equip[i],attr);
			}

			//计算套装的额外加成
			const ItemAttrContain* attrContain = GetAttrContain( item->id, item->Quality());
			uint32 baseFlag = item->attr_flag_base;
			float baseValue = 0;
			for( uint32 i = 0 ; i < CHARACTER_VALUES_COUNT; ++i )
			{
				if ( Utility::TestBit( baseFlag, i) )
				{
					baseValue = attrContain->attrBase->value[i];
				}
				else
				{
					baseValue = 0;
				}
				attr[i] +=  baseValue;
			}
		}
	}
	return;
}
/**********装备属性的计算*********/

//获得洗练属性个数
int ItemManager::GetItemMaxStarCount(uint32 itemid)
{
	int starNum = 0;
	LogicItemProto* item = m_protoMap.pfind(itemid);
	if(item)
	{
		switch(item->Quality())
		{
		case 1:
		case 2:
		case 3:
			starNum = 0;
			break;
		case 4:
			starNum = 1;
			break;
		case 5:
			starNum = 3;
			break;
		case 6:
			starNum = 4;
			break;
		default:
			starNum = 0;
			break;
		}
	}
	return starNum;
}

int ItemManager::GetItemMaxXiLianLevel(uint32 itemid)
{
	int starNum = 0;
	LogicItemProto* item = m_protoMap.pfind(itemid);
	if(item)
	{
		switch(item->Quality())
		{
		case 1:
		case 2:
		case 3:
			starNum = 0;
			break;
		case 4:
			starNum = 2;
			break;
		case 5:
			starNum = 3;
			break;
		case 6:
			starNum = 5;
			break;
		default:
			starNum = 0;
			break;
		}
	}
	return starNum;
}

bool ItemManager::isLastStarHide(uint32 itemid)
{
	LogicItemProto* item = m_protoMap.pfind(itemid);

	if(item)
		return (item->Quality() == 5 || item->Quality() == 6);

	return false;
}

const ItemBagSoltSell* ItemManager::FindBagsoltSellInfo( ItemId itemId ) const
{
	BagSoltId2SellInfo::const_iterator soltIter = m_bagSoltId2SellInfo.find(itemId);
	if (soltIter != m_bagSoltId2SellInfo.end())
	{
		return soltIter->second;
	}
	return NULL;
}

const LogicItemProto* ItemManager::Find( ItemId itemId ) const
{
    return m_protoMap.pfind(itemId);
}

Item* ItemManager::CreateItemInst( ItemId itemId, uint32 count )
{
    const LogicItemProto * proto = Find(itemId);
    if ( proto == NULL)
    {
        return NULL;
    }
    return CreateItemInst( proto, count);
}

Item* ItemManager::CreateItemInst( const LogicItemProto* proto, uint32 count )
{
	if ( proto->CanBeEquiped())
    {
        return new ItemEquip(proto);
    }
    else
    {
        return new Item(proto, count);
    }
}

Item* ItemManager::CreateItemInst( const pb::GS2C_SlotItem& itemMsg )
{
    Item* item = CreateItemInst( itemMsg.item().proto_id());
    if ( item)
    {
        item->LoadFrom(itemMsg);
    }
    return item;
}



bool CompareAccExpBookExp( const ItemManager::AccExpBook& b1, const ItemManager::AccExpBook& b2 )
{
    return b1.exp < b2.exp;
}



bool ItemManager::TryGetCardItemId( uint32 charProtoId, uint32& itemid ) const
{
    CharProto2ItemProto::const_iterator it = m_charproto2itemproto.find(charProtoId);
    if ( it != m_charproto2itemproto.end())
    {
        itemid = it->second;
        return true;
    }
    return false;
}

uint32 ItemManager::GetAccItemBaseExp( uint32 itemQuality ) const
{
    uint32 ret = 0;
    switch (itemQuality)
    {
    case 0 :
        ret = GET_BASE_DEF_UINT( BD_ACC_QUALITY_0_EXP);
        break;
    case 1 :
        ret = GET_BASE_DEF_UINT( BD_ACC_QUALITY_1_EXP);
        break;
    case 2 :
        ret = GET_BASE_DEF_UINT( BD_ACC_QUALITY_2_EXP);
        break;
    case 3 :
        ret = GET_BASE_DEF_UINT( BD_ACC_QUALITY_3_EXP);
        break;
    case 4 :
        ret = GET_BASE_DEF_UINT( BD_ACC_QUALITY_4_EXP);
        break;
    case 5 :
        ret = GET_BASE_DEF_UINT( BD_ACC_QUALITY_5_EXP);
        break;
    default:
        break;
    }
    return ret;
}

void ItemManager::InitItemAttr()
{

    FOREACH_DB_ITEM(dbb, ItemAttrBase)
    {
        m_typeQuality2AttrContain[std::make_pair(dbb->equip_type, dbb->quality)].attrBase = dbb;
    }

    FOREACH_DB_ITEM(dba, ItemAttrAdd)
    {
        m_typeQuality2AttrContain[std::make_pair(dba->equip_type, dba->quality)].attrAdd = dba;
    }
}

const ItemAttrContain* ItemManager::GetAttrContain( uint32 itemType, uint32 quality ) const
{
    Itemtype_Quality2AttrContain::const_iterator it 
		= m_typeQuality2AttrContain.find( std::make_pair(itemType, quality));
    if ( it != m_typeQuality2AttrContain.end())
    {
        return &(it->second);
    }
    return NULL;

}


uint32 ItemManager::PickItemIdMatchRequire( const PQCL& condition ) const
{
    static const uint32 item_min_require_level = 0;
    typedef PQCL2ItemID::const_iterator Iter;
    
    PQCL lowerCondtion( condition);
    lowerCondtion.get<3>() = item_min_require_level;

    Iter low = m_PQCL2ItemId.lower_bound( lowerCondtion);
    Iter up = m_PQCL2ItemId.upper_bound( condition);
    
    // 并非线程安全
	static std::vector<uint32> itemIds;
    itemIds.reserve(128);
    itemIds.clear();

    for ( Iter it = low; it != up; ++it)
    {
        itemIds.push_back( it->second);
    }

    if ( itemIds.empty())
    {
        return 0;
    }
    else{
        return itemIds[ System::Rand(0, itemIds.size())];
    }
}

const ItemProtoMap& ItemManager::GetItemProtoMap() const
{
	return m_protoMap;
}
//返回一个洗练技能，攻击，兵力，防御。。。
uint32 ItemManager::GetAttrFromId( const uint32 id )
{
	switch(id)
	{
	case 0: return ItemEnum::ATTR_BINGLI;
	case 1: return ItemEnum::ATTR_ATTACK;
	case 2:	return ItemEnum::ATTR_DEFENCE;
	case 3: return ItemEnum::ATTR_CHUANTOU;
	case 4: return ItemEnum::ATTR_DIDANG;
	case 5: return ItemEnum::ATTR_POHUAI;
	case 6: return ItemEnum::ATTR_SHOUHU;
	default:
		return 0;
	}
}

