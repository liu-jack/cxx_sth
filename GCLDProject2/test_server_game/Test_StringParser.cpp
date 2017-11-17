#include "gtest/gtest.h"

#include "utility/StringParser.h"
#include <boost/throw_exception.hpp>
#include <boost/tokenizer.hpp>
#include <iostream>

TEST( StringParser, parser)
{
    std::string helloworld("hello world ");
    StringParser parser(helloworld, " ");
    std::string str;
    parser.NextString( str);
    EXPECT_EQ( str, "hello");

}