#include "CharacterAttr.h"

#include "def/ObjectDefines.h" // CHARACTER_VALUES_COUNT
#include "utility/Utility.h"

#include "DatabaseMgr.h"
#include "DbTool.h"

#include "item/ItemArray.h"
#include "item/ItemEquip.h"
#include "item/Item.h"
#include "item/ItemManager.h"

#include "character/Character.h"
#include "character/CharacterManager.h"

#include "Enum.pb.h"
#include "../BaoQi/PlayerBaoQiLog.h"
#include "Technology/PlayerTechnology.h"
#include "Technology/Table/TechnologyTableMgr.h"
#include "../object/Player.h"
#include "CharacterStorage.h"
#include "seige_force/SeigeForceLog.h"
using namespace pb;

//////////////////////////////////////////////////////////////////////////
CharacterAttr::CharacterAttr()
{
    m_attrs.resize( CHARACTER_VALUES_COUNT);
}

void CharacterAttr::RecalculateAttr(const ItemArray& bag, const PlayerBaoQiLog& baoqi, Character& character,Player* player,const uint32 siege_army_id, bool isLastAttrWork)
{
    RecalculateSelfAttr( character.ProtoID(), character.GetLevel(), character.GetQuality(), character.GetReinforce(),player);
    //AppendRandomAttr( character);
    for ( uint32 i = EQUIP_SLOT_WEAPON; i < EQUIP_SLOT_COUNT; ++i)
    {
        int32 equipId = character.GetSlotEquip(i);
        if ( equipId == 0)
            continue;
        const Item* item = bag.GetItem(equipId);
        if ( item == NULL)
            continue;
        const ItemEquip* equip = dynamic_cast<const ItemEquip*>( item);
        if ( equip == NULL)
            continue;

		//套装属性只加一次
		if((*equip).Proto()->IsSuit() && i != EQUIP_SLOT_WEAPON)
			continue;

		AppendEquipAttr(*equip,isLastAttrWork);	//附加装备属性
    }

    //宝器属性
    AppendBaoQiAttr(baoqi);
	//攻城部队改变血量
	player->m_SeigeForceLog->CalculateCharacterAttr(siege_army_id,this);

	//兵力溢出重置兵力
	int32 cur_troops = int32(GetAttr(UNIT_TROOPS) * character.GetPlayerSolderRowNum() * character.GetSolderColNum());
	if(cur_troops < character.CurHp())
	{
		character.MutableValues().SetInt(pb::CHAR_FIELD_CUR_TROOPS, cur_troops);
		//character.setRestHP(character.MutableValues().GetInt(pb::CHAR_FIELD_CUR_TROOPS));
	}
	else
	{
		character.MutableValues().SetInt(pb::CHAR_FIELD_CUR_TROOPS, character.CurHp());
	}

    //TODO：整理收集buff数据

    //将计算结果设至对象身上
    character.MutableValues().SetInt(pb::CHAR_FIELD_TROOPS, cur_troops);
    character.MutableValues().SetInt(pb::CHAR_FIELD_ATTACK, (int32)GetAttr(UNIT_ATTACK));
    character.MutableValues().SetInt(pb::CHAR_FIELD_DEFENSE, (int32)GetAttr(UNIT_DEFENSE));
    character.MutableValues().SetInt(pb::CHAR_FIELD_PUNCTURE, (int32)GetAttr(UNIT_PUNCTURE));
    character.MutableValues().SetInt(pb::CHAR_FIELD_PARRY, (int32)GetAttr(UNIT_PARRY));
    character.MutableValues().SetInt(pb::CHAR_FIELD_BROKEN_ARRAY, (int32)GetAttr(UNIT_BROKEN_ARRAY));
    character.MutableValues().SetInt(pb::CHAR_FIELD_IMPECCABLE, (int32)GetAttr(UNIT_IMPECCABLE));
    character.MutableValues().SetInt(pb::CHAR_FIELD_TONG, (int32)GetAttr(UNIT_TONG));
    character.MutableValues().SetInt(pb::CHAR_FIELD_YONG, (int32)GetAttr(UINT_YONG));
	character.MutableValues().SetInt(pb::CHAR_FIELD_MILITARY_POWER,(int32)character.CalculateCharacterMilitaryPower());

    character.NotifyStorageModified();
}

void CharacterAttr::RecalculateSelfAttr( uint32 charProto, uint32 level, uint32 quality, uint32 reinforce,Player* player )
{
    const DB_CharAttrBase* attrBase = DbTool::Get<DB_CharAttrBase>( charProto);
    MMO_ASSERT( attrBase );
    //const DB_CharAttrReinforce* attrReinforce = DbTool::Get<DB_CharAttrReinforce>(reinforce);
    //MMO_ASSERT( attrReinforce );
    const DB_CharAttrAdd* attrAdd = sCharacterMgr.GetAttrAdd(charProto, quality);
    MMO_ASSERT( attrAdd );
	const TecTable *tec = NULL;
    if ( attrBase && attrAdd /*&& attrReinforce*/)
    {
        for( uint32 i = 0 ; i < CHARACTER_VALUES_COUNT; ++i )
        {
            m_attrs[i] = ( attrBase->base_value[i] + level * attrAdd->add_value[i] );
        }
		//科技----只加一次
		if(player->HaveTechnology(FIGHT_ENFORCE1))
		{
			tec = sTechnologyTableMgr.GetTable(FIGHT_ENFORCE1);
			m_attrs[UNIT_TROOPS] += tec->m_AttrReward[0].second;
		}
		if(player->HaveTechnology(FIGHT_ENFORCE2))
		{
			tec = sTechnologyTableMgr.GetTable(FIGHT_ENFORCE2);
			m_attrs[UNIT_ATTACK] += tec->m_AttrReward[0].second;
			m_attrs[UNIT_DEFENSE] += tec->m_AttrReward[1].second;
		}
		if(player->HaveTechnology(FIGHT_ENFORCE4))
		{
			tec = sTechnologyTableMgr.GetTable(FIGHT_ENFORCE4);
			m_attrs[UNIT_ATTACK] += tec->m_AttrReward[0].second;
			m_attrs[UNIT_DEFENSE] += tec->m_AttrReward[1].second;
		}
		if(player->HaveTechnology(FIGHT_ENFORCE5))
		{
			tec = sTechnologyTableMgr.GetTable(FIGHT_ENFORCE5);
			m_attrs[UNIT_ATTACK] += tec->m_AttrReward[0].second;
			m_attrs[UNIT_DEFENSE] += tec->m_AttrReward[1].second;
		}
    }
}

