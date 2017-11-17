#pragma once
#include "def/TypeDef.h"
#include "ptr_container/PtrMap.h"

struct DB_Altar;

class AltarTable {
    IntPairVec          m_RewardWeight;
    IntPairVec          m_MaxRewardWeight;
public:
    const DB_Altar&     m_db;
public:
    AltarTable(const DB_Altar& db);

    int RandAddSoldierTimes(int level) const; //按权重随机奖励增兵次数
	uint32 AltarStudyCd();
};

class AltarTableMgr {
private:
    typedef PtrMap<int, const AltarTable> AltarMap;
private:
    AltarMap    m_AltarTable;
    uint32      m_MaxAltarLv;
public:
    static AltarTableMgr& Instance(){ static AltarTableMgr T; return T; }

    void Init();
    const AltarTable* GetAltar(int lv);
    uint32 GetMaxAltarLv() { return m_MaxAltarLv; }
};
#define sAltarTableMgr AltarTableMgr::Instance()