#pragma once

#include "def/TypeDef.h"
struct DB_MapBox;

class MapBox {
    const DB_MapBox& m_dbMapBox;
public:
    IntPairVec  m_RewardLst;    //½±Àø

   explicit MapBox(const DB_MapBox& db);
    uint32 Id() const;
};
