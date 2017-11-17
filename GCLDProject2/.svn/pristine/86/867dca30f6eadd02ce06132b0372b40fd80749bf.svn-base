#include "BaseDefineMgr.h"

#include "datastruct/struct_base_define.h"
#include "DatabaseMgr.h"
#include "DbTool.h"


BaseDefineMgr::BaseDefineMgr()
{
}

void BaseDefineMgr::Init()
{
    FOREACH_DB_ITEM( bd, BaseDefine)
    {
        if (m_baseDefVec.size() <= bd->id)
        {
            m_baseDefVec.resize( bd->id + 1);
        }
        m_baseDefVec[ bd->id] = bd;
    }
}

