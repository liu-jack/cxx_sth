#pragma once

/*//rempha LOAD_DB_TAB  (PhasingArea) \*/
	/*LOAD_DB_TAB  (Skill_Filter) \*/
	/*LOAD_DB_TAB  (SkillContain) \
	LOAD_DB_TAB  (SkillCondition) \
	LOAD_DB_TAB  (SkillGroupLink) \
	LOAD_DB_TAB  (Skill_Sub) \
	LOAD_DB_TAB  (CreatureAnimation) \
	LOAD_DB_TAB  (CreatureSkill) \
	LOAD_DB_TAB  (SkillBasicAttack) \
	LOAD_DB_TAB  (SkillVisualPattern)\
	LOAD_DB_TAB  (SkillGroupOwn) \
	LOAD_DB_TAB  (SkillVisualEffect)\
	LOAD_DB_TAB  (SkillVisualState)\
	LOAD_DB_TAB  (SkillProto) \
	LOAD_DB_TAB  (Festival) \
	LOAD_DB_TAB  (Aura_Runes)\
	LOAD_DB_TAB  (C_Keyframe)\
	LOAD_DB_TAB  (CombatWindowComment) \
	LOAD_DB_TAB  (CombatSchemeComment) \
    */
#define LOAD_ALL_DB_TABLES \
	LOAD_DB_TAB  (DB_Module) \
	LOAD_DB_TAB  (DB_BuildingProto) \
	LOAD_DB_TAB  (DB_BuildingLevelUp) \
	LOAD_DB_TAB  (DB_ImperialRebuild) \
	LOAD_DB_TAB  (DB_FunctionOpen) \
	LOAD_DB_TAB	 (DB_FunctionOpenOrder) \
	LOAD_DB_TAB  (DB_Build_buff) \
	LOAD_DB_TAB  (DB_Buff) \
    LOAD_DB_TAB  (DB_FubenProto) \
    LOAD_DB_TAB  (DB_FubenStage) \
	LOAD_DB_TAB  (DB_OpenFog)\
	LOAD_DB_TAB  (DB_SkillSlg) \
	LOAD_DB_TAB  (DB_Buff_Fight) \
	LOAD_DB_TAB  (DB_Buff_Relation) \
    LOAD_DB_TAB  (DB_Science) \
	LOAD_DB_TAB  (DB_MapCity) \
	LOAD_DB_TAB  (DB_MapBox) \
	LOAD_DB_TAB  (DB_MapEvent) \
	LOAD_DB_TAB  (DB_MapFog) \
	LOAD_DB_TAB  (DB_TerrainText) \
	LOAD_DB_TAB  (DB_Panzer) \
	LOAD_DB_TAB  (DB_Trigger) \
	LOAD_DB_TAB  (DB_MarketBlack) \
	LOAD_DB_TAB  (DB_MarketBuy) \
	LOAD_DB_TAB  (DB_MarketCollect) \
	LOAD_DB_TAB  (DB_MysteryShop) \
	LOAD_DB_TAB  (DB_CountryUp) \
	LOAD_DB_TAB  (DB_CountryInvest) \
	LOAD_DB_TAB  (DB_CountryInvestTitle) \
	LOAD_DB_TAB  (DB_CountryGuardUp) \
	LOAD_DB_TAB  (DB_CountryQuest) \
	LOAD_DB_TAB  (DB_Country_PK_Rank) \
	LOAD_DB_TAB  (DB_Country_Suppress) \
	LOAD_DB_TAB  (DB_Country_DefenceBorderland) \
	LOAD_DB_TAB	 (DB_CountryRoad) \
	LOAD_DB_TAB  (DB_MonsterCity) \
	LOAD_DB_TAB  (DB_Open_Frontier) \
	LOAD_DB_TAB  (DB_Attack_Defense_Quest) \
	LOAD_DB_TAB  (DB_Palace_Proto) \
	LOAD_DB_TAB  (DB_SoldierFood) \
	LOAD_DB_TAB  (DB_Dungeon) \
	LOAD_DB_TAB  (DB_Cane) \
	LOAD_DB_TAB  (DB_BaoQi) \
	LOAD_DB_TAB  (DB_BaoQi_Unlock) \
	LOAD_DB_TAB  (DB_BaoQi_UpStar) \
    LOAD_DB_TAB  (DB_CharProto) \
    LOAD_DB_TAB  (DB_CharUpgradeXP) \
    LOAD_DB_TAB  (DB_CharUpgradeLevelItem) \
	LOAD_DB_TAB  (DB_SoldierProto) \
    LOAD_DB_TAB  (DB_Altar)\
    LOAD_DB_TAB  (DB_Smelter)\
    LOAD_DB_TAB  (DB_Smithy)\
    LOAD_DB_TAB  (NpcProto)\
    LOAD_DB_TAB  (NpcAttrParamGrade)\
	LOAD_DB_TAB  (NpcAttrBaseAdditon)\
    LOAD_DB_TAB  (NpcAttrBase)\
    LOAD_DB_TAB  (NpcAttrParamCombat)\
    LOAD_DB_TAB  (BaseDefine)\
	LOAD_DB_TAB  (DB_ItemProtoType)\
    LOAD_DB_TAB  (ItemAttrBase)\
    LOAD_DB_TAB  (ItemAttrAdd)\
	LOAD_DB_TAB  (ItemEquipXiLian)\
	LOAD_DB_TAB  (ItemSuitComplex)\
	LOAD_DB_TAB  (ItemBagSoltSell)\
	LOAD_DB_TAB  (LootFeature)\
	LOAD_DB_TAB  (LootFeatureItems)\
	LOAD_DB_TAB  (Loot_Filter)\
	LOAD_DB_TAB	 (DB_SpecialEffect) \
	LOAD_DB_TAB  (SoundProto)\
	LOAD_DB_TAB  (DB_SellProgram)\
	LOAD_DB_TAB  (DB_SellProgItems)\
	LOAD_DB_TAB  (DB_SellProto)\
    LOAD_DB_TAB  (DB_CharAttrBase)\
    LOAD_DB_TAB  (DB_CharAttrAdd)\
    LOAD_DB_TAB  (DB_CharAttrReinforce)\
	LOAD_DB_TAB  (NpcDisplay)\
	LOAD_DB_TAB  (DB_QuestProto)\
	LOAD_DB_TAB  (DB_QuestReward)\
	LOAD_DB_TAB  (LootProto)\
    LOAD_DB_TAB  (DB_CharSetSkills)\
    LOAD_DB_TAB  (DB_QuestCounter)\
	LOAD_DB_TAB  (Event_mail)\
    LOAD_DB_TAB  (DB_CharAttrRandom)\
	LOAD_DB_TAB  (DB_HeadIconUnlock)\
	LOAD_DB_TAB  (DB_ResourceInit)\
	LOAD_DB_TAB  (GuildanceDialogueProto)\
	LOAD_DB_TAB  (GuildanceProto)\
    LOAD_DB_TAB  (DB_BasePlayer)\
    LOAD_DB_TAB  (DB_VipLevelPro)\
    LOAD_DB_TAB  (DB_FirstPay)\
    LOAD_DB_TAB  (DB_Military_Supplies)\
	LOAD_DB_TAB  (DB_TunTian_Up) \
	LOAD_DB_TAB (DB_ExploitRank)\
	LOAD_DB_TAB (DB_ExploitBoxProto)\
	LOAD_DB_TAB (DB_ChallengeRank)\
	LOAD_DB_TAB (DB_AttackCityRank)\
	LOAD_DB_TAB	(DB_Expedition)\
	LOAD_DB_TAB	(DB_CostSetProto)\
	LOAD_DB_TAB (DB_DailyQuest)\
	LOAD_DB_TAB (DB_DailyQuestReward)\
	LOAD_DB_TAB (DB_GrowUpQuestProto)\
	LOAD_DB_TAB (DB_GrowUpQuestGroup)\
	LOAD_DB_TAB (DB_PersonalQuest)\
	LOAD_DB_TAB (DB_SkipProto)\
	LOAD_DB_TAB (DB_Function_Notice)\
	LOAD_DB_TAB (DB_SignUp)\
	LOAD_DB_TAB (DB_RemindProto)\
	LOAD_DB_TAB (DB_ActivityStageRewardDiamond)\
	LOAD_DB_TAB (DB_ActivityStarBox)\
	LOAD_DB_TAB (DB_CrossKillQuest)\
	LOAD_DB_TAB (DB_ActivityXilian)\
	LOAD_DB_TAB (DB_ActivityObsidianCollect)\
	LOAD_DB_TAB (DB_ArmyGovText)\
	LOAD_DB_TAB (DB_ArmyGovRewardType)\
	LOAD_DB_TAB (DB_ArmyGovRewardNum)\
	LOAD_DB_TAB (DB_ArmyGovRewardCritical)\
	LOAD_DB_TAB (DB_ArmyGovLoyalReward)\
	LOAD_DB_TAB (DB_ActivityVisitFamousReward)\
	LOAD_DB_TAB (DB_ActivityVisitFamousNpc)\
	LOAD_DB_TAB (DB_ActivityVisitFamousEvent)\
	LOAD_DB_TAB (DB_TeachMapProto)\
	LOAD_DB_TAB (DB_TeachWorld)\
	LOAD_DB_TAB (DB_ActivityGeneralRewardBox)\
	LOAD_DB_TAB (DB_ActivityGeneralRewardProto)\
	LOAD_DB_TAB (DB_ActivityRechargeRebate)\
	LOAD_DB_TAB (DB_WorldFightAchievement)\
	LOAD_DB_TAB (DB_WorldFightAchievementBox)\
	LOAD_DB_TAB (DB_CharSiegeArmy)\
	LOAD_DB_TAB (DB_SiegeArmySpecific)\
	LOAD_DB_TAB (DB_SiegeArmyUp)\
	LOAD_DB_TAB (DB_MonthCard)\
	LOAD_DB_TAB (DB_Teach_Soldier)\
	LOAD_DB_TAB (DB_ContinueOccupyCity)\
	LOAD_DB_TAB (DB_strategy_world_map)\
	LOAD_DB_TAB (DB_strategy_become_stronger)\
	LOAD_DB_TAB (DB_strategy_country_grow)\
	LOAD_DB_TAB (DB_strategy_cross_war)\
	LOAD_DB_TAB (DB_strategy_game)\
	LOAD_DB_TAB (DB_strategy_quick_level)\
	LOAD_DB_TAB (DB_AddSoliderMatch)\
	LOAD_DB_TAB (DB_OccupationMatch)\
	LOAD_DB_TAB (DB_CountryMatch)\
	LOAD_DB_TAB (DB_FightValue)\
	LOAD_DB_TAB (DB_FightRankReward)\
	LOAD_DB_TAB (DB_CongratulateConfig)\
	LOAD_DB_TAB (DB_GuidanceProtoSpecial)\
	LOAD_DB_TAB (DB_LotteryDaily)\
	LOAD_DB_TAB (DB_StrongerQuest)\
	LOAD_DB_TAB (DB_StrongerGroup)\
	LOAD_DB_TAB (DB_PersonalEvent)\
	LOAD_DB_TAB (DB_WorldEvent)\
	LOAD_DB_TAB (DB_EventQuestPool)\
	LOAD_DB_TAB (DB_PersonalEventAward)\
	LOAD_DB_TAB (DB_WorldEventAward)\




    
    
	
