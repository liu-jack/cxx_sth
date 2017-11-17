#include "BaoQiTable.h"
#include "InterServer.pb.h"
#include "BaoQi.pb.h"

void BaoQiTable::SaveTo(pb::GS2C_BaoQi_All_Info& msg)
{
    for (int i = 0; i < ARRAY_SIZE(pro_lst); ++i)
    {
        pb::BaoQiPro* data = msg.add_property();
        BaoQiPro& ref = pro_lst[i];
        data->set_level(ref.level);
        data->set_exp(ref.exp);
        data->set_star(ref.star);
        data->set_have_hole(ref.have_hole);
        data->set_gem(ref.gem);
    }
	for(int i = 0;i < ARRAY_SIZE(item_lst);++i)
	{
		ItemPro& ref = item_lst[i];
		if(ref.item_id == 0) continue;
		pb::UsedItem *data = msg.add_item_used();
		data->set_itemid(ref.item_id);
		data->set_count(ref.count);
		data->set_used_time(ref.use_time);
	}
}
void BaoQiTable::LoadFrom(const pb::GS2C_BaoQi_All_Info& msg)
{
    for (int i = 0; i < msg.property_size(); ++i) {
        const pb::BaoQiPro& data = msg.property(i);
        pro_lst[i].level = data.level();
        pro_lst[i].exp = data.exp();
        pro_lst[i].star = data.star();
        pro_lst[i].have_hole = data.have_hole();
        pro_lst[i].gem = data.gem();
    }
	for(int i = 0;i < msg.item_used_size();++i)
	{
		const pb::UsedItem& data = msg.item_used(i);
		if(data.count() == 0) continue;
		item_lst[i].item_id = data.itemid();
		item_lst[i].use_time = data.used_time();
		item_lst[i].count = data.count();
	}
}
