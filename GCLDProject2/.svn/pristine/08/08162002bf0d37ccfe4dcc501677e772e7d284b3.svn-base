#pragma once

#include <vector>
#include "LootDefine.h"

class LootFilter;
class LootList;
class Player;

class LootFilterGroup
{
    typedef std::vector<LootFilter*> LootFilterArray;
    friend class LootManager;
public:
    LootFilterGroup();
public: 
    LootList* GenerateLoot(Player* player, LootParam& param ) const;
    void GenerateLoot(Player* player, LootParam& param, LootList& lootList) const;

    bool IsPriorityMod() const;

private:
    void FillLoot( Player* player, LootList* lootList, LootParam& param ,bool is_exploit = false) const; 
    void AddFilter( LootFilter* filter);
    void InitFiltersPriority();
private:
	LootFilterArray		m_filters;
    bool                m_isPriorityMod;
};