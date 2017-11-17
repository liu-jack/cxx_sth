#ifndef PLAYER_SESSION_CONTAINER_H__
#define PLAYER_SESSION_CONTAINER_H__

#include "PlayerSession.h"


#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>


struct TPlayerSessionBySessionId {} ;
struct TPlayerSessionByPlayerId {} ;
struct TPlayerSessionByPlayerName {} ;
struct TPlayerSessionByCountryId {} ;

using namespace boost::multi_index;

typedef boost::multi_index_container< PlayerSession*, 
indexed_by<
hashed_unique< tag<TPlayerSessionBySessionId>,  BOOST_MULTI_INDEX_CONST_MEM_FUN( PlayerSession, uint64, GetSessionId ) >,
hashed_unique< tag<TPlayerSessionByPlayerId>,   BOOST_MULTI_INDEX_CONST_MEM_FUN( PlayerSession, uint64, GetPlayerId ) >,
hashed_non_unique< tag<TPlayerSessionByCountryId>,   BOOST_MULTI_INDEX_CONST_MEM_FUN( PlayerSession, uint32, GetCountryId ) >,
hashed_unique< tag<TPlayerSessionByPlayerName>, BOOST_MULTI_INDEX_CONST_MEM_FUN( PlayerSession, const std::string&, GetPlayerName ) >
>
> PlayerSessionTable ;

// µü´úÆ÷±ðÃû
typedef PlayerSessionTable::iterator PlayerSessionTableItr ;

typedef PlayerSessionTable::index<TPlayerSessionBySessionId>::type PlayerSessionBySessionId;
typedef PlayerSessionBySessionId::iterator PlayerSessionBySessionIdItr ;

typedef PlayerSessionTable::index<TPlayerSessionByPlayerId>::type PlayerSessionByPlayerId;
typedef PlayerSessionByPlayerId::iterator PlayerSessionByPlayerIdItr ;

typedef PlayerSessionTable::index<TPlayerSessionByCountryId>::type PlayerSessionByCountryId;
typedef PlayerSessionByCountryId::iterator PlayerSessionByCountryIdItr ;
typedef std::pair< PlayerSessionByCountryIdItr, PlayerSessionByCountryIdItr > RangeCountryIdItr;

typedef PlayerSessionTable::index<TPlayerSessionByPlayerName>::type PlayerSessionByPlayerName;
typedef PlayerSessionByPlayerName::iterator PlayerSessionByPlayerNameItr ;



#endif

