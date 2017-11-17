#pragma once
#include "ptr_container/PtrMap.h"
#include "def/TypeDef.h"

namespace pb
{
    class GS2C_PlayerString; 
    class GS2C_PlayerStringGroup;
}

class Player;

class PlayerString
{
public:
    PlayerString()
        : m_strType(0)
    {};
    PlayerString( uint32 strType,const std::string& str);
	PlayerString( uint32 strType,uint32 idVal);

	bool AddId(uint32 idVal);
	bool RemoveId(uint32 idVal);
	bool ContainId(uint32 idVal);
	void SaveTo( pb::GS2C_PlayerString& msg) const;
    void LoadFrom( const pb::GS2C_PlayerString& msg);   

    void SetString( const std::string& str);
    const std::string& GetString() const;
private:
	void ParserStrIds(const ::std::string& strIds);
	void SaveStrIds(string& strApIds);
private:
    uint32 m_strType;
    std::string m_string;
	std::set<uint32> m_IdsSet;
};


class PlayerStringGroup
{
    typedef PtrMap< uint32, PlayerString> PlayerStringMap;
public:
    PlayerStringGroup();

	bool ContainId( uint32 str_type,uint32 idVal);
	bool AddId(Player*player, uint32 str_type,uint32 idVal);
	bool ReMoveId(Player* player,uint32 str_type, uint32 idVal);
	const string* GetString( uint32 str_type) const;
    void SetString(Player* player, uint32 str_type, const string& str);

	PlayerString* GetPlayerString( uint32 str_type );
	void LoadFrom( const pb::GS2C_PlayerStringGroup& msg);
private:
    PlayerStringMap m_stringMap;
    /*Player*         m_pPlayer;*/
};