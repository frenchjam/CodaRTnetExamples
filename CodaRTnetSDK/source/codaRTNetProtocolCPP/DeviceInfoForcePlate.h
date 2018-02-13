#pragma once

#include "DeviceInfo.h"
#include "codaRTNetProtocol/codartprotocol_forceplates.h"

namespace codaRTNet
{
	/** Summary
	    Use with RTNetClient::getDeviceInfo to retrieve information about 
			force platforms attached to a device.

			Description
			There are different devices which support force platform information,
			so the device of interest must be specified in the constructor.
			On successful completion of RTNetClient::getDeviceInfo, the 
			dev member will be filled with information about force platforms
			described by the specified device.
		*/
	class DeviceInfoForcePlate : public DeviceInfo
	{
	public:
		/** Summary
				Construct force plate information request ready to be filled by call to RTNetClient::getDeviceInfo 
				
				@params wDevice the device of interest
			*/
		DeviceInfoForcePlate(::WORD wDevice)
		{
			memset(&dev, 0, sizeof(dev));
			dev.base.wDevice = wDevice;
			dev.base.wInfoID = CODANET_DEVICEINFOID_FORCEPLATE;
		}

		virtual ::DWORD buffersize() const
		{ return sizeof(dev); }

		virtual void* buffer()
		{ return &dev; }

		virtual const void* buffer() const
		{ return &dev; }

		virtual void ntoh(void* data) const
		{ codanet_deviceinfo_forceplate_ntoh((CODANET_DEVICEINFO_FORCEPLATE*)data); }

		virtual void hton(void* data) const
		{ codanet_deviceinfo_forceplate_hton((CODANET_DEVICEINFO_FORCEPLATE*)data); }

	public:
		/** Device information retrieved */
		CODANET_DEVICEINFO_FORCEPLATE dev;
	};
}
