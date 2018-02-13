#pragma once

#include "HWConfigOptions.h"
#include "codaRTNetProtocol/codartprotocol_ni6221.h"
#include <memory.h>

namespace codaRTNet
{
	class HWConfigOptionsNI6221 : public HWConfigOptions
	{
	public:
		HWConfigOptionsNI6221(::DWORD dwConfigAddress)
		{
			memset(&dev, 0, sizeof(dev));
			dev.base.dwSize = sizeof(dev);
			dev.base.dwCommand = CODANET_HWCONFIGSETOPTIONS;
			dev.base.dwConfig = dwConfigAddress;
			dev.base.wDevice = DEVICEID_NI6221;
			dev.base.wOptionID = CODANET_HWCONFIGOPTIONID_NI6221;
		}

		virtual ::DWORD buffersize() const
		{ return sizeof(dev); }

		virtual void* buffer()
		{ return &dev; }

		virtual const void* buffer() const
		{ return &dev; }

		virtual void ntoh(void* data) const
		{ codanet_hwconfig_ni6221_ntoh((CODANET_HWCONFIG_NI6221*)data); }

		virtual void hton(void* data) const
		{ codanet_hwconfig_ni6221_hton((CODANET_HWCONFIG_NI6221*)data); }

	public:
		CODANET_HWCONFIG_NI6221 dev;
	};
}
