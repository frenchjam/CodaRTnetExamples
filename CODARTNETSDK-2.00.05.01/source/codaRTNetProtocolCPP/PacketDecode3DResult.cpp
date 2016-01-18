#include "codaRTNetProtocol/codartclient.h"
#include "PacketDecode3DResult.h"

namespace codaRTNet
{
	bool PacketDecode3DResult::decodeData(const BYTE* input)
	{
		DWORD numpoints = getNumMarkers();
		BYTE* iter_data = data;
		const BYTE* iter_input = input;
		for (DWORD ipoint = 0; ipoint < numpoints; ipoint++)
		{
			*(DWORD*)iter_data = codanet_ntohl(*(DWORD*)iter_input);
			iter_data += 4; iter_input += 4;
			*(DWORD*)iter_data = codanet_ntohl(*(DWORD*)iter_input);
			iter_data += 4; iter_input += 4;
			*(DWORD*)iter_data = codanet_ntohl(*(DWORD*)iter_input);
			iter_data += 4; iter_input += 4;
			*iter_data = *iter_input;
			iter_data++; iter_input++;
		}
		return true;
	}
}
