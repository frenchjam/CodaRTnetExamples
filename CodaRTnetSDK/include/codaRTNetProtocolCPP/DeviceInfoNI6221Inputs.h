#pragma once

#include "DeviceInfo.h"
#include "codaRTNetProtocol/codartprotocol_ni6221.h"

namespace codaRTNet
{
	/** Summary
	    Use with RTNetClient::getDeviceInfo to retrieve information about (analog) inputs
			on NI6221 interfaces.
		*/
	class DeviceInfoNI6221Inputs: public DeviceInfo
	{
	public:
		/** Summary
				Construct AMTI serial information request ready to be filled by call to RTNetClient::getDeviceInfo 
				
				@params wDevice the AMTI serial device of interest
			*/
		DeviceInfoNI6221Inputs()
		{
			memset(&dev, 0, sizeof(dev));
			dev.base.wDevice = DEVICEID_NI6221;
			dev.base.wInfoID = CODANET_DEVICEINFOID_NI6221_INPUTS;
		}

		virtual ::DWORD buffersize() const
		{ return sizeof(dev); }

		virtual void* buffer()
		{ return &dev; }

		virtual const void* buffer() const
		{ return &dev; }

		virtual void ntoh(void* data) const
		{ codanet_deviceinfo_ni6221_inputs_ntoh((CODANET_DEVICEINFO_NI6221_INPUTS*)data); }

		virtual void hton(void* data) const
		{ codanet_deviceinfo_ni6221_inputs_hton((CODANET_DEVICEINFO_NI6221_INPUTS*)data); }

	public:
		/** Device information retrieved */
		CODANET_DEVICEINFO_NI6221_INPUTS dev;
	};
}
