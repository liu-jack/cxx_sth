#include "AttrModSet.h"
#include "AttrModItemArray.h"
#include "AttrModGroup.h"
#include "def/MmoAssert.h"

AttrModSet::AttrModSet()
:m_impl(new AttrModItemArray())
{
}

AttrModSet::~AttrModSet()
{
}

float AttrModSet::CalcValue( int slotIdx, float oriValue ) const
{
	return m_impl->CalcValue(slotIdx, oriValue);
}

int AttrModSet::CalcValue( int slotIdx, int oriValue ) const
{
	return m_impl->CalcValue(slotIdx, oriValue);
}

IAttrModGroup* AttrModSet::CreateModItem() const
{
	return new AttrModGroup(m_impl);
}

bool AttrModSet::IsModified() const
{
	return m_impl->IsModified();
}

void AttrModSet::ClearModifyFlag()
{
	m_impl->ClearModifyFlag();
}
