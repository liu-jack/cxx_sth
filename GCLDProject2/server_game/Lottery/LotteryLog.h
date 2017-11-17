#pragma once
#include "ptr_container/PtrVector.h"
#include "def/TypeDef.h"

namespace pb
{
    class GS2C_LotteryFreeRecord;
    class GS2C_AllLotteryInfo;
    class GS2C_LootResult;
    class GS2C_LootList;
	class GS2C_LootShopEquipResult;
	class GS2C_SlotItem;
}

class Player;
class LootList;

class LotteryFreeRecord
{
public:
    LotteryFreeRecord( uint32 lotteryType);
    bool TryUseFreeChance();
    

    void SaveTo( pb::GS2C_LotteryFreeRecord& msg) const;
    void LoadFrom( const pb::GS2C_LotteryFreeRecord& msg);
private:
    void Init();
private:
    uint32 m_lotteryType;
    uint32 m_remainFreeCount;
    time_t m_nextFreeLotteryTime;
};


class LotteryLogger
{
    typedef PtrVector< LotteryFreeRecord> LotteryFreeRecords;
public:
    LotteryLogger();

    static const uint32 InValidIndex = -1;
    bool TryUseFreeLottery( Player * player, uint32 lotteryType);
    

    void LoadFrom( const pb::GS2C_AllLotteryInfo& msg);
    void SaveTo( pb::GS2C_AllLotteryInfo& msg) const;

    void SendFreeRecordToDB( Player * player) const;
private:
    LotteryFreeRecord* MutableLotteryRecord(uint32 lotteryType);
    void Init();
private:
    static uint32 GetLotteryTypeIdx( uint32 lotteryType ) ;

    LotteryFreeRecords m_freeRecords;
};


class LotteryLogic
{
public:
    static const uint32 InValidResult = -1;
    static uint32 GetSubstituteId( uint32 lotteryType); // 代用券
    static uint32 GetInterval( uint32 lotteryType);      // 乐透免费间隔时间
    static uint32 GetLotteryCurr( uint32 lotteryType, uint32 count);
    static uint32 GetFreeCount( uint32 lotteryType);

    static uint32 PlayerTryUseFreeLottery( Player* player, uint32 lotteryType, pb::GS2C_LootResult& msg );

    static uint32 PlayerTryLotteryOne( Player* player, uint32 lotteryType, pb::GS2C_LootResult& msg );
    static uint32 PlayerTryLotteryTen( Player* player, uint32 lotteryType, pb::GS2C_LootResult& msg );
	static uint32 PlayerTryLootShopEquip( Player* player, pb::GS2C_LootShopEquipResult& msg,bool isInit = false );
	static uint32 PlayerTryLootEquipSkillAttr( Player* player, uint32 purifyType, uint32 bagSoltId);
	static uint32 PlayerTryRetrievalPurify( Player* player,uint32 bagSoltId);
	static int PlayerLootKuangChangRebuildEffectGem( Player& player);
	static uint32 PlayerTryLootEquipSkillAttrByXilianItem(Player* player,uint32 purifyType,uint32 bagSoltId,uint32 attr_id );



private:
    static bool PlayerLootLottery(Player* player, uint32 lotteryType, pb::GS2C_LootList& lootMsg, uint32 count = 1);
	static bool PlayerLootShopEquip( Player* player, pb::GS2C_LootShopEquipResult&  lootMsg);
	static bool PlayerLootShopEquipSkillAttr( Player* player, pb::GS2C_SlotItem& lootMsg);
	static uint32 PlayerTryLottery( Player* player, uint32 lotteryType, pb::GS2C_LootResult& msg, uint32 count);
};