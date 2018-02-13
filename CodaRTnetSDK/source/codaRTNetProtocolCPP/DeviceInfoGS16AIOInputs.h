#pragma once

#include "DeviceInfo.h"
#include "codaRTNetProtocol/codartprotocol_gs16aio.h"

namespace codaRTNet
{
	/** Summary
	    Use with RTNetClient::getDeviceInfo to retrieve information about GS16AIO analog interface.

			Description
			On successful completion of RTNetClient::getDeviceInfo, the dev member will be filled
			with information about the GS16AIO device.
		*/
	class DeviceInfoGS16AIOInputs : public DeviceInfo
	{
	public:
		/** Summary
				Construct GS16AIO information request ready to be filled by call to RTNetClient::getDeviceInfo 
			*/
		DeviceInfoGS16AIOInputs()
		{
			memset(&dev, 0, sizeof(dev));
			dev.base.wDevice = DEVICEID_GS16AIO;
			dev.base.wInfoID = CODANET_DEVICEINFOID_GS16AIO_INPUTS;
		}

		virtual ::DWORD buffersize() const
		{ return sizeof(dev); }

		virtual void* buffer()
		{ return &dev; }

		virtual const void* buffer() const
		{ return &dev; }

		virtual void ntoh(void* data) const
		{ codanet_deviceinfo_gs16aio_inputs_ntoh((CODANET_DEVICEINFO_GS16AIO_INPUTS*)data); }

		virtual void hton(void* data) const
		{ codanet_deviceinfo_gs16aio_inputs_hton((CODANET_DEVICEINFO_GS16AIO_INPUTS*)data); }

	public:
		/** Device information retrieved */
		CODANET_DEVICEINFO_GS16AIO_INPUTS dev;
	};
}
