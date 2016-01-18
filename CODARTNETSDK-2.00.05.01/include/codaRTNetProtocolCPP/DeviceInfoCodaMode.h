#pragma once

#include "DeviceInfo.h"
#include "codaRTNetProtocol/codartprotocol_cx1.h"

namespace codaRTNet
{
	/** Summary
	    Use with RTNetClient::getDeviceInfo to retrieve current mode settings.

			Description
			On successful completion, the dev member will be filled with information about
			the current mode.  To set the mode, use
			RTNetClient::setDeviceOptions with a DeviceOptionsCodaMode class.

		*/
	class DeviceInfoCodaMode : public DeviceInfo
	{
	public:
		/** Construct alignment request ready to be filled by call to RTNetClient::getDeviceInfo */
		DeviceInfoCodaMode()
		{
			memset(&dev, 0, sizeof(dev));
			dev.base.wDevice = DEVICEID_CX1;
			dev.base.wInfoID = CODANET_DEVICEINFOID_CODAMODE;
		}

		virtual ::DWORD buffersize() const
		{ return sizeof(dev); }

		virtual void* buffer()
		{ return &dev; }

		virtual const void* buffer() const
		{ return &dev; }

		virtual void ntoh(void* data) const
		{ codanet_deviceinfo_codamode_ntoh((CODANET_DEVICEINFO_CODAMODE*)data); }

		virtual void hton(void* data) const
		{ codanet_deviceinfo_codamode_hton((CODANET_DEVICEINFO_CODAMODE*)data); }

	public:
		/** Device information retrieved */
		CODANET_DEVICEINFO_CODAMODE dev;
	};
}
