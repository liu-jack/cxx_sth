#ifndef PLAYERINDEXTABLE_CONTAINER_H__
#define PLAYERINDEXTABLE_CONTAINER_H__

#include "../object/Player.h"
#include "boost/smart_ptr/shared_ptr.hpp"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>

typedef boost::shared_ptr<Player> PlayerPtr;
struct MPI {} ;
struct MAI {} ;
struct MFI {} ;
struct MCI {} ;
struct MOL {} ;
struct MCY {} ;
namespace bm =boost::multi_index ;
struct OPI:public bm::hashed_unique< bm::tag<MPI>,  BOOST_MULTI_INDEX_CONST_MEM_FUN( Player, uint64,  GetGuid) >
{
};
struct OAI:public bm::hashed_non_unique< bm::tag<MAI>,   BOOST_MULTI_INDEX_CONST_MEM_FUN( Player, uint64, AccountId ) >
{
};
struct OFI:public bm::ordered_non_unique< bm::tag<MFI>,   BOOST_MULTI_INDEX_CONST_MEM_FUN( Player, uint32, GetPlatformId ) >
{
};
struct OCI:public bm::ordered_non_unique< bm::tag<MCI>,   BOOST_MULTI_INDEX_CONST_MEM_FUN( Player, uint32, GetChannelId ) >
{
};

struct OOL:public bm::ordered_non_unique< bm::tag<MOL>,   BOOST_MULTI_INDEX_CONST_MEM_FUN( Player, bool, IsOnline ) >
{
};
struct OCY:public bm::ordered_non_unique< bm::tag<MCY>,   BOOST_MULTI_INDEX_CONST_MEM_FUN( Player, uint32, GetCountryId ) >
{
};

typedef bm::multi_index_container<PlayerPtr, bm::indexed_by< OOL,OPI,OAI,OFI,OCI,OCY> > MultC;

 //µü´úÆ÷±ðÃû
typedef MultC::iterator PItr ;

typedef MultC::index<MPI>::type PV;
typedef PV::iterator PVItr ;

typedef MultC::index<MAI>::type AV;
typedef AV::iterator AVItr ;

typedef MultC::index<MFI>::type FV;
typedef FV::iterator FVItr ;
typedef std::pair< FVItr, FVItr > RangeFVItr;

typedef MultC::index<MCI>::type CV;
typedef CV::iterator CVItr ;
typedef std::pair< CVItr, CVItr > RangeCVItr;

typedef MultC::index<MOL>::type OV;
typedef OV::iterator OVItr ;
typedef std::pair< OVItr, OVItr > RangeOVItr;

typedef MultC::index<MCY>::type OY;
typedef OY::iterator OYItr ;
typedef std::pair< OYItr, OYItr > RangeOYItr;

#endif

