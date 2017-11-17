#include "Buff.h"
#include "OS.h"
#include "DbTool.h"
#include "datastruct/struct_buff.h"
#include "utility/Utility.h"
#include "Buff.pb.h"

void CBuff::SaveToClient(pb::GS2C_Buff_All_Info& msg)
{
    for (std::map<uint32, uint64>::const_iterator it = m_buff.begin(); it != m_buff.end(); ++it)
    {
        pb::PairTime* pair = msg.add_buff();
        pair->set_first(it->first);
        pair->set_second(it->second);
    }
}
void CBuff::SaveToDB(pb::GS2C_Buff_All_Info& msg)
{
    for (std::map<uint32, uint64>::const_iterator it = m_buff.begin(); it != m_buff.end(); ++it)
    {
        if (const PlayerBuffTable* table = PlayerBuffTable::GetTable(it->first))
        {
            if (table->IsSave())
            {
                pb::PairTime* pair = msg.add_buff();
                pair->set_first(it->first);
                pair->set_second(it->second);
            }
        }
    }
}
void CBuff::LoadFrom(const pb::GS2C_Buff_All_Info& msg)
{
    for (int i = 0; i < msg.buff_size(); ++i)
    {
        const pb::PairTime& pair = msg.buff(i);
		m_buff.insert(std::make_pair(pair.first(), pair.second()));
    }
}
bool CBuff::HaveBuff(uint32 id) const
{
    std::map<uint32, uint64>::const_iterator it = m_buff.find(id);

    if (it == m_buff.end())
        return false;

    if (sOS.TimeSeconds() >= it->second)
        return false;

    return m_buff.find(id) != m_buff.end();
}
bool CBuff::HaveBuff(Type typ) const
{
    for (std::map<uint32, uint64>::const_iterator it = m_buff.begin(); it != m_buff.end(); ++it)
    {
        if (const PlayerBuffTable* table = PlayerBuffTable::GetTable(it->first))
        {
            if (table->GetBuffTyp() == typ)
            {
                return true;
            }
        }
    }
    return false;
}
void CBuff::DelOneTypeBuff(Type typ)
{
    for (std::map<uint32, uint64>::iterator it = m_buff.begin(); it != m_buff.end(); )
    {
        if (const PlayerBuffTable* table = PlayerBuffTable::GetTable(it->first))
        {
            if (table->GetBuffTyp() == typ)
            {
                it = m_buff.erase(it);
                continue;
            }
        }
        ++it;
    }
}
void CBuff::AddBuff(const PlayerBuffTable* table, uint32 lastMsec /* = 0 */)
{
    if (table == NULL) return;

    if (lastMsec == 0) lastMsec = table->TimeLong();

    uint64 endTime = (lastMsec > 0 ? OS::TimeMS() + lastMsec : 0);

    const uint32 buffId = table->GetBuffId();
    const uint32 buffLv = table->GetBuffLv();
    const uint32 buffTyp = table->GetBuffTyp();

    MMO_ASSERT(buffTyp < _Type_Max);

    //同种类型，覆盖检测
    for (std::map<uint32, uint64>::iterator it = m_buff.begin(); it != m_buff.end(); ++it)
    {
        if (const PlayerBuffTable* table = PlayerBuffTable::GetTable(it->first))
        {
            if (table->GetBuffTyp() == buffTyp && buffLv >= table->GetBuffLv())
            {
                m_buff.erase(it);
				m_buff.insert(std::make_pair(buffId, endTime));
                return;
            }
        }
    }
	m_buff.insert(std::make_pair(buffId, endTime));
}
void CBuff::DelBuff(uint32 id)
{
    m_buff.erase(id);
}
void CBuff::Refresh(uint64 timeNowMsec)
{
    for (std::map<uint32, uint64>::iterator it = m_buff.begin(); it != m_buff.end();)
    {
        if (it->second > 0 && timeNowMsec >= it->second)
        {
            it = m_buff.erase(it);
            continue;
        }
        ++it;
    }
}