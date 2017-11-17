
//#ifndef PAY_HANDLER_H__
//#define PAY_HANDLER_H__

HandleOpcode(CMSG_TAKE_FIRST_PAY_REWARD);
HandleOpcode(CMSG_TAKE_VIP_REWARD);
#ifdef _MMO_SERVER_
HandleOpcode(CMSG_TAKE_MONTH_CARD_REWARD);
#endif
//#endif

