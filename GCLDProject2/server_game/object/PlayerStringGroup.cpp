#include "PlayerStringGroup.h"


#include "server_client/DbProxyServerClient.h"
#include "CommomDef.pb.h"
#include "ServerOpcode.pb.h"
#include "object/Player.h"

PlayerString::PlayerString( uint32 strType,const std::string& str ) : m_strType( strType)
, m_string( str)
{
	ParserStrIds(str);
}

PlayerString::PlayerString( uint32 strType,uint32 idVal ): m_strType( strType)
{
	AddId(idVal);
}

bool PlayerString::AddId(uint32 idVal)
{
	if(m_IdsSet.find(idVal) == m_IdsSet.end())
	{
		m_IdsSet.insert(idVal);
		SaveStrIds(m_string);
		return true;
	}
	return false;
}

bool PlayerString::RemoveId(uint32 idVal)
{
	if(m_IdsSet.find(idVal) != m_IdsSet.end())
	{
		m_IdsSet.erase(idVal);
		SaveStrIds(m_string);
		return true;
	}
	return false;
}

bool PlayerString::ContainId( uint32 idVal )
{
	return m_IdsSet.find(idVal) != m_IdsSet.end();
}

void PlayerString::SaveTo( pb::GS2C_PlayerString& msg ) const
{
    msg.set_str_type( m_strType);
    msg.set_str( m_string.c_str());
}

void PlayerString::LoadFrom( const pb::GS2C_PlayerString& msg )
{
    m_string = msg.str();
    m_strType = msg.str_type();
	ParserStrIds(m_string);
}

void PlayerString::SetString( const std::string& str )
{
	if(m_string.compare(str) != 0)
	{
		m_string = str;
		ParserStrIds(str);
	}
}

const std::string& PlayerString::GetString() const
{
	return m_string;
}

void PlayerString::ParserStrIds( const ::std::string& strIds )
{
	std::string temStr = strIds;
	std::vector<std::string> value_vec;
	char* p = strtok(const_cast<char*>(temStr.c_str()),",");
	while(p != NULL)
	{
		value_vec.push_back(p);
		p = strtok(NULL,",");
	}
	for (size_t i = 0;i < value_vec.size();i++)
	{
		std::string apinfo = value_vec[i];
		m_IdsSet.insert(atoi(apinfo.c_str()));
	}
	value_vec.clear();
}

void PlayerString::SaveStrIds( string& strIds )
{
	strIds.clear();
	std::set<uint32>::iterator iterId  = m_IdsSet.begin();
	for (;iterId != m_IdsSet.end();++iterId)
	{
		uint32 valId = (*iterId);
		char temp[32];
		memset(temp,0,32);
		SPRINTF(temp,"%u,",valId);
		strIds.append(temp);
	}
}

//////////////////////////////////////////////////////////////////////////

PlayerStringGroup::PlayerStringGroup( )
{
       
}

bool PlayerStringGroup::ContainId( uint32 str_type,uint32 idVal)
{
	bool isContain = false;
	PlayerString* playerstr = m_stringMap.pfind( str_type);
	if ( playerstr)
	{
		isContain = playerstr->ContainId(idVal);
	}
	return isContain;
}

bool PlayerStringGroup::AddId(Player*player, uint32 str_type,uint32 idVal)
{
	bool isAdded = false;
	PlayerString* playerstr = m_stringMap.pfind( str_type);
	if ( playerstr)
	{
		isAdded = playerstr->AddId(idVal);
	}
	else
	{
		playerstr = new PlayerString( str_type, idVal);
		if ( playerstr)
		{
			m_stringMap[ str_type ] = playerstr;

			isAdded = true;
		}
	}

	if ( playerstr && player && isAdded)
	{
		pb::GS2C_PlayerString msg;
		playerstr->SaveTo( msg);
		sDbProxyClient.SendPlayerPack( *player, pb::SG2D_UPDATE_PLAYER_STRING, msg);
	}

	return isAdded;
}

bool PlayerStringGroup::ReMoveId(Player* player,uint32 str_type, uint32 idVal)
{
	bool isDel = false;
	PlayerString* playerstr = m_stringMap.pfind( str_type);
	if ( playerstr)
	{
		isDel = playerstr->RemoveId(idVal);
	}
	if ( playerstr && player && isDel)
	{
		pb::GS2C_PlayerString msg;
		playerstr->SaveTo( msg);
		sDbProxyClient.SendPlayerPack( *player, pb::SG2D_UPDATE_PLAYER_STRING, msg);
	}

	return isDel;
}

void PlayerStringGroup::SetString(Player* player, uint32 str_type, const string& str )
{
    PlayerString* playerstr = m_stringMap.pfind( str_type);
    if ( playerstr)
    {
        playerstr->SetString( str);
    }
    else
    {
        playerstr = new PlayerString( str_type, str);
        if ( playerstr)
        {
            m_stringMap[ str_type ] = playerstr;
        }
    }

    if ( playerstr && player)
    {
        pb::GS2C_PlayerString msg;
        playerstr->SaveTo( msg);
        sDbProxyClient.SendPlayerPack( *player, pb::SG2D_UPDATE_PLAYER_STRING, msg);
    }
}

PlayerString* PlayerStringGroup::GetPlayerString( uint32 str_type ) 
{
	PlayerString* playerstr = m_stringMap.pfind( str_type);
	return playerstr;
}

const string* PlayerStringGroup::GetString( uint32 str_type ) const
{
    PlayerString* playerstr = m_stringMap.pfind( str_type);
    if ( playerstr)
    {
        return &playerstr->GetString();
    }
    return NULL;
}

void PlayerStringGroup::LoadFrom( const pb::GS2C_PlayerStringGroup& msg )
{
    for ( int i = 0; i < msg.player_strings_size(); ++i)
    {
        const pb::GS2C_PlayerString& strMsg = msg.player_strings(i);

        PlayerString* playerstr = new PlayerString( strMsg.str_type(), strMsg.str());
        if ( playerstr)
        {
            m_stringMap[ strMsg.str_type() ] = playerstr;
        }
    }
}


