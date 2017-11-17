#include<math.h>
#include "CharAttrCalculator.h"
#include "def/ObjectDefines.h"
#include "CharacterProto.h"
#include "Logger.h"

#include "Enum.pb.h"

CharAttrCalculator::CharAttrCalculator(ValueArray& valueArray,const CharacterProto& proto)
: AttrCalculator( valueArray)
, m_charProto( proto)
{
}

CharAttrCalculator::~CharAttrCalculator()
{

}

void CharAttrCalculator::InitBaseAttributes()
{
    //for(uint32 i = 0; i < CHARACTER_VALUES_COUNT; ++i)
    //{
    //    // TODO, 要知道那个属性是int 还是float
    //    //SetBaseValueFloat( i, m_charProto.GenCharacterValues( i, Level(), Quality(), Reinforce()));
    //}    
}

int32 CharAttrCalculator::Level() const
{
    return GetAttrInt( pb::CHAR_FIELD_LEVEL);
}

int32 CharAttrCalculator::CurXp() const
{
    return GetAttrInt( pb::CHAR_FIELD_CUR_XP);
}

int32 CharAttrCalculator::Quality() const
{
    return 0;//GetAttrInt( pb::CHAR_FIELD_QUALITY);
}

int32 CharAttrCalculator::Reinforce() const
{
    return 0;// GetAttrInt( pb::CHAR_FIELD_REINFORCE);
}

bool CharAttrCalculator::IsAttrNeedToCalc( int valIdx)
{
	switch(valIdx){
		case UNIT_TROOPS:
		case UNIT_ATTACK:
		case UNIT_DEFENSE:
		case UNIT_PUNCTURE:
		case UNIT_PARRY:
		case UNIT_BROKEN_ARRAY:
		case UNIT_IMPECCABLE:
		case UNIT_TONG:
		case UINT_YONG:
		case UA_OUT_DAMAGE_FACTOR:
		case UA_IN_DAMAGE_FACTOR:
			break;
		default:
			break;

	}
    return false;
}
