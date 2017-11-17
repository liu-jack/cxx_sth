#ifndef LogicItemProto_h__
#define LogicItemProto_h__
#include "def/TypeDef.h"
#include "datastruct/struct_item.h"

struct DB_ItemProtoType;
class LootList;
class Player;

namespace pb
{
    class GS2C_LootList;
    class GS2C_ItemUse;
}

class LogicItemProto : public DB_ItemProtoType
{
public:
    explicit LogicItemProto(const DB_ItemProtoType& db);
    ~LogicItemProto(){};
    
    bool CanBeCreateToBag() const;
    uint32 Id() const;
    bool CanStack() const;
	bool IsXilianEquip() const;
    uint32 Quality() const; // E_QUALITY
    int Class1() const; //
    int Class2() const; //
    int Class3() const; //

	int Value1() const;
	int Value2() const;
	int Value3() const;
    //int ItemType() const;


	uint32 ACCItemBaseExp() const;
    uint32 ObjectValue() const;

    bool CanBeEquiped() const;
    bool CanEquipToSlot( uint32 slot) const;
    uint32 GetEquipSlot() const;    
    bool TryGetPuzzleComposeCount( int& out_count) const;
    bool MatchCharClassRequired( int char_class) const;
    bool MatchCharLevelRequired( int char_level) const;
  
};




#endif // LogicItemProto_h__
