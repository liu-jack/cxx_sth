#pragma once
#include "common/auto_update_base/AutoUpdateBase.h"
#include "common/auto_update_base/UpdateEventBase.h"

class AutoUpdateLobby:public AutoUpdateBase
{
public:
	static AutoUpdateLobby& GetInstance(){ static AutoUpdateLobby T; return T; }

	virtual void InitAddFunctions();
	virtual void Update(uint64 nowTimeMS,int64 diffMS);

	bool doUpdateZero(time_t nowtime, boost::shared_ptr<UpdateEventBase> upe);

	void DoUpdatePer_Day_Zero();				//更新  每日		0点
	void DoUpdatePer_Hour_Zero();				//更新  每时		0点
};

#define sAutoUpdateLobby AutoUpdateLobby::GetInstance()