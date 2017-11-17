#ifndef AttrCalculator_h__
#define AttrCalculator_h__

#include "def/TypeDef.h"
#include "AttrModSet.h"
#include "memory_buffer/ValueArray.h"

class AttrCalculator
{
public:
    explicit AttrCalculator(ValueArray& valueArray);
    ~AttrCalculator();
public:
    void SetBaseValueInt(int valIdx, int val);
    void SetBaseValueFloat(int valIdx, float val);

    IAttrModGroup* CreateBaseAttrModGroup() const;
    IAttrModGroup* CreateAttrModGroup() const; 
    
    void    ForceCalcAttributes();

    virtual bool IsAttrNeedToCalc( int valIdx) { return false;};

protected:
    // 获得直接作用后的属性（例如 装备）
    float   CalcExtraBaseValueFloat(int valIdx) const;
    int     CalcExtraBaseValueInt(int valIdx) const; 

    float   CalcFinalAttrFloat(int valIdx) const;
    int     CalcFinalAttrInt(int valIdx) const; 

    virtual void InitBaseAttributes(){};
    void    CalcBaseToFinalAttrs();

    // 获得最后计算属性
    float   GetAttrFloat(int valIdx) const; 
    int     GetAttrInt(int valIdx) const;

private:
    // 设置最后计算属性
    void    SetAttrFloat(int valIdx, float val);
    void    SetAttrInt(int valIdx, int val);

    // 获得基础属性
    float   GetBaseValueFloat(int valIdx) const;
    int     GetBaseValueInt(int valIdx) const;

private:
    ValueArray						m_baseValues;			// 基础属性 
    //     值      ：X

    AttrModSet						m_baseValueModSet;		// 直接作用于基础属性的修改值	(a,b)
                                                            //     作用公式：Y =(1+a)X+b  

    AttrModSet						m_valueModSet;			// 作用于基础属性修改值修正后的 修改值(c,d)
                                                            //     作用公式：Z =(1+c)Y+d

    ValueArray&						m_valueAttrs;			// 计算后则为    Z
};


#endif // AttrCalculator_h__
