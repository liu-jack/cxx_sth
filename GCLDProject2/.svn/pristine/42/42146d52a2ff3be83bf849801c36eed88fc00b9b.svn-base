#include "ItemTable.h"
#include "Item.pb.h"

void ItemBaseTable::SaveTo( pb::GxDB_ItemInfo& msg )
{
    msg.set_count( count);
    msg.set_is_new( is_new != 0 );
    msg.set_proto_id( proto_id);
	msg.set_is_sold(is_sold !=0);
	msg.set_repo_end_time(repo_end_time);
}

void ItemBaseTable::LoadFrom( const pb::GxDB_ItemInfo& msg )
{
    count = msg.count();
    is_new = msg.is_new() ? 1 :0;
    proto_id = msg.proto_id();
	is_sold = msg.is_sold() ? 1 :0;
	repo_end_time = msg.repo_end_time();
}

void ItemEquipTable::SaveTo( pb::GxDB_EquipInfo& msg )
{
    msg.set_equip_to_charid( equip_char_id);
	for (int i=0; i < EQUIPITEMSKILLATTRCOUNT; i++)
	{
		pb::PairValue* skill = msg.add_skill_attrs();
		skill->set_first(attr[i]);
		skill->set_second(level[i]);
	}
}

void ItemEquipTable::LoadFrom( const pb::GxDB_EquipInfo& msg )
{
    equip_char_id = msg.equip_to_charid();
	for (int i=0; i < msg.skill_attrs_size(); i++)
	{
		const pb::PairValue& curValue = msg.skill_attrs(i);
		attr[i] = curValue.first();
		level[i] = curValue.second();
	}
}

void LootRecordTable::SaveTo( pb::GS2C_LootRecord& msg ) const
{
    msg.set_loot_type_id( loot_type_id);
    msg.set_roll_success_count( roll_success_count);
    msg.set_roll_count( roll_count);
}

void LootRecordTable::LoadFrom( const pb::GS2C_LootRecord& msg )
{
    loot_type_id = msg.loot_type_id();
    roll_success_count = msg.roll_success_count();
    roll_count = msg.roll_count();
}

void LotteryFreeTable::SaveTo( pb::GS2C_LotteryFreeRecord& msg ) const
{
    msg.set_lottery_type( lottery_type);
    msg.set_remain_free_count( remain_free_count);
    msg.set_next_free_lottery_time( next_free_time);
    
}

void LotteryFreeTable::LoadFrom( const pb::GS2C_LotteryFreeRecord& msg )
{
    lottery_type = msg.lottery_type();
    remain_free_count = msg.remain_free_count();
    next_free_time = msg.next_free_lottery_time();
}
