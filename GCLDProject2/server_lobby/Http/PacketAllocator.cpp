#include "PacketAllocator.h"
#include "memory_buffer/NetPack.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PacketAllocator::FreePacketList( FLockPacketQueue& packetQue )
{
	DeleteAllPtrInLFQueue(packetQue);
}

void PacketAllocator::FreeAllPackets()
{
	SPacketSet::iterator setIter = mPackSet.begin();
	for (;setIter != mPackSet.end(); ++setIter)
	{
		NetPack* curPack = (*setIter);
		delete curPack;
		curPack = NULL;
	}
}

NetPack* PacketAllocator::AllocOnePacket(uint16 opCode,size_t pSize)
{
	NetPack* curPack = new NetPack(opCode,pSize);
	mPackSet.insert(curPack);
	return curPack;
}

void PacketAllocator::FreePacket( NetPack* pack)
{
	SPacketSet::iterator setIter = mPackSet.find(pack);
	if (setIter != mPackSet.end())
	{
		NetPack* curPack = (*setIter);
		delete curPack;
		curPack = NULL;
	}
}
