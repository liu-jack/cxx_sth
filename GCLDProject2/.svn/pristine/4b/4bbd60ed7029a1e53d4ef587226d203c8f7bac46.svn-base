#ifndef DBPROXYDATADEFINE_H__
#define DBPROXYDATADEFINE_H__
#include <boost/shared_ptr.hpp>
#include "def/TypeDef.h"
typedef std::pair< int, int >   SessionPairId;

class PlayerData ;
typedef uint64 PlayerId ;
typedef boost::shared_ptr< PlayerData >     PlayerDataPtr ;
typedef std::map< PlayerId, PlayerDataPtr > PlayerDataMap ;
typedef PlayerDataMap::iterator             PlayerDataMapItr ;

class SessionData;
typedef boost::shared_ptr< SessionData >    SessionDataPtr ;
typedef std::map<SessionPairId,SessionDataPtr> SessionDataMap;
typedef SessionDataMap::iterator              SessionDataMapItr;

class GlobalData;
typedef boost::shared_ptr< GlobalData > GloabalDataPtr;


const time_t MAIL_NOLOGIN_CANT_RECEIVE_TIME = 2592123; //30天未登录者不再能接收邮件

#endif //DBPROXYDATADEFINE_H__

