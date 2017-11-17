#include "AttrCalculator.h"
#include<math.h>



AttrCalculator::AttrCalculator( ValueArray& valueArray )
: m_baseValues(valueArray.Size())
, m_valueAttrs(valueArray)
{

}

AttrCalculator::~AttrCalculator()
{

}

void AttrCalculator::SetBaseValueInt( int valIdx, int val )
{
	m_baseValues.SetInt(valIdx, val);
}

void AttrCalculator::SetBaseValueFloat( int valIdx, float val )
{
	m_baseValues.SetFloat(valIdx, val);
}


IAttrModGroup* AttrCalculator::CreateBaseAttrModGroup() const
{
	return m_baseValueModSet.CreateModItem();
}

IAttrModGroup* AttrCalculator::CreateAttrModGroup() const
{
	return m_valueModSet.CreateModItem();
}

void AttrCalculator::SetAttrFloat( int valIdx, float val )
{
    m_valueAttrs.SetFloat(valIdx, val);
}

void AttrCalculator::SetAttrInt( int valIdx, int val )
{
    m_valueAttrs.SetInt(valIdx, val);
}

float AttrCalculator::GetAttrFloat( int valIdx ) const
{
	return m_valueAttrs.GetFloat(valIdx);
}

int AttrCalculator::GetAttrInt( int valIdx ) const
{
	return m_valueAttrs.GetInt(valIdx);
}

float AttrCalculator::GetBaseValueFloat( int valIdx ) const
{
    return m_baseValues.GetFloat(valIdx);
}

int AttrCalculator::GetBaseValueInt( int valIdx ) const
{
    return m_baseValues.GetInt(valIdx);
}

float AttrCalculator::CalcExtraBaseValueFloat( int valIdx ) const
{
    float base = GetBaseValueFloat(valIdx);
    return m_baseValueModSet.CalcValue(valIdx, base);
}

int AttrCalculator::CalcExtraBaseValueInt( int valIdx ) const
{
    int base = GetBaseValueInt(valIdx);
    return m_baseValueModSet.CalcValue(valIdx, base);
}

float AttrCalculator::CalcFinalAttrFloat( int valIdx ) const
{
    float extraBase = CalcExtraBaseValueFloat(valIdx);
    return m_valueModSet.CalcValue(valIdx, extraBase);
}

int AttrCalculator::CalcFinalAttrInt( int valIdx ) const
{
    int extraBase = CalcExtraBaseValueInt(valIdx);
    return m_valueModSet.CalcValue(valIdx, extraBase);
}


void AttrCalculator::ForceCalcAttributes()
{
    InitBaseAttributes();
    CalcBaseToFinalAttrs(); 

    m_valueModSet.ClearModifyFlag();
    m_baseValueModSet.ClearModifyFlag();
}

void AttrCalculator::CalcBaseToFinalAttrs()
{
    for( uint32 i = 0 ; i < m_baseValues.Size(); ++i)
    {
        if ( !IsAttrNeedToCalc(i))
        {
            return;
        }
        if (m_baseValues.IsFloat(i))
        {
            SetAttrFloat(i, CalcFinalAttrFloat(i));
        }
        else
        {
            SetAttrInt(i, CalcFinalAttrInt(i));
        }
    }
}






