#include "DeviceInfo.h"
#include "codaRTNetProtocol/codartclient.h"

namespace codaRTNet
{
	int DeviceInfo::readfrom(CODANET_SOCKET skt, CODANET_DEVICESTATUS_ARRAY* status)
	{
		const CODANET_DEVICEINFO_BASE* base = (const CODANET_DEVICEINFO_BASE*)buffer();
		struct CODANET_DEVICEINFO_REQUEST req;
		req.dwSize = codanet_htonl( sizeof(req) );
		req.dwCommand = codanet_htonl( CODANET_GETDEVICEINFO );
		req.wDevice = codanet_htons( base->wDevice );
		req.wInfoID = codanet_htons( base->wInfoID );
		int resultcode = codanet_commandstructquery(skt, status, sizeof(req), &req, buffersize(), buffer());
		ntoh(buffer());

		return resultcode;
	}

	void DeviceInfo::serialise(::BYTE* info) const
	{
		// get pointer to new buffer
		void* netbuffer = info;

		// copy current buffer
		memcpy(netbuffer, buffer(), buffersize());

		// convert to network byte order
		hton(netbuffer);
	}

}
