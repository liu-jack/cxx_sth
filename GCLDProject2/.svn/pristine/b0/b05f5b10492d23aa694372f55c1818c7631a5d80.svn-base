#ifndef MAIL_CONST_H
#define MAIL_CONST_H

//enum MAIL_TYPE {
//	SYSTEM_MAIL  =1, //系统邮件
//	PERSONAL_MAIL=2,  //私人邮件
//	BATTLE_MAIL = 3, // 战斗邮件
//	MAIL_TYPE_COUNT
//};

enum SYSTEM_MAIL_TYPE{
	SYSTEM_TO_ONLINE_PLAYERS = 1, //发送给在线玩家
	SYSTEM_TO_ALLPLAYERS = 2,//发送给所有玩家
	SYSTEM_TO_PLAYERS   = 3,  //发送给指定玩家
	SYSTEM_MAIL_TYPE_COUNT
};

enum PERSONAL_MAIL_TYPE{
	PERSONAL_MESSAGE_MAIL =1, //私人普通邮件
	PERSONAL_ADDFRIEND    =2, //私人加好友
	PERSONAL_MAIL_TYPE_COUNT
};

enum BATTLE_MAIL_TYPE{
	BATTLE_MESSAGE_ROB_GEM = 1, // 夺宝 
	BATTLE_MESSAGE_FRIEND_HELP = 2, // 好友求助 
	BATTLE_MESSAGE_ARENA_MAIL = 3, //竞技场
	BATTLE_MAIL_TYPE_COUNT
};
enum MAIL_READ_STATE{
	MAIL_INIT  = 0,
	MAIL_NEW   = 1,
	MAIL_READ  = 2,  //邮件已经阅读
};

enum MAIL_OPTION_STATE{
	OPTION_SUCCESS   = 1,//操作成功
	OPTION_FAIL  = 2,  //操作失败
	OPTION_ARCHIVE_FAILURE =3   //存档失败
};


enum {
	UNSUPPORTED      =-1,
	SEND_MAIL_OK     =0,
	TARGET_NOT_EXIST =1, //目标用户不存在
	TARGET_NOT_FRIEND=2, //目标用户在黑名单中
	SAVE_DB_FAILE    =3,  //存档失败，可以在发送
};

enum {
	MAIL_OPREATE_SECCESS =1, //操作成功
	MAIL_PLAYERS_NOT_EXIST =2, //有玩家不存在
	MAIL_OPREATE_FAILD    =3,  //操作失败
	MAIL_PLAYERS_ONLINE =4,  //玩家都在线
};

enum {
	MAIL_OPREATE_ADD =1, //添加邮件
	MAIL_OPREATE_DEL =2, //删除邮件
	MAIL_OPREATE_UPDATE    =3,//更新邮件
};

#define NOT_USE_MAIL_TYPE      0
#define SYSTEM_MAIL_UID   0   //系统UID按照0
#define MAIL_AWARD_ID_NOTEXIST 0   //表明附件不存在的奖励ID值
#define MAIL_NOT_EXIST     0   // 邮件不存在时的ID标记
#define MAIL_PLATFORM_ID 1 // 后台发送物品的邮件ID
#define MAIL_MAX_COUNT 20 // 邮箱容量
#define MAIL_MAX_EXPIRE_TIME 2592000 //30*24*3600
#define MAIL_SEND_CHARGE 100
#endif