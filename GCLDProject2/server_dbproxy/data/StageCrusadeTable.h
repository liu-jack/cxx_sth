#ifndef _STAGE_CRUSADE_TABLE_H_
#define _STAGE_CRUSADE_TABLE_H_

#include "structDef.h"
#include "def/ConstDef.h"
//一位对齐
#pragma pack(push,1)

namespace pb{
	class GxDB_Combat_Crusade;
}

struct StageCrusadeTable : public BaseStruct
{
public:
	
	INIT_GAMEDATA( StageCrusadeTable);

public:
	static const bool HasIndex()
	{
		return true;
	}

	const Key GetKey(void) const
	{
		return id;
	}

	uint64 GetLogicalKey(void) const
	{
		return player_id;
	}

	void SetDbIncreaseId(const IdType& id_)
	{
		id = id_;
	}

	static const char* GetDbIncreaseIdName(){ return "id"; }
	static const char* GetLogicalKeyName(void)
	{
		return "player_id";
	}

	static const char* GetTableName()
	{
		return "stage_crusade";
	}

	static const char* GetFormat()
	{
		return "bbuuuubu"
			"uuuuuuuuuu"
			"uuuuuuuuuu"
			"uuuuuuuuuuu";
	}

	void SaveTo(pb::GxDB_Combat_Crusade& msg);
	void LoadFrom(const pb::GxDB_Combat_Crusade& msg);

public:
	IdType		id;
	uint64		player_id;
	uint32		restReset;	//剩余重置次数
	uint32		currStage;	//当前副本哪一关卡
	uint32		isDead;		//当前副本是否结束，0未结束，1结束
	uint32		addCharTimes;//填充武将次数
	uint64		award_state;
	uint32		isCrusadeOver;
	uint32		CharTimes[CHARACTER_COUNT];///每个武将的增兵次数
};

#pragma pack(pop)
#endif