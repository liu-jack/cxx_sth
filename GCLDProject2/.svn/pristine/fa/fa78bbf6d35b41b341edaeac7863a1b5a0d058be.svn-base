/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#ifndef __GROW_UP_QUEST__H
#define __GROW_UP_QUEST__H
#include "Table/GrowUpQuestTableMgr.h"
#include "def/TypeDef.h"
#include "Base.h"
#include "boost/noncopyable.hpp"
class Player;
#include "Quest.pb.h"
#include "../CrossLogic/PlayerGrowUpQuestDB.h"
namespace pb
{
	class GS2C_Grow_Up_Quest_All_Info;
	class GxDB_Grow_Up_Quest;
	enum GrowUpQuestType;
}

struct GrowUpQuestStruct
{
	uint32 group_id;
	uint32 quest_type;
	uint32 cur_value;
	uint32 is_finished;
	uint32 is_take_reward;
	GrowUpQuestStruct() {ZeroMemoryThis;}
};

struct GrowUpQuestGroupStruct
{
	//uint32 group_id;
	uint32 is_finished;
	uint32 is_take_group_reward;
	GrowUpQuestGroupStruct() {ZeroMemoryThis;}

};
typedef std::map<uint32,GrowUpQuestStruct> GrowUpQuestStructMap;
typedef std::map<uint32,GrowUpQuestGroupStruct> GrowUpQuestGroupStructMap;

class GrowUpQuestLog : public boost::noncopyable
{
private:
	GrowUpQuestStructMap grow_up_quest_struct_map_;
	GrowUpQuestGroupStructMap grow_up_quest_group_struct_map_;
public:
	void Init();
	GrowUpQuestLog();
	~GrowUpQuestLog();
	void SaveTo(pb::GxDB_Grow_Up_Quest& msg);
	void SaveTo(pb::GxDB_Grow_Up_Quest_Group& msg);

	void SaveTo(Player & player,pb::GS2C_Grow_Up_Quest_All_Info& msg);
	void LoadFrom(const pb::GxDB_Grow_Up_Quest& msg);
	void LoadFrom(const pb::GxDB_Grow_Up_Quest_Group& msg);

	GrowUpQuestStructMap& GetGrowUpQuestStructMap();
	GrowUpQuestGroupStructMap& GetGrowUpQuestGroupStructMap();
	
	void UpdateGrowUpData(Player* player,pb::GrowUpQuestType type,const uint32 value);
	void UpdateGroupData(Player* player,const uint32 GroupId);
	bool CheckQuestFinished(const uint32 quest_id) const;
	bool CheckGroupFinished(const uint32 group_id) const;
	void SendUpdateDataToClient(Player& player,const uint32 quest_id);
	void SendUpdateGroupDataToClient(Player& player,const uint32 GroupId);
	
	//GrowUpQuestStruct GetGrowUpQuestStruct(const uint32 quest_id);
	//GrowUpQuestGroupStruct GetGrowUpQuestGroupStruct(const uint32 group_id);
	bool CanTakeReward(pb::GrowUpRewardType type,const uint32 quest_id);
	bool CanSendRewardEmail() const;
	void MarkTheLastQuestCompleted(Player* player);


	void GM_Finish(Player& player,const uint32 quest_id);
	void GM_FinishGroup(Player& player,const uint32 group_id);
	void Clear(Player& player);
};

#endif