#include "RTNetworkPacket.h"
#include <memory.h>

namespace codaRTNet
{
	RTNetworkPacket::RTNetworkPacket(WORD wDeviceID, WORD wPacketID, DWORD dwTick, DWORD dwPage, DWORD dwDataSize)
	{
		codanet_packet_create(this, wDeviceID, wPacketID, dwTick, dwPage, dwDataSize);
	}

	RTNetworkPacket::RTNetworkPacket()
	{
	  dwTransmitSize = 0UL;
	  data = NULL;
	}

	RTNetworkPacket::~RTNetworkPacket()
	{
		struct CODANET_PACKET* base = dynamic_cast<struct CODANET_PACKET*> (this);
		codanet_packet_free(base);
	}

	bool RTNetworkPacket::readHeader(CODANET_PACKET_HEADER* header) const
	{
		return (codanet_packet_readheader(this, header) == 0);
	}

	void RTNetworkPacket::computeCheckSum()
	{
		codanet_packet_checksum_set(this);
	}

	bool RTNetworkPacket::verifyCheckSum() const
	{
		DWORD dwDiff = codanet_packet_checksum_diff(this);
		return (dwDiff == 0);
	}

	DWORD RTNetworkPacket::getDataSize() const
	{
		return codanet_packet_datasize(this);
	}

	int RTNetworkPacket::sendTo(CODANET_SOCKET sktUDP, const sockaddr_in* addr) const
	{
		return codanet_packet_send(this, sktUDP, addr);
	}

	int RTNetworkPacket::recvFrom(CODANET_SOCKET sktUDP, DWORD timeoutusec)
	{
		return codanet_packet_receive(this, sktUDP, timeoutusec);
	}
}
