#pragma once
#include "def/TypeDef.h"
#include "ptr_container/PtrMap.h"

struct DB_Smithy;
struct DB_Smelter;
class SmithyTableMgr {
private:
    typedef PtrMap<int, const DB_Smithy>    SmithyMap;
    typedef PtrMap<int, const DB_Smelter>   SmelterMap;
private:
    SmithyMap   m_SmithyMap;
    SmelterMap  m_SmelterMap;
    uint32      m_MaxSmithyLv;
	uint32      m_field;
public:
    static SmithyTableMgr& Instance(){ static SmithyTableMgr T; return T; }

    void Init();
    const DB_Smithy* GetSmithy(int lv);
    const DB_Smelter* GetSmelter(int field, int star);

    uint32 GetMaxSmithyLv() { return m_MaxSmithyLv; }
	uint32 GetMaxFields() { return m_field;}
};
#define sSmithyTableMgr SmithyTableMgr::Instance()