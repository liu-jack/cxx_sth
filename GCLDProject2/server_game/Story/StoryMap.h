#pragma once
#include "../Map/base/BaseMap.h"
#include "memory_buffer/MemoryPool.h"


#define MAX_STORY_COUNTRY_ID 10


class StoryMap : public CMap 
{
    DECLARE_OBJ_POOL(StoryMap)
public:
    StoryMap(uint32 mapTid, MapEnum typ);

public:
    //virtual func
    void Init();
};
