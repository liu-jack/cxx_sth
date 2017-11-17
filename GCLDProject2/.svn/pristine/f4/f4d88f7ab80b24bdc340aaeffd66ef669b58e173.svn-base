#pragma once

#include <memory>
#include "AttrDef.h"
#include "SharedPtr.h"

class AttrModItemArray;

class AttrModSet
{
public:
	AttrModSet();
	~AttrModSet();

	float CalcValue (int slotIdx, float oriValue) const;
	int CalcValue (int slotIdx, int oriValue) const;

	IAttrModGroup* CreateModItem() const;
	
	bool IsModified() const;
	void ClearModifyFlag();

private:
	SharedPtr<AttrModItemArray>	m_impl;
};
