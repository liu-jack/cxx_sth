#include "Fuben.h"
#include "datastruct/struct_fuben.h"

uint32 Fuben::Id() const { return m_dbFubenProto.id; }
uint32 Fuben::ReqLv() const { return m_dbFubenProto.lv_req; }		//���鸱�����Ϊ50��
bool Fuben::IsCommonFuben() const
{
    return m_dbFubenProto.type == Common;
}
bool Fuben::IsCrusadeFuben() const
{
    return m_dbFubenProto.type == Crusade;
}
uint32 Fuben::GetStageMode() const
{
	return m_dbFubenProto.type;
}

Fuben::Fuben( const DB_FubenProto& db )
:m_dbFubenProto(db)
{

}
