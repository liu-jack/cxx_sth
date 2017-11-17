#ifndef Item_h__
#define Item_h__

#include "def/TypeDef.h"
#include "LogicItemProto.h"

class Player;
class Character;

namespace pb
{
    class GS2C_SlotItem;
}

typedef int ErrorCode; 

class Item
{
    friend class ItemArray;
public:
    Item(int itemId, int count = 1);
    Item(const LogicItemProto* proto, int count = 1);

    virtual ~Item(void){};
    const LogicItemProto* Proto() const { return m_proto; }
    
    uint32  Entry() const;
    uint32  BagSlot() const;
    int     StackCount() const;
	bool     CanStack() const;
    bool    IsNew() const;
	bool IsSold() const;
	time_t RepoEndTime() const;
	bool IsJewel() const;
	bool IsGem() const;
	bool    IsEquip() const;
	uint32  SellPrice() const;
	bool   IsBagItemType() const;
	inline uint32 Quality() const   {  return Proto()->Quality();  }
    inline uint32 ProtoId() const   {  return Proto()->id;  }

    virtual bool    CanBeDestroyed() const{ return true; };
	virtual bool    CanBeSell() const;
	

	virtual void SaveTo( pb::GS2C_SlotItem& msg ) const;
    virtual void LoadFrom( const pb::GS2C_SlotItem& msg );


protected:
    const LogicItemProto*	m_proto;

private:
    int DoStack(int count);
    void TakeOut(int count);
    void SetOld();
public:
	void SetSlot(uint32 slot);
	void SetSold(bool isSold);
	time_t GetRepoEndTime() const;
	void SetRepoEndTime(time_t curT);
	void SetIsBoughtInMarket(bool judge);
	bool IsBoughtInMarket();
private:
    int						m_count;
    bool					m_isNew;
    uint32                  m_slot ;
	bool					m_isSold;
	time_t					m_repo_end_time;
	bool					m_isBoughtInMarket;	//在商店中购买了
};




//////////////////////////////////////////////////////////////////////////


struct PredCanDestroy
{
    bool operator () (const Item * item) const
    {
        return item->CanBeDestroyed();
    }
};

//////////////////////////////////////////////////////////////////////////










/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Item Contianer
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/tag.hpp>

#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>


struct ByProtoId {} ;
struct ByItemSlot {} ;

// 命名空间别名
namespace bm =boost::multi_index ;

typedef bm::ordered_unique< bm::tag<ByItemSlot>, BOOST_MULTI_INDEX_CONST_MEM_FUN( Item, uint32, BagSlot ) > OrderSlot ;
typedef bm::ordered_non_unique< bm::tag<ByProtoId>, BOOST_MULTI_INDEX_CONST_MEM_FUN( Item, uint32, Entry ) > OrderProtoId ;

// 类型别名
typedef bm::multi_index_container< Item*, bm::indexed_by< OrderSlot, OrderProtoId > > MultiKeyItemTable;


// 迭代器别名
typedef MultiKeyItemTable::iterator MultiKeyItemTableItr ;
typedef MultiKeyItemTable::const_iterator CMultiKeyItemTableItr ;

typedef MultiKeyItemTable::reverse_iterator RMultiKeyItemTableItr ;
typedef MultiKeyItemTable::const_reverse_iterator CRMultiKeyItemTableItr ;

// by slot
typedef MultiKeyItemTable::index<ByItemSlot>::type TableByItemSlot;

typedef TableByItemSlot::iterator TableByItemSlotItr ;
typedef TableByItemSlot::const_iterator CTableByItemSlotItr ;

typedef TableByItemSlot::reverse_iterator RTableByItemSlotItr ;
typedef TableByItemSlot::const_reverse_iterator CRTableByItemSlotItr ;

// by proto id
typedef MultiKeyItemTable::index<ByProtoId>::type TableByProtoId;

typedef TableByProtoId::iterator TableByProtoIdItr ;
typedef TableByProtoId::const_iterator CTableByProtoIdItr ;

typedef TableByProtoId::reverse_iterator RTableByProtoIdItr ;
typedef TableByProtoId::const_reverse_iterator CRTableByProtoIdItr ;

typedef std::pair< TableByProtoIdItr, TableByProtoIdItr > EqualRangeByProtoId;







#endif // Item_h__
