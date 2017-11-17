#include "IronsmithTableMgr.h"
#include "DbTool.h"
#include "datastruct/struct_smithy.h"
#include "utility/Utility.h"
#include "System.h"

static int _Calc_Smelter_Key(int field, int star)
{
    return field * 100 + star;
}

void SmithyTableMgr::Init()
{
    m_MaxSmithyLv = 0;
	m_field = 0;

    FOREACH_DB_ITEM(ptr1, DB_Smithy)
    {
        m_SmithyMap[ptr1->smithy_level] = new DB_Smithy(*ptr1);

        if (ptr1->smithy_level > m_MaxSmithyLv) m_MaxSmithyLv = ptr1->smithy_level;

    }
    FOREACH_DB_ITEM(ptr2, DB_Smelter)
    {
        int key = _Calc_Smelter_Key(ptr2->field, ptr2->smelter_star);
        m_SmelterMap[key] = new DB_Smelter(*ptr2);
		if (ptr2->field > m_field)
		{
			m_field = ptr2->field;
		}
    }
}
const DB_Smithy* SmithyTableMgr::GetSmithy(int lv)
{
    return m_SmithyMap.pfind(lv);
}
const DB_Smelter* SmithyTableMgr::GetSmelter(int field, int star)
{
    int key = _Calc_Smelter_Key(field, star);
    return m_SmelterMap.pfind(key);
}
