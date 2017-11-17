
//#include "PreHeader.h"
#include <assert.h>
#include "Cast.h"
#include <string.h>
#include <math.h>




std::wstring wstring_cast(const int& num)
{
	static wchar_t _itoa_buffer[16];
	std::wstring result;
	int t = num;
	int digit = -1;
	if (num < 0) {
		t = -num;
	}
	while (t > 0) {
		digit++;
		t /= 10;
	}
	if (digit < 0) {
		digit = 0;
	}
	t = num;
	if (num < 0) {
		_itoa_buffer[0] = L'-';
		digit++;
		t = -num;
	}
	int count = digit + 1;
	do {
		_itoa_buffer[digit--] = L'0' + (t % 10);
		t /= 10;
	} while (t > 0 && digit >= 0);
	result.assign(_itoa_buffer, count);
	return result;
}

std::wstring wstring_cast(const float& arg)
{
	std::wstring result;
	int intValue = (int)floorf(arg);
	int decimalValue = (int)((arg - floor(arg)) * 100.0f + 0.5f);
	result = wstring_cast(intValue);
	result += L'.';
	result += (L'0' + (decimalValue / 10));
	result += (L'0' + (decimalValue % 10));
	return result;
}

inline wchar_t C2W(char ch)
{
	return (wchar_t)ch;
}

wchar_t* cstr2wstr(const char *pcstr, wchar_t *pwstr)
{
	wchar_t* result = pwstr;
	if(pwstr != NULL && pcstr != NULL) {
		while(*pcstr != 0) {
			*pwstr = C2W(*pcstr);
			pwstr++;
			pcstr++;
		}
		*pwstr = L'\0';
		return result;
	}
	return 0;
}

std::wstring cstr2wstr(const char *pcstr)
{
	int length = strlen(pcstr);
	wchar_t* wstr = new wchar_t[length + 1];
	cstr2wstr(pcstr, wstr);
	std::wstring result = wstr;
	delete [] wstr;
	return result;
}


bool utf82unicode(const char* inStr, wchar_t* outStr, int buffLen)
{
	int inLen = strlen(inStr);
	return utf82unicode(inStr, inLen, outStr, buffLen);
}

/*inline*/ bool utf82unicode(const char* inStr, const int inLen, wchar_t* outStr, int& outLen)
{
	if(!inStr || !outStr) {
		return false;
	}

	int curLen = 0;
	assert(outLen > 1 && outLen > inLen);
	int maxLen = outLen - 1;

	int charIndex = 0;
	int unicodeIndex = 0;
	while(charIndex < inLen) {
		if (curLen + 1 >= maxLen)
		{
			assert(0);
			break;
		}
		//read one char.
		int charBytes = 0;
		const char* utf8 = &inStr[charIndex];
		wchar_t* unicodeChar = &outStr[unicodeIndex];
		*unicodeChar = (wchar_t)0;

		if( (utf8[0] & 0x80) == 0x0) {
			charBytes = 1;
			*unicodeChar = utf8[0];
		} else if( (utf8[0] & 0xE0) == 0xC0 ) {
			charBytes = 2;
			*unicodeChar = (utf8[0] & 0x1F) << (6);
		} else if( (utf8[0] & 0xF0) == 0xE0 ) {
			charBytes = 3;
			*unicodeChar = (utf8[0] & 0x0F) << (12);
		} 
#if 0
		else if( (utf8[0] & 0xF8) == 0xF0 ) {
			charBytes = 4;
			*unicodeChar = (utf8[0] & 0x07) << (18);
		} else if( (utf8[0] & 0xFC) == 0xF8 ) {
			charBytes = 5;
			*unicodeChar = (utf8[0] & 0x03) << (24);
		} else if( (utf8[0] & 0xFE) == 0xFC ) {
			charBytes = 6;
			*unicodeChar = (utf8[0] & 0x01) << (30);
		}
#endif	
		for(int c = 1; c < charBytes; c++) {
			*unicodeChar |= (utf8[c] & 0x3F) << (charBytes - c - 1) * 6;
		}

		if(charBytes == 0) {
			charIndex ++;
		} else {
			charIndex += charBytes;
			unicodeIndex ++;
			curLen++;
		}
		
	}
	//
	outLen = unicodeIndex;
	outStr[outLen] = 0;

	return true;
}

bool unicode2utf8(const wchar_t* inStr, char* outStr, int buffLen)
{
	int inLen = wcslen(inStr);
	return unicode2utf8(inStr, inLen, outStr, buffLen);
}

