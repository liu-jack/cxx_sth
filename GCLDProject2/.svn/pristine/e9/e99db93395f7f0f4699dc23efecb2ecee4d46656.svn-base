#include "FixFakeRandomBitSet.h"

#include "def/MmoAssert.h"

#include <math.h>

#include <iostream>
#include <string>



FixFakeRandomBitSet::FixFakeRandomBitSet( U32 size, U32 hitCount )
: m_bits( size)
{
    MMO_ASSERT( size > hitCount);
    MMO_ASSERT( hitCount >= 1);
    MMO_ASSERT( U32(ceilf( size/2.0f )) >= hitCount) ;
    bool special = ( size/2 + 1) == hitCount;
    U32 U32erval = special ? 2 : (size / hitCount); 
    U32 begin = special ? 0 : (size % hitCount + U32erval -1);
    for ( U32 i = 0; i < hitCount; ++i)
    {
        m_bits.set(begin + i * U32erval);
    }
    //std::string str;
    //boost::to_string_helper(m_bits, str, false);
    //std::cout<< str << std::endl;
}

bool FixFakeRandomBitSet::IsHit( U32 pos ) const
{
    return m_bits.test(pos);
}

U32 FixFakeRandomBitSet::GetRemaintoHit( U32 pos ) const
{
    U32 i = 0;
    if ( pos != 0 )
    {
        i = m_bits.find_next( pos - 1);
    }
    else
    {
        i = m_bits.find_first();
    }
    if ( i != boost::dynamic_bitset<uint32>::npos)
    {
        return i - pos + 1;
    }
    i = m_bits.find_first();
    MMO_ASSERT( i != boost::dynamic_bitset<uint32>::npos);
    if ( i != boost::dynamic_bitset<uint32>::npos)
    {
        return m_bits.size() - pos + i ;
    }
    return 0;
}


// A 次里面必出 B 次 , C 为当前次数 - 1 , 判断C是否被命中
// ！！！ 仅支持 ceilf(float(A)/2 >= B   
bool FixFakeRandomCalculator::IsHitFixCorrect( U32 A, U32 B, U32 C ) const
{
    MMO_ASSERT( A > C && C >= 0);
    if ( A > C && C >= 0)
    {
        FixFakeRandomBitSet* bit = GetBit( A, B);
        if ( bit != NULL)
        {
            return bit->IsHit(C);
        } 
    }  
    return false;
}

bool FixFakeRandomCalculator::TryGetRemaintoHit( U32 A, U32 B, U32 C, U32& Remain ) const
{
    MMO_ASSERT( A > C && C >= 0);
    if ( A > C && C >= 0)
    {
        FixFakeRandomBitSet* bit = GetBit( A, B);
        if ( bit != NULL)
        {
            Remain = bit->GetRemaintoHit(C);
            return true;
        } 
    }
    return false;
}

FixFakeRandomBitSet* FixFakeRandomCalculator::GetBit( U32 A, U32 B ) const
{
    if ( U32(ceilf( A/2.0f)) >= B )
    {
        FixFakeRandomBitSet* bit = m_bitsets[A].pfind(B);
        if ( bit == NULL)
        {
            bit = new FixFakeRandomBitSet(A, B);
            m_bitsets[A][B] = bit;
        }
        return bit;
    }
    return NULL;
}


