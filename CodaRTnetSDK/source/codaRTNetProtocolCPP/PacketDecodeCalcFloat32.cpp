#include "codaRTNetProtocol/codartclient.h"
#include "PacketDecodeCalcFloat32.h"

namespace codaRTNet
{
	bool PacketDecodeCalcFloat32::decodeData(const BYTE* input)
	{
		DWORD numvalues = datasize / sizeof(float);

		// this commented lines are very good examples of how things are done
//		DWORD numvalues = datasize / sizeof(long);
/*		int k=0;
		for (DWORD i = 0; i < numvalues; i++)
		{	long x=codanet_ntohl(*(long*) &input[k]);
			((float*)data)[i] = *(float*) &x;
			k+=4;
		}
	*/
		//for (DWORD i = 0; i < numvalues; i++)
		//{
		//	long x = codanet_ntohl(((long*)input)[i]);
		//	((float*)data)[i] = *(float*) &x;
		//}

		for (DWORD i = 0; i < numvalues; i++)
		{
			((float*)data)[i] = codanet_ntohf(((float*)input)[i]);
		}

		return true;
	}
}
