#pragma once
#include "boost/dynamic_bitset.hpp"

#include "def/TypeDef.h"
#include "def/MmoAssert.h"
#include "utility/MsgTool.h"

template<typename T, class TFullMsg, class TUpdateMsg>
class TValueArray
{
    typedef std::vector<T> TVec;

public:
    TValueArray(int size);
    TValueArray(const TValueArray<T, TFullMsg, TUpdateMsg>& other);
    ~TValueArray(){};

    bool operator==( const TValueArray<T, TFullMsg, TUpdateMsg>& other) const;
    bool operator!=( const TValueArray<T, TFullMsg, TUpdateMsg>& other) const;

    bool Set(int idx, const T& tValue);
    const T& Get(int idx) const;

    bool IsModified() const;
	bool IsModifiedId(int idx) const;
    void SetUnmodified();

    int Size() const;

    void BuildUpdateMsg(TUpdateMsg& msg) const;
    void BuildFullUpdateMsg(TFullMsg& msg) const;
    
    void ApplyUpdateMsg(const TUpdateMsg& msg ); 
    void ApplyFullUpdateMsg(const TFullMsg& msg ); 



private:
    boost::dynamic_bitset<uint32>	m_modifyBits;
    TVec							m_TValues;

};

template<typename T, class TFullMsg, class TUpdateMsg>
void TValueArray<T, TFullMsg, TUpdateMsg>::ApplyFullUpdateMsg( const TFullMsg& msg )
{
    for ( size_t i = 0 ; i < m_TValues.size() && i < static_cast<size_t>( msg.values_size() ); ++i)
    {
        m_TValues[i] = msg.values(i);
    }
}

template<typename T, class TFullMsg, class TUpdateMsg>
void TValueArray<T, TFullMsg, TUpdateMsg>::ApplyUpdateMsg( const TUpdateMsg& msg )
{
    MsgTool::SaveUpdateMsgTo(msg, m_TValues);
}

template<typename T, class TFullMsg, class TUpdateMsg>
int TValueArray<T, TFullMsg, TUpdateMsg>::Size() const
{
    return m_modifyBits.size();
}

template<typename T, class TFullMsg, class TUpdateMsg>
bool TValueArray<T, TFullMsg, TUpdateMsg>::operator!=( const TValueArray<T, TFullMsg, TUpdateMsg>& other ) const
{
    return !this->operator ==(other);
}

template<typename T, class TFullMsg, class TUpdateMsg>
bool TValueArray<T, TFullMsg, TUpdateMsg>::operator==( const TValueArray<T, TFullMsg, TUpdateMsg>& other ) const
{
    return m_TValues == other.m_TValues && m_modifyBits == other.m_modifyBits;
}

template<typename T, class TFullMsg, class TUpdateMsg>
TValueArray<T, TFullMsg, TUpdateMsg>::TValueArray( const TValueArray<T, TFullMsg, TUpdateMsg>& other )
: m_modifyBits( other.m_modifyBits)
, m_TValues( other.m_TValues)
{

}

template<typename T, class TFullMsg, class TUpdateMsg>
TValueArray<T, TFullMsg, TUpdateMsg>::TValueArray( int size)
: m_modifyBits( size)
, m_TValues( size)
{

}

template<typename T, class TFullMsg, class TUpdateMsg>
void TValueArray<T, TFullMsg, TUpdateMsg>::BuildFullUpdateMsg( TFullMsg& msg ) const
{
    for (size_t i=0; i< m_TValues.size(); i++)
    {
        msg.add_values(m_TValues[i]);
    }
}

template<typename T, class TFullMsg, class TUpdateMsg>
void TValueArray<T, TFullMsg, TUpdateMsg>::BuildUpdateMsg( TUpdateMsg& msg ) const
{
    msg.set_bit_count(m_modifyBits.size());
    boost::to_block_range(m_modifyBits, RepeatedFieldBackInserter(msg.mutable_bit_masks()));

    for (size_t i=m_modifyBits.find_first(); i != boost::dynamic_bitset<uint32>::npos; i = m_modifyBits.find_next(i))
    {
        msg.add_values(m_TValues[i]);
    }
}

template<typename T, class TFullMsg, class TUpdateMsg>
void TValueArray<T, TFullMsg, TUpdateMsg>::SetUnmodified()
{
    m_modifyBits.reset();
}

template<typename T, class TFullMsg, class TUpdateMsg>
bool TValueArray<T, TFullMsg, TUpdateMsg>::IsModified() const
{
    return m_modifyBits.any();
}

template<typename T, class TFullMsg, class TUpdateMsg>
bool TValueArray<T, TFullMsg, TUpdateMsg>::IsModifiedId(int idx) const
{
	ASSERT(static_cast< typename TVec::size_type >( idx ) < m_TValues.size());
	return m_modifyBits[idx];
}

template<typename T, class TFullMsg, class TUpdateMsg>
const T& TValueArray<T, TFullMsg, TUpdateMsg>::Get( int idx ) const
{
	ASSERT(static_cast< typename TVec::size_type >( idx ) < m_TValues.size());
    return m_TValues[idx];
}

template<typename T, class TFullMsg, class TUpdateMsg>
bool TValueArray<T, TFullMsg, TUpdateMsg>::Set( int idx, const T& tValue )
{
	ASSERT(static_cast< typename TVec::size_type >( idx ) < m_TValues.size());
    if (m_TValues[idx] == tValue)
    {
        return false;
    }

    m_TValues[idx] = tValue;
    m_modifyBits[idx] = true;
	return true;
}
