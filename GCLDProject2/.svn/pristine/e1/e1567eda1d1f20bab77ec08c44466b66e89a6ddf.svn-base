#ifndef ACCOUNTDATACONTAINER_H_
#define ACCOUNTDATACONTAINER_H_

#include "../data/Account.h"

#include <tuple>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>


struct TPId {} ;
struct TAId {} ;

using namespace boost::multi_index;

typedef boost::multi_index_container< AccountRoleInfo*, 
indexed_by<
hashed_unique< tag<TPId>,   BOOST_MULTI_INDEX_CONST_MEM_FUN( AccountRoleInfo, uint64, GetPlayerId ) >,
hashed_non_unique< tag<TAId>,   BOOST_MULTI_INDEX_CONST_MEM_FUN( AccountRoleInfo, uint64, GetAccountId )>
>
> MutiAccountRoles;

// µü´úÆ÷±ðÃû
typedef MutiAccountRoles::iterator MutiAccountRolesItr ;

typedef MutiAccountRoles::index<TPId>::type RolesByPlayerId;
typedef RolesByPlayerId::iterator RolesByPlayerIdItr;

typedef MutiAccountRoles::index<TAId>::type RolesByAccountId;
typedef RolesByAccountId::iterator RolesByAccountIdItr ;
typedef std::pair< RolesByAccountIdItr, RolesByAccountIdItr > RangeAccountIdRolesItr;

typedef std::tr1::tuple<uint32, uint32, std::string> ActupleId;
typedef std::map<ActupleId,AccountInfo*> AccountInfoMap;

typedef std::map<SessionPairId,AccountRoleInfo*> AccoutRoleInfoMap;
typedef std::map<SessionPairId,AccountRoleInfo*>::iterator AccoutRoleInfoMapItr;

#endif //ACCOUNTDATACONTAINER_H_

