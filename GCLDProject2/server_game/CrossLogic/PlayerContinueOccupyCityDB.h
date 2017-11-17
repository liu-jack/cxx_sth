/************************************************************************/
/*Add by:zhoulunhao													*/
/*Email	:zhoulunhao@hotmail.com											*/
/************************************************************************/

#ifndef INCLUDE_CONTINUE_OCCUPY_CITY_DB_H
#define INCLUDE_CONTINUE_OCCUPY_CITY_DB_H


class Player;
class ContinueOccupyCityLog;
class ContinueOccupyCityDB
{
public:
	static void SendInfoToDb(Player& player, ContinueOccupyCityLog& log);
};


#endif ////INCLUDE_CONTINUE_OCCUPY_CITY_DB_H