#pragma once
//数字序号不能改，一一对应，加表的时候只能数字往下加，这是用于生成唯一表索引id，合服时不冲突
#define LOAD_ALL_PLAYER_MAP_TAB\
    LOAD_TAB (ItemBaseTable)\
    LOAD_TAB (ItemEquipTable)\
    LOAD_TAB (CharacterTable)\
	LOAD_TAB (CharacterJueXingTable)\
    LOAD_TAB (CharacterTrainingTable)\
    LOAD_TAB (QuestCounterTable)\
    LOAD_TAB (QuestLogTable)\
    LOAD_TAB (PlayerMailTable)\
	LOAD_TAB (FuBenPassStageTable)\
	LOAD_TAB (FuBenHideTable)\
	LOAD_TAB (DungeonHeroTable)\
	LOAD_TAB (PlayerStringTable)\
	LOAD_TAB (TechnologyStudyTable)\
	LOAD_TAB (MapLogicCityEventTable)\
	LOAD_TAB (ModuleTable)\
	LOAD_TAB (BuildingTable)\
	LOAD_TAB (FogRecMap )\
	LOAD_TAB (DailyQuestTable)\
	LOAD_TAB (GrowUpQuestTable)\
	LOAD_TAB (GrowUpQuestGroupTable)\
	LOAD_TAB (SignUpTable)\
	LOAD_TAB (PassStageRewardRecordTable)\
	LOAD_TAB (ActivityStarBoxTable)\
	LOAD_TAB (PalaceOfficeAchievement)\
	LOAD_TAB (GeneralRewardTable)\
	LOAD_TAB (WorldFightAchievementTable)\
	LOAD_TAB (SeigeForceTable)\
	LOAD_TAB (ContinueOccupyCityTable)\
	LOAD_TAB (CongratulateTable)\
	LOAD_TAB (CongratulateBeenCongratulatedTable)\
	LOAD_TAB (BranchFuBenTable)\
	LOAD_TAB (WannaBeStrongerTable)\



#define LOAD_ALL_PLAYER_KEY_TAB2\
    LOAD_TAB (BattleArrayTable)\
    LOAD_TAB (QuestCompleteTable)\
	LOAD_TAB (FuBenBaseTable)\
	LOAD_TAB (MarketTable)\
	LOAD_TAB (SmithyTable)\
	LOAD_TAB (AltarTable)\
	LOAD_TAB (BaoQiTable)\
	LOAD_TAB (DungeonTable)\
	LOAD_TAB (MapLogicTable)\
	LOAD_TAB (StageCrusadeTable)\
	LOAD_TAB (ZhengWuTable)\
	LOAD_TAB (PersonalECTable)\


#define LOAD_ALL_PLAYER_KEY_TAB\
    LOAD_TAB (PlayerTable)\
    LOAD_ALL_PLAYER_KEY_TAB2


// 这里的表格为每个游戏服一份，不单独设定在那个玩家身上的数据
// LOAD_TAB (ServerGobalValueTable)
//LOAD_TAB (DB_FriendInfo)
#define LOAD_ALL_GOBAL_TAB\
	LOAD_TAB (CountryBaseTable)\
	LOAD_TAB (PalaceOfficeTable)\
	LOAD_TAB (ActivityProtoTable)\
	LOAD_TAB (ActivityPlanTable)\
	LOAD_TAB (ServerOpenTimeTable)\
	LOAD_TAB (ActivityRewardTable)\
	LOAD_TAB (ActivityCountryOccupyCityTable)\



#define LOAD_ALL_GOBAL_MAP_TAB\
    LOAD_TAB (PlatfromMailTable)\
	LOAD_TAB (CountryCityTable)\
	LOAD_TAB (CountryLevelUpTaskPlayerTable)\


#define LOAD_ALL_TAB\
    LOAD_ALL_PLAYER_MAP_TAB\
    LOAD_ALL_PLAYER_KEY_TAB\
    LOAD_ALL_GOBAL_TAB\
    LOAD_ALL_GOBAL_MAP_TAB\


