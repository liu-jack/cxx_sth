#pragma once
#include "def/TypeDef.h"
#include "sell/SellDefine.h"

class Player;
class LogicSellItem;


class PlayerSellLogic
{
public:
    typedef uint32 ErrCode ;
public:
	static ErrCode HandleBuyEquipShopSellItem( Player& player, uint32 solt_id, uint32 count);
	static ErrCode HandleBuySellItem(
        Player& player, SellId sell_id, uint32 count,
        ItemId& item_id, uint32& item_extra_count, uint32& critical_count);

	static ErrCode HandleSellBagItem( Player& player, uint32 solt_id);
	static ErrCode HandleBuyBackItem( Player& player, uint32 solt_id);

    static ErrCode HandleManualRefresh( Player& player, VictimType victim_type);
    static ErrCode HandleRequestSellProgram( Player& player, VictimType vitim_type);

    static void ResetSellProgram( Player &player, ProgramID programId );

    static ErrCode TryAddSellRecord( Player& player, const LogicSellItem& logicSellItem, uint32 count);

    static void InitVictimRDProgram( Player& player);
    /*
    static bool IsGlobalConfiguredProgram( VictimType victim);

    static int GetGlobalProgram( VictimType victim);
    */
private:
    static void SendVictimRDProgramUpdate( Player& player, VictimType victim, bool sendToClient = true);
    static void SendSellProgramInfoUpdate( Player& player, ProgramID programid, SellId sell_id = 0);


};