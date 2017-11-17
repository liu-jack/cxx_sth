#ifndef DB_STRING_MGR_H
#define DB_STRING_MGR_H

#include "def/TypeDef.h"
#include "multi_language/multi_lang_def.h"


#ifdef MYSQL_SUPPORT
class CStringRow {
public:
	CStringRow();
	~CStringRow();
	void ResetStrings();


	enum {
		MAX_STRING_LENGTH = 4096,
	};
	wchar_t m_strArray[LANG_MAX][MAX_STRING_LENGTH];
	

};
#endif

template<class T>
class CDbStringMgr
{
public:
	CDbStringMgr(void) {};
	~CDbStringMgr(void) {};

	//Default Language Setting
	static void SetCurLang(MULTILANG curLang) {
		m_curLang = curLang;

#ifdef _AFANTY_
		m_curLang = LANG_EN;
#endif

	};

	static MULTILANG GetCurLang() {
		return m_curLang;
	};	
	
	
	//Operation to String in Cur Language
#ifdef _TOOL
	static bool GetString(uint32 strId, wchar_t * strArray, int buffLen = 1024) {
		return GetString(strId, strArray, buffLen, m_curLang);
	};
#else
	static bool GetString(uint32 strId, wchar_t * strArray, int buffLen) {
		return GetString(strId, strArray, buffLen, m_curLang);
	};
#endif

#ifdef MYSQL_SUPPORT
	static bool SaveString(uint32 strId, const wchar_t * strArray) {
		return SaveString(strId, strArray, m_curLang);
	};
	static uint32 NewString(const wchar_t * strArray){
		return NewString(strArray, m_curLang);
	};
#endif
	//static const char * GetUtf8String(int strId) {
	//	return GetUtf8String(strId, m_curLang);
	//};
	static bool GetUtf8String(uint32 strId, char * buffer) {
		return GetUtf8String(strId, buffer, m_curLang);
	};
	//static const char * GetUtf8String(int strId, MULTILANG speLang);
	static bool GetUtf8String(uint32 strId, char * buffer, MULTILANG speLang);


	static bool GetString(uint32 strId, wchar_t * strArray, int buffLen, MULTILANG speLang);
#ifdef MYSQL_SUPPORT 
	static bool SaveString(uint32 strId, const wchar_t * strArray, MULTILANG speLang);
	static uint32 NewString(const wchar_t * strArray, MULTILANG speLang);
	static uint32 DeleteString(uint32 strId);
	//Operation to String in All Language

	static bool GetStringRow(uint32 strId, CStringRow & strRow);

	static bool SaveStringRow(uint32 strId,  const CStringRow & strRow);
	static uint32 NewStringRow(const CStringRow & strRow);
#endif


private:
#ifdef MYSQL_SUPPORT
	static uint32	m_nextStrID;
#endif
	static MULTILANG m_curLang;
};

#endif