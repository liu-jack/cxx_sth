#ifndef CURLRequests_h__
#define CURLRequests_h__

#include "def/TypeDef.h"
#include <bitset>

enum REQUEST_OP
{
    REQUEST_OP_PAY_ITEM_INFO = 0,
    REQUEST_OP_GLOBAL_KEY_VALUE = 1,
	REQUEST_OP_COUNT,
	REQUEST_OP_SDK_DATA_UPLOAD,
	REQUEST_OP_SDK_CHARGE,
    
};


class CURLRequests
{
    typedef void (CURLRequests::*RequestSender)() const;
public:
    CURLRequests();

    void SendCURLRequest( int op_type) ;
    bool IsNeedToSend( int op_type) const;
    void SetIsNeedToSend( int op_type, bool need = true);

public:
    void SendPayItemRequest() const;
    void SendGlobalKeyValueRequest() const;

private:
    std::bitset<REQUEST_OP_COUNT>   m_isNeedToSend;
    RequestSender                   m_requestSenders[REQUEST_OP_COUNT];
};


#endif // CURLRequests_h__
