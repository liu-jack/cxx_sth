#pragma once

#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <boost/throw_exception.hpp>
#include <boost/tokenizer.hpp>
#include "SimpleSingleton.h"

class Player;
class NetPack;


class StringParser;
class GmManager : public SimpleSingleton<GmManager>
{
	friend class SimpleSingleton<GmManager>;

public:
	bool HandleCommand(Player* player, const std::string& text);


private:
	GmManager(void);
	~GmManager(void);

private:
};

#define sGmMgr GmManager::Instance()