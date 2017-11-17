#pragma once

#include <string>
#include <memory>
#include "def/TypeDef.h"
class StringParserImpl;

class StringParser
{
public:
	explicit StringParser(const std::string& str, const std::string& sep);
	~StringParser();
	bool HasNext() const;

    void NextString( std::string& outVal, const std::string& defaultValue = "");
	void NextInt(    int& outVal,   int defaultValue = 0);
	void NextUint64( uint64& outVal, uint64 defaultValue = 0);
	void NextFloat(  float& outVal, float defaultValue = 0.0);

private:
	std::auto_ptr<StringParserImpl>		_impl;
};