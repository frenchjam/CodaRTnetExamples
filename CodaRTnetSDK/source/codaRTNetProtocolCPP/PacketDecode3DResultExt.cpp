#include "codaRTNetProtocol/codartclient.h"
#include "PacketDecode3DResultExt.h"

namespace codaRTNet
{
	bool PacketDecode3DResultExt::decodeData(const BYTE* input)
	{
		WORD npoints = *(WORD*)data = codanet_ntohs( *(WORD*)input );
		BYTE ncameras = data[2] = input[2];
		BYTE nresiduals = data[3] = input[3];

		// limit points to valid within data
		WORD maxpoints = (WORD)((datasize-4) / (13 + ncameras + 2*nresiduals));
		if (maxpoints < npoints)
			npoints = maxpoints;

		BYTE* iter_data = data + 4;
		const BYTE* iter_input = input + 4;
		for (WORD ipoint = 0; ipoint < npoints; ipoint++)
		{
			// 4 * float position
			*(DWORD*)iter_data = codanet_ntohl(*(DWORD*)iter_input);
			iter_data += 4; iter_input += 4;
			*(DWORD*)iter_data = codanet_ntohl(*(DWORD*)iter_input);
			iter_data += 4; iter_input += 4;
			*(DWORD*)iter_data = codanet_ntohl(*(DWORD*)iter_input);
			iter_data += 4; iter_input += 4;
			
			// byte validity
			*iter_data = *iter_input;
			iter_data++; iter_input++;

			// ncameras intensity
			for (BYTE icam = 0; icam < ncameras; icam++)
			{
				*iter_data = *iter_input;
				iter_data++; iter_input++;
			}

			// nresiduals residuals
			for (BYTE ires = 0; ires < nresiduals; ires++)
			{
				*(WORD*)iter_data = codanet_ntohs(*(WORD*)iter_input);
				iter_data += 2; iter_input += 2;
			}
		}
		return true;
	}
}
