#include "SellTable.h"
#include "Item.pb.h"


void SellRecordTable::SaveTo( pb::GS2C_ItemSellRecord& msg )
{
    msg.set_sell_id( sell_id);
    msg.set_sell_count( sell_count);
}

void SellRecordTable::LoadFrom( const pb::GS2C_ItemSellRecord& msg )
{
    sell_id = msg.sell_id();
    sell_count = msg.sell_count();
}

void SellRandomProgramTable::SaveTo( pb::GS2C_SellRandomProgram& msg )
{
    msg.set_victim_type( victim_type);
    msg.set_program_id( random_program_id);
    msg.set_today_manual_refresh_count( today_manual_refresh_count);
    msg.set_auto_refresh_time( auto_refresh_time);
    msg.set_last_manual_refresh_time( last_maual_refresh_time);
}

void SellRandomProgramTable::LoadFrom( const pb::GS2C_SellRandomProgram& msg )
{   
    victim_type = msg.victim_type();
    random_program_id = msg.program_id();
    today_manual_refresh_count = msg.today_manual_refresh_count();
    auto_refresh_time = msg.auto_refresh_time();
    last_maual_refresh_time = msg.last_manual_refresh_time();
}

void SellProgramInfoTable::SaveTo( pb::GS2C_SellProgramInfo& msg )
{
    msg.set_program_id( sell_program_id);
    msg.set_record_valid_time( record_valid_time);
}

void SellProgramInfoTable::LoadFrom( const pb::GS2C_SellProgramInfo& msg )
{
    sell_program_id = msg.program_id();
    record_valid_time = msg.record_valid_time();    
}
