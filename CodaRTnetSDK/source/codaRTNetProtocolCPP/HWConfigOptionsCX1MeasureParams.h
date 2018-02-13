#pragma once

#include "HWConfigOptions.h"
#include "codaRTNetProtocol/codartprotocol_cx1.h"
#include <memory.h>

namespace codaRTNet
{
	class HWConfigOptionsCX1MeasureParams : public HWConfigOptions
	{
	public:
		HWConfigOptionsCX1MeasureParams(::DWORD dwConfigAddress)
		{
			memset(&dev, 0, sizeof(dev));
			dev.base.dwSize = sizeof(dev);
			dev.base.dwCommand = CODANET_HWCONFIGSETOPTIONS;
			dev.base.dwConfig = dwConfigAddress;
			dev.base.wDevice = DEVICEID_CX1;
			dev.base.wOptionID = CODANET_HWCONFIGOPTIONID_CX1MEASUREPARAMS;
		}

		virtual ::DWORD buffersize() const
		{ return sizeof(dev); }

		virtual void* buffer()
		{ return &dev; }

		virtual const void* buffer() const
		{ return &dev; }

		virtual void ntoh(void* data) const
		{ codanet_hwconfig_cx1measureparams_ntoh((CODANET_HWCONFIG_CX1MEASUREPARAMS*)data); }

		virtual void hton(void* data) const
		{ codanet_hwconfig_cx1measureparams_hton((CODANET_HWCONFIG_CX1MEASUREPARAMS*)data); }

	public:
		CODANET_HWCONFIG_CX1MEASUREPARAMS dev;
	};
}
