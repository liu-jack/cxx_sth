#include "PlayerData.h"

// PlayerData.cpp 部分实现

//////////////////////////////////////////////////////////////////////////

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/typeof/typeof.hpp>
#include "data/DbProxyDataMgr.h"
#include "utility/MsgTool.h"


#include "Character.pb.h"


bool PlayerData::save_mode_character()
{
    m_CharacterTable.SaveMod();
    m_CharacterTrainingTable.SaveMod();
    return true;
} 

bool PlayerData::save_battle_array()
{
    m_BattleArrayTable.SaveMod();
    return true;
}


void PlayerData::UpdateCharacter( const pb::GS2C_CharacterUpdate& charUpdate )
{
    CharacterTable* character = m_CharacterTable.GetElement( charUpdate.id());
    if ( character)
    {
        bool remainInfo = false;
        character->LoadUpdateFrom( charUpdate, &remainInfo);
        if ( remainInfo)
        {
            if (CharacterJueXingTable* trainingChar = m_CharacterJueXingTable.GetElement(charUpdate.id()))
            {
                trainingChar->LoadUpdateFrom( charUpdate);
                m_CharacterJueXingTable.SetModifiedID( charUpdate.id());
            }
            else
            {
                CharacterJueXingTable ctt;
                ctt.char_id = character->char_id;
                ctt.LoadUpdateFrom( charUpdate);
                ctt.player_id = GetPlayerId();

                m_CharacterJueXingTable.AddAndAddToCache(&ctt);
            }
        }
        m_CharacterTable.SetModifiedID( charUpdate.id());
        SetIsDirty(DIRTY_CHARACTER);
    }
}

void PlayerData::UpdateBattleArray( const pb::GS2C_BattleCharacter& battleArrayMsg )
{
    
    if ( m_BattleArrayTable)
    {
        m_BattleArrayTable->LoadFrom(battleArrayMsg);
        SetIsDirty(DIRTY_BATTLE_ARRAY);
    }
    else
    {
        BattleArrayTable BA;
        BA.player_id = GetPlayerId();
        BA.LoadFrom( battleArrayMsg);

        m_BattleArrayTable.SetAndAddToCache( &BA);
    }
}

void PlayerData::WriteCharacterStorage( pb::GS2C_CharacterStorage& charInfo )
{
    _WriteBattleArray(charInfo);


    for ( BOOST_AUTO( it , m_CharacterTable.Begin());
        it != m_CharacterTable.End(); ++it)
    {
        CharacterTable* character = it->second;
        pb::GS2C_CharacterCreate * charMsg = MsgTool::SaveToRepeatField(character, charInfo.mutable_characters());
        if ( charMsg)
        {
            CharacterJueXingTable* charTraining = m_CharacterJueXingTable.GetElement( character->char_id);
            if ( charTraining)
            {
                charTraining->SaveTo(*charMsg);
            }
            else
            {
                emptyCharaJuXing.SaveTo( *charMsg);
            }

        }
    }
}


void PlayerData::_WriteBattleArray( pb::GS2C_CharacterStorage &charInfo )
{
    pb::GS2C_BattleCharacter* battle_array = charInfo.mutable_battle_character();
    if (m_BattleArrayTable && battle_array)
    {
        m_BattleArrayTable->SaveTo(*battle_array);    
    }
}






void PlayerData::AddCharacter( const pb::GS2C_CharacterCreate& charMsg )
{
    if ( m_CharacterTable.Contain( charMsg.id()))
    {
        ASSERT( false);
        return;
    }

    /////////////////////////////////////////////////////////////
    // for test
//#ifdef _DEBUG
//
//	size_t erseCount =m_hadDeleteCharId.erase( charMsg.id() ) ;
//
//    std::string sql;
//    Utility::FormatString( sql, "select * from characters where player_id = %llu and char_id = %d", GetPlayerId(), charMsg.id() );
//    CharacterTableCache tmp;
//    std::vector<CharacterTable*> vecCharacterTable ;
//    if( tmp.SqlLoadAndAddData( sql.c_str(), &vecCharacterTable, FlagReload ) )
//    {
//		if( erseCount == 0 ) // 当卡牌被删除，但是删除语句还在数据库线程没有执行的时候，这里会查询到重复
//		{
//			ASSERT( false);
//		}
//    }
//#endif

    CharacterTable character;
    character.player_id = GetPlayerId();
    character.LoadFrom( charMsg);
    m_CharacterTable.AddAndAddToCache( &character);
}

void PlayerData::DestoryCharacter( const pb::Uint32List& charIds )
{
    for ( int i = 0 ; i < charIds.values_size(); ++i)
    {
        uint32 charId = charIds.values(i);
        m_CharacterTable.DeleteEntry( charId);
        m_CharacterTrainingTable.DeleteEntry( charId);

//#ifdef _DEBUG
//		m_hadDeleteCharId.insert( charId ) ;
//#endif

		DLOG( "PlayerData::DestoryCharacter( player id:: %llu delte protoid %u )", GetPlayerId(), charId ) ;
    }
}

void PlayerData::GetBattleArray( std::set<uint32>& charIds ) const
{
    if( m_BattleArrayTable)
    {        
        for ( int i = 0; i < BattleArrayTable::battle_array_size; ++i)
        {
            if ( m_BattleArrayTable->char_id[i] != 0)
            {
                charIds.insert( m_BattleArrayTable->char_id[i]);
            }             
        }
    }
}



void PlayerData::GetEquip( const std::set<uint32>& charIds, std::set<uint32>& equipIds ) const
{
    for ( std::set<uint32>::const_iterator it = charIds.begin();
        it != charIds.end(); ++it)
    {
        const CharacterTable* character = m_CharacterTable.GetElement(*it);
        if ( character )
        {
            character->GetEquipIds( equipIds);
        }
    }
}
