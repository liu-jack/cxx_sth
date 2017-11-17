
#ifndef CLIENT_VERSION__
#define CLIENT_VERSION__

#include <map>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// every client version
struct ClientVersion 
{
public:
	ClientVersion( void )
	{
		id =0 ;
		channelId =0 ;
		platformId =0 ;
		curVersionId =0 ;
		isHotUpdate = 0;
		apkSize = 0;
		isVerify = 0;
		resourceId =0 ;
	}

	bool IsResourceMatch(uint32 resId) const
	{
		return resourceId == resId;
	}

    bool IsVersionMatch( uint32 ver )const
    {
        return curVersionId == ver;
    }

public:
	uint32      id ;
	uint32      channelId ;
	uint32      platformId ;
	uint32      curVersionId ;
	float       apkSize;
	uint32      isHotUpdate;
	uint32      isVerify ;
	uint32      resourceId ;
	std::string noticeAddress ;
	std::string maxVersionAddress ;
	std::string resourceAddress ;
} ;


typedef uint32                                   ChannelId ;
typedef uint32                                   PlatformId ;
typedef std::pair< ChannelId, PlatformId >       OrignInfoId ;
typedef std::map< OrignInfoId, ClientVersion >   VersionInfoTable ;
typedef VersionInfoTable::iterator               VersionInfoTableItr ;
typedef VersionInfoTable::const_iterator         VersionInfoTableCItr ;


#endif
