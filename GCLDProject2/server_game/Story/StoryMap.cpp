#include "StoryMap.h"
#include "def/ConstDef.h"
#include "../object/Player.h"
#include "../GameServer.h"
#include "../Map/city/city.h"
#include "../Map/country/country.h"
#include "../Map/Table/MapTableMgr.h"

StoryMap::StoryMap(uint32 mapTid, MapEnum typ)
    : CMap(mapTid, typ)
{

}
void StoryMap::Init()
{
    CMap::Init();
}
