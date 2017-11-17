#ifndef ItemManager_h__
#define ItemManager_h__

#include "ptr_container/PtrMap.h"
#include "boost/tuple/tuple.hpp"
#include "boost/tuple/tuple_comparison.hpp"
#include "SimpleSingleton.h"
#include "def/TypeDef.h"
#include "LogicItemProto.h"
#include "mysqldata.h"
#include "def/ObjectDefines.h"

class Item;
struct ItemAttrBase;
struct ItemAttrAdd;

namespace pb
{
    class GS2C_SlotItem;
}


class IItemFactory
{
public:
    virtual ~IItemFactory(){};
    virtual Item* CreateItemInst( uint32 itemId, uint32 count = 1 ) = 0;
    virtual Item* CreateItemInst( const pb::GS2C_SlotItem& itemMsg ) = 0;
    virtual Item* CreateItemInst( const LogicItemProto* proto,  uint32 count = 1) = 0;
};                                                                        

typedef uint32 RLevel; // reinforce Lv, 物品强化等级(精炼等级)
typedef uint32 Level;  // item lv 
typedef uint32 UpgradeIdx; // item upgrade idx , item_upgrade 表的主键;


struct ItemAttrContain
{
    ItemAttrContain():attrBase(NULL), attrAdd(NULL){}
    const ItemAttrBase* attrBase;
    const ItemAttrAdd*  attrAdd;
};

typedef boost::tuple< uint32, uint32, uint32, uint32> PQCL; // Part, Quality, Class, Level(require)
typedef PtrMap<ItemId, LogicItemProto>	ItemProtoMap;
class ItemManager 
    : public SimpleSingleton<ItemManager>
    , public IItemFactory
{
    friend class SimpleSingleton<ItemManager>;
public:
     
private:
	typedef std::map<ItemId, const ItemEquipXiLian*> XiLianLevel2Attr;

	typedef std::map<ItemId, const ItemSuitComplex*> SuitId2Complex;

	typedef std::map<ItemId, const ItemBagSoltSell*> BagSoltId2SellInfo;

    typedef std::map<uint32, uint32> CharProto2ItemProto;

    typedef std::map<std::pair<uint32, uint32>,  ItemAttrContain > Itemtype_Quality2AttrContain;


    typedef std::multimap< PQCL, uint32> PQCL2ItemID;

    typedef std::map< uint32, std::set< uint32> > AccComposeMaterialMap; // set: material protoId

public:
    struct AccExpBook
    {
		public:
			AccExpBook( void ) : itemId( 0 ), exp( 0 ) {}

		public:
			uint32 itemId;
			uint32 exp;
	};

    typedef std::vector<AccExpBook> AccExpBooks;


public:
    void Init();
    const LogicItemProto* Find( ItemId itemId) const;
    bool TryGetCardItemId( uint32 charProtoId, uint32& itemid) const;
    uint32 PickItemIdMatchRequire( const PQCL& condition) const;


    // Item Factory
    virtual Item* CreateItemInst( ItemId itemId, uint32 count = 1 );
    virtual Item* CreateItemInst( const LogicItemProto* proto,  uint32 count = 1);
    virtual Item* CreateItemInst( const pb::GS2C_SlotItem& itemMsg );

    uint32 GetAccItemBaseExp( uint32 itemQuality) const;

    const ItemAttrContain* GetAttrContain( uint32 itemType, uint32 quality) const;
	const ItemSuitComplex* FindSuitComplex( ItemId itemId ) const;
	const ItemBagSoltSell* FindBagsoltSellInfo( ItemId itemId ) const;

	IntPair TranslateAttr(uint32 itemid,uint32 level);

	void GetSuitAttr(uint32 itemid, float (&attr)[CHARACTER_VALUES_COUNT]);		//获取套装的基础属性加成
	void GetNormalEquipAttr(uint32 itemid, float (&attr)[CHARACTER_VALUES_COUNT]);	//获取普通装备的基础属性加成				
	void GetRealItemAttr(uint32 itemid, float (&attr)[CHARACTER_VALUES_COUNT]);		//获取一件装备的基础属性加成
	
	int	GetItemMaxStarCount(uint32 itemid);
	int GetItemMaxXiLianLevel(uint32 itemid);
	bool isLastStarHide(uint32 itemid);
	//仅为功勋排行榜使用
	const ItemProtoMap& GetItemProtoMap() const ;

	//从Id得到一个秘技
	uint32 GetAttrFromId(const uint32 id);
	
	//按时间恢复的逻辑写在这里
	//void RecoverItemNums();
	
	//void _recoverPurifyTimes(); 

private:
    ItemManager(){};
    ~ItemManager(){};
	void InitLogicItemProto();
    void InitItemAttr();
private:
    ItemProtoMap		  m_protoMap;
	XiLianLevel2Attr      m_xiLianLevel2Attr;
	SuitId2Complex		  m_suitId2Complex;			//套装资料
	BagSoltId2SellInfo    m_bagSoltId2SellInfo;
    AccExpBooks           m_AccExpBooks;
    CharProto2ItemProto   m_charproto2itemproto;
    Itemtype_Quality2AttrContain m_typeQuality2AttrContain;
    AccComposeMaterialMap m_accComposeMaterialMap;
    PQCL2ItemID           m_PQCL2ItemId;

};

#define sItemMgr (ItemManager::Instance())

#endif // ItemManager_h__
