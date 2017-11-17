#pragma once

struct IAttrModGroup
{
	virtual ~IAttrModGroup(){}

	virtual void Modify(int slotIdx, float percent, float modValue) = 0;
    virtual void UpdateByStack(float stack) = 0;
};

