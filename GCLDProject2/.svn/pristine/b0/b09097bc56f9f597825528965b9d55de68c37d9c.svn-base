#pragma once
#include "def/TypeDef.h"
#include "PlayerMultiContainer.h"
#include "google/protobuf/message.h"
class Player;
class PlayerPool {
private:
	MultC m_playerIndex;
	uint32 m_offlines;

public:
	PlayerPool();
	~PlayerPool();
    static PlayerPool& Instance(){ static PlayerPool T; return T; }
    void Put(PlayerPtr& player);
    Player* GetByPlayerId(uint64 playerId);
	PlayerPtr GetPtrByPlayerId(uint64 playerId);
	void RemoveByPlayerId(uint64 playerId);
	MultC& GetPlayerMap() { return m_playerIndex; }
	uint32 GetPlayerCount();
	uint32 GetOfflinePlayerCount() { return m_offlines;}
	bool   IsInCachePool(uint64 playerId);

	void   DoUpdatePer_Day_Zero();
	void   DoUpdatePer_FourHour_Zero();
	void   DoUpdatePer_Hour_Zero();
	void   DoUpdatePer_Minute_Zero(std::vector<int>& vec);

	void UpdateOffLinePlayer(uint64 nowTimeMS, int64 diffMS);
	void BroadcastMsgByPlatformId(const int opCode, const ::google::protobuf::Message &msg,uint32 platformid);
	void BroadcastMsgToAllPlayerByCountry(const int opCode, const ::google::protobuf::Message &msg, uint32 countryid,const uint64 self_player_id = 0,const uint64 timeNow = 0);
	void BroadcastMsgToAll(const int opCode, const ::google::protobuf::Message &msg);
	void RemoveAllAccounts();
	//void BroadcastMsgToPlayerCountryExpectSelf(const int opCode, const ::google::protobuf::Message &msg,const uint32 countryid,const uint64 self_player_id);
#ifdef _SERVER_CROSS_
	void Update( uint64 nowTimeMS, int64 diffMS );
#endif
};
#define sPlayerPool PlayerPool::Instance()

#define FOREACH_PLAYERPTR(playerId, playerPtr)\
	uint64 playerId = 0;\
	PlayerPtr playerPtr;\
	MultC& playerContain = sPlayerPool.GetPlayerMap();\
	const PV& bySlot = playerContain.get<MPI>() ;\
	PVItr itr = bySlot.begin();\
	for (;\
	itr != bySlot.end() && (playerPtr = *itr) && (playerId = playerPtr->GetGuid());\
	 ++itr\
	)

#define FOREACH_ONLINE_PLAYERPTR(playerId, playerPtr)\
	uint64 playerId = 0;\
	PlayerPtr playerPtr;\
	MultC& playerContain = sPlayerPool.GetPlayerMap();\
	const OV &byId =playerContain.get<MOL>() ;\
	RangeOVItr rangPlats = byId.equal_range( true );\
	for (OVItr itr = rangPlats.first;  itr != rangPlats.second && (playerPtr = *itr) && (playerId = playerPtr->GetGuid()); ++itr)


#define FOREACH_OFFLINE_PLAYERPTR(playerId, playerPtr)\
	uint64 playerId = 0;\
	PlayerPtr playerPtr;\
	MultC& playerContain = sPlayerPool.GetPlayerMap();\
	const OV &byId =playerContain.get<MOL>() ;\
	RangeOVItr rangPlats = byId.equal_range( false );\
	for (OVItr itr = rangPlats.first;  itr != rangPlats.second && (playerPtr = *itr) && (playerId = playerPtr->GetGuid()); ++itr)
