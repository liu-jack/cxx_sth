#include "CharacterPredicate.h"
#include "Character.h"
#include "datastruct/struct_character.h"




PredCharLevelMatch::PredCharLevelMatch( int lv )
: m_level( lv)
{

}

bool PredCharLevelMatch::operator()( const Character * chara ) const
{
    return chara->GetLevel() >= m_level;
}


PredCharQualityMatch::PredCharQualityMatch( int quality )
: m_quality( quality)
{

}

bool PredCharQualityMatch::operator()( const Character * chara ) const
{
    return chara->GetQuality() >= m_quality;
}

PredCharReinforceMatch::PredCharReinforceMatch( int reinforce )
: m_reinforce( reinforce)
{

}

bool PredCharReinforceMatch::operator()( const Character * chara ) const
{
    return chara->GetReinforce() >= m_reinforce;
}

PredCharSkillLvMatch::PredCharSkillLvMatch( int lv )
: m_level( lv)
{

}

bool PredCharSkillLvMatch::operator()( const Character * chara ) const
{
    for ( uint32 i = 0 ; i < DB_CharSetSkills::MAX_SKILL_INDEX; ++i)
    {
        if( chara->GetSkillLevel(i) >= m_level)
        {
            return true;
        }
    }
    return false;
}
