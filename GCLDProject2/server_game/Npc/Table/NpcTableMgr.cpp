#include "NpcTableMgr.h"
#include "DbTool.h"
#include "def/ObjectDefines.h"

NpcTableMgr::NpcTableMgr()
{
	m_attrs.resize( CHARACTER_VALUES_COUNT);
}

void NpcTableMgr::Init()
{
    FOREACH_DB_ITEM(ptr1, NpcProto)
    {
        m_ProtoLst[ptr1->Id] = ptr1;
    }
}
const NpcProto* NpcTableMgr::GetProto(uint32 id)
{
    return m_ProtoLst.pfind(id);
}
void NpcTableMgr::RecalculateAttr(uint32 npcProtoId, uint32 level)
{
    const NpcProto* proto = m_ProtoLst.pfind(npcProtoId);
    MMO_ASSERT(proto);
    if (proto) {
        const NpcAttrBase* attrBase = DbTool::Get<NpcAttrBase>(level);
        MMO_ASSERT(attrBase);
        const NpcAttrParamGrade* attrGrade = DbTool::Get<NpcAttrParamGrade>(proto->grade_option);
        MMO_ASSERT(attrGrade);
        const NpcAttrParamCombat* attrCombat = DbTool::Get<NpcAttrParamCombat>(proto->combat_option);
        MMO_ASSERT(attrCombat);
		const NpcAttrBaseAdditon* attrBaseAdditon = DbTool::Get<NpcAttrBaseAdditon>(proto->attack_option);
        if (attrBase && attrGrade && attrCombat)
        {
            for (uint32 i = 0; i < CHARACTER_VALUES_COUNT; ++i)
            {
				if(attrBaseAdditon)
					m_attrs[i] = attrBase->value[i] * attrGrade->value[i] * attrCombat->value[i] + attrBaseAdditon->value[i];
				else
					m_attrs[i] = attrBase->value[i] * attrGrade->value[i] * attrCombat->value[i];
            }
        }
    }
}
float NpcTableMgr::GetAttr(uint8 idx) const
{
    if (idx < CHARACTER_VALUES_COUNT)
    {
        return m_attrs[idx];
    }
    return 0;
}
