#ifndef ItemArray_h__
#define ItemArray_h__

#include <vector>
#include <list>
#include <set>
#include "Item.h"
#include "ModMarker.h"
#include "def/TypeDef.h"
#include "Enum.pb.h"

class LogicItemProto;
class Item;
class ItemEquip;
namespace pb
{
	class GS2C_Bag;
}

typedef std::list< Item *> ItemPtrList;

typedef std::list< const Item *> ConstItemPtrList;
class ItemArray
    : public ModMarker<int>
{
public:
    explicit ItemArray( int capacity = 0x7FFFFFFF);
    ~ItemArray();;

    enum{ MIN_SLOT = 1};
    
    // ���� ��Ʒ�� ����п��ܲ������ƣ� ��������������� ������player�еġ�
    bool CreateItem( int entry,uint32 bagCapcity, int count = 1, ItemPtrList* outList = NULL,ItemPtrList* sell_list = NULL);
    bool CreateItem( const LogicItemProto* proto,uint32 bagCapcity, int count = 1, ItemPtrList* outList = NULL,ItemPtrList* sell_list = NULL);

	uint32 CurBagItemCount();
	// ɾ��
    bool    DestroyItem(int entry, int count );
    bool    DestroyItem(const ItemID2Count& itemid2count);
    bool    DestroySlotItem(int slot, int& out_item_proto_id, int& out_count);
    bool    TakeOutSlotItem(int slot, int count, int& out_item_proto_id);
	bool	SellSlotItem( int slot, int& out_item_proto_id, int& out_count, bool forceSell = false);
	bool	BuyBackSlotItem( int slot, int& out_item_proto_id, int& out_count);
	void    DestroyAll();
	bool	RemoveExpireItem(std::vector<int>& itemIdVec);
	bool	CheckExpireItem(std::vector<int>& itemIdVec);

    
    void    ItemChecked(int slot); // for setold  

    // ��ѯ �� ��ȡ
    const Item* GetItem(int slot) const; 

	bool IsEquipForBagSolt( int slot ) const;
	ItemEquip* MutableItemEquip( int slot ) const;
	ItemEquip* MutableItemEquipByProId( int entry ) const;
	bool HasSameMaxLvAttrEquip(int entry, int attrid,int& out_item_solt) const;
	void GetItems(int entry, ConstItemPtrList& itemList) const; 
    Item*   MutableItem(int slot) const; 
	Item* MutableOneItemByProId( int entry, bool isSold ) const;
	int     GetCountItem(int entry) const;
    int     CountItem(int entry) const;
	int     CountItemBySolt(int slot) const;
	bool	CanAddItemToBag( int entry, uint32 bagCapcity );
	bool    EnoughItem(int entry, int count) const;
    bool    EnoughItem(const ItemID2Count& itemid2count) const;
    
    template< class Pred >
    int     CountItem(int entry, const Pred& pred) const;
    template< class Pred >
    int     EnoughItem(int entry, int count, const Pred& pred) const;  
	int     ItemCount() const;

    bool    HasItem(int entry) const; 
	bool    IsValidSlot( int slot ) const;  

	bool    SetMaxSolt(int nMax);
	int     GetMaxSolt() const;

	bool    HasFreeSolt() const;
	void WriteAllItemTo( pb::GS2C_Bag& bagMsg) const;
	//uint32 WearQualityAndStarCount(uint32 quality,uint32 star);//������ӦƷ�ʺ��Ǽ���װ������
	uint32 GetQualityAndStarCount(uint32 quality,uint32 star = 0);//�����ӦƷ�ʼ����Ϻ��Ǽ���װ��������
	uint32 GetQualityAndAnyStar(uint32 quality,uint32 star); //����ض�Ʒ�ʣ������Ǽ���װ��
	uint32 HaveEquipClass(uint type);   //ӵ����Ӧ���� ��װ��
	uint32 CountEquipClass(uint type); // �����Ӧ���� ��װ��
	uint32 GetQualityEquipOfSuitCount( const uint32 quality);//���Ʒ��װ��������
	uint32 GetGreaterQualityEquipOfSuitCount(const uint32 quality);//���Ʒ��װ��������
	uint32 GetHasMijiEquipCount(const uint32 quality) const;
public:
    // Save Load
    template< typename T> void SaveTo( T& msg) const;

    template< typename T> void LoadFrom( const T& msg);

    template< typename T> void WriteModifiedTo( T& msg) const;
    // ������ �ͻ�����
    template< typename T> void LoadModifiedFrom( const T& msg);

public:
    // ���á���
    void    CollectFreeEquipAndSort(std::vector<ItemEquip*>& equip_group);


protected:
    void    Add(int slot, Item* item, bool isNewCreated);
	int     CountGem() const;
    virtual void AfterItemAdded(int slot, Item* item, bool isNewCreated);

private:     
	bool   CreateItemOnFreeSlots(const LogicItemProto* proto, int count,uint32 bagCapcity, ItemPtrList* outList = NULL,ItemPtrList* sell_list = NULL);
    uint32  GetOneFreeSlotId() const;
    void    EraseItem( TableByProtoIdItr &itr );
    void    EraseItem( TableByItemSlotItr &itr);
	EqualRangeByProtoId GetEqualRangeByProtoId( int entry) const;
protected:
    int				        m_capacity;
	int						m_maxSolts;
    MultiKeyItemTable       m_itemTable;
	bool			        m_needCheckExpire;
};














//////////////////////////////////////////////////////////////////////////

template< class Pred >
int ItemArray::CountItem( int entry, const Pred& pred ) const
{
    int32 total = 0;
    EqualRangeByProtoId pItr = GetEqualRangeByProtoId( entry ) ;
    for( TableByProtoIdItr itr =pItr.first; itr != pItr.second; ++itr )
    {
        const Item *pItem = *itr;
        if (! pred( pItem))
            continue;
        total += pItem->StackCount();
    }
    return total;
}


template< class Pred >
int ItemArray::EnoughItem( int entry, int count, const Pred& pred ) const
{
    return CountItem(entry, pred) >= count;
}


#endif // IItemArray_h__
