#ifndef ItemEquip_h__
#define ItemEquip_h__

#include "def/TypeDef.h"
#include "Item.h"
#include "def/ObjectDefines.h"

#define  EQUIPITEMSKILLATTRCOUNT  4
#define  EQUIPITEMSKILLATTRMAXLEVEL  5

#define MAX_EQUIP_LEVLE 1000000
#define MAX_REINFORCE_TIMES 111111111

namespace pb
{
    class GS2C_SlotItem;
}

class ItemEquip : public Item
{
public:
    ItemEquip(const LogicItemProto* proto);
    virtual ~ItemEquip(void){};

    virtual bool CanBeDestroyed() const;


    bool IsEquiped() const;


    void AttachToCharacter( uint32 charId);
    void DettachFromCharacter( uint32 charId);

    virtual void SaveTo( pb::GS2C_SlotItem& msg ) const;
    virtual void LoadFrom( const pb::GS2C_SlotItem& msg );
	virtual void LoadFrom( const ItemEquip* other);

	inline uint32 Level() const     {  return 0;/*m_equip_level; */ }
    inline uint32 Reinforce() const {  return 0;/*m_equip_reinforce;*/ }
    inline uint32 EquiperId() const {  return m_equipedCharId; }

	void UpdateAttrLv(IntPairVec& alv);
    bool TryAddLevel( uint32 level );
    bool TryAddReinforce( uint32 rein);    
	bool IsSameAtrrMaxLv() const ;
	bool ReTrievalSameAttrMaxLv();
	bool CheckUpdateSameAtrrMaxLv();

    uint32 GetSumXp() const;
    void Rebirth();
	//是否开启秘技属性
	bool isHasLastAttr(const uint32 ItemId) const;

	uint32 GetAttr(int solt) const;
	void   SetAttr(int solt, uint32 attrId);
	uint32 GetAttrLevel(int solt) const;
	void   SetAttrLevel(int solt, uint32 attrlevel);
	void   AddAttrLevelOne(int solt);//没个属性升一级（不超过5）
	bool  IsAllAttrIsMaxLevel() const;
	bool IsHasMiji() const;
	uint32 GetAttrCount() const;

	bool IsSameMaxLevelAttr(uint32 atrrId);

	const void GetTotalAttr(float (&attr)[CHARACTER_VALUES_COUNT], bool isLastAttrWork) const;		//获得所有属性加成	最后一个属性是否生效

private:
    //uint32& MutableLevelWithXp();
   // uint32  GetLevelWithXp() const;
    uint32  GetMinLevelWithXp() const;

private:
    uint32  m_equipedCharId;
	//uint32 m_equip_level;
	//uint32 m_equip_reinforce;
	uint32  m_attr[EQUIPITEMSKILLATTRCOUNT];
	uint32  m_level[EQUIPITEMSKILLATTRCOUNT];
};

bool CompareItemEquip( const ItemEquip* EQ1, const ItemEquip* EQ2 );



#endif // ItemEquip_h__
