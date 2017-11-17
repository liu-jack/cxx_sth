#pragma once

#include "def/TypeDef.h"

struct LootFeatureItems;

// reviewed by zy


class LootFeatureItemLogic
{
    friend class LootFeatureLogic;
public:
	static bool IsValid(const LootFeatureItems* pItem);

	LootFeatureItemLogic(const LootFeatureItems* pItem);
	~LootFeatureItemLogic(void);

	uint32 Id() const;
	uint32 GeneCount() const;
	bool TryRandomPick(float curRate) const;
	bool   IsMoney() const { return m_isMoney; }
	uint32 MoneyType() const { return m_moneyType;}
private:
	const LootFeatureItems*		m_dbItem;
	bool m_isMoney;
	uint32 m_moneyType;
};
