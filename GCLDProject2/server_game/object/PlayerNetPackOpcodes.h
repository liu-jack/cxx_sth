
HandleOpcode( CMSG_PING);

HandleOpcode( CMSG_PLAYER_CHANGE_NAME);
HandleOpcode( CMSG_SET_GUIDE_PROCESS);
HandleOpcode( CMSG_SET_NAME_AND_HEADID);
HandleOpcode( CMSG_CONGRATULATE_ACCEPT);
HandleOpcode( CMSG_CONGRATULATE_CURRENT_CONGRATULATION);
HandleOpcode( CMSG_LOTTERY_DAILY);
////////Wanna be stronger
HandleOpcode( CMSG_WANNA_BE_STRONGER_ALL_INFO);
HandleOpcode( CMSG_WANNA_BE_STRONGER_TAKE_REWARD);
///////////////
#ifdef _MMO_SERVER_
// ткс╙
HandleOpcode( CMSG_START_AUTO_RECOVER_HP);
HandleOpcode( CMSG_ACTIIVTY_START_ALL_INFO);
#endif

#ifdef _MMO_SERVER_
#include "../fuben/FubenHandler.h"
#include "../Market/PlayerMarketHandler.h"
#include "../Altar/PlayerAltarHandler.h"
#include "../Activity/PlayerActHandler.h"
#include "../Ironsmith/PlayerIronsmithHandler.h"
#include "../Dungeon/PlayerDungeonHandler.h"
#include "../sell/SellNetPackHandler.h"
#include "../Mail/MailPacket_Handler.h"
#include "../Lottery/LotteryNetPackHandler.h"
#include "../pay/PayHandler.h"
#include "../CrossWar/CrossWarHandler.h"
#include "../TeachMap/TeachMapHandler.h"
#endif
#include "../AutoCountryWar/AutoCountryWarHandler.h"

#include "../chat/ChatPacket_Handler.h"
#include "../BaoQi/PlayerBaoQiHandler.h"
#include "../character/CharacterHandler.h"
#include "../Palace/PlayerPalaceHandler.h"
#include "../Combat/CombatHandler.h"
#include "../Map/country/CountryHandler.h"
#include "../Map/city/CityHandler.h"
#include "../Map/PlayerMapLogicHandler.h"
#include "../Technology/PlayerTechnologyHandler.h"
#include "../item/ItemHandler.h"
#include "../quest/QuestHandler.h"
//#include "../Social/SocialPacket_Handler.h"
//#include "../Guild/GuildPacket_Handler.h"
#include "../Building/BuildingHandler.h"
#include "../Exploit/ExploitHandle.h"
#include "../DailyQuest/DailyQuestHandler.h"
#include "../GrowUpQuest/GrowUpQuestHandler.h"
#include "../SignUp/SignUpHandler.h"
#include "../PassStageActivity/ActivtyStageHandler.h"
#include "../seige_force/SeigeForceHandler.h"


#ifdef _MMO_SERVER_
#include "../ActivityXiLian/ActivityXilianHandler.h"
#include "../GovernmentAffairs/GovAffairsHandler.h"
#include "../ActivityCelebrity/ActivityCelebrityHandler.h"
#include "../ActivityLimitedTimeShop/ActivityLimitedTimeShopHandler.h"
#include "../ActivityStarBox/ActivityStarBoxHandler.h"
#include "../ActivityHeroReward/ActivityHeroRewardHandler.h"
#include "../ActivityMoneyToGold/ActivityMoneyToGoldHandler.h"
#include "../WorldFightAchievement/WorldFightAchievementHandler.h"
#include "../ContinueOccupyCity/ContinueOccupyCityHandler.h"
#include "../ActivityRank/ActivityRankHandler.h"
#endif

#ifdef _SERVER_CROSS_
#include "../../server_cross/cross_wall/PlayerCrossWarLogicHandler.h"
#endif
