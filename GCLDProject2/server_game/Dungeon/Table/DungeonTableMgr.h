#pragma once
#include "def/TypeDef.h"
#include "ptr_container/PtrMap.h"

struct DB_Cane;
struct DB_Dungeon;

class DungeonTableMgr {
    typedef PtrMap<int, const DB_Cane>      CaneMap;
    typedef PtrMap<int, const DB_Dungeon>   DungeonMap;
private:
    CaneMap     m_CaneLst;
    DungeonMap  m_DungeonLst;
    uint32         m_MaxDungeonLv;

public:
    static DungeonTableMgr& Instance(){ static DungeonTableMgr T; return T; }

    void Init();

    const DB_Cane* GetCane(uint32 lv);
    const DB_Dungeon* GetDungeon(uint32 lv);
    uint32 GetMaxDungeonLv() { return m_MaxDungeonLv; }
};
#define sDungeonTableMgr DungeonTableMgr::Instance()