#include "LogicItemProto.h"
#include "datastruct/struct_item.h"
#include "DbTool.h"

#include "ItemManager.h"

#include "BaseDefineMgr.h"

#include "BaseDefine.pb.h"
#include "Opcode.pb.h"
#include "Enum.pb.h"    


LogicItemProto::LogicItemProto( const DB_ItemProtoType& db )
: DB_ItemProtoType( db)
{
   /* FOREACH_DB_ITEM( item_upgrade, ItemUpgrade)
    {
        if ( item_upgrade->category_1 == this->category_1
            && item_upgrade->category_2 == this->category_2
            && item_upgrade->category_3 == this->category_3
            && item_upgrade->quality_option == this->quality_option)
        {
            m_item_upgrade_id = item_upgrade->index_id;
            break;
        }
    }*/
//#ifdef _DEBUG //slg无装备升级
//    if ( IsAccessory() || IsEquip())
//    {
//        if ( m_item_upgrade_id == 0)
//        {
//            ELOG( "Equip or Acc: %d can't find ItemUpgrade", Id());
//            MASSERT( false, "Can't find upgrade ID");
//        }
//    }
//#endif
    
}

uint32 LogicItemProto::Id() const
{
    return id;
}



bool LogicItemProto::CanStack() const
{
    if ( CanBeEquiped() || IsMatAccLV() || IsCard())
    {
        return false;
    }
    return true;
}


bool LogicItemProto::IsXilianEquip() const
{
  if(IsEquip())
  {
	  return IsFlag() || IsWeapon() || IsBook() || IsJade() || IsChest() || IsHorse();
  }
  return false;
}

uint32 LogicItemProto::Quality() const
{
    return quality_option;
}

int LogicItemProto::Class1() const
{
	return category_1;
}

int LogicItemProto::Class2() const
{
	return category_2;
}

int LogicItemProto::Class3() const
{
	return category_3;
}

int LogicItemProto::Value1() const
{
	return value01;
}

int LogicItemProto::Value2() const
{
	return value02;
}

int LogicItemProto::Value3() const
{
	return value03;
}

bool LogicItemProto::CanBeCreateToBag() const
{
    return !IsCard() && !IsCurrency() && !IsBox() && !IsAttr()  && !IsMaterial() && !IsPuzzle();
}

bool LogicItemProto::CanBeEquiped() const
{
    return IsEquip();
}

bool LogicItemProto::CanEquipToSlot( uint32 slot ) const
{
    using namespace pb;
    switch( slot )
    {
    case EQUIP_SLOT_WEAPON  :   if (IsWeapon() || IsSuit())   return true;  break;
    case EQUIP_SLOT_HORSE    :  if (IsHorse() || IsSuit())     return true;  break;
    case EQUIP_SLOT_CHEST   :   if (IsChest() || IsSuit())    return true;  break;                 
    case EQUIP_SLOT_BOOK     :  if (IsBook() || IsSuit())	return true;  break;
    case EQUIP_SLOT_JADE    :   if (IsJade() || IsSuit()) return true;  break;
    case EQUIP_SLOT_FLAG:		if (IsFlag() || IsSuit()) return true;  break;
	case EQUIP_SLOT_HOLY:		if (IsAcc()) return true;  break;
	case EQUIP_SLOT_ARMY:		if (IsMc()) return true;  break;
    default:
        return false;
    }
    return false;
}


uint32 LogicItemProto::GetEquipSlot() const
{
    using namespace pb;
    switch( category_1)
    {
    case CLASS_1_Equip:
        switch( category_2)
        {
        case CLASS_2_Weapon:   return EQUIP_SLOT_WEAPON;
        case CLASS_2_Horse:    return EQUIP_SLOT_HORSE;
        case CLASS_2_Chest:    return EQUIP_SLOT_CHEST;
        case CLASS_2_Book:     return EQUIP_SLOT_BOOK;
		case CLASS_2_Jade:     return EQUIP_SLOT_JADE;
		case CLASS_2_Flag:     return EQUIP_SLOT_FLAG;
		case CLASS_2_Suit:     return EQUIP_SLOT_WEAPON;
		case CLASS_2_Acc:      return EQUIP_SLOT_HOLY;
        default: break;
        }
        break;
	case CLASS_1_Card:
		switch( category_2)
		{
		case CLASS_2_Mc:	   return EQUIP_SLOT_ARMY;
		default: break;
		}
    default:
        break;
    }
    return uint32(-1);
}



