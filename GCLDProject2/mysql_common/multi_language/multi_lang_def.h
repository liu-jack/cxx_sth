#ifndef __MULTILANGDEF_H__
#define __MULTILANGDEF_H__

#pragma once

enum MULTILANG
{
	LANG_CN = 0,
	LANG_EN,
	LANG_JP,     
	LANG_KR,     
	LANG_FR,      
	LANG_DE,      
	LANG_IT,      
	LANG_SP,      
	LANG_BR,
	LANG_RU,
	LANG_COMMENT,
	LANG_COUNT = LANG_COMMENT,
	LANG_MAX
};
#define LANG_ALL (int32)-1

/* ** GLLIVE language definition **
#define GLIVE_LANG_EN          0
#define GLIVE_LANG_FR          1
#define GLIVE_LANG_DE          2
#define GLIVE_LANG_IT          3
#define GLIVE_LANG_SP          4
#define GLIVE_LANG_JP          5
#define GLIVE_LANG_KR          6
#define GLIVE_LANG_CN          7
#define GLIVE_LANG_BR          8
#define GLIVE_LANG_RU          9
// */

/// iso639-1 encode of language name
extern const char s_LangISO6391Str[LANG_MAX][8];

extern const char s_LangISO6391UpperStr[LANG_MAX][8];

extern const char s_LangIphoneStr[LANG_MAX][8];

extern const char s_LangStr[LANG_MAX][32];

extern const char s_fontName[LANG_MAX][64];

extern const char s_flagImageName[LANG_MAX][32];

#endif // ifndef __MULTILANGDEF_H__