void CharacterAttr::AppendRandomAttr( const Character& character )
{
    for( uint32 i = 0 ; i < DB_CharAttrRandom::MAX_ATTR_INDEX_CAN_RANDOM; ++i )
    {
        m_attrs[i] += character.GetTrainedAttr(i);
    }
}

void CharacterAttr::AppendEquipAttr( const ItemEquip& equip, bool isLastAttrWork)
{
 //   const ItemAttrContain* attrContain = sItemMgr.GetAttrContain( equip.ProtoId(), equip.Quality());
	//if (attrContain == NULL)
	//{
	//	return;
	//}
 //   if ( attrContain->attrBase == NULL)
 //   {
 //       MASSERT( false, "item attr data error");
 //       return;
 //   }

 //   uint32 baseFlag = equip.Proto()->attr_flag_base;
 //   float baseValue = 0

 //   for( uint32 i = 0 ; i < CHARACTER_VALUES_COUNT; ++i )
 //   {
 //       if ( Utility::TestBit( baseFlag, i) )
 //       {
 //           baseValue = attrContain->attrBase->value[i];
 //       }
 //       else
 //       {
 //           baseValue = 0;
 //       }

 //       m_attrs[i] +=  baseValue;
 //   }

	//int hideExid = 0;	//隐藏attr的id
	//int lastExid = 0;	//用来比较
	//bool attrOutUse = false;
	//for (int i=0; i < (int)EQUIPITEMSKILLATTRCOUNT; i++)	//四个属性
	//{
	//	if(equip.GetAttr(i) != 0)
	//	{
	//		IntPair newpair;
	//		newpair = sItemMgr.TranslateAttr(equip.GetAttr(i),equip.GetAttrLevel(i));

	//		if(i == 0)
	//			lastExid = newpair.first;

	//		if(i != 0 && attrOutUse == false)
	//		{
	//			if(lastExid != newpair.first || equip.GetAttrLevel(i) != 5)
	//			{
	//				attrOutUse = true;
	//			}
	//		}
	//		
	//		if(i == 3 && attrOutUse == true)
	//			break;

	//		m_attrs[newpair.first] += newpair.second;
	//	}8
	//}
	
	float tmpattr[CHARACTER_VALUES_COUNT] = {0.0};
	equip.GetTotalAttr(tmpattr,isLastAttrWork);
	for(int i=0; i<CHARACTER_VALUES_COUNT; i++)
	{
		m_attrs[i] += tmpattr[i];
	}

}
void CharacterAttr::AppendBaoQiAttr(const PlayerBaoQiLog& baoqi)
{
    for (uint32 id = 0; id < BaoQi_MaxCnt; ++id)
    {
        const PlayerBaoQiLog::BaoQiPro& pro = baoqi.m_BaoQiLst[id];

        if (const DB_BaoQi::Property* table = sBaoQiTableMgr.GetBaoQi(id, pro.level))
        {
            float sumVal = 0;
            // 升星
            for (uint i = 1; i <= pro.star; ++i)
            {
                if (const DB_BaoQi_UpStar* table = sBaoQiTableMgr.GetBaoQiUpStar(i))
                {
                    sumVal += table->baoqi_value[id];
                }
            }
            // 镶嵌的宝石
            if (const LogicItemProto* proto = sItemMgr.Find(pro.gem))
            {
                if (const ItemAttrContain* attrContain = sItemMgr.GetAttrContain(proto->id, proto->Quality()))
                {
                    sumVal += attrContain->attrBase->value[table->attr_id];
                }
            }
            m_attrs[table->attr_id] += table->attr_value + sumVal;
        }
    }
}

int CharacterAttr::GetBattlePower() const
{
    int power = 0;
    MMO_ASSERT( m_attrs.size() >= 5 );
    power = (int)(
        m_attrs[0]*1/15 
        + m_attrs[1]*4/5 
        + m_attrs[2]*4/5 
        + m_attrs[3]
        + m_attrs[4]);
    return power;
}
float CharacterAttr::GetAttr(uint8 idx) const
{
    if (idx < CHARACTER_VALUES_COUNT)
    {
        return m_attrs[idx];
    }
    return 0;
}

void CharacterAttr::SetAttr(uint8 idx,float value)
{
	if (idx < CHARACTER_VALUES_COUNT)
	{
		m_attrs[idx]= value;
	}
}
