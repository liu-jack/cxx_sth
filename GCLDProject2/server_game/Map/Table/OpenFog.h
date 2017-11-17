
#pragma once
#include "def/TypeDef.h"
struct DB_OpenFog;

class OpenFog
{
	const DB_OpenFog& open_fog_;
public:
	explicit OpenFog(const DB_OpenFog& db);

	uint32 Id() const;
	uint32 OpenNum() const;
	uint32 RewardId() const;
	uint32 RewardNum() const;
};