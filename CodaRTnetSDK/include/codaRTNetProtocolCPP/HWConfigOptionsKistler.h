#pragma once

#include "HWConfigOptions.h"
#include "codaRTNetProtocol/codartprotocol_kistler.h"
#include <memory.h>

namespace codaRTNet
{
	class HWConfigOptionsKistler : public HWConfigOptions
	{
	public:
		HWConfigOptionsKistler(::DWORD dwConfigAddress)
		{
			memset(&dev, 0, sizeof(dev));
			dev.base.dwSize = sizeof(dev);
			dev.base.dwCommand = CODANET_HWCONFIGSETOPTIONS;
			dev.base.dwConfig = dwConfigAddress;
			dev.base.wDevice = DEVICEID_KISTLER;
			dev.base.wOptionID = CODANET_HWCONFIGOPTIONID_KISTLER;
		}

		virtual ::DWORD buffersize() const
		{ return sizeof(dev); }

		virtual void* buffer()
		{ return &dev; }

		virtual const void* buffer() const
		{ return &dev; }

		virtual void ntoh(void* data) const
		{ codanet_hwconfig_kistler_ntoh((CODANET_HWCONFIG_KISTLER*)data); }

		virtual void hton(void* data) const
		{ codanet_hwconfig_kistler_hton((CODANET_HWCONFIG_KISTLER*)data); }

	public:
		CODANET_HWCONFIG_KISTLER dev;
	};
}
