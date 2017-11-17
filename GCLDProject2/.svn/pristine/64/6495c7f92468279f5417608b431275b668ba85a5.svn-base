#include "DailyTimeFrame.h"

#include "boost/date_time/c_local_time_adjustor.hpp"
using namespace boost::gregorian;
using namespace boost::posix_time;


boost::gregorian::date DailyTimeFrame::calibration_time( 2012, boost::gregorian::Jan, 1);

DailyTimeFrame::DailyTimeFrame( const time_duration& begin, const time_duration& end )
    : time_period( ptime( calibration_time, begin), ptime( calibration_time, end)) 
{
}

bool DailyTimeFrame::Contains( const boost::posix_time::ptime& t ) const
{
    return time_period::contains( t - (t.date() - calibration_time));
}
