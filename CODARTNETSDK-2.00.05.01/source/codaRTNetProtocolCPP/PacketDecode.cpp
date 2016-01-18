#include "PacketDecode.h"
#include "codaRTNetProtocol/codartclient.h"

namespace codaRTNet
{
	void PacketDecode::clear()
	{
		if (data != NULL)
		{
			delete [] data;
			data = NULL;
			datasize = 0;
		}
	}

	bool PacketDecode::canDecodePacket(const RTNetworkPacket& packet) const
	{
		CODANET_PACKET_HEADER testheader;
		packet.readHeader(&testheader);
		return canDecode(testheader.wPacketID);
	}

	bool PacketDecode::decode(const RTNetworkPacket& packet)
	{
		clear();
		packet.readHeader(&header);
		if (!canDecode(header.wPacketID))
			return false;
		datasize = packet.getDataSize();
		data = new BYTE[datasize];
		if (!decodeData(packet.getData()))
		{
			clear();
			return false;
		}
		return true;
	}
}
