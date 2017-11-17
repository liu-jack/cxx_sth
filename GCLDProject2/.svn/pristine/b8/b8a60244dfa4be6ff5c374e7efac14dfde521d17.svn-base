

#ifndef __SGIN_UP__H
#define __SGIN_UP__H
#include <map>
#include "def/TypeDef.h"
#include "boost/noncopyable.hpp"
class Player;
namespace pb
{
	class GxDB_Sign_Up;
	class GS2C_Sign_Up_All_info;
}	

struct SignUpStruct
{
	uint32 is_signed;
	uint32 sign_type;
};


typedef std::map<uint32,SignUpStruct> SignUpStructMap;

class SignUpLog :public boost::noncopyable
{
private:
	SignUpStructMap sign_up_struct_map_;
public:
	SignUpLog();
	~SignUpLog();
	void Init();
	void LoadFromDB(const pb::GxDB_Sign_Up& msg);
	void SaveToDB(pb::GxDB_Sign_Up& msg);
	bool CheckSigned(uint32 day);
	void UpdateSigned(uint32 day,uint32 type);
	void SendToDB(Player& player,uint32 day);

	void GM_Finish(Player& player,uint32 day);
	SignUpStructMap& GetSignUpStructMap();
};

#endif

