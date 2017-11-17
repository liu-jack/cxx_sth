#include "TeachMap.h"
#include "DbTool.h"
#include "utility/Utility.h"
#include "System.h"

TeachMap::TeachMap( const DB_TeachMapProto& db )
:m_dbTeachMapProto(db)
{
	Utility::SplitStr(m_dbTeachMapProto.teach_value, teachValue, ',');

	IntPairVec tmppair;
	Utility::SplitStr2(m_dbTeachMapProto.city_initialize, tmppair);
	for(IntPairVec::iterator it = tmppair.begin(); it != tmppair.end(); it++)
	{
		std::vector<uint32>& cityvec = city_initialize[it->second];
		cityvec.push_back(it->first);
	}

	IntV4Vec tmpv4att;
	Utility::SplitStr4(m_dbTeachMapProto.city_attack_npc, tmpv4att);
	for(IntV4Vec::iterator ite = tmpv4att.begin(); ite != tmpv4att.end(); ite++)
	{
		npcinfo npcone;
		npcone.protoid = ite->first;
		npcone.level = ite->second;
		npcone.num = ite->third;
		city_attack_lst.insert(std::make_pair(ite->fourth, npcone));
	}

	IntV4Vec tmpv4def;
	Utility::SplitStr4(m_dbTeachMapProto.city_defense_npc, tmpv4def);
	for(IntV4Vec::iterator itr = tmpv4def.begin(); itr != tmpv4def.end(); itr++)
	{
		npcinfo npcone;
		npcone.protoid = itr->first;
		npcone.level = itr->second;
		npcone.num = itr->third;
		city_defend_lst.insert(std::make_pair(itr->fourth, npcone));
	}
}

void TeachMapTableMgr::Init()
{
	FOREACH_DB_ITEM(ptr1, DB_TeachMapProto)
	{
		m_TeachMapTable[ptr1->id] = new TeachMap(*ptr1);
	}

	FOREACH_DB_ITEM(ptr2, DB_TeachWorld)
	{
		m_TeachWorldTable[ptr2->countryid] = new TeachWorld(*ptr2);
	}
}

TeachMap* TeachMapTableMgr::GetTeachMap(int id)
{
	return m_TeachMapTable.pfind(id);
}

void TeachMapTableMgr::GetTaskIdMap(std::map<uint32,uint32>& taskmap)
{
	for(TeachMapPtr::iterator it = m_TeachMapTable.begin(); it != m_TeachMapTable.end(); it++)
	{
		taskmap[it->first] = it->second->QuestId();
	}
}

uint32 TeachMapTableMgr::GetLastInitTeachId(uint32 teachid)
{
	TeachMapPtr::reverse_iterator it = m_TeachMapTable.rbegin();
	for(; it != m_TeachMapTable.rend(); it++)
	{
		TeachMap* teachone = it->second;
		if(teachone->IsRefresh() == 1 && it->first<=(int)teachid)
			return it->first;
	}
	return 0;
}

uint32 TeachMapTableMgr::GetNextTeachId(uint32 teachid)
{
	TeachMapPtr::iterator it = m_TeachMapTable.find(teachid);
	if(it != m_TeachMapTable.end())
	{
		it++;
		if(it != m_TeachMapTable.end())
		{
			return it->first;
		}
	}
	return 0;
}

TeachWorld::TeachWorld( const DB_TeachWorld& db )
:m_dbTeachWorld(db)
{
	Utility::SplitStr(m_dbTeachWorld.teach_map, citylst, ',');

}

TeachWorld* TeachMapTableMgr::GetTeachWorld(uint32 countryid)
{
	return m_TeachWorldTable.pfind(countryid);
}