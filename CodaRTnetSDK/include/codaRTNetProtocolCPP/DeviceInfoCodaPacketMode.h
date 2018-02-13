#pragma once

#include "DeviceInfo.h"
#include "codaRTNetProtocol/codartprotocol_cx1.h"

namespace codaRTNet
{
	/** Summary
	    Use with RTNetClient::getDeviceInfo to retrieve current packet settings for CODA data stream.

			Description
			On successful completion, the dev member will be filled with information about
			the current packet settings for CODA data stream.  To set these, use
			RTNetClient::setDeviceOptions with a DeviceOptionsCodaPacketMode class.

		*/
	class DeviceInfoCodaPacketMode : public DeviceInfo
	{
	public:
		/** Construct alignment request ready to be filled by call to RTNetClient::getDeviceInfo */
		DeviceInfoCodaPacketMode()
		{
			memset(&dev, 0, sizeof(dev));
			dev.base.wDevice = DEVICEID_CX1;
			dev.base.wInfoID = CODANET_DEVICEINFOID_CODAPACKETMODE;
		}

		virtual ::DWORD buffersize() const
		{ return sizeof(dev); }

		virtual void* buffer()
		{ return &dev; }

		virtual const void* buffer() const
		{ return &dev; }

		virtual void ntoh(void* data) const
		{ codanet_deviceinfo_codapacketmode_ntoh((CODANET_DEVICEINFO_CODAPACKETMODE*)data); }

		virtual void hton(void* data) const
		{ codanet_deviceinfo_codapacketmode_hton((CODANET_DEVICEINFO_CODAPACKETMODE*)data); }

	public:
		/** Device information retrieved */
		CODANET_DEVICEINFO_CODAPACKETMODE dev;
	};
}
