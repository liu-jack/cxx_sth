/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#include "GrowUpQuest/Table/GrowUpQuestTableMgr.h"
#include "utility/Utility.h"
#include "DbTool.h"

GrowUpQuestProto::GrowUpQuestProto( const DB_GrowUpQuestProto& grow_up_quest_proto )
:db_grow_up_quest_proto_(grow_up_quest_proto)
{

}

uint32 GrowUpQuestProto::Id() const
{
	return db_grow_up_quest_proto_.id;
}

uint32 GrowUpQuestProto::QuestGroup() const
{
	return db_grow_up_quest_proto_.quest_group;
}

uint32 GrowUpQuestProto::QuestType() const
{
	return db_grow_up_quest_proto_.quest_type;
}

uint32 GrowUpQuestProto::Value1() const
{
	return db_grow_up_quest_proto_.val1;
}

uint32 GrowUpQuestProto::DescriptionId() const
{
	return db_grow_up_quest_proto_.description_id;
}

bool GrowUpQuestProto::operator==( const GrowUpQuestProto& other )
{
	return QuestGroup() == other.QuestGroup() && QuestType() == other.QuestType();
}


GrowUpQuestGroup::GrowUpQuestGroup( const DB_GrowUpQuestGroup& group_up_quest_group )
:db_group_up_quest_group_(group_up_quest_group)
{

}

uint32 GrowUpQuestGroup::Id() const
{
	return db_group_up_quest_group_.id;
}

uint32 GrowUpQuestGroup::QuestGroup() const
{
	return db_group_up_quest_group_.quest_group;
}

uint32 GrowUpQuestGroup::OpenTime() const
{
	return db_group_up_quest_group_.open_time;
}

uint32 GrowUpQuestGroup::GroupNameId() const
{
	return db_group_up_quest_group_.group_name_id;
}

// std::string& GrowUpQuestGroup::Reward() const
// {
// 	return db_group_up_quest_group_.reward;
// }


void GrowUpQuestTableMgr::Init()
{
	FOREACH_DB_ITEM(grow_up_proto,DB_GrowUpQuestProto)
	{
		uint32 Id = grow_up_proto->id;
		GrowUpQuestProto * growUpProto = new GrowUpQuestProto(*grow_up_proto);
		grow_up_quest_proto_map_[Id] = growUpProto;
		IntPairVec vec;
		Utility::SplitStr2(grow_up_proto->reward, vec);
		grow_up_quest_proto_reward_.insert(std::pair<uint32,IntPairVec>(Id,vec));
	}

	FOREACH_DB_ITEM(grow_up_group,DB_GrowUpQuestGroup)
	{
		//uint32 Id = grow_up_group->id;
		GrowUpQuestGroup * growUpGroup = new GrowUpQuestGroup(*grow_up_group);
		grow_up_quest_group_map_[grow_up_group->quest_group] = growUpGroup;
		IntPairVec vec;
		Utility::SplitStr2(grow_up_group->reward, vec);
		grow_up_quest_group_reward_.insert(std::pair<uint32,IntPairVec>(grow_up_group->quest_group,vec));
	}
}

bool GrowUpQuestTableMgr::GetGrowUpProtoRewardById( const uint32 id,IntPairVec* vec ) const
{
	GrowUpQuestRewardVec::const_iterator Iter = grow_up_quest_proto_reward_.find(id);
	if(Iter != grow_up_quest_proto_reward_.end())
	{
		*vec = Iter->second;
		return true;
	}
	return false;
}

bool GrowUpQuestTableMgr::GetGrowUpGroupRewardById( const uint32 id,IntPairVec* vec ) const
{
	GrowUpQuestRewardVec::const_iterator Iter = grow_up_quest_group_reward_.find(id);
	if(Iter != grow_up_quest_group_reward_.end())
	{
		*vec = Iter->second;
		return true;
	}
	return false;
}

const GrowUpQuestProto* GrowUpQuestTableMgr::GetGrowUpQuestProto( const uint32 id ) const
{
	return grow_up_quest_proto_map_.pfind(id);
}

const GrowUpQuestGroup* GrowUpQuestTableMgr::GetGrowUpQuestGroup( const uint32 group_id ) const
{
	return grow_up_quest_group_map_.pfind(group_id);
}

const GrowUpQuestProtoMap& GrowUpQuestTableMgr::GetGrowUpQuestProtoMap() const
{
	return grow_up_quest_proto_map_;
}

const GrowUpQuestGroupMap& GrowUpQuestTableMgr::GetGrowUpQuestGroupMap() const
{
	return grow_up_quest_group_map_;
}

const uint32 GrowUpQuestTableMgr::GetQuestIdByGroupIdAndType( const uint32 GroupId,const pb::GrowUpQuestType type,const uint32 value ) const
{
	for(GrowUpQuestProtoMap::const_iterator iter = grow_up_quest_proto_map_.begin();iter != grow_up_quest_proto_map_.end();)
	{
		GrowUpQuestProtoMap::const_iterator old_iter = iter;
		GrowUpQuestProtoMap::const_iterator new_iter = ++old_iter;
		if(iter->second->QuestGroup() == GroupId && iter->second->QuestType() == static_cast<uint32>(type))
		{
			if(LIKELY(new_iter != grow_up_quest_proto_map_.end()))
			{
				if(*(new_iter->second) == *(iter->second))
				{
					if(value <= iter->second->Value1() )
						return iter->first;
					else if(value > iter->second->Value1() && value < new_iter->second->Value1())
					{
						return new_iter->first;
					}
				}
				else
				{
					return iter->first;
				}
			}
			else
				return iter->first;
		}
		iter++;
	}
	return 0;
}


const GrowUpQuestRewardVec& GrowUpQuestTableMgr::GetGrowUpQuestProtoRewardVec() const 
{
	return grow_up_quest_proto_reward_;
}

const GrowUpQuestRewardVec& GrowUpQuestTableMgr::GetGrowUpQuestGroupRewardVec() const
{
	return grow_up_quest_group_reward_;
}

const uint32 GrowUpQuestTableMgr::GetGroupIdFromType( const uint32 type) const
{
	for(GrowUpQuestProtoMap::const_iterator iter = grow_up_quest_proto_map_.begin();iter != grow_up_quest_proto_map_.end();++iter)
	{
		if(iter->second->QuestType() == type)
		{
			return iter->second->QuestGroup();
		}
	}
	return 0;
}

const uint32 GrowUpQuestTableMgr::GetOpenTimeFromGroupId(const uint32 group_id) const
{
	GrowUpQuestGroupMap::const_iterator iter = grow_up_quest_group_map_.find(group_id);
	if(iter != grow_up_quest_group_map_.end())
	{
		return iter->second->OpenTime();
	}
	return 0;
}

const uint32 GrowUpQuestTableMgr::getMaxValueFromGroupId(const uint32 groupId)
{
	uint32 max_value = 0;
	for(GrowUpQuestProtoMap::const_iterator iter = grow_up_quest_proto_map_.begin();iter != grow_up_quest_proto_map_.end();++iter)
	{
		if(iter->second->QuestGroup() == groupId)
		{
			if(iter->second->Value1() > max_value)
			{
				max_value = iter->second->Value1();
			}
		}
	}
	return max_value;
}
