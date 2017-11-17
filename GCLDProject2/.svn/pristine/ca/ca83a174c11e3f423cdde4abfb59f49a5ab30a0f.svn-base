#pragma once
#include "def/TypeDef.h"
#include "ptr_container/PtrMap.h"
#include "game_data/datastruct/struct_teach_map.h"

struct npcinfo{
	uint32 protoid;
	uint32 level;
	uint32 num;
};

class TeachMap{
	const DB_TeachMapProto&       m_dbTeachMapProto;

public:
	TeachMap(const DB_TeachMapProto& db);

public:
	uint32 QuestId() { return m_dbTeachMapProto.questid; }
	uint32 TeachType() { return m_dbTeachMapProto.teach_type; }
	uint32 IsRefresh() { return m_dbTeachMapProto.is_fresh; }

public:
	std::vector<int> teachValue;
	std::map<uint32, std::vector<uint32>> city_initialize; //<国家id,城池id>
	std::multimap<uint32, npcinfo> city_attack_lst;	//<城池id,npc信息>
	std::multimap<uint32, npcinfo> city_defend_lst;	//<城池id,npc信息>
};

class TeachWorld{
	const DB_TeachWorld&	m_dbTeachWorld;

public:
	TeachWorld(const DB_TeachWorld& db);

public:
	uint32 Id() { return m_dbTeachWorld.id; }
	uint32 CountryId() { return m_dbTeachWorld.countryid; }

public:
	std::vector<int> citylst;
};

class TeachMapTableMgr {
public:
	typedef PtrMap<int, TeachMap> TeachMapPtr;
	typedef PtrMap<uint32, TeachWorld> TeachWorldPtr;
public:
	TeachMapPtr			m_TeachMapTable;
	TeachWorldPtr		m_TeachWorldTable;
public:
	static TeachMapTableMgr& Instance(){ static TeachMapTableMgr T; return T; }
	
    void Init();

	TeachMap* GetTeachMap(int id);
	void GetTaskIdMap(std::map<uint32,uint32>& taskmap);
	uint32 GetLastInitTeachId(uint32 teachid);
	uint32 GetNextTeachId(uint32 teachid);

	TeachWorld* GetTeachWorld(uint32 countryid);
};

#define sTeachMapTableMgr TeachMapTableMgr::Instance()
