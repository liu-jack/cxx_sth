#pragma once

#include <map>
#include "AttrModItem.h"

class AttrModItemArray
{
	typedef std::map<int,AttrModItem> SlotModAttrMap;    // slotIdx => AttrModItem

public:
	AttrModItemArray()
		:m_modified(false)
	{

	}

	bool IsModified() const
	{
		return m_modified;
	}

    void SetModifyFlag(bool flag)
    {
        m_modified = flag;
    }
	void ClearModifyFlag()
	{
		m_modified = false;
	}

	template<typename T>
	T CalcValue (int slotIdx, T oriValue) const
	{
		SlotModAttrMap::const_iterator it = m_items.find(slotIdx);
		if (it == m_items.end())			
		{
			return oriValue;
		}

		return it->second.CalcValue(oriValue);
	}

    void SetValue(int slotIdx, const AttrModItem& modItem)
    {
        m_items[slotIdx] = modItem;
        m_modified = true;
    }
	void Apply(int slotIdx, const AttrModItem& modItem)
	{
		m_items[slotIdx] += modItem;
		m_modified = true;
	}

	void Unapply(int slotIdx, const AttrModItem& modItem)
	{
		m_items[slotIdx] -= modItem;
		m_modified = true;
	}

private:
	SlotModAttrMap		m_items;
	bool			m_modified;
};