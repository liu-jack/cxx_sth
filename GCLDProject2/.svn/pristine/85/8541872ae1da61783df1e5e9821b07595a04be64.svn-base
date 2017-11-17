#pragma once

#include "def/TypeDef.h"
class Player;
namespace pb{
	class GS2C_Module_Buy_Gain_Mode_Res;
	class GS2C_Building_State;
}

class PlayerBuildingLogic {
public:
	typedef uint32 ErrCode ;
public:
	static void HandlePlayerBuyGainMode(Player& player, uint32 modeProid, pb::GS2C_Module_Buy_Gain_Mode_Res& rspMsg);

	static void HandleGetMouleInfo(Player& player,uint32 moduleId, pb::GS2C_Building_State& logInfo);

	static ErrCode HandlePlayerBuyLabor(Player& player);

	
	static ErrCode HandlePlayerStartRebuild(Player& player);

	static ErrCode HandlePlayerImperialRebuildInvest(Player& player,uint32 investType);

	static ErrCode HandlePlayerAccelerateRebuild(Player& player);

	static ErrCode HandlePlayerHarvestFood(Player& player);

	static ErrCode HandlePlayerDeepGem(Player& player);

	static ErrCode HandlePlayerSupportHavest(Player& player);
};