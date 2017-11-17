/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/
#include "PlayerSignUpDB.h"
#include "SignUp/SignUp.h"
#include "../object/Player.h"
#include "Quest.pb.h"
#include "../server_client/DbProxyServerClient.h"
#include "ServerOpcode.pb.h"
using namespace pb;

void PlayerSignUpDB::SendInfoToDB( Player& player,uint32 day,SignUpStruct& ref )
{
#ifdef _MMO_SERVER_
	pb::GxDB_Sign_info msg;
	msg.set_dayth(day);
	msg.set_issigned(ref.is_signed);
	msg.set_type(ref.sign_type);
	sDbProxyClient.SendPlayerPack(player, SG2D_SIGN_UP_UPDATE, msg);
#endif
}