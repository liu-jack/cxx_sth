#include "boost/date_time/posix_time/posix_time.hpp"


class DailyTimeFrame : private boost::posix_time::time_period
{

private:
    static boost::gregorian::date calibration_time;  // 校准时间
public:
    // begin : 初始时间 的时间间隔
    // end   : 结束时间 的时间间隔
    DailyTimeFrame( const boost::posix_time::time_duration& begin, const boost::posix_time::time_duration& end);

    bool Contains( const boost::posix_time::ptime& t ) const;


};