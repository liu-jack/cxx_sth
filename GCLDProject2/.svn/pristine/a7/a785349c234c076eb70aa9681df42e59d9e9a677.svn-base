#pragma once
#include "def/TypeDef.h"
#include "Table/BaoQiTableMgr.h"

class Player;
namespace pb
{
    class GS2C_BaoQi_All_Info;
	class GS2C_BaoQi_Create;
	class ShenQiMsg;
}
struct item_use
{
	uint32 item_id_;
	uint32 count_;
	item_use()
		:item_id_(0)
		,count_(0){}
	item_use(uint32 item_id,uint32 count)
		:item_id_(item_id),count_(count)
	{}
};
class PlayerBaoQiLog {
public:
    struct BaoQiPro 
    {
        uint32 level;
        uint32 exp;
		uint32 star;
        uint32 gem;  //itemId
        uint32 have_hole;
    };
    BaoQiPro m_BaoQiLst[BaoQi_MaxCnt];
	std::map<uint64,item_use> ItemUsed_;
	uint32 unlock_gem_id_;
public:
    PlayerBaoQiLog();
    void SaveTo(pb::GS2C_BaoQi_All_Info& msg);
    void LoadFrom(const pb::GS2C_BaoQi_All_Info& msg);
	void SaveToMsg(pb::ShenQiMsg& msg);
	//abandon 
    bool Create(Player& player, uint id, pb::GS2C_BaoQi_Create& msg);
    bool BuyDrawing(Player& player, uint id); //¹ºÂòÍ¼Ö½
	////////////////////////////
    int  AddExp(Player& player, uint id);
    void LevelUp(Player& player, uint id);
    void StarUp(Player& player, uint id);
	void UpdateGemAttr(Player& player);
	void GM_SetShenQiLevel(Player& player,uint id,uint level);
    //±¦Ê¯´ò¿×¡¢×°±¸¡¢²ðÐ¶
    bool Punch(Player& player, uint id);
    bool KnitGem(Player& player, uint id, uint gemItemId);
    bool UnKnitGem(Player& player, uint id);

    uint32 GetMaxLevel() const;
	std::map<uint64,item_use>::iterator FindItemInItemUsed(const uint32 ItemId);
	void GetMultiByItemId(ItemId item_id,std::map<uint64,item_use>::iterator & iter,int &multi);
	uint32 CalcMulti(uint32 id,uint32 level);
	void CalcShenQiBuff(int id,int level,int &multi);
	uint64 ShenQiBuffLastTime(uint64 time_now,uint32 buff_last_time);
	void SendCongratulateMsg(Player& player);
	void FreshAllChara(Player& player);
	const uint32 GetCountOfBaoQi(const uint32 level) const;
private:
	const uint32 FindMinBaoQiLevel() const;
};