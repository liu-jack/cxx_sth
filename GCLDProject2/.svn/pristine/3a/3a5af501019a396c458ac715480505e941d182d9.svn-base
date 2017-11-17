#pragma once


#include "../object/attr/AttrCalculator.h"


class CharacterProto;

class CharAttrCalculator : public AttrCalculator
{
public:
	CharAttrCalculator(ValueArray& valueArray,const CharacterProto& proto);
	~CharAttrCalculator();

    virtual bool IsAttrNeedToCalc( int valIdx);

    int32 Level() const;
    int32 CurXp() const;
    int32 Quality() const;
    int32 Reinforce() const;

private:
    virtual void InitBaseAttributes();
    const CharacterProto& m_charProto;
    
};
