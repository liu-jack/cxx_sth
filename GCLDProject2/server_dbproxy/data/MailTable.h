#ifndef MAIL_TABLE_H__
#define MAIL_TABLE_H__

#include "structDef.h"

//set byte alignment 
#pragma pack(push,1)

namespace pb{
	class SG2G_Mail_SendMail;
	class SG2D_MailInfoData;
	class Mail_UserMailInfo;
};
struct PlatfromMailTable : public BaseStruct
{
public:
	INIT_GAMEDATA(PlatfromMailTable);
public:
	static const bool HasIndex()
	{
		return true;
	}

	const Key& GetKey( void ) const
	{
		return id ;
	}

    void SetDbIncreaseId(const IdType& id_)
    {
        id = id_;
    }

    uint64 GetLogicalKey( void) const
    {
        return id;
    }

	static const char* GetDbIncreaseIdName()
	{
		return "id";
	}

	static const char* GetLogicalKeyName( void )
	{
		return "id" ;
	}

	static const char* GetTableName()
	{
		return "platform_mail";
	}

	static const char* GetFormat()
	{
		return "buuutsss";
	}

	void LoadFrom(const pb::SG2G_Mail_SendMail &mail);
	void SaveTo(pb::SG2G_Mail_SendMail &mail);
	void SaveTo(pb::Mail_UserMailInfo &mail);
public:
	IdType      id ;
	uint32      region_id;
	uint32      server_id;
	uint32      mail_type;
	time_t      create_time;
	string      title;
	string      content;
	string      items;
} ;

struct PlayerMailTable : public BaseStruct
{
public:
	INIT_GAMEDATA_AND_EXTRA(PlayerMailTable,extraData);
public:
	static const bool HasIndex()
	{
		return true;
	}

	const Key GetKey(void) const
	{
		return id;
	}
    void SetDbIncreaseId(const IdType& id_)
    {
        id = id_;
    }

    uint64 GetLogicalKey( void) const
    {
        return id;
    }

	static const char* GetDbIncreaseIdName()
	{
		return "id";
	}

	static const char* GetLogicalKeyName(void)
	{
		return "id";
	}

	static const char* GetTableName()
	{
		return "player_mail";
	}

	static const char* GetFormat()
	{
		return "bbbuutus";
	}

	void LoadFrom(const pb::Mail_UserMailInfo &mail);
	void LoadFrom(const pb::SG2D_MailInfoData &mail);
	void SaveTo(pb::Mail_UserMailInfo &mail);
	bool IsAutoTake();
	bool		IsModify();
	void		SetModify();
	void		ResetModify();
	bool operator < (const PlayerMailTable* rt)
	{  
		return this->send_time < rt->send_time;
	}

public:
	IdType      id;
	uint64      player_id;
	uint64      mail_id;
	uint32      mail_type;
	uint32      mail_state;
	time_t		send_time;
	uint32      is_take;
	string		items;

	ExtraDataStruct extraData;
};

#pragma pack(pop)

#endif

