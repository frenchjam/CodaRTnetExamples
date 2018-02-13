#pragma once

#include "HWConfigOptions.h"
#include "codaRTNetProtocol/codartprotocol_video.h"
#include <memory.h>

namespace codaRTNet
{
	class HWConfigOptionsVideoInterfaceEnable : public HWConfigOptions
	{
	public:
		HWConfigOptionsVideoInterfaceEnable(::DWORD dwConfigAddress, ::WORD wDevice)
		{
			memset(&dev, 0, sizeof(dev));
			dev.base.dwSize = sizeof(dev);
			dev.base.dwCommand = CODANET_HWCONFIGSETOPTIONS;
			dev.base.dwConfig = dwConfigAddress;
			dev.base.wDevice = wDevice;
			dev.base.wOptionID = CODANET_HWCONFIGOPTIONID_VIDEO_INTERFACE_ENABLE;
		}

		virtual ::DWORD buffersize() const
		{ return sizeof(dev); }

		virtual void* buffer()
		{ return &dev; }

		virtual const void* buffer() const
		{ return &dev; }

		virtual void ntoh(void* data) const
		{ codanet_hwconfig_video_interface_enable_ntoh((CODANET_HWCONFIG_VIDEO_INTERFACE_ENABLE*)data); }

		virtual void hton(void* data) const
		{ codanet_hwconfig_video_interface_enable_hton((CODANET_HWCONFIG_VIDEO_INTERFACE_ENABLE*)data); }

	public:
		CODANET_HWCONFIG_VIDEO_INTERFACE_ENABLE dev;
	};
}
