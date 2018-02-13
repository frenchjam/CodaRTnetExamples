#include "codaRTNetProtocol/codartclient.h"
#include "PacketDecodeADC16.h"

namespace codaRTNet
{
	bool PacketDecodeADC16::decodeData(const BYTE* input)
	{
		DWORD numvalues = datasize / sizeof(short);
		for (DWORD i = 0; i < numvalues; i++)
			((short*)data)[i] = codanet_ntohs(((short*)input)[i]);
		return true;
	}
}
