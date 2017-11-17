#ifndef CharacterStorage_h__
#define CharacterStorage_h__

#include "def/TypeDef.h"
#include "Character.h"
#include "CharacterAttr.h"

#include "ptr_container/PtrMap.h"
#include "ptr_container/PtrVector.h"
#include "ModMarker.h"


namespace pb
{
    class GS2C_CharacterStorage;
    class GS2C_BattleCharacter;
	class CharacterMsg;
}
class Player;

typedef std::vector< CharacterId> CharacterIds;
typedef std::vector< Character*>  CharacterPtrs;
typedef PtrMap< CharacterId, Character >    CharacterContainer;

class CharacterStorage : public ModMarker<int>
{
    typedef PtrVector<CharacterAttr> BattleCharAttr;
public:
	explicit CharacterStorage();
    ~CharacterStorage();
    static const uint32 MaxBattleMemberCount = 4;

    bool HasCharacter( CharacterId charId) const;
    uint32 HasCharacterProto( uint32 protoId) const;

    Character* CreateCharacter(Player* player, uint32 protoId);
    bool DestroyCharacter( CharacterId charId);


    const CharacterIds& GetBattleArray() const;  //non-mutable
    void SetBattleArray(Player* player,const CharacterIds& battleArray);
    bool IsInBatterArray( CharacterId charId) const;    
	
	////
	const uint32 GetCountOfContainer() const;
    const Character* GetCharacter( CharacterId charId) const;
    Character* MutableCharacter( CharacterId charId);
    Character* GetCharacterByTableId(uint32 tableId);

	Character* MutableFakeCharacter(CharacterId charId);

    template< class Pred >
    CharacterId CollectCharacter(Pred& pred, CharacterIds& outIds);

    template< class Pred >
    CharacterId CollectCharacter(Pred& pred, CharacterPtrs& outIds);
    
    bool CanBeDestroy( CharacterId charId) const;

	///uint32 GetCurBattleCharacterCount() const;
	uint32 CharacterHasZhaoMu(CharacterId charId);
	bool CharacterHasMoved();//任意武将移动
	uint32 GetLeaderProtoId() const;
    
    template< class Pred>
    uint32 GetBattleArrayMatchCount( const Pred& pred) const;

    template< class Pred>
    void ForEachBattleCharacter(Pred& pred);

	template< class Pred>
	void ForEachBattleCharacterInCombat(Pred& pred, uint64 combatid);

    uint32 GetCountBAMatchLevel( int value) const;
    uint32 GetCountBAMatchReinforce( int value) const;
    uint32 GetCountBAMatchQuality( int value) const;
    uint32 GetCountBAMatchSkillLv( int value) const;
    uint32 GetWearEquip(int value) const;
	void   InitCharactersCombatData();

    CharacterAttr* GetCharacterAttr( Player* player,uint32 battleArrayIdx);
    void DeleteCharacterAttr( Player* player,uint32 battleArrayIdx);

	bool HasCharacterInCity(uint16 cityID);
	//穿上特定品质及以上的装备 1星
	uint32 WearQualityAndStarCount(Player* player,uint32 quality,uint star);
	//穿上特定品质及任意星级
	uint32 WearQualityAndAnyStar(Player* player,uint32 quality,uint star);
	uint32 WearEquipClass(Player* player,uint type);

	void FullCharHP();
public:
    // message
    void LoadFrom(Player*player, const pb::GS2C_CharacterStorage& msg );
    void LoadFrom(Player*player, const pb::GS2C_CharacterCreate& msg);

    void SaveTo( pb::GS2C_CharacterStorage& msg ) const;
    void SaveBattleArrayToMsg( pb::GS2C_BattleCharacter& msg) const;
	void SaveToMsg(pb::CharacterMsg & msg) const;

