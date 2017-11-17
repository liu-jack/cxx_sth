#pragma once

#include <vector>
#include "AttrDef.h"
#include "AttrModItemArray.h"
#include "SharedPtr.h"

class AttrModGroup : public IAttrModGroup
{
	class SlotAttrModItem
	{
	public:
		SlotAttrModItem(int slotIdx, float percent, float modValue)
			:m_slotIdx(slotIdx),m_item(percent,modValue)
		{
		}

		int SlotIdx() const
		{
			return m_slotIdx;
		}

		const AttrModItem& Item() const
		{
			return m_item;
		}

        void UpdateByStack(float stack)
        {
            m_item.UpdateByStack(stack);
        }

	private:
		int			m_slotIdx;
		AttrModItem m_item;
	};

	typedef std::vector<SlotAttrModItem> SlotAttrModItemList;

public:
	explicit AttrModGroup(SharedPtr<AttrModItemArray> rootItemArray)
		:m_rootItemArray(rootItemArray)
	{
	}

	~AttrModGroup()
	{
		for (SlotAttrModItemList::iterator it = m_modItems.begin(); it != m_modItems.end(); ++it)
		{
			SlotAttrModItem& slotItem = *it;
			m_rootItemArray->Unapply(slotItem.SlotIdx(), slotItem.Item());
		}
	}

	virtual void Modify(int slotIdx, float percent, float modValue)
	{
		if (percent == 0 && modValue == 0)
		{
			return;
		}

		m_modItems.push_back(SlotAttrModItem (slotIdx, percent, modValue));
		m_rootItemArray->Apply(slotIdx, m_modItems.back().Item());
	}
    virtual void UpdateByStack(float stack)
    {
        size_t count = m_modItems.size();
        for(size_t i = 0; i < count; ++i)
        {
            SlotAttrModItem& node = m_modItems[i];
            node.UpdateByStack(stack);
            m_rootItemArray->SetValue(node.SlotIdx(), node.Item());
        }
    }

private:
	SharedPtr<AttrModItemArray>		m_rootItemArray;	// modify数值 (a+b+c)percent, i+j+k 总结果
	SlotAttrModItemList				m_modItems;			// 用于记录modify的内容(a,i),(b,j),(c,k)
};