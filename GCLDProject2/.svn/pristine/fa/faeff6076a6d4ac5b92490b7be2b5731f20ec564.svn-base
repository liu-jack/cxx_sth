#pragma once
#include "def/TypeDef.h"
#include "ptr_container/PtrMap.h"
#include "datastruct/struct_bao_qi.h"

#define BaoQi_MaxCnt 6

class BaoQiTable {
public:
    const DB_BaoQi&     m_db;
    IntPairVec m_CriticalWeight[BaoQi_MaxCnt]; //²»±©»÷¡¢2±¶±©»÷¡¢4±¶±©»÷¡¢10±¶±©»÷
public:
    explicit BaoQiTable(const DB_BaoQi& db);
};



class BaoQiTableMgr {
private:
    typedef PtrMap<int, const BaoQiTable>     BaoQiUpMap;
    typedef PtrMap<int, const DB_BaoQi_Unlock> BaoQiUnlockMap;
    typedef PtrMap<int, const DB_BaoQi_UpStar> BaoQiUpStarMap;
private:
    BaoQiUpMap      m_BaoQiMap;
    BaoQiUnlockMap  m_UnlockMap;
    BaoQiUpStarMap  m_UpStarMap;
    uint32          m_BaoQiMaxLv;
public:
    static BaoQiTableMgr& Instance(){ static BaoQiTableMgr T; return T; }

    void Init();
    const DB_BaoQi::Property* GetBaoQi(int id, int lv);
    const DB_BaoQi_Unlock* GetBaoQiUnlock(int id);
    const DB_BaoQi_UpStar* GetBaoQiUpStar(int tid);
    uint32 GetBaoQiMaxLv(){ return m_BaoQiMaxLv; }
    int RandCriticaMulti(int id, int lv,int value = 0);
};
#define sBaoQiTableMgr BaoQiTableMgr::Instance()