uint32 LogicItemProto::ACCItemBaseExp() const
{
    return sItemMgr.GetAccItemBaseExp(quality_option);
}

uint32 LogicItemProto::ObjectValue() const
{
    return object_id;
}

namespace {
    std::vector<int>& GetCardComposeNeedCountVec()
    {
        static std::vector<int> vec;
        if ( vec.empty())
        {
            vec.push_back( GET_BASE_DEF_INT( pb::BD_CARD_COMPOSE_TYPE_0_COUNT));
            vec.push_back( GET_BASE_DEF_INT( pb::BD_CARD_COMPOSE_TYPE_1_COUNT));
        }
        return vec;
    }

    std::vector<int>& GetEquipComposeNeedCountVec()
    {
        static std::vector<int> vec;
        if ( vec.empty())
        {
            vec.push_back( GET_BASE_DEF_INT( pb::BD_EQUIP_COMPOSE_QUALITY0_COUNT));
            vec.push_back( GET_BASE_DEF_INT( pb::BD_EQUIP_COMPOSE_QUALITY1_COUNT));
            vec.push_back( GET_BASE_DEF_INT( pb::BD_EQUIP_COMPOSE_QUALITY2_COUNT));
            vec.push_back( GET_BASE_DEF_INT( pb::BD_EQUIP_COMPOSE_QUALITY3_COUNT));
            vec.push_back( GET_BASE_DEF_INT( pb::BD_EQUIP_COMPOSE_QUALITY4_COUNT));
            vec.push_back( GET_BASE_DEF_INT( pb::BD_EQUIP_COMPOSE_QUALITY5_COUNT));
        }
        return vec;
    }

    std::vector<int>& GetAccComposeNeedCountVec()
    {
        static std::vector<int> vec;
        if ( vec.empty())
        {
            vec.push_back( GET_BASE_DEF_INT( pb::BD_ACC_COMPOSE_QUALITY0_COUNT));
            vec.push_back( GET_BASE_DEF_INT( pb::BD_ACC_COMPOSE_QUALITY1_COUNT));
            vec.push_back( GET_BASE_DEF_INT( pb::BD_ACC_COMPOSE_QUALITY2_COUNT));
            vec.push_back( GET_BASE_DEF_INT( pb::BD_ACC_COMPOSE_QUALITY3_COUNT));
            vec.push_back( GET_BASE_DEF_INT( pb::BD_ACC_COMPOSE_QUALITY4_COUNT));
            vec.push_back( GET_BASE_DEF_INT( pb::BD_ACC_COMPOSE_QUALITY5_COUNT));
        }
        return vec;
    }
}

bool LogicItemProto::TryGetPuzzleComposeCount( int& out_count) const
{
    if ( IsPuzzle())
    {
        switch( category_3)
        {
        case DB_ItemProtoType::CLASS_3_PuzzleCard:
            {
                /*std::vector<int>& vec = GetCardComposeNeedCountVec();
                const DB_CharProto* chara = DbTool::Get<DB_CharProto>( ObjectValue());
                if ( chara)
                {
                    if ( chara->type < vec.size())
                    {
                        out_count = vec[ chara->type];
                        return true;
                    }                    
                }*/
            }
            break;
        case DB_ItemProtoType::CLASS_3_PuzzleEquip:
            {
                /*std::vector<int>& vec = GetEquipComposeNeedCountVec();
                if ( Quality() < (int)vec.size())
                {
                    out_count = vec[ Quality()];
                    return true;
                }                */
            }
            break;
        //case DB_ItemProtoType::CLASS_3_PuzzleAcc:
        //    {
        //        std::vector<int>& vec = GetAccComposeNeedCountVec();
        //        if ( Quality() < (int)vec.size())
        //        {
        //            out_count = vec[ Quality()];
        //            return true;
        //        }   
        //    }
        //    break;
        //case DB_ItemProtoType::CLASS_3_PuzzleRune:
        //    {
        //        out_count = 1;
        //        return true;
        //    }
        //    break;
        default:
            break;
        }
    }
    return false;
}



//int LogicItemProto::ItemType() const
//{
//    return Class1()*10000 + Class2()*100 + Class3();
//}

bool LogicItemProto::MatchCharClassRequired( int char_class ) const
{
    if ( require_char == -1)
        return true;

    return require_char == char_class;
}

bool LogicItemProto::MatchCharLevelRequired( int char_level ) const
{
    return static_cast<int>(require_level) <= char_level;
}
