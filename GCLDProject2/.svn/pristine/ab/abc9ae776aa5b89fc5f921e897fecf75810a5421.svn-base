#include "stdlib.h"
#include <boost/throw_exception.hpp>
#include <boost/tokenizer.hpp>
#include "StringParser.h"
#include "def/MmoAssert.h"
#include<sstream>

class StringParserImpl
{
	typedef boost::char_separator<char> CharSep;
	typedef boost::tokenizer<CharSep> Tokenizer;
	typedef Tokenizer::const_iterator TokenIter;

public:
	StringParserImpl(const std::string& str, const std::string& sep )
		:_sep(sep.c_str())
		,_tok(str, _sep)
	{
		_it = _tok.begin();
        _itEnd = _tok.end();
	}

	bool HasNext() const
	{
		return _it != _itEnd;
	}

	bool Next( std::string& out)
	{
		if(HasNext())
        {
            out = *_it++;
            return true;
        } 
        else
        {
            return false;
        }
		
	}

private:
	CharSep		_sep;
	Tokenizer	_tok;
	TokenIter	_it;
    TokenIter   _itEnd;
};

//=======================================================================

StringParser::StringParser( const std::string& str, const std::string& sep )
:_impl(new StringParserImpl(str,sep))
{
}

StringParser::~StringParser()
{
}

bool StringParser::HasNext() const
{
	return _impl->HasNext();
}

void StringParser::NextString( std::string& outVal, const std::string& defaultValue)
{
    if ( ! _impl->Next( outVal))
    {
        outVal = defaultValue;
    }
}

void StringParser::NextInt( int& outVal, int defaultValue)
{
    std::string tag;
    if ( _impl->Next( tag))
    {
        outVal = atoi(tag.c_str());
    }
    else
    {
        outVal = defaultValue;
    }
}

void StringParser::NextUint64( uint64& outVal, uint64 defaultValue)
{
	std::string tag;
	if ( _impl->Next( tag))
	{
		std::stringstream time_value;
		time_value << tag;
		uint64 curtag;
		time_value >> curtag;
		outVal = curtag;
	}
	else
	{
		outVal = defaultValue;
	}
}


void StringParser::NextFloat( float& outVal, float defaultValue)
{
    std::string tag;
    if ( _impl->Next( tag))
    {
        outVal = (float)::atof(tag.c_str());
    }
    else
    {
        outVal = defaultValue;
    }

}

