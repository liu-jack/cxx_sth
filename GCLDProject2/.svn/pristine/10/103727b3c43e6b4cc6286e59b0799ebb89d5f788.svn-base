#include "MarketTable.h"
#include "InterServer.pb.h"
#include "Market.pb.h"

void MarketTable::SaveTo(pb::GS2C_Market_All_Info& msg)
{
    pb::GS2C_Market_Mystery_Shop& mystery = *msg.mutable_mystery();
    pb::GS2C_Market_Collect& collect = *msg.mutable_collect();
    pb::GS2C_Market_Black& black = *msg.mutable_black();
    pb::GS2C_Market_Buy& buy = *msg.mutable_buy();

    black.set_time(black_time);
    buy.set_left_num_add_time(leftBuyNumAddTime);
    buy.set_content_refresh_time(buyContentRefreshTime);
    for (int i = 0; i < ARRAY_SIZE(buy_content); ++i) {
        pb::MarketBuyContent* ptr = buy.add_content();
        ptr->set_cost_typ(buy_content[i][0]);
        ptr->set_cost_val(buy_content[i][1]);
        ptr->set_give_typ(buy_content[i][2]);
        ptr->set_give_val(buy_content[i][3]);
        ptr->set_quality(buy_content[i][4]);
    }
    for (int i = 0; i < ARRAY_SIZE(mystery_shop_progress); ++i)
    {
        mystery.add_progress(mystery_shop_progress[i]);
        mystery.add_take_item_time(mystery_shop_take_item_time[i]);
    }
    for (int i = 0; i < 14; ++i)
    {
        pb::PairValue* pair = mystery.add_suits_part();
        pair->set_first(suits_part_item[i][0]);
        pair->set_second(suits_part_item[i][1]);
    }
	for(int i = 0;i < 5;++i)
	{
		pb::GxDB_Market_Collect_Info *info = collect.add_info();
		info->set_table_id(i+1);
		info->set_times(collect_times[i]);
	}
	for(int i = 0;i < ARRAY_SIZE(item_lst);++i)
	{
		pb::GS2C_Iron_Collect *data = collect.add_iron_collect_info();
		ItemPro& ref = item_lst[i];
		if(ref.item_id == 0) continue;
		data->set_item_id(ref.item_id);
		data->set_use_time(ref.use_time);
		data->set_left_times(ref.left_times);
	}
}
void MarketTable::LoadFrom(const pb::GS2C_Market_Collect& msg)
{
	for(int i = 0;i < msg.info_size();++i)
	{
		const pb::GxDB_Market_Collect_Info &info = msg.info(i);
		collect_times[info.table_id()-1] = info.times();
	}
	for(int i = 0;i < msg.iron_collect_info_size();++i)
	{
		const pb::GS2C_Iron_Collect& data = msg.iron_collect_info(i);
		item_lst[i].item_id = data.item_id();
		item_lst[i].use_time = data.use_time();
		item_lst[i].left_times = data.left_times();
	}
}
void MarketTable::LoadFrom(const pb::GS2C_Market_Black& msg)
{
    black_time = msg.time();
}
void MarketTable::LoadFrom(const pb::GS2C_Market_Buy& msg)
{
    leftBuyNumAddTime = msg.left_num_add_time();
    buyContentRefreshTime = msg.content_refresh_time();
    for (int i = 0; i < ARRAY_SIZE(buy_content); ++i) {
        const pb::MarketBuyContent& content = msg.content(i);
        buy_content[i][0] = content.cost_typ();
        buy_content[i][1] = content.cost_val();
        buy_content[i][2] = content.give_typ();
        buy_content[i][3] = content.give_val();
        buy_content[i][4] = content.quality();
    }
}
void MarketTable::LoadFrom(const pb::GS2C_Market_Mystery_Shop& msg)
{
    for (int i = 0; i < msg.progress_size(); ++i) {
        mystery_shop_progress[i] = msg.progress(i);
        mystery_shop_take_item_time[i] = msg.take_item_time(i);
    }
    for (int i = 0; i < msg.suits_part_size(); ++i) {
        const pb::PairValue& pair = msg.suits_part(i);
        suits_part_item[i][0] = pair.first();
        suits_part_item[i][1] = pair.second();
    }
}