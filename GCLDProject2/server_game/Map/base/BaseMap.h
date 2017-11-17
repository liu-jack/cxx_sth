/***********************************************************************
* @ ��ͼ
* @ brief
    1��������������ͼ�������ң����ٵ�ͼ����ͬ��ʱ�Ĺ㲥��

* @ author zhoumf
* @ date 2016-11-28
************************************************************************/
#pragma once
#include "def/TypeDef.h"
#include "boost/function.hpp"
#include "../Table/CityGraph.h"
#ifdef USE_PTR_HASH_MAP
#include <boost/unordered_map.hpp>
#endif // USE_PTR_HASH_MAP
class Player;
class City;
class MapCity;
class Country;
namespace google{
    namespace protobuf{
        class Message;
    }
}


namespace pb{
		class SD2G_Country_AllInfo;
		class GS2C_Country_Info;
		class CountryCrossWarInfo;
		class SG2B_Country_Info;
}


class CMap {
public:
    enum MapEnum {
        World,          // �����ͼ
        Crusade,          // Զ��
        EventStory,     // �¼��籾
    };

    const uint32  Tid; //��id
    const MapEnum type;
    CityGraph     m_cityGraph;
protected:
//#ifndef USE_PTR_HASH_MAP
	typedef std::map<uint32,City*> MapCityConter;
	typedef std::map<uint32, Country*> MapCountry;
//#else
//	typedef typename boost::unordered_map<uint32,City*> MapCityConter;
//	typedef typename boost::unordered_map<uint32,Country*> MapCountry;
//#endif // USE_PTR_HASH_MAP
    std::set<Player*>           m_PlayerLst;
    MapCityConter     m_cityLst;		//�����б�	<����id������ָ��>
    MapCountry  m_countryLst;	//�����б� <����ID��1��2,3����ǿ������4��������ָ��>

public:
    CMap(uint32 mapTid, MapEnum typ);
	virtual ~CMap();

    void AddToMap(Player& player);
    void DelFromMap(Player& player);
    void Broadcast(int opCode, google::protobuf::Message& msg);
    void ForEachPlayer(boost::function<void(Player*)>& func);

    City* GetCity(uint32 id);
    void ForEachCity(boost::function<void(City*)> func);
    void BuildEdgeCityLst(std::vector<City*>& lst);
    bool HavePartnerCity(uint32 id, uint32 exceptId = 0);
    void ResetEncircledCity(); //������崻����ؽ��ǳصġ���Χ��״̬
	bool AnyCountryCanCrossWar();

    Country* GetCountry(uint32 id);
	void SaveCountryData(Country* country, pb::SD2G_Country_AllInfo& msg);
	void SaveAllCountryData(pb::SD2G_Country_AllInfo& msg);
	void SaveAllCountryInfo(pb::GS2C_Country_Info& msgInfo);
	void UpdateCountryWarInfo(const pb::CountryCrossWarInfo& msgInfo);
	void BroadcastInCountry(int opCode, google::protobuf::Message& msg, uint32 countryid);
	void SaveCountryDevideinfo(pb::SG2B_Country_Info& msgInfo);

	void ClearCountryEndCitys();
private:
    void _InitCountryCity(const MapCity& city);

public:
    virtual void Init(); // ����ͼ��ʼ���Լ���
};