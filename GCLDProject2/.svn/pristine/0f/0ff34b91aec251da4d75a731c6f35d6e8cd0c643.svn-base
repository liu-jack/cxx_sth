#include "FormulaInterface.h"
#include "GameServer.h"
#include "GameServerLogicalTaker.h"
#include "boost/numeric/conversion/cast.hpp"

float FormulaInterface::Call( const char* funName, float p1 )
{
    vector<float> params;
    float result;
    params.push_back(  p1);
    sGameSvr.GetGSLogicalTaker()->CallLuaFormula(funName, result, params);
    return result;
}

float FormulaInterface::Call( const char* funName, float p1, float p2 )
{
    vector<float> params;
    float result;
    params.push_back( p1 );
    params.push_back( p2 );
    sGameSvr.GetGSLogicalTaker()->CallLuaFormula(funName, result, params);
    return result;
}

float FormulaInterface::Call( const char* funName, float p1, float p2, float p3 )
{
    vector<float> params;
    float result;
    params.push_back(  p1 );
    params.push_back(  p2 );
    params.push_back(  p3 );
    sGameSvr.GetGSLogicalTaker()->CallLuaFormula(funName, result, params);
    return result;
}
