#pragma once

#include "def/TypeDef.h"


struct IScriptHost
{
	friend class LuaToCStack;

protected:
	IScriptHost();
    virtual bool IsValid() { return true; };
	virtual ~IScriptHost();
};



