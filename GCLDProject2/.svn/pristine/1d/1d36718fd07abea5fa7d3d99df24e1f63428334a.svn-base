#pragma once
#include "def/TypeDef.h"
#include "Combat/CombatObj.h"
#include "Combat/CombatGroup.h"

class Player;

namespace pb
{
	class GS2C_Get_Teach_Rec;
	class C2GS_Request_Teach;
	class GS2C_Answer_Teach;
	class C2GS_Rush_Teach;
	class GS2C_Rush_Teach_Ret;
}

struct CityCombat
{
	uint32 cityid;
	uint32 countryid;
	uint32 groupid;
};

struct TeachRec
{
	uint32 teachid;
	std::vector<CityCombat> cityCombatMap;
};

typedef	std::map<uint64, TeachRec > TeachRecMap;	//<playerid,teachrec>

class TeachMapMgr{
public:
	static TeachMapMgr& Instance(){ static TeachMapMgr T; return T; }

public:
	void FreshTeachTask(Player* player);
	void DealAskTeachRec(Player* player, pb::GS2C_Get_Teach_Rec& sendmsg);
	void DealRequestTeachMap(Player* player, pb::C2GS_Request_Teach getMsg, pb::GS2C_Answer_Teach& sendMsg);
	void DealRushTeachMap(Player* player, pb::C2GS_Rush_Teach getMsg, pb::GS2C_Rush_Teach_Ret& sendMsg);

	void CreateCombatGroupLst(uint32 teachid, uint32 cityid, std::vector<Combat::CombatObj*>& attlst, std::set<Combat::CombatObj*>& deflst);

	void _OnCombatEnd(Player* player, uint32 teachid, uint32 cityid, Combat::CombatGroup* pGroup, bool isAttackWin);

	void ResetTeachCitys(Player* player, TeachRec& teachRec);

public:
	TeachRecMap player_teach_map_rec;	//玩家教学地图信息

};

#define sTeachMapMgr TeachMapMgr::Instance()