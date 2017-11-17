#include "gtest/gtest.h"

#include "utility/DailyTimeFrame.h"
#include "utility/TimeParser.h"

TEST( DailyTimeFrame,DailyTimeFrameContain )
{
    using namespace boost::gregorian;
    using namespace boost::posix_time;

    ptime now( second_clock::local_time());

    ptime::time_duration_type dur = now.time_of_day();

    DailyTimeFrame frame( dur, dur + seconds(1) );


    EXPECT_TRUE( frame.Contains(now));
}

TEST( time_parser, TimeTXString1)
{
    std::string str;
    TimeT2String( 0, str);
    time_t t;
    String2TimeT( str, t);

    EXPECT_EQ( t, 0);

}

TEST( time_parser, TimeTXString2)
{
    std::string str;
    TimeT2String( 0, str, false);
    time_t t;
    String2TimeT( str, t, false);

    EXPECT_EQ( t, 0);

}