#pragma once

#include "DeviceInfo.h"
#include "codaRTNetProtocol/codartprotocol_amtiserial.h"

namespace codaRTNet
{
	/** Summary
	    Use with RTNetClient::getDeviceInfo to retrieve information about AMTI serial force platform
			interface

			Description
			In case more than one type of digital interface is available, the device number must
			be specified in the constructor.  On successful completion of RTNetClient::getDeviceInfo,
			the dev member will be filled with information about the specified AMTI serial device.
		*/
	class DeviceInfoAMTISerial: public DeviceInfo
	{
	public:
		/** Summary
				Construct AMTI serial information request ready to be filled by call to RTNetClient::getDeviceInfo 
				
				@params wDevice the AMTI serial device of interest
			*/
		DeviceInfoAMTISerial()
		{
			memset(&dev, 0, sizeof(dev));
			dev.base.wDevice = DEVICEID_AMTISERIAL;
			dev.base.wInfoID = CODANET_DEVICEINFOID_AMTISERIAL;
		}

		virtual ::DWORD buffersize() const
		{ return sizeof(dev); }

		virtual void* buffer()
		{ return &dev; }

		virtual const void* buffer() const
		{ return &dev; }

		virtual void ntoh(void* data) const
		{ codanet_deviceinfo_amtiserial_ntoh((CODANET_DEVICEINFO_AMTISERIAL*)data); }

		virtual void hton(void* data) const
		{ codanet_deviceinfo_amtiserial_hton((CODANET_DEVICEINFO_AMTISERIAL*)data); }

	public:
		/** Device information retrieved */
		CODANET_DEVICEINFO_AMTISERIAL dev;
	};
}
