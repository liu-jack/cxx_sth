/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#ifndef __GROW_UP_QUEST_TABLE_MGR__H
#define __GROW_UP_QUEST_TABLE_MGR__H

#include "SimpleSingleton.h"
#include "ptr_container/PtrMap.h"
#include "game_data/datastruct/struct_grow_up_quest.h"
#include <map>
#include "Quest.pb.h"

class Player;

class GrowUpQuestProto
{
private:
	const struct DB_GrowUpQuestProto& db_grow_up_quest_proto_;
public:
	GrowUpQuestProto(const DB_GrowUpQuestProto& grow_up_quest_proto);
	uint32 Id() const;
	uint32 QuestGroup() const;
	uint32 QuestType() const;
	uint32 Value1() const;
	uint32 DescriptionId() const;
	bool operator==(const GrowUpQuestProto& other);
	//std::string& Reward() const;
};

class GrowUpQuestGroup
{
private:
	const struct DB_GrowUpQuestGroup& db_group_up_quest_group_;
public:
	GrowUpQuestGroup(const DB_GrowUpQuestGroup& group_up_quest_group);
	uint32 Id() const;
	uint32 QuestGroup() const;
	uint32 OpenTime() const;
	uint32 GroupNameId() const;
	//std::string& Reward() const;
};

typedef PtrMap<uint32,GrowUpQuestProto> GrowUpQuestProtoMap;
typedef PtrMap<uint32,GrowUpQuestGroup> GrowUpQuestGroupMap;
typedef std::map<uint32,IntPairVec> GrowUpQuestRewardVec;


class GrowUpQuestTableMgr : public SimpleSingleton<GrowUpQuestTableMgr>
{
private:
	GrowUpQuestProtoMap grow_up_quest_proto_map_;
	GrowUpQuestGroupMap grow_up_quest_group_map_;
	GrowUpQuestRewardVec grow_up_quest_proto_reward_;
	GrowUpQuestRewardVec grow_up_quest_group_reward_;


public:
	void Init();
	static GrowUpQuestTableMgr& Instance(){ static GrowUpQuestTableMgr T; return T; }	//��̬����
	bool GetGrowUpProtoRewardById(const uint32 id,IntPairVec* vec) const;
	bool GetGrowUpGroupRewardById(const uint32 id,IntPairVec* vec) const;
	const GrowUpQuestProtoMap& GetGrowUpQuestProtoMap() const;
	const GrowUpQuestGroupMap& GetGrowUpQuestGroupMap() const;
	const GrowUpQuestProto* GetGrowUpQuestProto(const uint32 id) const;
	const GrowUpQuestGroup* GetGrowUpQuestGroup(const uint32 group_id) const;
	const GrowUpQuestRewardVec& GetGrowUpQuestProtoRewardVec() const;
	const GrowUpQuestRewardVec& GetGrowUpQuestGroupRewardVec() const;
	const uint32 GetGroupIdFromType(const uint32 type) const;
	const uint32 GetOpenTimeFromGroupId(const uint32 type) const;
	const uint32 getMaxValueFromGroupId(const uint32 groupId);
	const uint32 GetQuestIdByGroupIdAndType(const uint32 GroupId,const pb::GrowUpQuestType type,const uint32 value) const;
};

#define sGrowUpQuestTableMgr GrowUpQuestTableMgr::Instance()


#endif