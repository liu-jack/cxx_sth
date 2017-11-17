#include "CharacterTable.h"

#include "Character.pb.h"
#include "Social.pb.h"
#include "utility/MsgTool.h"
#include "def/MmoAssert.h"

//CharacterTrainingTable emptyCharaTraining;
CharacterJueXingTable  emptyCharaJuXing;

void CharacterTable::SaveTo( pb::GS2C_CharacterCreate& msg, int dataFlag /*=0*/ )
{
    msg.set_id( char_id);
    msg.set_protoid( char_proto_id);
	msg.set_isjuexing(juexing !=0 ? true:false);
	switch( dataFlag )
	{
	case pb::PALYER_BASE_DATA_ALL:
		{
			pb::Int32List* intlist = msg.mutable_values();
			for ( int i = 0; i < CharacterNormalAttrCount; ++i )
			{
				intlist->add_values( attr[i]);
			}
		} break ;
	}
    msg.set_cur_city_id(cur_city_id);
    msg.set_dungeon_owner(dungeon_owner_id);
	//msg.set_isincombat(isInCombat==1);
	//msg.set_combatgroupid(combatGroupId);
}

void CharacterTable::LoadFrom( const pb::GS2C_CharacterCreate& msg )
{
    char_id = msg.id();
    char_proto_id = msg.protoid();
    const pb::Int32List &intlist = msg.values();
    for ( int i = 0; i < CharacterNormalAttrCount; ++i )
    {
        if ( i < intlist.values_size())
        {
            attr[i] = intlist.values(i);
        }        
    }
    cur_city_id = msg.cur_city_id();
    dungeon_owner_id = msg.dungeon_owner();
	//isInCombat = (msg.isincombat()==true? 1:0);
	//combatGroupId = msg.combatgroupid();
}

void CharacterTable::LoadUpdateFrom( const pb::GS2C_CharacterUpdate& msg , bool* remainInfo /*= NULL*/ )
{
    MsgTool::SaveUpdateMsgTo( msg.update_values(), attr, CharacterNormalAttrCount, 0, remainInfo);

	//if(msg.has_isincombat())
	//{
	//	isInCombat = (uint8)msg.isincombat();
	//}

	//if(msg.has_cur_city_id())
	//{
	//	cur_city_id = msg.cur_city_id();
	//}

	//if(msg.has_combatgroupid())
	//{
	//	combatGroupId == msg.combatgroupid();
	//}
}

void CharacterTable::GetEquipIds( std::set<uint32>& ids ) const
{
    for ( uint32 i = pb::CHAR_FIELD_WEAPON; i <= pb::CHAR_FIELD_ARMY; ++i)
    {
        if ( attr[i] != 0)
        {
            ids.insert( attr[i]);
        }
    }
}

//////////////////////////////////////////////////////////////////////////
void CharacterJueXingTable::SaveTo( pb::GS2C_CharacterCreate& msg )
{
	msg.set_juxingtime(juexing_time);
	pb::Int32List* intlist = msg.mutable_values();
	MMO_ASSERT( intlist->values_size() > 0);
	for ( int i = 0; i < CharacterJueXingTable::CharacterJueXingAttrCount; ++i )
	{
		intlist->add_values( attr[i]);
	}
}

void CharacterJueXingTable::LoadFrom( const pb::GS2C_CharacterCreate& msg )
{
	char_id = msg.id();
	juexing_time = msg.juxingtime();
	const pb::Int32List &intlist = msg.values();
	for ( int i = 0; i < CharacterJueXingTable::CharacterJueXingAttrCount; ++i )
	{
		if ( i < intlist.values_size())
		{
			attr[i] = intlist.values(i + CharacterJueXingTable::CharacterJueXingAttrCount);
		}        
	} 
}

void CharacterJueXingTable::LoadUpdateFrom( const pb::GS2C_CharacterUpdate& msg )
{
	if(msg.has_juxingtime())
		juexing_time = msg.juxingtime();
	MsgTool::SaveUpdateMsgTo( msg.update_values(), attr, CharacterJueXingAttrCount,
		CharacterTable::CharacterNormalAttrCount);
}



//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
void CharacterTrainingTable::SaveTo( pb::GS2C_CharacterCreate& msg )
{

    pb::Int32List* intlist = msg.mutable_values();
    MMO_ASSERT( intlist->values_size() > 0);
    for ( int i = 0; i < CharacterTable::CharacterNormalAttrCount; ++i )
    {
        intlist->add_values( attr[i]);
    }
}

void CharacterTrainingTable::LoadFrom( const pb::GS2C_CharacterCreate& msg )
{
    char_id = msg.id();
    const pb::Int32List &intlist = msg.values();
    for ( int i = 0; i < CharacterTable::CharacterNormalAttrCount; ++i )
    {
        if ( i < intlist.values_size())
        {
            attr[i] = intlist.values(i + CharacterTable::CharacterNormalAttrCount);
        }        
    } 
}

void CharacterTrainingTable::LoadUpdateFrom( const pb::GS2C_CharacterUpdate& msg )
{
    MsgTool::SaveUpdateMsgTo( msg.update_values(), attr, CharacterTrainingAttrCount,
        CharacterTable::CharacterNormalAttrCount);
}



//////////////////////////////////////////////////////////////////////////


void BattleArrayTable::SaveTo( pb::GS2C_BattleCharacter& msg )
{
    MsgTool::SaveVecToMsg( char_id, battle_array_size, *msg.mutable_battle_array());
}

void BattleArrayTable::LoadFrom( const pb::GS2C_BattleCharacter& msg )
{
    MsgTool::SaveMsgToVec( msg.battle_array(), char_id, battle_array_size);
}


