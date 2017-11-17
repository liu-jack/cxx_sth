
/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#ifndef __DAILY_QUEST__H
#define __DAILY_QUEST__H
#include "Table/DailyQuestTableMgr.h"
#include "def/TypeDef.h"
#include "Base.h"
#include "DailyQuest/daily_quest_def.h"
#include "boost/noncopyable.hpp"
namespace pb
{
	class GxDB_Daily_Quest;
	class GS2C_Daily_Quest_All_Info;
}
class Player;
struct DailyQuestStruct {
	uint32  finish_times;						//研究结束时间
	uint32  is_finished;
	DailyQuestStruct(){ ZeroMemoryThis; }
};
typedef std::map<uint32, ::DailyQuestStruct>     DailyQuestStructMap;
class DailyQuestLog :public boost::noncopyable
{
	DailyQuestStructMap daily_quest_struct_map_;
public:
	DailyQuestLog();
	~DailyQuestLog();
	void Init();
	void SaveTo(pb::GxDB_Daily_Quest& msg);
	void SaveTo(Player& player,pb::GS2C_Daily_Quest_All_Info& msg);
	void LoadFrom(const pb::GxDB_Daily_Quest& msg);
	//bool CheckDailyQuestFinished(const DailyQuestType type,const uint32 city_id) const ;
	void UpdateDailyQuestData(Player& player,const uint32 quest_id,const uint32 times);
	DailyQuestStruct& GetDailyQuestStruct(const uint32 quest_id);
	const uint32 GetScoreFromType(const uint32 quest_id) const;
	void SendUpdateInfoToClient(Player& player,const uint32 quest_id );

	const uint32 GetScoreFromQuestId(const uint32 quest_id) const;
	bool CheckDailyQuestFinished(const uint32 quest_id) const ;
	DailyQuestStructMap& GetMutableDailyQuestStructMap();

	void GM_finish(Player& player,uint32 quest_id );
};

#endif