bool unicode2utf8(const wchar_t* inStr, const int inLen, char* outStr, int& outLen)
{
	if(!inStr || !outStr) {
		return false;
	}
	
	int curLen = 0;
	int maxLen = inLen * 6;
	if (outLen > 0) 
		maxLen = outLen - 1;

	int unicodeIndex = 0;
	char * outChar = outStr;
	while(unicodeIndex < inLen) {
		wchar_t curUnicodeChar = *(inStr + unicodeIndex);

		if (curLen + 1 >= maxLen)
		{
			assert(0);
			break;
		}
		int charBytes = 0;
		if(curUnicodeChar < 0x7F)
		{
			charBytes = 1;
			*outChar++= curUnicodeChar & 0x7F;
		}
		else if(curUnicodeChar < 0x7FF)
		{
			charBytes = 2;
			*outChar++=0xC0 | (curUnicodeChar >> 6);
		}
		else if(curUnicodeChar < 0xFFFF)
		{
			charBytes = 3;
			*outChar++=0xE0 | (curUnicodeChar >> 12);
		}
#if 0
		else if(curUnicodeChar < 0x1FFFFF)
		{
			charBytes = 4;
			*outChar++=0xF0 | (curUnicodeChar >> 18);
		}
		else if(curUnicodeChar < 0x3FFFFFF)
		{
			charBytes = 5;
			*outChar++=0xF8 | (curUnicodeChar >> 24);
		}
		else if(curUnicodeChar < 0x7FFFFFFF)
		{
			charBytes = 6;
			*outChar++= 0xFC | (curUnicodeChar >> 30);
		}
#endif

		if (curLen + charBytes >= maxLen)
		{
			outChar--;
			assert(0);
			break;
		}
		curLen += charBytes;
		for(int c = 1; c < charBytes; c++) {
			*outChar++ = 0x80 | ((curUnicodeChar >> (charBytes - c - 1) * 6) & 0x3F);
		}

		unicodeIndex ++;
		
	}
	*outChar = 0;
	outLen = outChar - outStr;


	return true;
}

int countStringCN2EU1_utf8(const char* inStr)
{
	int count = 0;
	int charIndex = 0;
	while(inStr[charIndex]) {
		//read one char.
		int charBytes = 0;
		const char utf8_char = inStr[charIndex];

		if( (utf8_char & 0x80) == 0x0) {
			charBytes = 1;
		} else if( (utf8_char & 0xE0) == 0xC0 ) {
			charBytes = 2;
		} else if( (utf8_char & 0xF0) == 0xE0 ) {
			charBytes = 3;
		} 
#if 0
		else if( (utf8_char & 0xF8) == 0xF0 ) {
			charBytes = 4;
		} else if( (utf8_char & 0xFC) == 0xF8 ) {
			charBytes = 5;
		} else if( (utf8_char & 0xFE) == 0xFC ) {
			charBytes = 6;
		}
#endif	
		count += charBytes >=3 ? 2 : 1;
		if(charBytes == 0) {
			charIndex ++;
		} else {
			charIndex += charBytes;
		}
	}
	return count;
}


int countStringCN2EU1_unicode(const wchar_t* inStr)
{
	int count = 0;
	int unicodeIndex = 0;
	for( ;inStr[unicodeIndex]; unicodeIndex ++)
	{
		wchar_t curUnicodeChar = inStr[unicodeIndex];

		int charBytes = 0;
		if(curUnicodeChar < 0x7F)
		{
			charBytes = 1;
		}
		else if(curUnicodeChar < 0x7FF)
		{
			charBytes = 2;
		}
		else if(curUnicodeChar < 0xFFFF)
		{
			charBytes = 3;
		}
#if 0
		else if(curUnicodeChar < 0x1FFFFF)
		{
			charBytes = 4;
		}
		else if(curUnicodeChar < 0x3FFFFFF)
		{
			charBytes = 5;
		}
		else if(curUnicodeChar < 0x7FFFFFFF)
		{
			charBytes = 6;
		}
#endif
		count += charBytes >=3 ? 2 : 1;

		
	}
	return count;
}

bool limitStringCN2EU1_utf8(char *inStr, int limitLen)
{
	int count = 0;
	int charIndex = 0;
	while(inStr[charIndex]) {
		//read one char.
		int charBytes = 0;
		const char utf8_char = inStr[charIndex];

		if( (utf8_char & 0x80) == 0x0) {
			charBytes = 1;
		} else if( (utf8_char & 0xE0) == 0xC0 ) {
			charBytes = 2;
		} else if( (utf8_char & 0xF0) == 0xE0 ) {
			charBytes = 3;
		} 
#if 0
		else if( (utf8_char & 0xF8) == 0xF0 ) {
			charBytes = 4;
		} else if( (utf8_char & 0xFC) == 0xF8 ) {
			charBytes = 5;
		} else if( (utf8_char & 0xFE) == 0xFC ) {
			charBytes = 6;
		}
#endif	
		count += charBytes >=3 ? 2 : 1;
		if(count > limitLen)
		{
			inStr[charIndex] = 0;
			return true;
		}

		if(charBytes == 0) {
			charIndex ++;
		} else {
			charIndex += charBytes;
		}
	}
	return false;
}

