#pragma once

#include "def/TypeDef.h"

class Player;
class FubenLog;
struct DB_FubenStage;
struct CrusadeLog;
struct stage_info;
struct BranchBaseInfo;
struct HideFubenBaseInfo;
class PlayerFuBenDB {
public:
    static void SendFuBenBaseToDb(Player& player, FubenLog& log);
	static void SendHideFubenToDb(Player& player, uint32 stageId, HideFubenBaseInfo& p);//
    static void SendPassStageToDb(Player& player, uint32 stage_type,stage_info* p);   //打过的特殊关
	static void SendCrusadeToDb(Player& player, FubenLog& log);
	static void SendBranchFubenToDb(Player& player, const uint32 stageId,const BranchBaseInfo& info,FubenLog& log);
};