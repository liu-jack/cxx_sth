
#pragma once
#include <string>
#include "struct_base.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Data Format
/*
uint32				'u'
int32				'i'
float				'f'
string				's'
char				'c'
ignore				'x'
index string		'k'
*/


//set byte alignment 
#pragma pack(push,1)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Item Table

STRING_DB_STRUCT(String_Zones)

struct Zone
{
	enum
	{
		DEF_RECT,
		DEF_PEN,
	};

	Zone()
	{
		INIT_MEMBER_DATA; 

	//	polyInit = 0;
	}
	static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
		return Id;
	}
	void SetKey(uint32 _id) //only for editor
	{
		Id = _id;
	}
	static const char* GetKeyName()
	{
		return "Id";
	}
	static const char* GetTableName()
	{
		return "map_zone";
	}
	static const char* GetFormat()
	{
		//return "iiiuuisffffffffuuffuuffuuffuuffuussssssfffffusiisiusssis";
		return "uuuufffffffusuuuuuis";
	}
	bool IsPointInside(float pt_x, float pt_z)
	{
	//ge	if (definition == DEF_RECT)
		{
			return ( pt_x >= x && pt_x <= x1) &&
				( pt_z >= z && pt_z <= z1);
		}
	//ge	return IsPointInsidePolygon(pt_x, pt_z, 0.0f);
	}
	bool IsPointInsideExtend(float pt_x, float pt_z, float range)
	{
	//ge	if (definition == DEF_RECT)
		{
			return (pt_x > x - range && pt_x <= x1 + range) &&
				(pt_z > z - range && pt_z <= z1 + range);
		}
	//	return IsPointInsidePolygon(pt_x, pt_z, range);
	}

	// algorithm: http://alienryderflex.com/polygon/
	bool IsPointInsidePolygon(float pt_x, float pt_z, float range = 0.0f)
	{
/*ge		if (!polyInit)
		{
			InitPath();
			polyInit = 1;
		}

		int i = 0, j = polySides - 1;*/
		bool oddNodes = false;

	/*ge	for (i = 0; i < (int)polySides; i++)
		{
			if ((polyZ[i] < pt_z && polyZ[j] >= pt_z) || (polyZ[j] < pt_z && polyZ[i] >= pt_z))
					oddNodes ^= (pt_z * multiple[i] + constant[i] < pt_x);
			j = i;
		}
		*/
		return oddNodes;
	}

	bool InitPath()
	{
	/*ge	if (definition == DEF_RECT)
			return true;

		float value = 0.0f;
		size_t offset = 0;

		do
		{
			offset = !offset ? 0 : offset + 1;
			sscanf(points.c_str() + offset, "%f", &value);
			polyX.push_back(value);
			offset = points.find(',', offset);

			offset = !offset ? 0 : offset + 1;
			sscanf(points.c_str() + offset, "%f", &value);
			polyY.push_back(value);
			offset = points.find(',', offset);

			offset = !offset ? 0 : offset + 1;
			sscanf(points.c_str() + offset, "%f", &value);
			polyZ.push_back(value);
			offset = points.find(',', offset);
		}
		while (offset < points.length() && offset != std::string::npos);

		polySides = polyX.size();

		int i = 0, j = polySides - 1;

		for (i = 0; i < (int)polySides; i++)
		{
			if (polyZ[j] == polyZ[i])
			{
				constant.push_back(polyX[i]);
				multiple.push_back(0);
			}
			else
			{
				float c = polyX[i] - (polyZ[i] * polyX[j]) / (polyZ[j] - polyZ[i]) + (polyZ[i] * polyX[i]) / (polyZ[j] - polyZ[i]);
				float m = (polyX[j] - polyX[i]) / (polyZ[j] - polyZ[i]);
				constant.push_back(c);
				multiple.push_back(m);
			}
			j = i;
		}
		*/
		return true;
	}

// 	uint32 Id;
// 	int level;
// 	int parent_ID;
// 	uint32 map_id;
// 	uint32 phasing_id;
// 	int definition;	// 0: rectangle, 1: pen, see DEF_XXX enumeration.
// 	std::string points;
// 	float x;
// 	float y;
// 	float z;
// 	float radius;
// 	float x1;
// 	float y1;
// 	float z1;
// 	float box_orientation;
// 	uint32 priority;
// 	uint32 fog_color;
// 	float fog_near;
// 	float fog_half;//fog_far;
// 	//float fog_nearH;
// 	//float fog_farH;
// 	uint32 light_diffuse;
// 	uint32 light_ambient;
// 	float tone_near;
// 	float tone_half;
// 	uint32 tone_color_near;
// 	uint32 tone_color_far;
// 	float sky_near;
// 	float sky_half;
// 	uint32 sky_color_near;
// 	uint32 sky_color_far;
// 	float bloom_color_decrease;
// 	float bloom_intensity;
// 	uint32 music;
// 	//std::string strMusic;
// 	//std::string strSfx;
// 	uint32 sfx;
// 	std::string strSkybox;
// 	std::string strSkyboxHill;
// 	std::string strSkyboxSun;
// 	std::string strSkyboxGlow;
// 	std::string strSkyboxHalo;
// 	std::string strSkyboxAnimObj;
// 	float skybox_sun_size;
// 	float skybox_glow_size;
// 	float skybox_halo_size;
// 	float skybox_sun_y_angle;
// 	float skybox_sun_x_angle;
// 	uint32 skybox_halo_color;
// 	std::string strIcon;
// 	int pvp;
// 	int weather;
// 	std::string weather_extra;
// 	int friendly_level;
// 	uint32 zc_refresh_time_sec;
// 	std::string sound_music_label;
// 	std::string sound_env_label;
// 	std::string sound_reverb;
// 	int zone_skill;
// 	std::string zone_script;
// 	//////////////////////////////////////////////////
// 	std::string strName;
// 
// 	uint32 polyInit;
// 	size_t polySides;
// 	std::vector<float> polyX;
// 	std::vector<float> polyY;
// 	std::vector<float> polyZ;
// 	std::vector<float> constant;
// 	std::vector<float> multiple;


	uint32 Id;
	uint32 map_id;
	uint32 phasing_id;
	uint32 priority;
	float x;
	float y;
	float z;
	float x1;
	float y1;
	float z1;
	float box_orientation;
	uint32 zone_level;
	std::string des;
	uint32 name_id;
	uint32 sound_bgm_normal;
	uint32 sound_bgm_combat;
	uint32 sound_bgm_amb;
	uint32 sound_reverb;
	int weather;
	std::string weather_extra;

	DECLARE_EXTRA_SIZE_BY_MEMBER(Zone,des)
};

