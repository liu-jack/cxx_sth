#include "MultiValueArray.h"
#include "def/MmoAssert.h"
#include "Logger.h"

#include "CommomDef.pb.h"
MultiValueArray::MultiValueArray( uint32 int32Size, uint32 uint64Size, uint32 floatSize, uint32 stringSize )
:m_in32Value( int32Size )
, m_uin64Value( uint64Size)
,m_floatValue( floatSize )
, m_stringValue( stringSize)
{
}

MultiValueArray::MultiValueArray( const MultiValueArray& other )
:m_in32Value( other.m_in32Value )
, m_uin64Value( other.m_uin64Value)
,m_floatValue( other.m_floatValue )
, m_stringValue( other.m_stringValue)
{
}

bool MultiValueArray::operator==( const MultiValueArray& other ) const
{
	return this->m_in32Value == other.m_in32Value
		&& this->m_uin64Value == other.m_uin64Value
		&& this->m_floatValue == other.m_floatValue
		&& this->m_stringValue == other.m_stringValue ;
}

bool MultiValueArray::operator!=( const MultiValueArray& other ) const
{
	return !this->operator ==(other);
}

MultiValueArray::~MultiValueArray(void)
{
}



int MultiValueArray::Int32ValuesSize() const
{
	return m_in32Value.Size();
}

int MultiValueArray::Uint64ValuesSize() const
{
	return m_uin64Value.Size();
}

int MultiValueArray::FloatValuesSize() const
{
	return m_floatValue.Size();
}

int MultiValueArray::StringValuesSize() const
{
	return m_stringValue.Size();
}


bool MultiValueArray::IsModified() const
{
	return IsInt32ValuesModified() || IsUint64ValuesModified() || IsFloatValuesModified() || IsStringValuesModified() ;
}


bool MultiValueArray::IsInt32ValuesModified() const
{
	return m_in32Value.IsModified();
}

bool MultiValueArray::IsUint64ValuesModified() const
{
	return m_uin64Value.IsModified();
}

bool MultiValueArray::IsFloatValuesModified() const
{
	return m_floatValue.IsModified();
}

bool MultiValueArray::IsStringValuesModified() const
{
	return m_stringValue.IsModified();
}


void MultiValueArray::SetUnmodified()
{
	m_in32Value.SetUnmodified();
	m_uin64Value.SetUnmodified();
	m_floatValue.SetUnmodified();
	m_stringValue.SetUnmodified();
}


bool MultiValueArray::SetInt32(uint32 idx, int32 val)
{
	return m_in32Value.Set(idx, val);
}

int32 MultiValueArray::GetInt32(uint32 idx) const
{
	return m_in32Value.Get(idx);
}


bool MultiValueArray::SetUint64(uint32 idx, uint64 val)
{
	return m_uin64Value.Set(idx, val);
}

uint64 MultiValueArray::GetUint64(uint32 idx) const
{
	return m_uin64Value.Get(idx);
}

bool MultiValueArray::SetFloat(uint32 idx, float val)
{
	return	m_floatValue.Set(idx, val);
}

float MultiValueArray::GetFloat(uint32 idx) const
{
	return m_floatValue.Get(idx);
}

bool MultiValueArray::SetString(uint32 idx, const string &val)
{
	return m_stringValue.Set(idx, val);
}

const std::string& MultiValueArray::GetString(uint32 idx) const
{
	return m_stringValue.Get(idx);
}



void MultiValueArray::BuildUpdateMsg(pb::ObjectValueUpdate& msg) const
{
	if (IsInt32ValuesModified())
	{
		pb::Int32ListUpdate &I32Update =*msg.mutable_update_int32() ;
		m_in32Value.BuildUpdateMsg( I32Update );
	}

	if (IsUint64ValuesModified())
	{
		pb::Uint64ListUpdate &u64Update =*msg.mutable_update_uint64() ;
		m_uin64Value.BuildUpdateMsg( u64Update );
	}

	if (IsFloatValuesModified())
	{
		pb::FloatListUpdate &floatUpdate =*msg.mutable_update_float() ;
		m_floatValue.BuildUpdateMsg( floatUpdate );
	}

	if (IsStringValuesModified())
	{
		pb::StringListUpdate &strUpdate =*msg.mutable_update_string() ;
		m_stringValue.BuildUpdateMsg( strUpdate );
	}

}

void MultiValueArray::BuildFullUpdateMsg(pb::ObjectFullValue& msg) const
{
	m_in32Value.BuildFullUpdateMsg( *msg.mutable_int32_values());
	m_uin64Value.BuildFullUpdateMsg( *msg.mutable_uint64_values());
	m_floatValue.BuildFullUpdateMsg( *msg.mutable_float_values());
	m_stringValue.BuildFullUpdateMsg( *msg.mutable_string_values());
}

void MultiValueArray::ApplyUpdateMsg( const pb::ObjectValueUpdate& msg )
{
    if ( msg.has_update_float())
    {
        m_floatValue.ApplyUpdateMsg( msg.update_float());
    }
    if ( msg.has_update_int32())
    {
        m_in32Value.ApplyUpdateMsg( msg.update_int32());
    }
    if ( msg.has_update_uint64())
    {
        m_uin64Value.ApplyUpdateMsg( msg.update_uint64());
    }
    if ( msg.has_update_string())
    {
        m_stringValue.ApplyUpdateMsg( msg.update_string());
    }
}

void MultiValueArray::ApplyFullUpdateMsg( const pb::ObjectFullValue& msg )
{
    if ( msg.has_float_values())
    {
        m_floatValue.ApplyFullUpdateMsg( msg.float_values());
    }
    if ( msg.has_int32_values())
    {
        m_in32Value.ApplyFullUpdateMsg( msg.int32_values());
    }
    if ( msg.has_uint64_values())
    {
        m_uin64Value.ApplyFullUpdateMsg( msg.uint64_values());
    }
    if ( msg.has_string_values())
    {
        m_stringValue.ApplyFullUpdateMsg( msg.string_values());
    }
}
