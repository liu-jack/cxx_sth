#pragma once
#include "def/TypeDef.h"
struct DB_TunTian_Up;

class MapTunTian {
	const DB_TunTian_Up& m_dbMapTunTian;	//������Ϣ

public:

	explicit MapTunTian(const DB_TunTian_Up& db);

	//uint16 Id() const;	//������

	//uint32 CountryId() const;	//������
	//uint32 BattleCityId() const;	//�ǳ�id
	//uint32 StageId() const;	//ս���ؿ�
	//uint32 ShowLevel() const;	//��ʾ�ȼ�

	uint32 GetExp() const;
	uint32 GetAwardFactor() const;
};