#define ZONE_OBJECT_TYPE_NUM 6
struct Zone_Object
{
	INIT_GAMEDATA(Zone_Object)
	static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
		return Id;
	}
	static const char* GetKeyName()
	{
		return "zoneID";
	}
	static const char* GetTableName()
	{
		return "zone_object";
	}
	static const char* GetFormat()
	{
		return "uuuuuuuuuuuuuu";
	}
	uint32 Id;
	uint32 objectType[ZONE_OBJECT_TYPE_NUM];
	uint32 objectNum[ZONE_OBJECT_TYPE_NUM];
	uint32 totalNum;
};

/*
struct Zone_Respawn_Npc
{
	INIT_GAMEDATA(Zone_Respawn_Npc)
		static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
		return id;
	}
	static const char* GetKeyName()
	{
		return "id";
	}
	static const char* GetTableName()
	{
		return "map_zone_respawn_npc";
	}
	static const char* GetFormat()
	{
		return "uuuuuuuuuuuuuuuuuuuuuuuu";
	}
	void SetKey(uint32 _id) //only for editor
	{
		id = _id;
	}
	uint32 id;
	uint32 zoneId;
	uint32 respawn_category;
	uint32 respawn_type;
	uint32 respawn_count_min;
	uint32 respawn_count_max;
	uint32 respawn_01_value;
	uint32 respawn_02_value;
	uint32 respawn_03_value;
	uint32 respawn_04_value;
	uint32 respawn_05_value;
	uint32 respawn_06_value;
	uint32 respawn_07_value;
	uint32 respawn_08_value;
	uint32 respawn_09_value;
	uint32 respawn_10_value;
	uint32 respawn_11_value;
	uint32 respawn_12_value;
	uint32 respawn_13_value;
	uint32 respawn_14_value;
	uint32 respawn_15_value;
	uint32 respawn_16_value;
	uint32 respawn_17_value;
	uint32 respawn_18_value;
};
*/

struct Weather
{
	INIT_GAMEDATA(Weather)
		static const bool HasIndex()
	{
		return true;
	}
	const uint32 GetKey() const
	{
		return Id;
	}
	static const char* GetKeyName()
	{
		return "weather_type";
	}
	static const char* GetTableName()
	{
		return "weather_proto";
	}
	static const char* GetFormat()
	{
		return "ussss";
	}
	uint32 Id;
	std::string effectfile[3];
	std::string comment;
};

struct ZoneSpecialMusic
{
	INIT_GAMEDATA(ZoneSpecialMusic)
		static const bool HasIndex()
	{
		return true;
	}

	const uint32 GetKey() const
	{
		return zoneID;
	}

	static const char *GetKeyName()
	{
		return "zoneID";
	}
	static const char *GetTableName()
	{
		return "ZoneSpecialMusic";
	}
	static const char *GetFormat()
	{
		return "uu";
	}
	uint32 zoneID;
	uint32 musicID;

};

#define MAX_PHASING_AREA_TILES 100

//rempha 
// struct PhasingArea
// {
// 	INIT_GAMEDATA(PhasingArea)
// 	static const bool HasIndex()
// 	{
// 		return true;
// 	}
// 	const uint32 GetKey() const
// 	{
// 		return Id;
// 	}
// 	static const char* GetKeyName()
// 	{
// 		return "id";
// 	}
// 	static const char* GetTableName()
// 	{
// 		return "phasing_area";
// 	}
// 	static const char* GetFormat()
// 	{
// 		return "usuuu"
// 			"iiiiiiiiii"
// 			"iiiiiiiiii"
// 			"iiiiiiiiii"
// 			"iiiiiiiiii"
// 			"iiiiiiiiii"
// 
// 			"iiiiiiiiii"
// 			"iiiiiiiiii"
// 			"iiiiiiiiii"
// 			"iiiiiiiiii"
// 			"iiiiiiiiii"
// 			
// 			"iiiiiiiiii"
// 			"iiiiiiiiii"
// 			"iiiiiiiiii"
// 			"iiiiiiiiii"
// 			"iiiiiiiiii"
// 
// 			"iiiiiiiiii"
// 			"iiiiiiiiii"
// 			"iiiiiiiiii"
// 			"iiiiiiiiii"
// 			"iiiiiiiiii"
// 			;
// 	}
// 	void SetKey(uint32 _id) //only for editor
// 	{
// 		Id = _id;
// 	}
// 
// 	uint32 Id;
// 	std::string Name;
// 	uint32 MapId;
// 	uint32 Color;
// 	uint32 TileCount;
// 
// 	struct XZCrood
// 	{
// 		int32 X;
// 		int32 Z;
// 	};
// 
// 	XZCrood Tile[MAX_PHASING_AREA_TILES];
// };


#pragma pack(pop)