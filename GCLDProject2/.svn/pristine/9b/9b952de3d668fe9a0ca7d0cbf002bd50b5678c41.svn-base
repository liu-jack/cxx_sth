/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#pragma  once

#include "Quest.pb.h"
#include "Table/ActivtyStageRewardTableMgr.h"
#include "def/TypeDef.h"
#include "Base.h"
#include "boost/noncopyable.hpp"
#include <map>
#include "CrossLogic/PlayerPassStageReward.h"
class Player;
namespace pb
{
	class GS2C_Pass_Stage_Reward_All_Info;
	class GxDB_Pass_Stage_Reward;
}

struct PassStageRewardStruct
{
	uint32 stage_level;
	uint32 stage_is_passed;
	uint32 is_take_reward;
	PassStageRewardStruct() {ZeroMemoryThis;}
};

typedef std::map<uint32,PassStageRewardStruct> PassStageRewardStructMap;

class PassStageReward:public boost::noncopyable
{
private:
	PassStageRewardStructMap pass_stage_reward_struct_map_;
public:
	PassStageReward();
	~PassStageReward();
	void Init();
	void SaveTo(Player* player,pb::GS2C_Pass_Stage_Reward_All_Info& msg);
	void LoadFrom(const pb::GxDB_Pass_Stage_Reward& msg);
	void SaveTo(pb::GxDB_Pass_Stage_Reward& info);
	void UpdateStageIsPassed(const uint32 id);
	void UpdateStageIsTakeReward(const uint32 id);
	bool CheckStageIsPassed(const uint32 id);
	bool CheckStageIsTakeReward(const uint32 id);
	bool CanTakeReward(const uint32 id);
	PassStageRewardStructMap& GetPassStageRewardStructMap();
	void UpdateStagePassByStageLevel(Player* player,const uint32 stage_level);
	void GM_Finish(Player* player,const uint32 id);
	void Clear(Player& player);
};