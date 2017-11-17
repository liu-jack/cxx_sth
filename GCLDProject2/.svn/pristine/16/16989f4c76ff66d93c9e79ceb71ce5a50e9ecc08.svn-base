#ifndef PVP_RANK_TABLE_H__
#define PVP_RANK_TABLE_H__

#include "structDef.h"
#include "def/TypeDef.h"


//set byte alignment 
#pragma pack(push,1)

#define PvPFightRecordMaxCount 15


struct PvPRankTableExtra
{
public:
	PvPRankTableExtra() :isFighting( false ), fightRecordIndex( 0 ), targetPlayerId( 0 ) {}

public:
	bool   isFighting ;
	int    fightRecordIndex ;
	uint64 targetPlayerId ;
};

struct PvPRankTable : public BaseStruct
{
public:
	typedef uint64 Key ;

public:
	INIT_GAMEDATA_AND_EXTRA( PvPRankTable, extra ) ;

public:
	static const bool HasIndex()
	{
		return true;
	}

	const Key& GetKey( void ) const
	{
		return playerId ;
	} 

	uint32 GetLogicalKey( void) const
	{
		return rank;
	}

	void SetDbIncreaseId(const IdType& id_)
	{
		id = id_;
	}

	static const char* GetDbIncreaseIdName()
	{
		return "id";
	}

	static const char* GetLogicalKeyName( void )
	{
		return "rank" ;
	}

	static const char* GetTableName()
	{
		return "pvp_rank";
	}

	static const char* GetFormat()
	{
		return "bbbuuuu"
			"sssssssssssssss";
	}

	void AddOneFightRecord( const std::string &str )
	{
		fightRecord[extra.fightRecordIndex] =str ;
		++extra.fightRecordIndex ;
		extra.fightRecordIndex = extra.fightRecordIndex % PvPFightRecordMaxCount ;
	}

public:
	IdType      id ;
	uint64      accountId ;
	uint64      playerId ;
	uint32      rank ;
	uint32      historyRank ;
	uint32      regionId ;
	uint32      serverId ;
	
	std::string fightRecord[PvPFightRecordMaxCount] ;

public:
	PvPRankTableExtra extra ;
} ;


#pragma pack(pop)

#endif
