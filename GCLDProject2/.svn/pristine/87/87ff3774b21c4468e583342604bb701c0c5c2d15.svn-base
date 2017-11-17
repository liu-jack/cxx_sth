#ifndef SESSION_TYPE_DEF_H__
#define SESSION_TYPE_DEF_H__

#include <map>
#include <set>
#include <string>

#include "lock_free/LockFreeQueue.h"
#include "def/TypeDef.h"

typedef std::string                            Port ;
typedef std::string                            IP ;
typedef std::pair< Port, IP >                  SessionAddress ;   // Port use first is faster.
typedef std::set< SessionAddress >             SessionAddressSet ;

typedef int                                    serverId ;
typedef int                                    regionId ;
typedef std::pair< serverId, regionId >        SessionPairId ;
typedef std::set< SessionPairId >              SessionPairIdSet ;

typedef uint64                                    SessionId ;
typedef int                                    SetssionType ;

class Session ;

typedef std::vector< Session* >                SessionVec ;

typedef std::map< SessionId, Session* >        SessionMap ;
typedef SessionMap::iterator                   SessionMapItr ;

typedef std::map< serverId, Session* >         SessionByServerIdMap ;
typedef SessionByServerIdMap::iterator         SessionByServerIdMapItr ;


typedef std::map< SessionPairId, Session* >    PairIdSessionMap ;
typedef PairIdSessionMap::iterator             PairIdSessionMapItr ;


// Multi thread queue
typedef ServerQueue< Session* >                      MLockSessionQueue ;
typedef ServerQueue< Session* >                      DLockSessionQueue ;

typedef ServerQueue< SessionId >                     MLockSessionIdQueue ;
typedef ServerQueue< SessionId >                     DLockSessionIdQueue ;

class NetPack ;
typedef ServerQueue< NetPack* >                      DLockPacketQueue ;
typedef LockFreeQueue< NetPack* >                    FLockPacketQueue ;
typedef std::set< NetPack* >                         SPacketSet;



#endif


