#pragma once

#include "DeviceInfo.h"
#include "codaRTNetProtocol/codartprotocol_DI720USB.h"

namespace codaRTNet
{
	/** Summary
	    Use with RTNetClient::getDeviceInfo to retrieve information about DI720USB analog interface.

			Description
			On successful completion of RTNetClient::getDeviceInfo, the dev member will be filled
			with information about the DI720USB device.
		*/
	class DeviceInfoDI720USBInputs : public DeviceInfo
	{
	public:
		/** Summary
				Construct DI720USB information request ready to be filled by call to RTNetClient::getDeviceInfo 
			*/
		DeviceInfoDI720USBInputs()
		{
			memset(&dev, 0, sizeof(dev));
			dev.base.wDevice = DEVICEID_DI720USB;
			dev.base.wInfoID = CODANET_DEVICEINFOID_DI720USB_INPUTS;
		}

		virtual ::DWORD buffersize() const
		{ return sizeof(dev); }

		virtual void* buffer()
		{ return &dev; }

		virtual const void* buffer() const
		{ return &dev; }

		virtual void ntoh(void* data) const
		{ codanet_deviceinfo_DI720USB_inputs_ntoh((CODANET_DEVICEINFO_DI720USB_INPUTS*)data); }

		virtual void hton(void* data) const
		{ codanet_deviceinfo_DI720USB_inputs_hton((CODANET_DEVICEINFO_DI720USB_INPUTS*)data); }

	public:
		/** Device information retrieved */
		CODANET_DEVICEINFO_DI720USB_INPUTS dev;
	};
}
