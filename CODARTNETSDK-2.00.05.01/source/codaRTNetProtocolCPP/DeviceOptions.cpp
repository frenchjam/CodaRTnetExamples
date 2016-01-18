#include "DeviceOptions.h"
#include <memory.h>
#include <stdlib.h>

namespace codaRTNet
{
	int DeviceOptions::sendto(CODANET_SOCKET skt, CODANET_DEVICESTATUS_ARRAY* status) const
	{
		const CODANET_DEVICEOPTIONS_BASE* base = (const CODANET_DEVICEOPTIONS_BASE*)buffer();
		BYTE* sendbuf = new BYTE[buffersize()];
		memcpy(sendbuf, buffer(), buffersize());
		hton(sendbuf);
		int retval = codanet_commandstructquery(skt, status, buffersize(), sendbuf, 0UL, NULL);
		delete [] sendbuf;
		return retval;
	}
}
