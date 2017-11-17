#include "PalaceTableMgr.h"
#include "DbTool.h"
#include "utility/Utility.h"
#include "System.h"

static const int Ticket_Sell_ProtoId    = 1;

static uint32 g_max_office_id = 1;

void PalaceTableMgr::Init()
{
    FOREACH_DB_ITEM(ptr1, DB_Palace_Proto)
    {
        m_ProtoTable[ptr1->id] = ptr1;

        if (ptr1->id > g_max_office_id) g_max_office_id = ptr1->id;
    }
}
uint32 PalaceTableMgr::GetMaxOfficeId()
{
    return g_max_office_id;
}
const DB_Palace_Proto* PalaceTableMgr::GetProto(int id)
{
    return m_ProtoTable.pfind(id);
}

