#include "ValueArray.h"
#include "def/MmoAssert.h"
#include "Logger.h"
#include "utility/MsgTool.h"

#include "CommomDef.pb.h"

ValueArray::ValueArray(uint32 size)
:m_values(size)
,m_isFloat(size)
,m_modifyBits(size)
{
}

ValueArray::ValueArray( const ValueArray& other )
:m_values(other.m_values)
,m_isFloat(other.m_isFloat)
,m_modifyBits(other.m_modifyBits)
{
}

bool ValueArray::operator==( const ValueArray& other ) const
{
	if (this->Size() != other.Size())
	{
		return false;
	}

	for (size_t i=0; i< m_values.size(); i++)
	{
		if ( IsFloat(i))
		{
			if (!other.IsFloat(i))
			{
				return false;
			}

			float a = this->InnerGetFloat(i);
			float b = other.InnerGetFloat(i);

			if (a != b && ::abs(a-b)>0.000001f )
			{
				return false;
			}
		}
		else
		{
			if (other.IsFloat(i))
			{
				return false;
			}

			if (m_values[i] != other.m_values[i])
			{
				return false;
			}
		}
	}

	return true;
}

bool ValueArray::operator!=( const ValueArray& other ) const
{
	return !this->operator ==(other);
}

ValueArray::~ValueArray(void)
{
}

uint32 ValueArray::Size() const
{
	return m_modifyBits.size();
}

bool ValueArray::IsModified() const
{
	return m_modifyBits.any();
}

void ValueArray::SetUnmodified()
{
	m_modifyBits.reset();
}

#define ASSERT_IS_FLOAT(idx) MMO_ASSERT(IsFloat(idx) || m_values[idx]==0)
#define ASSERT_IS_INT(idx) MMO_ASSERT(!IsFloat(idx))
#define SET_IS_FLOAT(idx) m_isFloat[idx] = true
#define SET_IS_INT(idx) m_isFloat[idx] = false

void ValueArray::SetInt(uint32 idx, int32 val)
{
	MMO_ASSERT (idx < m_values.size());

	if (m_values[idx] == val)
	{
		return;
	}

	m_values[idx] = val;
	m_modifyBits[idx] = true;
	SET_IS_INT(idx);
}

int32 ValueArray::GetInt(uint32 idx) const
{
	MMO_ASSERT (idx < m_values.size());
	ASSERT_IS_INT(idx);

	return m_values[idx];
}

void ValueArray::SetFloat(uint32 idx, F32 val)
{
	MMO_ASSERT (idx < m_values.size());

	if (InnerGetFloat(idx) == val)
	{
		return;
	}

	m_values[idx] = *((int32*)&val);
	m_modifyBits[idx] = true;
	SET_IS_FLOAT(idx);
}

F32 ValueArray::InnerGetFloat(uint32 idx) const
{
	return *((F32*)&m_values[idx]);
}

F32 ValueArray::GetFloat(uint32 idx) const
{
	MMO_ASSERT (idx < m_values.size());
	ASSERT_IS_FLOAT(idx);

	return InnerGetFloat(idx);
}

void ValueArray::SetFlag(uint32 idx, int32 flag)
{
	MMO_ASSERT (idx < m_values.size());

	if((m_values[idx] & flag) == flag)
	{
		return;
	}

	m_values[idx] |= flag;
	m_modifyBits[idx] = true;
	SET_IS_INT(idx);
}

void ValueArray::SetFlag( uint32 idx, int32 flag, bool isSet )
{
	if (isSet)
	{
		SetFlag(idx, flag);
		return;
	}

	RemoveFlag(idx, flag);
}

bool ValueArray::HasAllFlag(uint32 idx, int32 flag) const
{
	MMO_ASSERT (idx < m_values.size());
	ASSERT_IS_INT(idx);

	return (m_values[idx] & flag) == flag;
}

bool ValueArray::HasAnyFlag(uint32 idx, int32 flag) const
{
	MMO_ASSERT (idx < m_values.size());
	ASSERT_IS_INT(idx);

	return (m_values[idx] & flag) != 0;
}

void ValueArray::RemoveFlag(uint32 idx, int32 flag)
{
	MMO_ASSERT (idx < m_values.size());

	if((m_values[idx] & flag) == 0)
	{
		return;
	}

	m_values[idx] &= ~flag;
	m_modifyBits[idx] = true;
	SET_IS_INT(idx);
}

void ValueArray::BuildUpdateMsg(pb::Int32ListUpdate& msg) const
{
	ASSERT( IsModified() ) ;

	msg.set_bit_count(m_modifyBits.size());
	boost::to_block_range(m_modifyBits, RepeatedFieldBackInserter(msg.mutable_bit_masks()));

	for (size_t i=m_modifyBits.find_first(); i != boost::dynamic_bitset<uint32>::npos; i = m_modifyBits.find_next(i))
	{
		msg.add_values(m_values[i]);
	}
}

void ValueArray::BuildFullUpdateMsg(pb::Int32List& msg) const
{
	for (size_t i=0; i< m_values.size(); ++i)
	{
		msg.add_values(m_values[i]);
	}
}

void ValueArray::BuildFullUpdateMsg(pb::Int32ListUpdate& msg) const
{
	//msg.set_bit_count(m_values.size());//zhoulunhao 
	msg.set_bit_count(m_values.size());
	for (size_t i=0; i< m_values.size(); ++i)
	{
		msg.add_values(m_values[i]);
	}
}


void ValueArray::ApplyUpdateMsg(const pb::Int32ListUpdate& msg ) 
{
    MsgTool::SaveUpdateMsgTo( msg, m_values);     
}

void ValueArray::ApplyFullUpdateMsg(const pb::Int32List& msg ) 
{
    for ( size_t i = 0 ; i < m_values.size() && i < static_cast<size_t>( msg.values_size() ); ++i)
    {
        m_values[i] = msg.values(i);
    }
}



F32 ValueArray::GetIntOrFloat( uint32 idx ) const // for lua script
{
	if (idx >= m_values.size())
	{
		ELOG("Out of index when call ValueArray::GetIntOrFloat(%d)", idx);
		return 0;
	}

	if (IsFloat(idx))
	{
		return InnerGetFloat(idx);
	}

	return (F32)m_values[idx];
}

void ValueArray::CopyFrom( const ValueArray& other, uint32 idx )
{
	MMO_ASSERT (idx < m_values.size());
	MMO_ASSERT (idx < other.m_values.size());

	if (other.IsFloat(idx))
	{
		SetFloat(idx, other.GetFloat(idx));
		return;
	}

	SetInt(idx, other.GetInt(idx));
}

bool ValueArray::IsFloat( uint32 idx ) const
{
    return m_isFloat[idx] != 0;
}


#define MAX_SINT32 0x7FFFFFFF
void ValueArray::ModifyInt( uint32 idx, int32 val )
{
    if( val == 0 )
    {
        return ;
    }

    int32 orignValue =GetInt( idx ) ;
    int32 finalValue = orignValue + val ;

    if( val > 0 ) 
    {
        finalValue < orignValue ? finalValue = MAX_SINT32 : 0 ;   // ( finalValue < orignValue ) 成立则向上溢出了, 设置成最大值
    }
    else // val < 0, 等于0前面 return 了
    {
        finalValue < 0 ? finalValue = 0 : 0 ;
    }

    SetInt( idx, finalValue ) ;
}


