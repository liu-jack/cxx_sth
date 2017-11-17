

#include "SignUp/SignUp.h"
#include "../object/Player.h"
#include "Table/SignUpTableMgr.h"
#include "Quest.pb.h"
#include "CrossLogic/PlayerSignUpDB.h"


SignUpLog::SignUpLog()
{
	Init();
}

SignUpLog::~SignUpLog()
{

}

void SignUpLog::Init()
{
	sign_up_struct_map_.clear();
	const SignUpMap& sign_up_map = sSignUpTableMgr.GetSignUpMap();
	for(SignUpMap::const_iterator iter = sign_up_map.begin();iter != sign_up_map.end();++iter)
	{
		SignUpStruct& ref = sign_up_struct_map_[iter->first];
		::memset(&ref,0,sizeof(SignUpStruct));
	}
}

void SignUpLog::LoadFromDB(const  pb::GxDB_Sign_Up& msg )
{
	for(int i = 0; i < msg.info_size();++i)
	{
		const pb::GxDB_Sign_info& info = msg.info(i);
		SignUpStruct& ref = sign_up_struct_map_[info.dayth()];
		ref.is_signed = info.issigned();
		ref.sign_type = info.type();
	}
}

void SignUpLog::SaveToDB( pb::GxDB_Sign_Up& msg )
{
	for(SignUpStructMap::iterator iter = sign_up_struct_map_.begin();iter != sign_up_struct_map_.end();++iter)
	{
		pb::GxDB_Sign_info* info = msg.add_info();
		info->set_dayth(iter->first);
		info->set_issigned(iter->second.is_signed);
		info->set_type(iter->second.sign_type);
	}
}

bool SignUpLog::CheckSigned( uint32 day )
{
	SignUpStruct& ref = sign_up_struct_map_[day];
	return ref.is_signed == 1;
}

void SignUpLog::UpdateSigned( uint32 day,uint32 type )
{
	SignUpStruct& ref = sign_up_struct_map_[day];
	ref.is_signed = 1;
	ref.sign_type = type;
}

void SignUpLog::GM_Finish( Player& player,uint32 day )
{
	if(CheckSigned(day)) return;
	SignUpStruct& ref = sign_up_struct_map_[day];
	ref.is_signed = 1;
	ref.sign_type = pb::MORMAL_SIGN;
	PlayerSignUpDB::SendInfoToDB(player,day,ref);
}

void SignUpLog::SendToDB(Player&player, uint32 day )
{	
	SignUpStruct& ref = sign_up_struct_map_[day];
	PlayerSignUpDB::SendInfoToDB(player,day,ref);
}

SignUpStructMap& SignUpLog::GetSignUpStructMap()
{
	return sign_up_struct_map_;
}

