#include <string>
#include <boost/thread.hpp>
#include "../IScriptHost.h"
#include "../LuaStack.h"
#include "../MapScriptGroup.h"

#include "Logger.h"

boost::thread_specific_ptr<std::string>	g_CurScriptFileName;
boost::thread_specific_ptr<char>		g_CurScriptLuaFuncName;
boost::thread_specific_ptr<char>		g_CurScriptCFuncName;

class LuaCFuncNameAutoInit
{
public:
	LuaCFuncNameAutoInit(const char* funcName)
	{
		m_funcName = g_CurScriptCFuncName.release();
		g_CurScriptCFuncName.reset((char*)funcName);
	}

	~LuaCFuncNameAutoInit()
	{
		g_CurScriptCFuncName.release();
		g_CurScriptCFuncName.reset(m_funcName);
	}

private:
	char*			m_funcName;
};

//=================================================================
#define LUA_C_BODY(name, param_min, param_max) \
	int LuaC_##name(lua_State * L); \
	bool _LuaCFuncFlag_##name = MapScriptGroup::AddFunction(#name, LuaC_##name); \
	int LuaC_##name(lua_State * L) \
	{ \
		LuaCFuncNameAutoInit autoInit(#name);\
		LuaToCStack in(L);	\
		CToLuaStack out(L);	\
		int param_count = in.ParamCount();\
		if (param_count < param_min || param_count > param_max)\
		{\
			ELOG("%s => %s() : Param count error when call %s()", \
				g_CurScriptFileName->c_str(), \
				g_CurScriptLuaFuncName.get(), \
				#name);\
			return 0;\
		}

//=================================================================
#define LUA_C_RETURN \
		} \
	return out.ParamCount()

//=================================================================
#define NULL_PTR_LOG(Func) \
	ELOG("%s => %s() : object ptr is NULL  when call %s()", \
		g_CurScriptFileName->c_str(), \
		g_CurScriptLuaFuncName.get(), \
		#Func \
	)

//=====================================================================================
// Lua C functions start
//=====================================================================================
/*
LUA_C_BODY(Print, 1, 1)
{
	const char* text = in.ReadStr();

	SLOG(text);

	LUA_C_RETURN;
}

LUA_C_BODY(SearchForEnemy, 1, 1)
{
	IScriptHost* host = in.ReadHostSafely();
	host->SearchForEnemy();

	LUA_C_RETURN;
}

LUA_C_BODY(HasEnemy, 1, 1)
{
	IScriptHost* host = in.ReadHostSafely();

	out << host->HasEnemy();

	LUA_C_RETURN;
}

LUA_C_BODY(SetCombat, 2, 2)
{
	IScriptHost* host = in.ReadHostSafely();
	bool isCombat = in.ReadBool(false);

	host->SetCombat(isCombat);

	LUA_C_RETURN;
}

LUA_C_BODY(SetCanCombat, 2, 2)
{
	IScriptHost* host = in.ReadHostSafely();
	bool canCombat = in.ReadBool(false);

	host->SetCanCombat(canCombat);

	LUA_C_RETURN;
}

LUA_C_BODY(GetTarget, 1, 1)
{
	IScriptHost* host = in.ReadHostSafely();

	out << host->GetTarget();

	LUA_C_RETURN;
}

LUA_C_BODY(SetTargetByThreat, 1, 2)
{
	IScriptHost* host = in.ReadHostSafely();
	int threadPosi = in.ReadInt(0);

	host->SetTargetByThreat(threadPosi);

	LUA_C_RETURN;
}

LUA_C_BODY(GetPlayersInArea, 1, 1)
{
	int areaId = in.ReadInt(0);

	out <<  MapInstance::CurThreadInst()->AreaTriggerMgr()->GetAreaPlayerGroup(areaId);

	LUA_C_RETURN;
}

LUA_C_BODY(CollectPlayersInArea, 1, 1)
{
	int areaId = in.ReadInt(0);

	std::vector<IScriptHost*> players;
	MapInstance::CurThreadInst()->AreaTriggerMgr()->CollectAreaPlayers(areaId, players);

	out << players;

	LUA_C_RETURN;
}

LUA_C_BODY(GetEnemyByThreat, 1, 2)
{
	IScriptHost* host = in.ReadHostSafely();
	int threadPosi = in.ReadInt(0);
	
	out << host->GetEnemyByThreat(threadPosi);

	LUA_C_RETURN;
}

LUA_C_BODY(IsNull, 1, 1)
{
	IScriptHost* host = in.ReadHost();
	bool isNull = (host == NULL);
	
	out << isNull;

	LUA_C_RETURN;
}

LUA_C_BODY(IsDead, 1, 1)
{
	IScriptHost* host = in.ReadHostSafely();
	
	out << host->IsDead();

	LUA_C_RETURN;
}

LUA_C_BODY(IsMoving, 1, 1)
{
	IScriptHost* host = in.ReadHostSafely();

	out<< host->IsMoving();

	LUA_C_RETURN;
}

LUA_C_BODY(IsSwimming, 1, 1)
{
	IScriptHost* host = in.ReadHostSafely();

	out<< host->IsSwimming();

	LUA_C_RETURN;
}

LUA_C_BODY(IsObserver, 1, 1)
{
	IScriptHost* host = in.ReadHostSafely();
	
	out << host->IsQuestObserver();

	LUA_C_RETURN;
}

LUA_C_BODY(IsPlayer, 1, 1)
{
	IScriptHost* host = in.ReadHostSafely();

	out << host->IsPlayer();

	LUA_C_RETURN;
}

LUA_C_BODY(CanMeleeAttack, 2, 2)
{
	IScriptHost* host = in.ReadHostSafely();
	IScriptHost* target = in.ReadHost();
	
	out << host->CanMeleeAttack(UNIT_CAST(target));

	LUA_C_RETURN;
}

LUA_C_BODY(MeleeAttack, 2, 2)
{
	IScriptHost* host = in.ReadHostSafely();
	IScriptHost* target = in.ReadHost();

	host->MeleeAttack(UNIT_CAST(target));

	LUA_C_RETURN;
}

LUA_C_BODY(SetGangLeader, 2, 2)
{
	Creature *host = CREATURE_CAST(in.ReadHostSafely());
	Unit *target = UNIT_CAST(in.ReadHost());

	host->SetGangLeader(target, true);//force follow leader

	LUA_C_RETURN;
}

LUA_C_BODY(Follow, 2, 5)
{
	IScriptHost* host = in.ReadHostSafely();
	LocatableObject* target = LOCATABLEOBJECT_CAST(in.ReadHost());
	float radius = in.ReadFloat(2.0f);
	float disappearRadius = in.ReadFloat(0);
	bool rebornAfterDisappear = in.ReadBool(false);

	if (target == NULL)
	{
		NULL_PTR_LOG(Follow);
		return 0;
	}

	host->FollowTarget(target, radius, disappearRadius, rebornAfterDisappear);

	LUA_C_RETURN;
}

LUA_C_BODY(StopFollow, 1, 1)
{
	IScriptHost* host = in.ReadHostSafely();

	host->StopFollowTarget();

	LUA_C_RETURN;
}

LUA_C_BODY(MoveToTarget, 2, 2)
{
	IScriptHost* host = in.ReadHostSafely();
	IScriptHost* target = in.ReadHost();

	host->MoveToTarget(LOCATABLEOBJECT_CAST(target));

	LUA_C_RETURN;
}

LUA_C_BODY(FindNpcBySpawn, 2, 5)
{
	IScriptHost* host = in.ReadHostSafely();
	int spawnId = in.ReadInt(0);
	float radius = in.ReadFloat(0);
	int subPhasingId = in.ReadInt(0);
	int visiblePlayerId = in.ReadInt(0);

	IScriptHost* pUnit = host->FindNpcBySpawn(spawnId, radius, subPhasingId, visiblePlayerId);
	out << pUnit;

	LUA_C_RETURN;
}

LUA_C_BODY(FindNpcByEntryId, 2, 5)
{
	IScriptHost* host = in.ReadHostSafely();
	int entry = in.ReadInt(0);
	float radius = in.ReadFloat(0);
	int subPhasingId = in.ReadInt(0);
	int visiblePlayerId = in.ReadInt(0);

	IScriptHost* pUnit = host->FindNpcByEntryId(entry, radius, subPhasingId, visiblePlayerId);
	out << pUnit;

	LUA_C_RETURN;
}

LUA_C_BODY(FindObjBySpawn, 2, 5)
{
	IScriptHost* host = in.ReadHostSafely();
	int spawnId = in.ReadInt(0);
	float radius = in.ReadFloat(0);
	int subPhasingId = in.ReadInt(0);
	int visiblePlayerId = in.ReadInt(0);

	IScriptHost* pObj = host->FindObjBySpawn(spawnId, radius, subPhasingId, visiblePlayerId);
	out << pObj;

	LUA_C_RETURN;
}

LUA_C_BODY(FindObjByEntryId, 2, 5)
{
	IScriptHost* host = in.ReadHostSafely();
	int entry = in.ReadInt(0);
	float radius = in.ReadFloat(0);
	int subPhasingId = in.ReadInt(0);
	int visiblePlayerId = in.ReadInt(0);

	IScriptHost* obj = host->FindObjByEntryId(entry, radius, subPhasingId, visiblePlayerId);
	out << obj;

	LUA_C_RETURN;
}

LUA_C_BODY(FindPlayerById, 1, 1)
{
	int playerId = in.ReadInt(0);

	out << MapInstance::CurThreadInst()->FindPlayerByLowId(playerId);

	LUA_C_RETURN;
}

LUA_C_BODY(MapFindNpcBySpawn, 1, 1)
{
	int spawnId = in.ReadInt(0);

	out <<  MapInstance::CurThreadInst()->FindCreatureBySpawn(spawnId);

	LUA_C_RETURN;
}

LUA_C_BODY(MapFindObjBySpawn, 1, 1)
{
	int spawnId = in.ReadInt(0);

	out <<  MapInstance::CurThreadInst()->FindStaticObjBySpawn(spawnId);

	LUA_C_RETURN;
}

LUA_C_BODY(Chat, 3, 8)
{
	IScriptHost* host = in.ReadHostSafely();
	LocatableObject* target = LOCATABLEOBJECT_CAST(in.ReadHost());
	int strId = in.ReadInt(0);
	int type = in.ReadInt(0);
	int emoteId = in.ReadInt(0);
	int delay = in.ReadInt(0);
	bool loop = in.ReadBool(false);
	Player* player = PLAYER_CAST(in.ReadOptHost());

	host->Chat(target, strId, type, emoteId, delay, loop, player);

	LUA_C_RETURN;
}

LUA_C_BODY(AddTimer, 3, 6)
{
	IScriptHost* host = in.ReadHostSafely();
	int delay = in.ReadInt(0);
	if( delay < 0 || delay > 1000000000 )
	{
		ELOG("Timer value(%d) exceed wheel range", delay);
		//throw std::runtime_error("Time exceed wheel range");
	}
	const char* fnName = in.ReadStr();
	int repeat = in.ReadInt(1);
	int interval = in.ReadInt(delay);
	IScriptHost* target = in.ReadOptHost();

	host->AddScriptCallTimer(OBJ_EVENT_SCRIPT, delay, fnName, repeat, interval, target);

	LUA_C_RETURN;
}

LUA_C_BODY(AddGlobalTimer, 3, 6)
{
	IScriptHost* host = in.ReadHostSafely();
	int time = in.ReadInt(0);
	const char* fnName = in.ReadStr();
	int repeat = in.ReadInt(1);
	int interval = in.ReadInt(time);
	IScriptHost* target = in.ReadOptHost();

	host->AddScriptCallTimer(OBJ_EVENT_SCRIPT_GLOBAL, time, fnName, repeat, interval, target);

	LUA_C_RETURN;
}

LUA_C_BODY(ClearTimers, 1, 1)
{
	//IScriptHost* host = in.ReadHostSafely();
	//host->ClearTimers(OBJ_EVENT_SCRIPT);

	LUA_C_RETURN;
}

LUA_C_BODY(ClearMemPlayer, 1, 1)
{
	IScriptHost* host = in.ReadHostSafely();
	host->ClearAroundPlayer();

	LUA_C_RETURN;
}

LUA_C_BODY(CountAroundNpc, 1, 3)
{
	IScriptHost* host = in.ReadHostSafely();
	int entry = in.ReadInt(0);
	float radius = in.ReadFloat(30.0f);
	
	out << host->CountAroundNpc(entry, radius);

	LUA_C_RETURN;
}

LUA_C_BODY(FindPlayer, 1, 3)
{
	IScriptHost* host = in.ReadHostSafely();
	int radius = in.ReadInt(10);
	
	std::set<int> idSet;
	in.ReadIntSet(idSet);

	Unit* player = host->FindPlayer((float)radius, idSet);
	
	out << player;

	LUA_C_RETURN;
}

LUA_C_BODY(MapFindByGuid, 1, 1)
{
	ObjGUID guid = in.ReadGuid(0);

	out << MapInstance::CurThreadInst()->FindScriptHost(guid);

	LUA_C_RETURN;
}

LUA_C_BODY(GetHpPercent, 1, 1)
{
	IScriptHost* host = in.ReadHostSafely();

	out << host->GetHpPercent();

	LUA_C_RETURN;
}

LUA_C_BODY(GetId, 1, 1)
{
	IScriptHost* host = in.ReadHostSafely();
	
	out << host->GetId();

	LUA_C_RETURN;
}

LUA_C_BODY(GetEntry, 1, 1)
{
	LocatableObject* host = LOCATABLEOBJECT_CAST(in.ReadHostSafely());

	int entry = 0;
	if (host)
	{
		entry = host->GetEntry();
	}
	out << entry;

	LUA_C_RETURN;
}

LUA_C_BODY(GetClass, 1, 1)
{
	IScriptHost* host = in.ReadHostSafely();

	out << host->GetClass();

	LUA_C_RETURN;
}

LUA_C_BODY(GetSpawnId, 1, 1)
{
	IScriptHost* host = in.ReadHostSafely();
	
	out << host->GetSpawnId();

	LUA_C_RETURN;
}

LUA_C_BODY(Disappear, 1, 1)
{
	IScriptHost* host = in.ReadHostSafely();
	host->Disappear();

	LUA_C_RETURN;
}

LUA_C_BODY(MoveByWP, 2, 3)
{
	IScriptHost* host = in.ReadHostSafely();
	int wpId = in.ReadInt(0);
	float speed = in.ReadFloat(0);

	host->MoveByWP(wpId, speed);

	LUA_C_RETURN;
}

LUA_C_BODY(StopMoveByWP, 1, 1)
{
	IScriptHost* host = in.ReadHostSafely();

	host->StopMoveByWayPoint();

	LUA_C_RETURN;
}

LUA_C_BODY(HasQuest, 2, 4)
{
	IScriptHost* host = in.ReadHostSafely();
	int questId =  in.ReadInt(0);
	int objIdx =  in.ReadInt(-1);
	int val = in.ReadInt(-1);
	
	out << host->HasQuestAtObj(questId, objIdx, val);

	LUA_C_RETURN;
}

LUA_C_BODY(HasQuestFinished, 2, 2)
{
	Player *questPlayer = dynamic_cast<Player*>(in.ReadHostSafely());
	int questId = in.ReadInt(0);
	
	bool result = false;
	if (questPlayer)
	{
		result = questPlayer->GetQuestLogger()->isQuestFinished(questId);
	}

	out << result;

	LUA_C_RETURN;
}

LUA_C_BODY(ChangeFaction, 2, 2)
{
	IScriptHost* host = in.ReadHostSafely();
	int faction = in.ReadInt(0);

	host->ChangeFaction(faction);

	LUA_C_RETURN;
}

LUA_C_BODY(SetCanDead, 2, 2)
{
	IScriptHost* host = in.ReadHostSafely();
	bool isDead = in.ReadBool(false);

	host->SetCanDead(isDead);

	LUA_C_RETURN;
}

LUA_C_BODY(UpdateQuest, 3, 4)
{
	IScriptHost* host = in.ReadHostSafely();
	int questId =  in.ReadInt(0);
	int objIdx =  in.ReadInt(0);
	int val =  in.ReadInt(1);
	
	out << host->UpdateQuestAtObj(questId, objIdx, val);

	LUA_C_RETURN;
}

LUA_C_BODY(UpdateQuestForSubPhasing, 3, 4)
{
	IScriptHost* host = in.ReadHostSafely();
	LocatableObject *hostOfObj = dynamic_cast<LocatableObject*>(host);
	if( !hostOfObj )
		return 0;

	float radius = hostOfObj->GetActiveRange();
	if( radius < 32.0f )
		radius = 32.0f;
	int questId =  in.ReadInt(0);
	int objIdx =  in.ReadInt(0);
	int val =  in.ReadInt(1);

	MapObjIter<Player> it(hostOfObj, MapTile_Player_Mask);
	while(Player *player = it.Next())
	{
		if( radius > 0.1f && !hostOfObj->DistanceLessThan(player, radius) )
			continue;

		player->UpdateQuestAtObj(questId, objIdx, val);
	}

	LUA_C_RETURN;
}

LUA_C_BODY(UpdateQuestForEnemyList, 3, 4)
{
	IScriptHost* host = in.ReadHostSafely();
	Creature *hostOfCreature = dynamic_cast<Creature*>(host);
	if( !hostOfCreature )
		return 0;

	float radius = hostOfCreature->GetActiveRange();
	if( radius < 32.0f )
		radius = 32.0f;
	int questId =  in.ReadInt(0);
	int objIdx =  in.ReadInt(0);
	int val =  in.ReadInt(1);

	//collect enemy id(only player), stores into 'enemiesId'
	std::set<uint32> affectedPlayers;
	EnemyList::Iterator itEnemy = hostOfCreature->Enemies().CreateIterator();
	while(true)
	{
		Unit *enemy = itEnemy.Next();
		if( !enemy )
			break;

		if( enemy->IsKindOf(TYPE_PLAYER) )
		{
			Player *player = PLAYER_CAST(enemy);
			TeamInfoCache const &infoTeam = player->GetTeamInfo();
			if( infoTeam.members.empty() )
			{//no team
				affectedPlayers.insert(player->LowId());
			}
			else
			{//has team, consider all team members
				for(std::set<uint64>::const_iterator itMember = infoTeam.members.begin(); itMember != infoTeam.members.end(); ++itMember)
					affectedPlayers.insert(GET_LOW_GUID(*itMember));
			}
		}
	}

	MapObjIter<Player> it(hostOfCreature, MapTile_Player_Mask);
	while(Player *player = it.Next())
	{
		if( radius > 0.1f && !hostOfCreature->DistanceLessThan(player, radius) )
			continue;	//too far away
		if( affectedPlayers.find(player->LowId()) == affectedPlayers.end() )
			continue;	//not enemy, or no threat

		player->UpdateQuestAtObj(questId, objIdx, val);
	}

	LUA_C_RETURN;
}

LUA_C_BODY(AddEnemy, 2, 3)
{
	IScriptHost* host = in.ReadHostSafely();
	IScriptHost* target = in.ReadHost();
	int thread = in.ReadInt(1);

	host->AddEnemy(target, thread);

	LUA_C_RETURN;
}

LUA_C_BODY(SetHpPercent, 2, 2)
{
	IScriptHost* host = in.ReadHostSafely();
	int hpPercent = in.ReadInt(1);

	host->SetHpPercent(hpPercent);

	LUA_C_RETURN;
}

LUA_C_BODY(SetPhasing, 2, 3)
{
	IScriptHost* host = in.ReadHostSafely();
	int phasingId = in.ReadInt(0);
	bool on = in.ReadBool(true);

	host->SetPhasing(phasingId, on);

	LUA_C_RETURN;
}

LUA_C_BODY(IsActive, 1, 1)
{
	IScriptHost* host = in.ReadHostSafely();
	
	out << host->IsActive();

	LUA_C_RETURN;
}

LUA_C_BODY(SetActive, 1, 2)
{
	IScriptHost* host = in.ReadHostSafely();
	bool active = in.ReadBool(true);

	host->SetActive(active);

	LUA_C_RETURN;
}

LUA_C_BODY(StopMoving, 1, 2)
{
	IScriptHost* host = in.ReadHostSafely();
	int delay = in.ReadInt(0);
	host->StopMoving(delay);

	LUA_C_RETURN;
}

LUA_C_BODY(CreateCreature, 4, 7)
{
	IScriptHost* host = in.ReadHostSafely();
	int entry = in.ReadInt(0);
	float angle = in.ReadFloat(0);
	float radius = in.ReadFloat(0);
	IScriptHost* faceTarget = in.ReadOptHost();
	int subPhasingId = in.ReadInt(0);
	IScriptHost* visibleToPlayer = in.ReadOptHost();
	
	IScriptHost *newCreature = host->CreateCreature(entry, angle, radius, faceTarget, visibleToPlayer, subPhasingId);
	//event quest observer interesting spawn of creature
	if( newCreature )
		MapInstance::CurThreadInst()->GetQuestObserver()->onCreateCreatureInScript(CREATURE_CAST(newCreature));
	out << newCreature;

	LUA_C_RETURN;
}

LUA_C_BODY(CreateCreatureBySpawn, 1, 3)
{
	int spawnId = in.ReadInt(0);
	int subPhasingId = in.ReadInt(0);
	Player* visibleToPlayer = PLAYER_CAST(in.ReadOptHost());
	
	out << MapInstance::CurThreadInst()->SpawnCreatureBySpawnId(spawnId, visibleToPlayer, subPhasingId);

	LUA_C_RETURN;
}

LUA_C_BODY(CreateObj, 4, 7)
{
	IScriptHost* host = in.ReadHostSafely();
	int entry = in.ReadInt(0);
	float angle = in.ReadFloat(0);
	float radius = in.ReadFloat(0);
	float relaOri = in.ReadFloat(0);
	int subPhasingId = in.ReadInt(0);
	IScriptHost* visibleToPlayer = in.ReadOptHost();

	out << host->CreateObject(entry, angle, radius, relaOri, visibleToPlayer, subPhasingId);

	LUA_C_RETURN;
}

LUA_C_BODY(CreateObjBySpawn, 1, 3)
{
	int spawnId = in.ReadInt(0);
	int subPhasingId = in.ReadInt(0);
	Player* visibleToPlayer = PLAYER_CAST(in.ReadOptHost());
	
	out << MapInstance::CurThreadInst()->SpawnStaticObjBySpawnId(spawnId, visibleToPlayer, subPhasingId);

	LUA_C_RETURN;
}

LUA_C_BODY(PlayEffect, 3, 5)
{
	IScriptHost* host = in.ReadHostSafely();
	std::string effName = in.ReadStr();
	std::string nodeName = in.ReadStr();
	int delay = in.ReadInt(0);
	float scale = in.ReadFloat(1.0f);

	host->PlayEffect(effName, nodeName, delay, scale);

	LUA_C_RETURN;
}

LUA_C_BODY(PlayMovie, 2, 3)
{
	IScriptHost* host = in.ReadHostSafely();
	std::string movName = in.ReadStr();
	std::string sndName = in.ReadStr();

	host->PlayMovie(movName, sndName);

	LUA_C_RETURN;
}

LUA_C_BODY(SendWarning, 2, 3)
{
	IScriptHost* host = in.ReadHostSafely();
	int strId = in.ReadInt(0);
	int type = in.ReadInt(0);

	host->SendWarning(type, strId);

	LUA_C_RETURN;
}

LUA_C_BODY(SendMapWarning, 1, 2)
{
	int strId = in.ReadInt(0);
	int type = in.ReadInt(0);

	pb::GS2C_Warning msg;
	msg.set_type(type);
	msg.set_str_id(strId);

	NetPack pack(pb::SMSG_WARNING);
	pack << msg;
	MapInstance::CurThreadInst()->Broadcast(pack);

	LUA_C_RETURN;
}

LUA_C_BODY(ShowIcon, 2, 4)
{
	IScriptHost* host = in.ReadHostSafely();
	std::string iconPath = in.ReadStr();
	std::string nodeName = in.ReadStr();
	float scale = in.ReadFloat(1.0f);

	host->ShowIcon(iconPath, nodeName, scale);

	LUA_C_RETURN;
}

LUA_C_BODY(HideIcon, 1, 1)
{
	IScriptHost* host = in.ReadHostSafely();

	host->HideIcon();

	LUA_C_RETURN;
}

LUA_C_BODY(ShowQuestUI, 2, 2)
{
	IScriptHost* host = in.ReadHostSafely();
	int questId = in.ReadInt(0);
	 
	host->ShowQuestUI(questId);

	LUA_C_RETURN;
}

LUA_C_BODY(ShowQuestAcceptUI, 3, 3)
{
	IScriptHost* host = in.ReadHostSafely();
	IScriptHost* player = in.ReadHost();
	int questId = in.ReadInt(0);

	host->ShowQuestAcceptUI(player, questId);

	LUA_C_RETURN;
}

LUA_C_BODY(RemoveQuest, 2, 2)
{
	IScriptHost* host = in.ReadHostSafely();
	int questId = in.ReadInt(0);
	
	out << host->RemoveQuestById(questId);

	LUA_C_RETURN;
}

LUA_C_BODY(PauseQuest, 2, 2)
{
	IScriptHost* host = in.ReadHostSafely();
	int questId = in.ReadInt(0);

	out << host->PauseQuestById(questId);

	LUA_C_RETURN;
}

LUA_C_BODY(SetQuestFailed, 2, 2)
{
	IScriptHost* host = in.ReadHostSafely();
	int questId = in.ReadInt(0);

	host->SetQuestFailed(questId);

	LUA_C_RETURN;
}

LUA_C_BODY(IsCombat, 1, 1)
{
	IScriptHost* host = in.ReadHostSafely();
	
	out << host->IsCombat();

	LUA_C_RETURN;
}

LUA_C_BODY(GetQuestObserver, 0, 0)
{
	out << dynamic_cast<IScriptHost*>(MapInstance::CurThreadInst()->GetQuestObserver());

	LUA_C_RETURN;
}

LUA_C_BODY(AddQuest, 2, 2)
{
	IScriptHost* host = in.ReadHostSafely();
	int questId = in.ReadInt(0);

	host->AddQuestById(questId);

	LUA_C_RETURN;
}

LUA_C_BODY(LinkQuest, 2, 2)
{
	IScriptHost* host = in.ReadHostSafely();
	int questId = in.ReadInt(0);

	host->LinkQuest(questId);

	LUA_C_RETURN;
}

LUA_C_BODY(UnlinkQuest, 2, 2)
{
	IScriptHost* host = in.ReadHostSafely();
	int questId = in.ReadInt(0);

	host->UnlinkQuest(questId);

	LUA_C_RETURN;
}

LUA_C_BODY(Teleport, 2, 3)
{
	IScriptHost* host = in.ReadHostSafely();
	int spotId = in.ReadInt(0);
	float radius = in.ReadFloat(0);

	host->TeleportToSpot(spotId, radius);

	LUA_C_RETURN;
}

LUA_C_BODY(DungeonCompleted, 2, 2)
{
	IScriptHost* host = in.ReadHostSafely();
	int mapId = in.ReadInt(0);

	host->SendDungeonCompleted(mapId);

	LUA_C_RETURN;
}

LUA_C_BODY(ShowCountdown, 2, 6)
{
	IScriptHost* host = in.ReadHostSafely();
	int totalSec = in.ReadInt(0);
	int warnSec = in.ReadInt(0);
	int startStrId = in.ReadInt(0);
	int procStrId = in.ReadInt(0);
	int warnStrId = in.ReadInt(0);

	host->ShowCountdownTimer(totalSec, warnSec, startStrId, procStrId, warnStrId);

	LUA_C_RETURN;
}

LUA_C_BODY(SetEquipDisplay, 3, 3)
{
	IScriptHost* host = in.ReadHostSafely();
	int slot = in.ReadInt(0);
	int itemId = in.ReadInt(0);

	host->SetEquipDisplay(slot, itemId);

	LUA_C_RETURN;
}

LUA_C_BODY(CalcDistance, 2, 2)
{
	LocatableObject* obj1 = LOCATABLEOBJECT_CAST(in.ReadHostSafely());
	LocatableObject* obj2 = LOCATABLEOBJECT_CAST(in.ReadHostSafely());
	
	float distance = 0;
	if (obj1 && obj2)
	{
		distance = obj1->Position().getDistanceFrom(obj2->Position());
	}

	out << distance;
	LUA_C_RETURN;
}

LUA_C_BODY(GetMoveDir, 1, 1)
{
	Unit* unit = UNIT_CAST(in.ReadHostSafely());

	VEC3 dir;
	if (unit)
	{
		dir = unit->SpeedVector();
		dir.normalize();
	}

	out << dir.X << dir.Y << dir.Z;
	LUA_C_RETURN;
}

LUA_C_BODY(GetBoundingRadius, 1, 1)
{
	Unit* unit = UNIT_CAST(in.ReadHostSafely());

	float radius = 0;
	if (unit)
	{
		radius = unit->BoundingRadius();
	}

	out << radius;
	LUA_C_RETURN;
}

LUA_C_BODY(SetData, 3, 3)
{
	IScriptHost* host = in.ReadHostSafely();
	int idx = in.ReadInt(0);
	int64 val = in.ReadGuid(0);

	host->SetScriptData(idx, val);

	LUA_C_RETURN;
}

LUA_C_BODY(GetData, 2, 2)
{
	IScriptHost* host = in.ReadHostSafely();
	int idx = in.ReadInt(0);

	out << host->GetScriptData(idx);

	LUA_C_RETURN;
}

LUA_C_BODY(GetPlayerItemNum, 2, 2)
{
	Player* player = PLAYER_CAST(in.ReadHostSafely());
	int itemId = in.ReadInt(0);
	
	int count = 0;
	if (player)
	{
		count = player->CountItemInBag(itemId);
	}

	out << count;

	LUA_C_RETURN;
}

LUA_C_BODY(TakePlayerItem, 2, 3)
{
	Player* player = PLAYER_CAST(in.ReadHostSafely());
	int itemId = in.ReadInt(0);
	int count = in.ReadInt(1);

	bool success = false;
	if (player && count >0)
	{
		success = player->DestroyItemInBag(itemId, count);
	}

	out << success;

	LUA_C_RETURN;
}

LUA_C_BODY(GivePlayerItem, 2, 3)
{
	Player* player = PLAYER_CAST(in.ReadHostSafely());
	int itemId = in.ReadInt(0);
	int count = in.ReadInt(1);

	bool success = false;
	if (player && count >0)
	{
		success = player->AddItemInBag(itemId, count);
	}

	out << success;

	LUA_C_RETURN;
}

LUA_C_BODY(ChangeIdleEmote, 2, 3)
{
	IScriptHost* host = in.ReadHostSafely();
	int idleEmote = in.ReadInt(0);
	int transEmote = in.ReadInt(0);

	host->ChangeIdleEmote(idleEmote, transEmote);

	LUA_C_RETURN;
}

LUA_C_BODY(Call, 2, 3)
{
	LocatableObject* host = LOCATABLEOBJECT_CAST(in.ReadHostSafely());
	const char* fnName = in.ReadStr();
	IScriptHost* target = in.ReadOptHost();

	if (host)
	{
		if (target == NULL)
		{
			host->CallLuaFunc(fnName);
		}
		else
		{
			host->CallLuaFuncWithTarget(fnName, target);
		}
	}

	LUA_C_RETURN;
}

LUA_C_BODY(CallWithParamInt, 3, 3)
{
	LocatableObject* host = LOCATABLEOBJECT_CAST(in.ReadHostSafely());
	const char* fnName = in.ReadStr();
	int param = in.ReadInt(0);

	if (host)
		host->CallLuaFunc(fnName, param);

	LUA_C_RETURN;
}

LUA_C_BODY(CreateSubPhasing, 1, 3)
{
	int subPhasingId = in.ReadInt(0);
	int areaTriggerId = in.ReadInt(0);
	bool clearNpcIfNoPlayer = in.ReadBool(false);

	DLOG("CreateSubPhasing(%d,%d,%d)", subPhasingId, areaTriggerId, (int)clearNpcIfNoPlayer);

	LUA_C_RETURN;
}

LUA_C_BODY(SubPhasingFindObjBySpawn, 2, 2)
{
	int subPhasingId = in.ReadInt(0);
	int spawnId = in.ReadInt(0);
	IScriptHost *obj = MapInstance::CurThreadInst()->SubPhasingMgr()->findObjectBySpawn(subPhasingId, spawnId);

	out << obj;

	LUA_C_RETURN;
}

LUA_C_BODY(GetSubPhasingId, 1, 1)
{
	LocatableObject* host = LOCATABLEOBJECT_CAST(in.ReadHostSafely());
	
	int subPhasingId = 0;
	if (host)
	{
		subPhasingId = host->SubPhasingId();
	}

	out << subPhasingId;

	LUA_C_RETURN;
}

LUA_C_BODY(SetSubPhasingId, 2, 2)
{
	IScriptHost* host = in.ReadHostSafely();
	int id = in.ReadInt(0);

	if( host->IsPlayer() && id == 0 )
	{
		DLOG("player leave sub phasing %d", LOCATABLEOBJECT_CAST(host)->SubPhasingId());
	}
	host->SetSubPhasingId(id);

	LUA_C_RETURN;
}

LUA_C_BODY(CollectSubPhasingObjAndNpc, 1, 1)
{
	int subPhasingId = in.ReadInt(0);
	if (subPhasingId == 0)
	{
		// ELOG
		return 0;
	}

	std::vector<IScriptHost*> result;
	MapInstance::CurThreadInst()->SubPhasingMgr()->getObjects(subPhasingId, result);

	out << result;

	LUA_C_RETURN;
}

LUA_C_BODY(CollectSubPhasingPlayers, 1, 1)
{
	int subPhasingId = in.ReadInt(0);
	if (subPhasingId == 0)
	{
		// ELOG
		return 0;
	}

	std::vector<IScriptHost*> result;
	MapInstance::CurThreadInst()->SubPhasingMgr()->getPlayers(subPhasingId, result);

	out << result;

	LUA_C_RETURN;
}

LUA_C_BODY(EnterSubPhasing, 2, 2)
{
	IScriptHost* host = in.ReadHostSafely();
	int areaTriggerId = in.ReadInt(0);
	if( areaTriggerId <= 0 )
		return 0;
	Player *player = dynamic_cast<Player*>(host);
	if( !player )
		return 0;

	out << MapInstance::CurThreadInst()->SubPhasingMgr()->enterSubPhasing(player, areaTriggerId, MapSubPhasingMgr::createSubPhasingIfNotExist);

	LUA_C_RETURN;
}

LUA_C_BODY(EnterSubPhasingIfExist, 2, 2)
{
	IScriptHost* host = in.ReadHostSafely();
	int areaTriggerId = in.ReadInt(0);
	if( areaTriggerId <= 0 )
		return 0;
	Player *player = dynamic_cast<Player*>(host);
	if( !player )
		return 0;

	out << MapInstance::CurThreadInst()->SubPhasingMgr()->enterSubPhasing(player, areaTriggerId, MapSubPhasingMgr::enterSubPhasingOnlyExist);

	LUA_C_RETURN;
}

LUA_C_BODY(CreateSubPhasingAndEnter, 1, 1)
{
	IScriptHost* host = in.ReadHostSafely();
	Player *player = dynamic_cast<Player*>(host);
	if( !player )
		return 0;

	out << MapInstance::CurThreadInst()->SubPhasingMgr()->createSubPhasingForVehicle(player);

	LUA_C_RETURN;
}

LUA_C_BODY(CloseSubPhasing, 1, 1)
{
	int subPhasingId = in.ReadInt(0);
	if( subPhasingId == 0 )
		return 0;

	MapInstance::CurThreadInst()->SubPhasingMgr()->closeSubPhasing(subPhasingId);

	LUA_C_RETURN;
}

LUA_C_BODY(SwitchElevator, 2, 2)
{
	IScriptHost* host = in.ReadHostSafely();
	int mode = in.ReadInt(0); // enum ElevatorLoopMode

	host->SwitchElevator(mode);

	LUA_C_RETURN;
}

LUA_C_BODY(SwitchObj, 2, 2)
{
	StaticObject* host = STATICOBJECT_CAST(in.ReadHostSafely());
	bool isOn = in.ReadBool(true);

	if (host)
	{
		host->SetSwitcherState(isOn);
	}

	LUA_C_RETURN;
}

LUA_C_BODY(GetRelationship, 2, 2)
{
	Unit* caster = UNIT_CAST(in.ReadHostSafely());
	Unit* target = UNIT_CAST(in.ReadHostSafely());

	int result = RELATIONSHIP_INVALID;

	if( caster && target )
	{
		if (caster->IsFriend(target))
		{
			result = RELATIONSHIP_FRIEND;
		}
		else if (caster->IsHostile(target))
		{
			result = RELATIONSHIP_ENEMY;
		}
		else
		{
			result = RELATIONSHIP_OTHER;
		}
	}
	out << result;

	LUA_C_RETURN;
}

LUA_C_BODY(SetVehicle, 2, 6)
{
	LocatableObject* rider = LOCATABLEOBJECT_CAST(in.ReadHostSafely());
	LocatableObject* horse = LOCATABLEOBJECT_CAST(in.ReadHostSafely());
	bool riderDrive = in.ReadBool(false);
	string node = in.ReadStr();
	int emoteId = in.ReadInt(0);
	int idleId = in.ReadInt(0);

	if (rider && horse)
	{
		rider->GetMountRider()->Mount(horse, riderDrive, node, emoteId, idleId);
	}

	LUA_C_RETURN;
}

LUA_C_BODY(CancelVehicle, 1, 2)
{
	LocatableObject* rider = LOCATABLEOBJECT_CAST(in.ReadHostSafely());
	bool horseDisappear = in.ReadBool(false);

	if (rider)
	{
		rider->GetMountRider()->Dismount(horseDisappear);
	}

	LUA_C_RETURN;
}

LUA_C_BODY(PlayersExitDungeon, 0, 0)
{
	if (MapInstance::CurThreadInst()->GetGameMap()->IsWorldMap())
	{
		return 0;
	}

	NetPack pack(pb::CMSG_LEAVE_DUNGEON);
	MapInstance::CurThreadInst()->PostInputPack(pack);

	LUA_C_RETURN;
}

LUA_C_BODY(SetAlpha, 2, 2)
{
	IScriptHost* host = in.ReadHostSafely();
	float alpha = in.ReadFloat(1.0f);
	
	host->SetAlpha(alpha);

	LUA_C_RETURN;
}

LUA_C_BODY(KillTarget, 2, 3)
{
	IScriptHost* killer = in.ReadHostSafely();
	IScriptHost* target = in.ReadHostSafely();
	bool asKiller = in.ReadBool(false);

	killer->Kill(target, asKiller);

	LUA_C_RETURN;
}

LUA_C_BODY(ChangeModel, 2, 2)
{
	Unit* host = UNIT_CAST(in.ReadHostSafely());
	int displayId = in.ReadInt(0);

	if (host)
	{
		host->ChangeModel(displayId);
	}
	
	LUA_C_RETURN;
}

LUA_C_BODY(SendSpawnInfo, 1, 1)
{
	LocatableObject* target = LOCATABLEOBJECT_CAST(in.ReadHostSafely());

	if (target)
	{
		MapInstance::CurThreadInst()->BroadcastSpawnEvent(target);
	}

	LUA_C_RETURN;
}

LUA_C_BODY(Taunt, 3, 3)
{
	Unit* caster = UNIT_CAST(in.ReadHostSafely());
	Unit* target = UNIT_CAST(in.ReadHostSafely());
	int duration = in.ReadInt(0);

	if (caster && target)
	{
		target->SetTopThreat(caster, duration);
	}

	LUA_C_RETURN;
}










//========================================================================
// skill
//========================================================================

LUA_C_BODY(CastSpell, 2, 3)
{
	IScriptHost* host = in.ReadHostSafely();
	int skillId = in.ReadInt(0);
	LocatableObject* target = LOCATABLEOBJECT_CAST(in.ReadOptHost());
	host->CastSpell(skillId,target);

	LUA_C_RETURN;
}

LUA_C_BODY(CameraMove, 2, 2)
{
	IScriptHost* host = in.ReadHostSafely();
	int wpId = in.ReadInt(0);

	host->CameraMove(wpId);

	LUA_C_RETURN;
}

LUA_C_BODY(ChangeHp, 6, 8)
{
	IScriptHost* host = in.ReadHostSafely();
	float hpChange = in.ReadFloat(-1);
	Unit* enemy = UNIT_CAST(in.ReadHost());
	int damageType = in.ReadInt(0);
	int skillId = in.ReadInt(0);
	int rune    = in.ReadInt(0);
	int buffId = in.ReadInt(0);
	int criticalType = in.ReadInt(0);

	host->ChangeHpBySkill(hpChange, enemy, damageType, skillId, buffId,rune, criticalType);

	LUA_C_RETURN;
}

LUA_C_BODY(AddBuff, 7, 8)
{
	Unit* caster = UNIT_CAST(in.ReadHostSafely());
	Unit* target = UNIT_CAST(in.ReadHostSafely());
	if( !caster||!target )
	{
		return 0;
	}
	int skillId  = in.ReadInt(0);
	int rune	 = in.ReadInt(0);
	int buffId   = in.ReadInt(0);
	int interval = in.ReadInt(0);
	int durTime  = in.ReadInt(0);
	int stack    = in.ReadInt(1);
	target->AddBuff(caster,skillId,rune,buffId,interval,durTime,stack);

	LUA_C_RETURN;
}

LUA_C_BODY(BeatedBack, 8, 8)
{
	Unit* caster = UNIT_CAST(in.ReadHostSafely());
	Unit* target = UNIT_CAST(in.ReadHostSafely());
	if( !caster||!target )
	{
		return 0;
	}
	f32 dis		 = in.ReadFloat(0.0);
	f32 speed	 = in.ReadFloat(0.0);
	f32 pos_x    = in.ReadFloat(0.0);
	f32 pos_y    = in.ReadFloat(0.0);
	f32 pos_z    = in.ReadFloat(0.0);
	VEC3 leapDir(pos_x,pos_y,pos_z);
	int flag  = in.ReadInt(0);
	target->BeatedBack(dis,speed,leapDir,flag);

	LUA_C_RETURN;
}

LUA_C_BODY(AddHalo, 6, 6)
{
	Unit* target = UNIT_CAST(in.ReadHostSafely());
	if( !target )
	{
		NULL_PTR_LOG(AddHalo);
		return 0;
	}
	int skillId  = in.ReadInt(0);
	int rune	 = in.ReadInt(0);
	int buffId   = in.ReadInt(0);
	int interval = in.ReadInt(0);
	int durTime  = in.ReadInt(0);
	target->AddHalo(skillId,rune,buffId,interval,durTime);

	LUA_C_RETURN;
}

LUA_C_BODY(GetSkillRuneType, 2, 2)
{
	Unit* pCaster = UNIT_CAST(in.ReadHostSafely());
	if( !pCaster )
	{
		NULL_PTR_LOG(GetSkillRuneType);
		return 0;
	}
	int skillId  = in.ReadInt(0);
	out <<(int) pCaster->GetSkillRuneType(skillId);
	LUA_C_RETURN;
}

LUA_C_BODY(IsNormalSkill, 1, 1)
{
	int skillId  = in.ReadInt(0);
	const Skill_Proto* sp = g_SkillMgrIns.GetSkillProto(skillId);
	if( !sp )
	{
		return 0;
	}
	out<<sp->hasAttribute(ATTRIBUTE_NORMAL_ATTACK);
	LUA_C_RETURN;
}

LUA_C_BODY(OpenStaticOBJ, 2, 2)
{
	Player* pCaster = PLAYER_CAST(in.ReadHostSafely());
	StaticObject* pTarget = STATICOBJECT_CAST(in.ReadHostSafely());
	if( !pCaster || !pTarget )
	{
		return 0;
	}
	pCaster->OnActiveStaticObject(pTarget);

	LUA_C_RETURN;
}

LUA_C_BODY(LaunchTrackBullet, 5, 8)
{
	Unit* caster = UNIT_CAST(in.ReadHostSafely());
	LocatableObject* target = LOCATABLEOBJECT_CAST(in.ReadHostSafely());
	if( !caster||!target )
	{
		return 0;
	}
	int   skillId  = in.ReadInt(0);
	int   rune	   = in.ReadInt(0);
	float speed    = in.ReadFloat(0.0);
	float accelspeed = in.ReadFloat(0.0);
	int   effectId   = in.ReadInt(0);
	int   flag   = in.ReadInt(0);

	caster->CreateTrackBallisticBullet(target,skillId,rune,speed,accelspeed,effectId,flag);

	LUA_C_RETURN;
}

LUA_C_BODY(GetPosition, 1, 1)
{
	IScriptHost* host = in.ReadHostSafely();
	LocatableObject* obj = LOCATABLEOBJECT_CAST(host);

	if (obj)
	{
		out << obj->Position().X << obj->Position().Y << obj->Position().Z;
	}

	LUA_C_RETURN;
}

LUA_C_BODY(GetDirection, 1, 1)
{
	IScriptHost* host = in.ReadHostSafely();
	LocatableObject* obj = LOCATABLEOBJECT_CAST(host);

	if (obj)
	{
		out << obj->Direction().X << obj->Direction().Y << obj->Direction().Z;
	}

	LUA_C_RETURN;
}


LUA_C_BODY(ModSkillChantTime, 2, 2)
{
	Unit* caster = UNIT_CAST(in.ReadHostSafely());
	if( !caster)
	{
		return 0;
	}
	int  value  = in.ReadInt(0);
	caster->ModCurSkillChantTime(value);

	LUA_C_RETURN;
}

LUA_C_BODY(PlayEffectAtPos, 5, 6)
{
	Unit* caster = UNIT_CAST(in.ReadHostSafely());
	if( !caster)
	{
		return 0;
	}
	int	effectid = in.ReadInt(0);
	f32 x        = in.ReadFloat(0.0);
	f32 y        = in.ReadFloat(0.0);
	f32 z        = in.ReadFloat(0.0);
	int	time     = in.ReadInt(-1);
	VEC3 pos(x,y,z);
	caster->Board_PlayEffectAtPos(pos,effectid,time);
	LUA_C_RETURN;
}

LUA_C_BODY(LeapAttack,3,6)
{
	Player* caster = PLAYER_CAST(in.ReadHostSafely());
	if( !caster)
	{
		return 0;
	}
	f32 range =  in.ReadFloat(0.0);
	f32 speed =  in.ReadFloat(0.0);
	f32 x        = in.ReadFloat(0.0);
	f32 y        = in.ReadFloat(0.0);
	f32 z        = in.ReadFloat(0.0); 
	VEC3 dir(x,y,z);
	if( range < 0 )
	{
		range = 0;
	}
	caster->Send_LeapAttack(range,speed,dir);

	LUA_C_RETURN;
}

LUA_C_BODY(ModUnitAttr, 6, 6)
{
	Unit* caster = UNIT_CAST(in.ReadHostSafely());
	if( !caster)
	{
		return 0;
	}
	int	buffId				= in.ReadInt(0);
	ObjGUID casterGuid = in.ReadGuid(0);
	int type				= in.ReadInt(0);
	f32 per					= in.ReadFloat(0.0);
	f32 value				= in.ReadFloat(0.0);
	caster->ModUnitAttrByBuff(buffId,casterGuid,type,per,value);

	LUA_C_RETURN;
}

LUA_C_BODY(LaunchRandomNromalBullet, 5, 7)
{
	Unit* caster = UNIT_CAST(in.ReadHostSafely());
	if( !caster )
	{
		return 0;
	}
	int   skillId  = in.ReadInt(0);
	int   rune	   = in.ReadInt(0);
	float speed    = in.ReadFloat(0.0);
	float maxDis   = in.ReadFloat(0.0);
	int   effectId = in.ReadInt(0);
	int   flag     = in.ReadInt(0);

	caster->CreateRandomDirNormalBullet(skillId,rune,speed,maxDis,effectId,flag);

	LUA_C_RETURN;
}

LUA_C_BODY(LaunchNormalBullet, 8, 11)
{
	Unit* caster = UNIT_CAST(in.ReadHostSafely());
	if( !caster )
	{
		return 0;
	}

	f32 x        = in.ReadFloat(0.0);
	f32 y        = in.ReadFloat(0.0);
	f32 z        = in.ReadFloat(0.0);
	VEC3 dir(x,y,z);
	int   skillId  = in.ReadInt(0);
	int   rune	   = in.ReadInt(0);
	float speed    = in.ReadFloat(0.0);
	float maxDis   = in.ReadFloat(0.0);
	int   effectId = in.ReadInt(0);
	int   flag     = in.ReadInt(0);
	int   isUpdate = in.ReadInt(1);
	caster->CreateDirNormalBullet(skillId,rune,dir,speed,maxDis,effectId,flag,isUpdate);

	LUA_C_RETURN;
}


LUA_C_BODY(ModuleTrigger, 2, 2)
{
	Creature* creature = CREATURE_CAST(in.ReadHostSafely());
	if (!creature)
	{
		return 0;
	}
	
	int moudleId = in.ReadInt(0);
	sTriggerMgr.ExecuteModule(moudleId);

	LUA_C_RETURN;
}

LUA_C_BODY(ModAllSkillCD, 2, 2)
{
	Unit* caster = UNIT_CAST(in.ReadHostSafely());
	if( !caster )
	{
		return 0;
	}
	int modValue = in.ReadInt(0);
	caster->ModAllSkillCD(modValue);

	LUA_C_RETURN;
}

LUA_C_BODY(PowerIncMonitor, 5, 5)
{
	Unit* target = UNIT_CAST(in.ReadHostSafely());
	if( !target )
	{
		return 0;
	}
	
	int buffid = in.ReadInt(0);
	ObjGUID casterGuid = in.ReadGuid(0);
	int value  =  in.ReadInt(0);
	const char* funcName = in.ReadStr();

	target->AddPowerIncMonitor(buffid,casterGuid,value,funcName);

	LUA_C_RETURN;
}

LUA_C_BODY(HpMonitor, 5, 6)
{
	Unit* target = UNIT_CAST(in.ReadHostSafely());
	if( !target )
	{
		NULL_PTR_LOG(LessThanMaxHpMonitor);
		return 0;
	}
	
	int buffid = in.ReadInt(0);
	float percent  =  in.ReadFloat(0.0f);
	ObjGUID casterGuid = in.ReadGuid(0);
	const char* less_funcName = in.ReadStr();
	const char* more_funcName = in.ReadStr();
	target->AddHpMonitor(buffid,casterGuid,percent,less_funcName,more_funcName);

	LUA_C_RETURN;
}

LUA_C_BODY(BuffTimer, 5, 5)
{
	Unit* target = UNIT_CAST(in.ReadHostSafely());
	if( !target )
	{
		return 0;
	}
	int buffid = in.ReadInt(0);
	ObjGUID casterGuid = in.ReadGuid(0);
	int triggerTime  =  in.ReadInt(0);
	const char* funcName = in.ReadStr();
	target->AddTimerMonitor(buffid,casterGuid,triggerTime,funcName);

	LUA_C_RETURN;
}

LUA_C_BODY(ChangePower,2,2)
{
	Unit* target = UNIT_CAST(in.ReadHostSafely());
	if( !target )
	{
		return 0;
	}
	int Value = in.ReadInt(0);
	target->ChangePower(Value);

	LUA_C_RETURN;
}

LUA_C_BODY(TarEqualTar,2,2)
{
	bool ret = false;
	Unit* target1 = UNIT_CAST(in.ReadOptHost());
	Unit* target2 = UNIT_CAST(in.ReadOptHost());
	if(!target1 || !target2)
	{
		out << ret;
		return 1;
	}
	 ret = (target1->Guid() == target2->Guid());
	out << ret;
	LUA_C_RETURN;
}

LUA_C_BODY(ModBuffDurTime,4,4)
{
	Unit* target = UNIT_CAST(in.ReadHostSafely());
	if( !target )
	{
		return 0;
	}
	int buffId = in.ReadInt(0);
	ObjGUID casterGuid = in.ReadGuid(0);
	int Value = in.ReadInt(0);
	target->ModBuffDurationTime(buffId,casterGuid,Value);

	LUA_C_RETURN;
}


LUA_C_BODY(GetObjAttr,2,2)
{
	Object* obj = dynamic_cast<Object*>(in.ReadHostSafely());
	int  attrIdx = in.ReadInt(0);

	if( !obj )
	{
		NULL_PTR_LOG(GetObjAttr);
		return 0;
	}

	out << obj->Values().GetIntOrFloat(attrIdx);

	LUA_C_RETURN;
}

LUA_C_BODY(GetSkillPowerCost,1,1)
{
	int  skillid = in.ReadInt(0);
	const Skill_Proto* sp =  g_SkillMgrIns.GetSkillProto(skillid);
	if( !sp )
	{
		return 0;
	}
	int cost = (int) sp->powerCost;
	out << cost;

	LUA_C_RETURN;
}

LUA_C_BODY(DeleteBuff,2,2)
{
	Unit* target = UNIT_CAST(in.ReadHostSafely());
	if( !target )
	{
		return 0;
	}
	int buffId = in.ReadInt(0);
	target->DeleteBuff(buffId);

	LUA_C_RETURN;
}

LUA_C_BODY(DeleteHalo,2,2)
{
	Unit* target = UNIT_CAST(in.ReadHostSafely());
	if( !target )
	{
		return 0;
	}
	int haloId = in.ReadInt(0);
	target->DeleteHalo(haloId);

	LUA_C_RETURN;
}

LUA_C_BODY(CalcSkillAttackResult,2,2)
{
	Unit* caster = UNIT_CAST(in.ReadHostSafely());
	Unit* target = UNIT_CAST(in.ReadHostSafely());
	if( !caster|| !target )
	{
		return 0;
	}
	int result = caster->CalcSkillAttackResult(target);
	out << result;

	LUA_C_RETURN;
}

LUA_C_BODY(CalcSkillAttackResultWithCR,3,3)
{
	Unit* caster = UNIT_CAST(in.ReadHostSafely());
	Unit* target = UNIT_CAST(in.ReadHostSafely());
	if( !caster|| !target )
	{
		return 0;
	}
	float value = in.ReadFloat(0.0f);
	int result = caster->CalcSkillAttackResult(target,1,value);
	out << result;

	LUA_C_RETURN;
}

LUA_C_BODY(CalcSkillAttackResultWithDR,3,3)
{
	Unit* caster = UNIT_CAST(in.ReadHostSafely());
	Unit* target = UNIT_CAST(in.ReadHostSafely());
	if( !caster|| !target )
	{
		return 0;
	}
	float value = in.ReadFloat(0.0f);
	int result = caster->CalcSkillAttackResult(target,2,value);
	out << result;

	LUA_C_RETURN;
}

LUA_C_BODY(CalcBuffAttackResult,3,3)
{
	Unit* host = UNIT_CAST(in.ReadHostSafely());
	if( !host )
	{
		return 0;
	}
	int buffid = in.ReadInt(0);
	ObjGUID casterGuid = in.ReadGuid(0);

	int result = host->CalcBuffAttackResult(buffid,casterGuid);
	out << result;

	LUA_C_RETURN;
}

LUA_C_BODY(CalcSkillDmg,11,11)
{
	Unit* host = UNIT_CAST(in.ReadHostSafely());
	Unit* enemy = UNIT_CAST(in.ReadHostSafely());
	if( !host || !enemy)
	{
		return 0;
	}
	int result = in.ReadInt(0);
	DamageType damageType =  (DamageType)in.ReadInt(0);
	float baseValue = in.ReadFloat(0.0f);
	float randomValue = in.ReadFloat(0.0f);
	float ppFactor = in.ReadFloat(0.0f);
	float mpFactor = in.ReadFloat(0.0f);
	float LvFactor1 = in.ReadFloat(0.0f);
	float LvFactor2 = in.ReadFloat(0.0f);
	float LvFactor3 = in.ReadFloat(0.0f);

	float  dmg = host->CalcSkillDmg(enemy,result,damageType,baseValue,randomValue,ppFactor,mpFactor,LvFactor1,LvFactor2,LvFactor3);
	out << dmg;

	LUA_C_RETURN;
}

LUA_C_BODY(CalcBuffDmg,12,12)
{
	Unit* host = UNIT_CAST(in.ReadHostSafely());
	if( !host )
	{
		return 0;
	}
	int buffid = in.ReadInt(0);
	ObjGUID casterGuid = in.ReadGuid(0);
	int result = in.ReadInt(0);
	DamageType damageType =  (DamageType)in.ReadInt(0);
	float baseValue = in.ReadFloat(0.0f);
	float randomValue = in.ReadFloat(0.0f);
	float ppFactor = in.ReadFloat(0.0f);
	float mpFactor = in.ReadFloat(0.0f);
	float LvFactor1 = in.ReadFloat(0.0f);
	float LvFactor2 = in.ReadFloat(0.0f);
	float LvFactor3 = in.ReadFloat(0.0f);

	float  dmg = host->CalcBuffDmg(buffid,casterGuid,result,damageType,baseValue,randomValue,ppFactor,mpFactor,LvFactor1,LvFactor2,LvFactor3);
	out << dmg;

	LUA_C_RETURN;
}

LUA_C_BODY(GetBuffRuneType, 3, 3)
{
	Unit* host = UNIT_CAST(in.ReadHostSafely());
	if( !host )
	{
		return 0;
	}
	int buffid  = in.ReadInt(0);
	ObjGUID casterGuid = in.ReadGuid(0);
	out << host->GetBuffRuneType(buffid,casterGuid);
	LUA_C_RETURN;
}

LUA_C_BODY(SetAuraState, 2, 2)
{
	Unit* host = UNIT_CAST(in.ReadHostSafely());
	if( !host )
	{
		return 0;
	}
	int stateFlag  = in.ReadInt(0);
	host->I_SkillCaster::SetAuraState( stateFlag );
	LUA_C_RETURN;
}

LUA_C_BODY(ClearAuraState, 2, 2)
{
	Unit* host = UNIT_CAST(in.ReadHostSafely());
	if( !host )
	{
		return 0;
	}
	int stateFlag  = in.ReadInt(0);
	host->I_SkillCaster::ClearAuraState( stateFlag );
	LUA_C_RETURN;
}

LUA_C_BODY(HasAuraState, 2, 2)
{
	Unit* host = UNIT_CAST(in.ReadHostSafely());
	if( !host )
	{
		return 0;
	}
	int stateFlag  = in.ReadInt(0);
	out << host->HasAuraState( stateFlag );
	LUA_C_RETURN;
}

LUA_C_BODY(LaunchChainBullet, 7, 9)
{
	Unit* caster = UNIT_CAST(in.ReadHostSafely());
	LocatableObject* target = LOCATABLEOBJECT_CAST(in.ReadHostSafely());
	if( !caster||!target )
	{
		return 0;
	}
	int   skillId  = in.ReadInt(0);
	int   rune      = in.ReadInt(0);
	float speed    = in.ReadFloat(0.0);
	int MaxCount = in.ReadInt(0);
	float MaxTransmissionDis = in.ReadFloat(0.0);
	int tarType = in.ReadInt(Skill_Event::TARGET_ENEMY);
	int effectId = in.ReadInt(0);
	int flag = in.ReadInt(0);

	caster->CreateChainBullet( target, skillId, rune,speed, MaxCount, MaxTransmissionDis,tarType,effectId,flag);

	LUA_C_RETURN;
}

LUA_C_BODY(CreateImmTrap, 6, 6)
{
	Unit* caster = UNIT_CAST(in.ReadHostSafely());
	if( !caster )
	{
		return 0;
	}

	f32 x        = in.ReadFloat(0.0);
	f32 y        = in.ReadFloat(0.0);
	f32 z        = in.ReadFloat(0.0);
	VEC3 pos(x,y,z);
	int  skillId  = in.ReadInt(0);
	int lifeCycle  =  in.ReadInt(0);
	
	out << caster->CreateImmTrap( pos,skillId,lifeCycle);

	LUA_C_RETURN;
}

LUA_C_BODY(CreateMinesTrap, 7, 7)
{
	Unit* caster = UNIT_CAST(in.ReadHostSafely());
	if( !caster )
	{
		return 0;
	}

	f32 x        = in.ReadFloat(0.0);
	f32 y        = in.ReadFloat(0.0);
	f32 z        = in.ReadFloat(0.0);
	VEC3 pos(x,y,z);
	int  skillId  = in.ReadInt(0);
	int lifeCycle = in.ReadInt(0);
	float radius  = in.ReadFloat(0.0);

	out << caster->CreateMinesTrap( pos,skillId,lifeCycle,radius);

	LUA_C_RETURN;
}

LUA_C_BODY(HasBuff, 2, 2)
{
	Unit* target = UNIT_CAST(in.ReadHostSafely());
	if( !target )
	{
		return 0;
	}
	int  buffId  = in.ReadInt(0);
	out << target->HasBuff(buffId);

	LUA_C_RETURN;
}

LUA_C_BODY(GetBuffStack, 2, 3)
{
	Unit* target = UNIT_CAST(in.ReadHostSafely());
	if( !target )
	{
		return 0;
	}
	int  buffId  = in.ReadInt(0);
	ObjGUID casterGuid = in.ReadGuid(0);
	if (casterGuid != 0)
	{
		out << target->GetBuffStack(buffId,casterGuid);
	}
	else
	{
		out << target->GetBuffStack(buffId,0);
	}		
	LUA_C_RETURN;
}


LUA_C_BODY(ModBuffStack, 3, 3)
{
	Unit* target = UNIT_CAST(in.ReadHostSafely());
	if( !target )
	{
		return 0;
	}
	int  buffId  = in.ReadInt(0);
	int  value   = in.ReadInt(0);
	target->ModBuffStack(buffId,value);

	LUA_C_RETURN;
}

LUA_C_BODY(EquipSkillToSlotByBuff, 4, 4)
{
	Unit* target = UNIT_CAST(in.ReadHostSafely());
	if( !target )
	{
		return 0;
	}
	int  buffId    = in.ReadInt(0);
	int  skillId   = in.ReadInt(0);
	int  slot      = in.ReadInt(0);
	target->EquipSkillToSlotByBuff(buffId,skillId,slot);

	LUA_C_RETURN;
}

LUA_C_BODY(BeAttackedMonitor, 4, 4)
{
	Unit* target = UNIT_CAST(in.ReadHostSafely());
	if( !target )
	{
		return 0;
	}
	
	int buffid = in.ReadInt(0);
	ObjGUID casterGuid = in.ReadGuid(0);
	const char* funcName = in.ReadStr();

	target->AddBeAttackedMonitor(buffid,casterGuid,funcName);

	LUA_C_RETURN;
}

LUA_C_BODY(BeKilledMonitor, 4, 4)
{
	Unit* target = UNIT_CAST(in.ReadHostSafely());
	if( !target )
	{
		return 0;
	}
	
	int buffid = in.ReadInt(0);
	ObjGUID casterGuid = in.ReadGuid(0);
	const char* funcName = in.ReadStr();

	target->AddBeKilledMonitor(buffid,casterGuid,funcName);

	LUA_C_RETURN;
}

LUA_C_BODY(AttackMonitor, 4, 4)
{
	Unit* target = UNIT_CAST(in.ReadHostSafely());
	if( !target )
	{
		return 0;
	}
	
	int buffid = in.ReadInt(0);
	ObjGUID casterGuid = in.ReadGuid(0);
	const char* funcName = in.ReadStr();

	target->AddBuffMonitorWithType(buffid,casterGuid,funcName,MT_ATTACK);
	LUA_C_RETURN;
}

LUA_C_BODY(CriticalMonitor, 4, 4)
{
	Unit* target = UNIT_CAST(in.ReadHostSafely());
	if( !target )
	{
		return 0;
	}	
	int buffid = in.ReadInt(0);
	ObjGUID casterGuid = in.ReadGuid(0);
	const char* funcName = in.ReadStr();

	target->AddBuffMonitorWithType(buffid,casterGuid,funcName,MT_CRITICAL);
	LUA_C_RETURN;
}

LUA_C_BODY(ModSkillCD, 3, 3)
{
	Unit* caster = UNIT_CAST(in.ReadHostSafely());
	if( !caster )
	{
		return 0;
	}
	int skillId  = in.ReadInt(0);
	int modValue = in.ReadInt(0);
	caster->ModSkillCD(skillId,modValue);

	LUA_C_RETURN;
}

LUA_C_BODY(AddSkillCD, 3, 3)
{
	Unit* caster = UNIT_CAST(in.ReadHostSafely());
	if( !caster )
	{
		return 0;
	}
	int skillId  = in.ReadInt(0);
	int cd = in.ReadInt(0);
	caster->AddCD(CD_TYPE_SKILL,skillId,cd);
	caster->Send_SkillCD(skillId,CD_TYPE_SKILL, cd);
	LUA_C_RETURN;
}

LUA_C_BODY(IsControllable,1,1)
{
	IScriptHost* host = in.ReadHostSafely();
	out << host->IsControllable();

	LUA_C_RETURN;	
}


LUA_C_BODY(CastSpellToPos, 5, 5)
{
	IScriptHost* host = in.ReadHostSafely();
	if( !host )
	{
		return 0;
	}

	int skillId = in.ReadInt(0);
	f32 x        = in.ReadFloat(0.0);
	f32 y        = in.ReadFloat(0.0);
	f32 z        = in.ReadFloat(0.0);
	VEC3 pos(x,y,z);
	host->CastSpellToPos(skillId,pos);

	LUA_C_RETURN;
}


LUA_C_BODY(IsCastingSkill, 1, 1)
{
	IScriptHost* host = in.ReadHostSafely();
	out << host->IsCastingSkill();

	LUA_C_RETURN;
}

LUA_C_BODY(ShowSkillRangeMarker, 14, 14)
{
	Unit* caster = UNIT_CAST(in.ReadHostSafely());
	if( !caster )
	{
		return 0;
	}
	int Id        = in.ReadInt(0);
	int skillId   = in.ReadInt(0);
	int rangeType = in.ReadInt(0);
	f32 pos_x        = in.ReadFloat(0.0);
	f32 pos_y        = in.ReadFloat(0.0);
	f32 pos_z        = in.ReadFloat(0.0);
	VEC3 centerPos(pos_x,pos_y,pos_z);
	f32 value    = in.ReadFloat(0.0);
	f32 extend_val = in.ReadFloat(0.0);
	
	f32 dir_x        = in.ReadFloat(0.0);
	f32 dir_y        = in.ReadFloat(0.0);
	f32 dir_z        = in.ReadFloat(0.0);
	VEC3 dir(dir_x,dir_y,dir_z);
	int delayTime    = in.ReadInt(0);

	LocatableObject* target = LOCATABLEOBJECT_CAST(in.ReadOptHost());

	caster->Borad_ShowSkillRangeMarker(Id,skillId,rangeType,centerPos,value,extend_val,dir,delayTime,target);

	LUA_C_RETURN;
}

LUA_C_BODY(HideSkillRangeMarker, 3, 3)
{
	Unit* caster = UNIT_CAST(in.ReadHostSafely());
	if( !caster )
	{
		return 0;
	}
	int skillId   = in.ReadInt(0);
	int Id        = in.ReadInt(0);
	caster->Borad_HideSkillRangeMarker(skillId,Id);

	LUA_C_RETURN;
}

LUA_C_BODY(FaceTarget,2,2)
{
	IScriptHost* host = in.ReadHostSafely();
	LocatableObject* target =  LOCATABLEOBJECT_CAST(in.ReadHostSafely());
	if( !host || !target )
	{
		return 0;
	}
	host->FaceTarget(target);

	LUA_C_RETURN;
}

LUA_C_BODY(FaceToWP, 2, 2)
{
	IScriptHost* host = in.ReadHostSafely();
	int wpId = in.ReadInt(0);

	host->FaceWayPoint(wpId);

	LUA_C_RETURN;
}

LUA_C_BODY( GetDirToTarget,2,2)
{	
	IScriptHost* host = in.ReadHostSafely();
	LocatableObject* target = LOCATABLEOBJECT_CAST(in.ReadHostSafely());
	LocatableObject* obj = LOCATABLEOBJECT_CAST(host);

	if (obj && target)
	{
		out << obj->GetDirToTarget(target).X << obj->GetDirToTarget(target).Y << obj->GetDirToTarget(target).Z;
	}

	LUA_C_RETURN;
	
}

LUA_C_BODY( MoveSkillMarker,7,7)
{	
	Unit* caster = UNIT_CAST(in.ReadHostSafely());
	if( !caster )
	{
		return 0;
	}
	int skillId   = in.ReadInt(0);
	int Id        = in.ReadInt(0);
	f32 pos_x        = in.ReadFloat(0.0);
	f32 pos_y        = in.ReadFloat(0.0);
	f32 pos_z        = in.ReadFloat(0.0);
	VEC3 centerPos(pos_x,pos_y,pos_z);
	int time     = in.ReadInt(0);
	caster->Borad_MoveSkillMarker(skillId,Id,centerPos,time);

	LUA_C_RETURN;
	
}

LUA_C_BODY(ScaleSkillMarker,6,6)
{	
	Unit* caster = UNIT_CAST(in.ReadHostSafely());
	if( !caster )
	{
		return 0;
	}
	int skillId   = in.ReadInt(0);
	int Id        = in.ReadInt(0);
	f32 value    = in.ReadFloat(0.0);
	f32 extend_val = in.ReadFloat(0.0);
	int time     = in.ReadInt(0);
	caster->Borad_ScaleSkillMarker(skillId,Id,value,extend_val,time);

	LUA_C_RETURN;
	
}

LUA_C_BODY(RotateSkillMarker,8,8)
{	
	Unit* caster = UNIT_CAST(in.ReadHostSafely());
	if( !caster )
	{
		return 0;
	}
	int skillId   = in.ReadInt(0);
	int Id        = in.ReadInt(0);
	f32 dir_x        = in.ReadFloat(0.0);
	f32 dir_y        = in.ReadFloat(0.0);
	f32 dir_z        = in.ReadFloat(0.0);
	VEC3 dir(dir_x,dir_y,dir_z);
	int time     = in.ReadInt(0);
	int rotateType = in.ReadInt(0);
	caster->Borad_RotateSkillMarker(skillId,Id,dir,time,rotateType);

	LUA_C_RETURN;
	
}

LUA_C_BODY(SetCurSkillTarget,3,3)
{	
	Unit* caster = UNIT_CAST(in.ReadHostSafely());
	LocatableObject* target = LOCATABLEOBJECT_CAST(in.ReadHostSafely());
	if( !caster )
	{
		return 0;
	}
	int skillId   = in.ReadInt(0);

	caster->SetCurSkillTarget(skillId,target);

	LUA_C_RETURN;
	
}


LUA_C_BODY(SetTarget,2,2)
{
	IScriptHost* host = in.ReadHostSafely();
	LocatableObject* target = LOCATABLEOBJECT_CAST(in.ReadHostSafely());
	if( !host)
		return 0;
	
	host->SetTarget(target);

	LUA_C_RETURN;
}


LUA_C_BODY(GetGuid,1,1)
{
	LocatableObject* host = LOCATABLEOBJECT_CAST(in.ReadHostSafely());
	if( !host)
		return 0;
	
	out << host->GetGuid();

	LUA_C_RETURN;
}

LUA_C_BODY(HasBuffAttri,3,3)
{
	Unit* host = UNIT_CAST(in.ReadHostSafely());
	if( !host)
		return 0;
	int skillId  = in.ReadInt(0);
	int attriNum  = in.ReadInt(0);
	out << host->HasBuffAttri(skillId,attriNum);

	LUA_C_RETURN;
}

LUA_C_BODY(EnterQTEState,5,5)
{
	Unit* host = UNIT_CAST(in.ReadHostSafely());
	if( !host)
		return 0;
	int skillId = in.ReadInt(0);
	int qteId = in.ReadInt(0);
	int lastTime = in.ReadInt(0);
	ObjGUID casterGuid = in.ReadGuid(0);
	host->EnterQteState(skillId,qteId,lastTime,casterGuid);

	LUA_C_RETURN;
}

LUA_C_BODY(StopCasting,2,2)
{
	Unit* host = UNIT_CAST(in.ReadHostSafely());
	if( !host)
		return 0;
	int skillId = in.ReadInt(0);
	host->CancelSkill(skillId);

	LUA_C_RETURN;
}

LUA_C_BODY(GetTrapHost,1,1)
{
	Unit* trap = UNIT_CAST(in.ReadHostSafely());
	if( !trap)
		return 0;

	out<< MapInstance::CurThreadInst()->FindScriptHost( trap->GetHostGuid());

	LUA_C_RETURN;
}

LUA_C_BODY(AddPlayerContribution,3,3)
{
	Player *player = PLAYER_CAST(in.ReadHostSafely());
	if( !player )
		return 0;
	int questId = in.ReadInt(0);
	int contributionValue = in.ReadInt(0);
	if( questId == 0 || contributionValue == 0 )
		return 0;

	MapInstance::CurThreadInst()->GetQuestObserver()->addPlayerContribution(questId, player->GetId(), contributionValue);

	LUA_C_RETURN;
}

LUA_C_BODY(GetNpcSightRange,1,1)
{
	Unit* npc = UNIT_CAST(in.ReadHostSafely());
	if( !npc)
		return 0;

	out<<npc->SightRange();

	LUA_C_RETURN;
}

LUA_C_BODY(AddSkillPoint,2,2)
{
	Player *player = PLAYER_CAST(in.ReadHostSafely());
	if( !player )
		return 0;
	int value = in.ReadInt(0);
	int skillPoint = player->AddSkillPoint(value, true);
	out<<skillPoint;

	LUA_C_RETURN;
}

LUA_C_BODY(DelAllEnvEquipBuff,1,1)
{
	Unit *target = UNIT_CAST(in.ReadHostSafely());
	if( !target )
		return 0;
	target->DeleteEnvEquipBuff();

	LUA_C_RETURN;
}

LUA_C_BODY(DelAllDeBuff,1,1)
{
	Unit* target = UNIT_CAST(in.ReadHostSafely());
	if( !target )
		return 0;
	target->DeleteDeBuff();

	LUA_C_RETURN;
}

LUA_C_BODY(DelAllVehicleBuff,1,1)
{
	Player *player = PLAYER_CAST(in.ReadHostSafely());
	if( !player )
		return 0;
	player->DeleteVehicleBuff();

	LUA_C_RETURN;
}

LUA_C_BODY(GetHorse,1,1)
{
	LocatableObject *host = LOCATABLEOBJECT_CAST(in.ReadHostSafely());
	if( !host )
	{
		out<<host;
	}
	else
	{
		out<<host->GetMountRider()->Horse();
	}

	LUA_C_RETURN;
}

LUA_C_BODY(UseRecipe, 2, 2)
{
	Player* player = PLAYER_CAST(in.ReadHostSafely());
	int recipeId = in.ReadInt(0);

	if (player && recipeId > 0)
	{
		const LogicItemRecipe* recipe = sCraftingMgr.FindRecipe(recipeId);
		if (recipe)
		{
			player->UseRecipe(recipe);
		}
	}

	LUA_C_RETURN;
}

LUA_C_BODY(SetScale, 2, 2)
{
	Unit* target = UNIT_CAST(in.ReadHostSafely());
	float scale = in.ReadFloat(1.0f);

	if (target && scale > 0)
	{
		target->SetExtraScale(scale);
	}

	LUA_C_RETURN;
}

LUA_C_BODY(SetCanGoBack, 1, 2)
{
	Creature* target = CREATURE_CAST(in.ReadHostSafely());
	bool goBack = in.ReadBool(true);

	if (target)
	{
		target->SetNeedGoBack(goBack);
	}

	LUA_C_RETURN;
}

LUA_C_BODY(IsCoolDown, 2, 2)
{
	Unit* target = UNIT_CAST(in.ReadHostSafely());
	int skillId  = in.ReadInt(0);

	out<<target->IsCoolDown(skillId);

	LUA_C_RETURN;
}

LUA_C_BODY(GetMaxCastRange, 1, 1)
{
	int skillId  = in.ReadInt(0);
	const Skill_Proto* sp = g_SkillMgrIns.GetSkillProto(skillId);
	if( !sp )
	{
		out<<-1;
		return 1;
	}
	uint32 runeId = sp->runes[0];
	const Skill_Runes* skillRune = g_SkillMgrIns.GetSkillRunes(runeId);
	if( !skillRune )
	{
		out<<-1;
		return 1;
	}

	if(  skillRune->minCastRange == 0 && skillRune->maxCastRange == 0 )
	{
		out<<0;
	}
	else
	{
		out<<skillRune->maxCastRange;
	}

	LUA_C_RETURN;
}

LUA_C_BODY(SetVisible, 1, 2)
{
	Unit* target = UNIT_CAST(in.ReadHostSafely());
	bool visible = in.ReadBool(true);

	if (target)
	{
		target->SetVisible(visible);
	}

	LUA_C_RETURN;
}

LUA_C_BODY(GetQuestExpireTime, 1, 1)
{
	int questId  = in.ReadInt(0);
	if( questId > 0 )
	{
		DataQuest const *questData = sQuestMgr->getQuestData(questId);
		if( questData )
			out << (int)questData->time;
	}

	LUA_C_RETURN;
}

LUA_C_BODY(IsLegend, 0, 0)
{
	out << MapInstance::CurThreadInst()->IsLegend();

	LUA_C_RETURN;
}

LUA_C_BODY(CanEnterDungeon, 2, 2)
{
	Player* player = PLAYER_CAST(in.ReadHostSafely());
	int mapId = in.ReadInt(0);

	bool canEnter = false;
	if (player && mapId)
	{
		int reqLv = 45;
		GErrorCode err = player->CalcEnterClassicDungeonResult(mapId, reqLv);
		if (err == CommonSuccess)
		{
			canEnter = true;
		}
		else if (err == YourLevelTooLow)
		{
			pb::GS2C_DungeonLvTooLow msg;
			msg.set_need_lv(reqLv);

			player->Send(pb::SMSG_DUNGEON_LV_TOO_LOW, msg);

		}
		else
		{
			player->SendGError(err);
		}
	}

	out << canEnter;
	LUA_C_RETURN;
}

LUA_C_BODY(AddBuffShieldValue, 4, 4)
{
	Unit* target = UNIT_CAST(in.ReadHostSafely());
	if( !target )
	{
		return 0;
	}
	
	int buffid = in.ReadInt(0);
	ObjGUID casterGuid = in.ReadGuid(0);
	int value = in.ReadInt(0);

	target->AddBuffShieldValue(buffid,casterGuid,value);

	LUA_C_RETURN;
}

LUA_C_BODY(GetBuffShieldValue, 3, 3)
{
	Unit* target = UNIT_CAST(in.ReadHostSafely());
	if( !target )
	{
		out<<-1;
		return 1;
	}
	
	int buffid = in.ReadInt(0);
	ObjGUID casterGuid = in.ReadGuid(0);

	out << target->GetBuffShieldValue(buffid,casterGuid);

	LUA_C_RETURN;
}

LUA_C_BODY(GetBuffRemainingTime, 3, 3)
{
	Unit* target = UNIT_CAST(in.ReadHostSafely());
	if( !target )
	{
		out<<-1;
		return 1;
	}
	
	int buffid = in.ReadInt(0);
	ObjGUID casterGuid = in.ReadGuid(0);

	out << target->GetBuffRemainingTime(buffid,casterGuid);

	LUA_C_RETURN;
}

LUA_C_BODY(IsAllowPvp, 1, 1)
{
	Player *player = PLAYER_CAST(in.ReadHostSafely());
	if( !player )
		return 0;
	
	out << player->IsAllowPvp();

	LUA_C_RETURN;
}

LUA_C_BODY(SetAllowPvp, 2, 2)
{
	Player *player = PLAYER_CAST(in.ReadHostSafely());
	if( !player )
		return 0;

	player->SetAllowPvp(in.ReadBool(true));

	LUA_C_RETURN;
}

LUA_C_BODY(SetPvp, 2, 2)
{
	Player *player = PLAYER_CAST(in.ReadHostSafely());
	if( !player )
		return 0;
	
	if( in.ReadBool(true) )
		player->SetAuraState(AURA_STATE_PVP_AREA);
	else
		player->ClearAuraState(AURA_STATE_PVP_AREA);

	LUA_C_RETURN;
}

LUA_C_BODY(SkillEnterCD, 2, 2)
{
	Unit *target = UNIT_CAST(in.ReadHostSafely());
	if( !target )
		return 0;
	int skillId  = in.ReadInt(0);
	target->EnterSkillCD(skillId);

	LUA_C_RETURN;
}


LUA_C_BODY(RefreshBuffDurTime, 4, 4)
{
	Unit* caster = UNIT_CAST(in.ReadHostSafely());
	if( !caster)
	{
		return 0;
	}
	int	buffId				= in.ReadInt(0);
	ObjGUID casterGuid      = in.ReadGuid(0);
	int32 value				= in.ReadInt(0);

	caster->RefreshDurTime(buffId,casterGuid,value);

	LUA_C_RETURN;
}

LUA_C_BODY(CancelTarget,1,1)
{
	IScriptHost* host = in.ReadHostSafely();	
	if( !host)
		return 0;
	
	host->SetTarget(NULL);

	LUA_C_RETURN;
}

LUA_C_BODY(GetBuffCasterPS, 3, 3)
{
	Unit* host = UNIT_CAST(in.ReadHostSafely());
	if( !host )
	{
		return 0;
	}
	int buffid  = in.ReadInt(0);
	ObjGUID casterGuid = in.ReadGuid(0);
	out << host->GetBuffCasterPS(buffid,casterGuid);
	LUA_C_RETURN;
}

LUA_C_BODY(GetBuffCasterMS, 3, 3)
{
	Unit* host = UNIT_CAST(in.ReadHostSafely());
	if( !host )
	{
		return 0;
	}
	int buffid  = in.ReadInt(0);
	ObjGUID casterGuid = in.ReadGuid(0);
	out << host->GetBuffCasterMS(buffid,casterGuid);
	LUA_C_RETURN;
}

LUA_C_BODY(IncreaseExhausting, 2, 2)
{
	Player* player = PLAYER_CAST(in.ReadHostSafely());
	int val = in.ReadInt(1);

	if (!player)
	{
		return 0;
	}

	player->IncreaseExhausting(val);

	LUA_C_RETURN;
}

LUA_C_BODY(EquipItem, 3, 3)
{
	Player* player = PLAYER_CAST(in.ReadHostSafely());
	int slot = in.ReadInt(0);
	int itemId = in.ReadInt(0);

	if (!player)
	{
		return 0;
	}

	if (!player->GM_ReplaceItem(pb::BODY, slot, itemId))
	{
		ELOG("Lua::EquipItem(slot=%d, itemId=%d) failed", slot, itemId);
	}

	LUA_C_RETURN;
}

LUA_C_BODY(GetSkillDmgInfo, 1, 2)
{
	int32 skillId = in.ReadInt(0);
	int32 runeId = in.ReadInt(0);

	const Skill_DmgDisplay* diplayInfo = NULL;
	diplayInfo = g_SkillMgrIns.GetSkillDisplayInfo(skillId, runeId);

	if( diplayInfo )
		out<<diplayInfo->dmg_display[0]<<diplayInfo->dmg_display[1]<<diplayInfo->dmg_display[2];	
	else
		out<<0<<0<<0;
	LUA_C_RETURN;
}

LUA_C_BODY(GetSkillDmgRate, 2, 2)
{
	int32 skillId =   in.ReadInt(0);
	int32 rateIndex = in.ReadInt(-1);

	const Skill_DmgDisplay* diplayInfo = NULL;
	diplayInfo = g_SkillMgrIns.GetSkillDisplayInfo(skillId, 0);
	if ( rateIndex < 1 || rateIndex > 7 || !diplayInfo)
	{
		return 0;
	}
	if ( rateIndex < 4)
	{
		out<<diplayInfo->dmg_display_rate[rateIndex - 1];	
	}
	else
	{
		out<<diplayInfo->dmg_dot_rate[rateIndex - 4];	
	}

	LUA_C_RETURN;
}

LUA_C_BODY(ResetSkillPoint, 1, 1)
{
	Player* player = PLAYER_CAST(in.ReadHostSafely());
	if (!player)
	{
		return 0;
	}
	player->ResetSkillPoint();

	LUA_C_RETURN;
}

LUA_C_BODY(ShowQuitButton, 1, 1)
{
	Player* player = PLAYER_CAST(in.ReadHostSafely());
	if (!player)
	{
		return 0;
	}

	MapInstAttrs &mapAttrs = MapInstance::CurThreadInst()->MutableAttrs();
	if( mapAttrs.GetRandomTeamId() != 0 && !mapAttrs.IsDungeonCompleted() )
	{
		mapAttrs.SetDungeonCompleted(true);
		sTeamMgr->changeRandomTeamToNormalTeam(mapAttrs.GetRandomTeamId());
	}

	player->SetCheckDungeonDeserter(false);
	NetPack pack(pb::SMSG_DUNGEON_WILL_QUIT);
	player->Send(pack);

	LUA_C_RETURN;
}

LUA_C_BODY(Refresh, 2, 2)
{
	Creature* npc = CREATURE_CAST(in.ReadHostSafely());
	int time = in.ReadInt(0);

	if (npc==NULL || time <=0)
	{
		return 0;
	}

	npc->SetRespawnTime(time);
	npc->Disappear();

	LUA_C_RETURN;
}
*/