	void Synchronization(Player*player, const pb::GS2C_CharacterStorage& msg);
	void Synchronization(Player*player, const pb::GS2C_CharacterCreate& msg);
	void CreateFakeCharacter(Player*player);
private:
    Character* CreateCharacterInst(Player*player, const pb::GS2C_CharacterCreate& msg);
    Character* CreateCharacterInst( Player*player,const CharacterProto& proto );
    Character* CreateCharacterInst(Player*player, uint32 protoId );

    int FindFistFreeId() const;
    int GenFreeId();
	int GenFakeCharId();

//slg武将相关------------------------------------------------------------
public:
    Character* GetMaxLvHero();
	Character* GetMaxLvHeroInCity(uint32 cityid);
	Character* GetMaxLvHeroInCombat(uint32 combatid);
	Character* GetMaxLvFakeHeroInCombat(uint32 combatid);
    void MoveTo(Player& player, uint8 charId, uint32 destCityId);
	uint32 GetMilitaryPower();
    bool CanSpeedUp(uint32 cityId);
    void SetSpeedUp(uint32 cityId);

	CharacterIds& GetCharIDLst();// mutable

	int GetAllCharMaxHp();
	bool CheckHasCombatChar();
	
	void BattleCharAttrResize(uint32 size);
	void SetBattleMemberCount(Player* player,uint32 count);
	const uint32 GetBattleMemberCount(Player* player) const;
	CharacterContainer& GetMutableCharacterContainer() ;
	void LoadTechnologyAttr(Player* player);

public:
    std::map<uint32, uint64> m_SpeedUpTime; //帅旗加速

//slg武将相关------------------------------------------------------------

protected:
   /* Player*                 m_player;*/
    CharacterContainer      m_characters;	//解锁的武将
    CharacterIds            m_battleArray;	//招募的武将
    int                     m_maxCharId;
	int						m_maxFakeCharId;

    BattleCharAttr          m_battleCharAttr;

	CharacterContainer		m_charactersFake;	//教学关卡的固定武将
};

template< class Pred>
uint32 CharacterStorage::GetBattleArrayMatchCount( const Pred& pred ) const
{
    uint32 count = 0;
    for ( CharacterIds::const_iterator it = m_battleArray.begin(); it != m_battleArray.end(); ++it )
    {
        const Character* character = GetCharacter( *it);
        if ( character && pred( character))
        {
            ++count;            
        }
    }
    return count;
}



template< class Pred >
CharacterId CharacterStorage::CollectCharacter(Pred& pred, CharacterIds& outIds )
{
    for ( CharacterContainer::iterator it = m_characters.begin();
        it != m_characters.end(); ++it)
    {
        if ( pred( it->second))
        {
            outIds.push_back( it->second->GetID());
			return it->first;
        }
    }
	return 0;
}

template< class Pred >
CharacterId CharacterStorage::CollectCharacter(Pred& pred, CharacterPtrs& outIds)
{
    for ( CharacterContainer::iterator it = m_characters.begin();
        it != m_characters.end(); ++it)
    {
        if ( pred( it->second))
        {
            outIds.push_back( it->second);
			return it->first;
        }
    }
	return 0;
}

template< class Pred >
void CharacterStorage::ForEachBattleCharacter(Pred& pred)
{
    for (CharacterIds::const_iterator it = m_battleArray.begin(); it != m_battleArray.end(); ++it)
    {
        if (Character* character = MutableCharacter(*it))
        {
            pred(character);
        }
    }
}

template< class Pred >
void CharacterStorage::ForEachBattleCharacterInCombat(Pred& pred, uint64 combatid)
{
	for (CharacterIds::const_iterator it = m_battleArray.begin(); it != m_battleArray.end(); ++it)
	{
		if (Character* character = MutableCharacter(*it))
		{
			if(character->nowCombatGroupID == combatid)
				pred(character);
		}
	}

	//假武将也查找
	for (CharacterContainer::iterator it = m_charactersFake.begin();
		it != m_charactersFake.end(); ++it)
	{
		if(it->second->nowCombatGroupID == combatid)
			pred(it->second);
	}
}

#endif // CharacterStorage_h__
