#ifndef PACKETALLOCATORH__
#define PACKETALLOCATORH__
#include "net_framework/sessionTypeDef.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class PacketAllocator
{
public:
 void FreePacketList(FLockPacketQueue& packetQue);
 void FreeAllPackets();
 NetPack* AllocOnePacket(uint16 opCode,size_t pSize);
 void FreePacket(NetPack* pack);

 private:
	SPacketSet mPackSet;
} ;

#endif
