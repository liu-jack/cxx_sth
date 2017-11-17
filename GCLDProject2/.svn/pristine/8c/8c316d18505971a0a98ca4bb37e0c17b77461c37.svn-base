#pragma once
#include "def/TypeDef.h"
#include "ptr_container/PtrMap.h"

struct DB_Palace_Proto;

class PalaceTableMgr {
public:
    typedef PtrMap<int, const DB_Palace_Proto> ProtoMap;
public:
    ProtoMap        m_ProtoTable;
public:
    static PalaceTableMgr& Instance(){ static PalaceTableMgr T; return T; }

    void Init();
    uint32  GetMaxOfficeId();

    const DB_Palace_Proto* GetProto(int id);
};
#define sPalaceTableMgr PalaceTableMgr::Instance()