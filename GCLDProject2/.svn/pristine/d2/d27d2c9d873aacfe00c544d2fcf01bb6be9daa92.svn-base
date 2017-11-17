#pragma once

#include "SimpleSingleton.h"
#include "ptr_container/PtrMap.h"
#include <boost/dynamic_bitset/dynamic_bitset.hpp>
#include "def/TypeDef.h"



class FixFakeRandomBitSet
{
public:
    FixFakeRandomBitSet(U32 size, U32 hitCount);
    bool IsHit(U32 pos) const;
    U32  GetRemaintoHit( U32 pos) const;
private:
    FixFakeRandomBitSet();
    boost::dynamic_bitset<> m_bits;
};


class FixFakeRandomCalculator : public SimpleSingleton<FixFakeRandomCalculator>
{
    friend class SimpleSingleton<FixFakeRandomCalculator>;

    typedef PtrMap< U32, FixFakeRandomBitSet> HitCount2FixFakeRandomBitSet;
    typedef std::map< U32, HitCount2FixFakeRandomBitSet> Size2HitCount2FixFakeRandomBitSet;

public:
    // A 次里面必出 B 次 , C 为当前次数 , 判断C是否被命中
    // ！！！ 仅支持 ceilf(float(A)/2 >= B
    bool IsHitFixCorrect( U32 A, U32 B, U32 C) const;
    bool TryGetRemaintoHit( U32 A, U32 B, U32 C, U32& Remain) const;
private:
    FixFakeRandomBitSet* GetBit( U32 A, U32 B) const;
    mutable Size2HitCount2FixFakeRandomBitSet m_bitsets;
};

#define sFixFakeRD FixFakeRandomCalculator::Instance()