bool limitStringCN2EU1_unicode(wchar_t *inStr, int limitLen)
{
	int count = 0;
	int unicodeIndex = 0;
	for( ;inStr[unicodeIndex]; unicodeIndex ++)
	{
		wchar_t curUnicodeChar = inStr[unicodeIndex];

		int charBytes = 0;
		if(curUnicodeChar < 0x7F)
		{
			charBytes = 1;
		}
		else if(curUnicodeChar < 0x7FF)
		{
			charBytes = 2;
		}
		else if(curUnicodeChar < 0xFFFF)
		{
			charBytes = 3;
		}
#if 0
		else if(curUnicodeChar < 0x1FFFFF)
		{
			charBytes = 4;
		}
		else if(curUnicodeChar < 0x3FFFFFF)
		{
			charBytes = 5;
		}
		else if(curUnicodeChar < 0x7FFFFFFF)
		{
			charBytes = 6;
		}
#endif
		count += charBytes >=3 ? 2 : 1;
		if(count > limitLen)
		{
			inStr[unicodeIndex] = 0;
			return true;
		}	
	}
	return false;

}


wchar_t wbuf[20000] =L"" ;
char    buf[60000] ="" ;

wchar_t *s_dbBufferUTF16 = wbuf;
char *s_dbBufferUTF8 = buf;

const char * filterString(const char *inStr, int &outLen, bool isFrench) //Special function to convert string in database for auto_line_feed and non-break-spaces
{
	//if(s_dbBufferUTF16 == NULL)
	//{
	//	s_dbBufferUTF16 = new wchar_t[20000];
	//	s_dbBufferUTF8 = new char[60000];
	//}

	utf82unicode(inStr, s_dbBufferUTF16, 20000);
#define HTML_TAG_START 0x12
#define HTML_TAG_END   0x13
	//int k=0;
	int inLen = 0;
	for(int i=0; s_dbBufferUTF16[i] != 0; i++)
	{
		if(s_dbBufferUTF16[i] == L'~')
		{
			s_dbBufferUTF16[i] = 17;
		}
		else if(s_dbBufferUTF16[i] == L'^')
		{
			s_dbBufferUTF16[i]= 0xA0;
		}
		//else if(s_dbBufferUTF16[i] == L'<' || s_dbBufferUTF16[i] == L'[')
		//{
		//	s_dbBufferUTF16[i]= HTML_TAG_START;
		//}
		//else if(s_dbBufferUTF16[i] == L'>' || s_dbBufferUTF16[i] == L']')
		//{
		//	s_dbBufferUTF16[i]= HTML_TAG_END;
		//}
		else if(isFrench) //Remove Accents
		{

			if(s_dbBufferUTF16[i] ==0x00C0)    
				s_dbBufferUTF16[i] = L'A';  	// A`		0xC0 ->A
			else if(s_dbBufferUTF16[i] ==0x00C2)    
				s_dbBufferUTF16[i] = L'A';  	// A^ 	0xC2 ->A
			else if(s_dbBufferUTF16[i] ==0x00C7)    
				s_dbBufferUTF16[i] = L'C';		// C		0xC7 ->C
			else if(s_dbBufferUTF16[i] ==0x00C8)    
				s_dbBufferUTF16[i] = L'E';		// E`		0xC8 ->E
			else if(s_dbBufferUTF16[i] ==0x00C9)   
				s_dbBufferUTF16[i] = L'E';		// E'		0xC9 ->E
			else if(s_dbBufferUTF16[i] ==0x00CA)    
				s_dbBufferUTF16[i] = L'E';		// E^		0xCA ->E
			else if(s_dbBufferUTF16[i] ==0x00CB)    
				s_dbBufferUTF16[i] = L'E';		// E:		0xCB ->E
			else if(s_dbBufferUTF16[i] ==0x00CE)    
				s_dbBufferUTF16[i] = L'I';		// I^		0xCE ->I
			else if(s_dbBufferUTF16[i] ==0x00CF)    
				s_dbBufferUTF16[i] = L'I';		// I:		0xCF ->I
			else if(s_dbBufferUTF16[i] ==0x00D4)    
				s_dbBufferUTF16[i] = L'O';		// O^	0xD4 ->O
			else if(s_dbBufferUTF16[i] ==0x00D9)    
				s_dbBufferUTF16[i] = L'U';		// U`		0xD9 ->O
			else if(s_dbBufferUTF16[i] ==0x00DB)    
				s_dbBufferUTF16[i] = L'U';		// U^		0xDB ->O

		}
		inLen++;
	}
#undef HTML_TAG_START
#undef HTML_TAG_END
	inLen++;
	outLen = 0;
	unicode2utf8(s_dbBufferUTF16, inLen, s_dbBufferUTF8, outLen);
	return s_dbBufferUTF8;
}
