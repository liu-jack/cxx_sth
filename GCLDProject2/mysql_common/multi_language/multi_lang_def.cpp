
#include "multi_lang_def.h"

const char s_LangStr[LANG_MAX][32] = {
	"English", "French", "German", "Italian",  "Spanish", "Brazilian", "Japanese", "Korean", "Chinese", "Russian" };

const char s_LangISO6391Str[LANG_MAX][8] = {
    "en", "fr", "de", "it", "es", "pt", "ja", "ko", "zh", "ru" };

const char s_LangISO6391UpperStr[LANG_MAX][8] = {
    "EN", "FR", "DE", "IT", "ES", "PT", "JA", "KO", "ZH", "RU" };

const char s_LangIphoneStr[LANG_MAX][8] = {
    "en", "fr", "de", "it", "es", "pt", "ja", "ko", "zh-Hans", "ru" };

const char s_fontName[LANG_MAX][64] = {
	"Fontin",  //LANG_EN_NOE	
	"Fontin",  //LANG_FR_NOE	
	"Fontin",  //LANG_DE
	"Fontin",  //LANG_IT
	"Fontin",  //LANG_SP_NOE
	"Fontin",  //LANG_BR
	"FontJP",  //LANG_JP -- JP use a merged font with JP komatuna-p.ttf		WenquanYi(CN not included in the font of JP) + komatuna-p (JP)+ Nanum (KR) + Fontin(EU) 
	"Fontin",  //LANG_KR	
	"Fontin",  //LANG_CN --All other languages use a merged font WenquanYi(CN, JP)+ Nanum (KR) + Fontin(EU) 	
	"Fontin",
};

const char s_flagImageName[LANG_MAX][32] = {
	"english.png",		//LANG_EN	
	"french.png",		//LANG_FR	
	"german.png",		//LANG_DE		
	"italian.png",		//LANG_IT		
	"spanish.png",		//LANG_SP
	"brazilian.png",	//LANG_BR
	"japanese.png",		//LANG_JP		
	"korean.png",		//LANG_KR	
	"chinese.png",		//LANG_CN
	"russia.png",       //LANG_RU
};