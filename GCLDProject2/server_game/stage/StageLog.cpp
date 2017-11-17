#include <boost/typeof/typeof.hpp>

#include "StageLog.h"

#include "utility/Utility.h"
#include "utility/MsgTool.h"
#include "utility/STLUtility.h"


#include "datastruct/struct_stage.h"
#include "StageDefine.h"
#include "StageManager.h"
#include "StageLogger.h"
#include "StageLevelLog.h"

#include "InterServer.pb.h"
#include "Stage.pb.h" 
#include "ServerOpcode.pb.h"

//1�ؿ� = 1�½�*n

using namespace pb;

//���캯����ʼ��,����һ���ؿ�����Ϣ
StageLog::StageLog( const DB_StageProto& Proto)
: m_StageProto(Proto)
{

}

StageLog::~StageLog()
{
}


//������������ݣ�����Ϊ�½���Ϣ����
void StageLog::SaveTo(pb::GS2C_AllStageInfo& info) const
{
	LevelLogsMap::const_iterator iterLevel = m_stageLevelsMap.begin();
	for (; iterLevel != m_stageLevelsMap.end(); iterLevel++)
	{
        MsgTool::SaveToRepeatField( iterLevel->second, info.mutable_stage_level_infos());
	}
}

//����һ���½�
bool StageLog::CreateLevel(const pb::GS2C_StageLevel& levelInfo)
{
	uint32 levelId = levelInfo.level_id();
	const DB_StageLevel* curLevel = sStageManager.GetStageLevelById(levelId);
    if ( curLevel == NULL)
        return false;
	std::auto_ptr<StageLevelLog> pStageLevelLog(new StageLevelLog(*curLevel, this));
	if (pStageLevelLog.get())
	{
		if (pStageLevelLog->LoadFrom(levelInfo))
		{
			m_stageLevelsMap[levelId] = pStageLevelLog.release();
			return true;
		}
	}
	return false;
}

//��ǰ�ؿ��Ƿ��Ѿ�ɨ��
bool StageLog::IsStageClearance() const
{
    uint32 levelIds = sStageManager.GetStageLevelIdCount( StageId());		//��øùؿ����½���
    if ( levelIds > m_stageLevelsMap.size())
    {
        return false;
    }

    for ( BOOST_AUTO( it, m_stageLevelsMap.begin()); it != m_stageLevelsMap.end(); ++it)
    {
        const StageLevelLog* levelLog = it->second;
        if ( levelLog)
        {
            if ( !levelLog->IsClearance())
            {
                return false;
            }
        }
    }

    return true;    
}

//��ùؿ�id
uint32 StageLog::StageId() const
{
    return m_StageProto.id;
}

//����ӣ����һ���½��߼�
StageLevelLog* StageLog::AddOrGetStageLevelLog( uint32 levelId , bool& newLevel)
{
    newLevel = false;
    const DB_StageLevel* theLevel = sStageManager.GetStageLevelById( levelId);
    if ( theLevel != NULL)
    {
        StageLevelLog* levelLog = m_stageLevelsMap.pfind( levelId);
        if ( levelLog == NULL )
        {
            levelLog = new StageLevelLog( *theLevel, this);
            if ( levelLog)
            {
                newLevel = true;
                m_stageLevelsMap[ levelId] = levelLog;
            }
        }
        return levelLog;
    }

    return NULL;
}

//���һ���߼��½�
StageLevelLog* StageLog::GetStageLevelLog( uint32 levelId )
{
    return m_stageLevelsMap.pfind( levelId);
}




