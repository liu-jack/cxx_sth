#pragma once

#include "def/TypeDef.h"

class Player;
struct DB_StageLevel;
class StageLevelLog;
class StageLog;
class StageLogger;
class PlayerValueOwner;

namespace pb
{
    class GS2C_Reward;
    class C2GS_BattleData;
    class GS2C_BattleResult;
    class GS2C_StageLevel;
}

class PlayerStageLogic
{
public:
    static void GiveRewardToPlayer(
        Player& player, const DB_StageLevel& sl, pb::GS2C_Reward* reward ,
        bool bGiveItem = true, float factor = 1.0f);
    static StageLevelLog* AddOrGetStageLevelLog( Player& player, uint32 level_id);
    static void CalculateBattleResult( Player& player, const pb::C2GS_BattleData& fromData, pb::GS2C_BattleResult& outResult);
    static void GmClearLevelById( Player& player, uint32 level_id);

    static bool TryCost( Player& player, const DB_StageLevel& level, uint32 count);
private:
    static void SendStageLogToDb( Player& player, const StageLog& log);

};


class PlayerStageLevelLogic
{
public:
    static void CalculateBattleResult(
        Player& player, StageLogger& stageloger,  StageLevelLog& log, 
        const pb::C2GS_BattleData& fromData, pb::GS2C_BattleResult& outResult 
        );
    static void SendStageLevelLogToDb( Player& player, const StageLevelLog& log);
private:
    static void OnFinishLevel( Player &player, StageLevelLog& log, pb::GS2C_BattleResult &outResult);
    static void TriggerQuestLevelFinish( Player &player, const DB_StageLevel& proto, int count = 1);
    static void TriggerQuestBranchSuccess( Player &pPlayer, const DB_StageLevel& proto);
};