#ifndef CAST_H_
#define CAST_H_

#include <string>

std::wstring wstring_cast(const int& num);

std::wstring wstring_cast(const float& arg);

wchar_t* cstr2wstr(const char *pcstr, wchar_t *pwstr);

std::wstring cstr2wstr(const char *pcstr);

#define array_sizeof(a) (sizeof(a)/sizeof(a[0]))

const char * filterString(const char *inStr, int &outLen, bool isFrench = false); //Special function to convert string in database for auto_line_feed and non-break-spaces

#ifdef _TOOL
bool utf82unicode(const char* inStr, wchar_t* outStr, int buffLen = 2048);
#else
bool utf82unicode(const char* inStr, wchar_t* outStr, int buffLen);
#endif

bool utf82unicode(const char* inStr, const int inLen, wchar_t* outStr, int& outLen);

#ifdef _TOOL
bool unicode2utf8(const wchar_t* inStr, char* outStr, int buffLen = 1024);
#else
bool unicode2utf8(const wchar_t* inStr, char* outStr, int buffLen);
#endif
bool unicode2utf8(const wchar_t* inStr, const int inLen, char* outStr, int& outLen);

int countStringCN2EU1_utf8(const char* inStr);

int countStringCN2EU1_unicode(const wchar_t* inStr);

bool limitStringCN2EU1_utf8(char *inStr, int limitLen);
bool limitStringCN2EU1_unicode(wchar_t *inStr, int limitLen);

#endif
