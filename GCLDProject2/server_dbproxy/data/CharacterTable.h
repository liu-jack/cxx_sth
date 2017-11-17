#pragma once

#include "structDef.h"
#include "Enum.pb.h"


namespace pb
{
    class GS2C_CharacterCreate;
    class GS2C_CharacterUpdate;
    class GS2C_BattleCharacter;
}

//set byte alignment 
#pragma pack(push,1)

struct CharacterTable : public BaseStruct
{
public:
	
	static const int CharacterNormalAttrCount = pb::CHAR_FIELD_JX_LUCK_VALUE;

public:
	static const bool HasIndex()
	{
		return true;
	}
	
	INIT_GAMEDATA( CharacterTable ); 

	const Key& GetKey( void ) const
	{
		return id ;
	}

    uint32 GetLogicalKey( void) const
    {
        return char_id;
    }

    void SetDbIncreaseId(const IdType& id_)
    {
        id = id_;
    }

	static const char* GetDbIncreaseIdName()
	{
		return "id";
	}

	static const char* GetLogicalKeyName( void )
	{
		return "id" ;
	}

	static const char* GetTableName()
	{
		return "characters";
	}

	static const char* GetFormat()
	{
		return "bbuuu"
            "uuuuu"
            "uuuuu"
            "uuuuu"
            "uuuuu"
			"uu"
            "ub";
	}

    void SaveTo(pb::GS2C_CharacterCreate& msg, int dataFlag =0);
    void LoadFrom(const pb::GS2C_CharacterCreate& msg);
    void LoadUpdateFrom(const pb::GS2C_CharacterUpdate& msg, bool* remainInfo = NULL );
    void GetEquipIds( std::set<uint32>& ids ) const; 

public:
	IdType      id ;
	uint64      player_id;
	uint32      char_id;
    uint32      char_proto_id;
	uint32      juexing;
    int         attr[CharacterNormalAttrCount];
    uint32      cur_city_id;
    uint64      dungeon_owner_id;
} ;

struct CharacterJueXingTable : public BaseStruct
{
public:
	

	static const int CharacterJueXingAttrCount = pb::CHAR_FIELD_JX_DEFENSE - pb::CHAR_FIELD_JX_LUCK_VALUE +1;
public:
	static const bool HasIndex()
	{
		return true;
	}

	INIT_GAMEDATA( CharacterJueXingTable ); 

	const Key& GetKey( void ) const
	{
		return id ;
	}

	uint32 GetLogicalKey( void) const
	{
		return char_id;
	}

	void SetDbIncreaseId(const IdType& id_)
	{
		id = id_;
	}

	static const char* GetDbIncreaseIdName()
	{
		return "id";
	}

	static const char* GetLogicalKeyName( void )
	{
		return "id" ;
	}

	static const char* GetTableName()
	{
		return "character_juexing";
	}

	static const char* GetFormat()
	{
		return "bbut"
			"uuuu";
	}

	void SaveTo(pb::GS2C_CharacterCreate& msg);
	void LoadFrom(const pb::GS2C_CharacterCreate& msg);
	void LoadUpdateFrom(const pb::GS2C_CharacterUpdate& msg);

public:
	IdType      id ;
	uint64      player_id;
	uint32      char_id;
	time_t      juexing_time;
	int         attr[CharacterJueXingAttrCount];       
} ;
extern CharacterJueXingTable  emptyCharaJuXing;

struct CharacterTrainingTable : public BaseStruct
{
public:
    

    static const int CharacterTrainingAttrCount = 15;//pb::CHAR_FIELD_TRAINING_COUNT_2 - pb::CHAR_FIELD_0 + 1;
public:
    static const bool HasIndex()
    {
        return true;
    }

    INIT_GAMEDATA( CharacterTrainingTable ); 

    const Key& GetKey( void ) const
    {
        return id ;
    }

    uint32 GetLogicalKey( void) const
    {
        return char_id;
    }

    void SetDbIncreaseId(const IdType& id_)
    {
        id = id_;
    }

    static const char* GetDbIncreaseIdName()
    {
        return "id";
    }

    static const char* GetLogicalKeyName( void )
    {
        return "id" ;
    }

    static const char* GetTableName()
    {
        return "characters_training";
    }

    static const char* GetFormat()
    {
        return "bbu"
            "uuuuu"
            "uuuuu"
            "uuuuu";
    }

    void SaveTo(pb::GS2C_CharacterCreate& msg);
    void LoadFrom(const pb::GS2C_CharacterCreate& msg);
    void LoadUpdateFrom(const pb::GS2C_CharacterUpdate& msg);

public:
    IdType      id ;
    uint64      player_id;
    uint32      char_id;
    int         attr[CharacterTrainingAttrCount];       
} ;

//extern CharacterTrainingTable emptyCharaTraining;



struct BattleArrayTable : public BaseStruct
{
public:
    

public:
    static const bool HasIndex()
    {
        return true;
    }

    INIT_GAMEDATA( BattleArrayTable ) ;

    const Key& GetKey( void ) const
    {
        return id ;
    }

    void SetDbIncreaseId(const IdType& id_)
    {
        id = id_;
    }

    static const char* GetDbIncreaseIdName()
    {
        return "id";
    }



    static const char* GetLogicalKeyName( void )
    {
        return "id" ;
    }

    static const char* GetTableName()
    {
        return "battle_array";
    }

    static const char* GetFormat()
    {
        return "bb"
            "uuuuu";
    }

    void SaveTo(pb::GS2C_BattleCharacter& msg);
    void LoadFrom(const pb::GS2C_BattleCharacter& msg);
    static const int battle_array_size = 5;
public:
    IdType      id ;
    uint64      player_id;
    uint32      char_id[battle_array_size];
} ;



#pragma pack(pop)
