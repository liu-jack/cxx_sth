#ifndef MODULE_TABLE_H__
#define MODULE_TABLE_H__

#include "structDef.h"


namespace pb
{
    class GxDB_Module;
    class GxDB_Building;
}

//set byte alignment 
#pragma pack(push,1)


struct ModuleTable : public BaseStruct
{
public:
    

public:
    static const bool HasIndex()
    {
        return true;
    }

    INIT_GAMEDATA( ModuleTable ) ; 

    const Key& GetKey( void ) const
    {
        return id ;
    } 

    void SetDbIncreaseId(const IdType& id_)
    {
        id = id_;
    }

    uint32 GetLogicalKey( void) const
    {
        return module_id;
    }

    static const char* GetDbIncreaseIdName()
    {
        return "id";
    }

    static const char* GetLogicalKeyName( void )
    {
        return "module_id";
    }

    static const char* GetTableName()
    {
        return "module";
    }

    static const char* GetFormat()
    {
        return "bbuutu";
    }

    void SaveTo(pb::GxDB_Module& msg);
    void LoadFrom(const pb::GxDB_Module& msg);

public:
    IdType      id ;
    uint64      player_id;
    uint32      module_id;
	uint32      gain_mod;
    time_t      expire_time;
	uint32      is_rebuild;
} ;


struct BuildingTable : public BaseStruct
{
public:
    

public:
    static const bool HasIndex()
    {
        return true;
    }

    INIT_GAMEDATA( BuildingTable ) ; 

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

	uint32 GetLogicalKey( void) const
	{
		return building_id;
	}

    static const char* GetLogicalKeyName( void )
    {
        return "build_id" ;
    }

    static const char* GetTableName()
    {
        return "building";
    }

    static const char* GetFormat()
    {
        return "bbuutu";
    }

    void SaveTo(pb::GxDB_Building& msg);
    void LoadFrom(const pb::GxDB_Building& msg);
public:
    IdType      id ;
    uint64      player_id;
    uint32      building_id;
	uint32      cur_lv;
	time_t      lvup_start_time;
	uint32      is_rebuild;
} ;
#pragma pack(pop)
#endif //MODULE_TABLE_H__