#define LOAD_ALL_DB_STRING_TABLES  \
    LOAD_DB_TAB  (String_Server)\
	LOAD_DB_TAB  (String_Ui)\
	LOAD_DB_TAB  (String_Country)\
	LOAD_DB_TAB  (String_Item) \
	LOAD_DB_TAB	 (String_Quest) \
	LOAD_DB_TAB  (String_Skill) \
	LOAD_DB_TAB  (String_Mail) \
	LOAD_DB_TAB  (String_Npc)\
	LOAD_DB_TAB  (String_Stage)\
    LOAD_DB_TAB  (String_Char)\
	LOAD_DB_TAB  (String_Buff)\
    LOAD_DB_TAB  (String_Palace)\
	LOAD_DB_TAB  (String_Name_Prefix)\
	LOAD_DB_TAB  (String_Name_Suffix)\
	LOAD_DB_TAB  (String_Dialogue)\
	LOAD_DB_TAB  (String_Cost_Set)\
	LOAD_DB_TAB  (String_Vip)\
	LOAD_DB_TAB	 (String_Daily_Quest)\
	LOAD_DB_TAB	 (String_Growup)\
	LOAD_DB_TAB	 (String_World_Events)\

/**/

#ifdef _TOOL 
#define LOAD_TOOL_TABLES \
	LOAD_DB_TAB	 (AreaTriggerGroup) \
	LOAD_DB_TAB  (ObjectFilter) \
    LOAD_DB_TAB  (QuestFilter)
#else  // _TOOL
#define LOAD_TOOL_TABLES
#endif  // _TOOL