#pragma once

#include "HWConfigOptions.h"
#include "codaRTNetProtocol/codartprotocol_amtianalog.h"
#include <memory.h>

namespace codaRTNet
{
	class HWConfigOptionsAMTIAnalog : public HWConfigOptions
	{
	public:
		HWConfigOptionsAMTIAnalog(::DWORD dwConfigAddress)
		{
			memset(&dev, 0, sizeof(dev));
			dev.base.dwSize = sizeof(dev);
			dev.base.dwCommand = CODANET_HWCONFIGSETOPTIONS;
			dev.base.dwConfig = dwConfigAddress;
			dev.base.wDevice = DEVICEID_AMTIANALOG;
			dev.base.wOptionID = CODANET_HWCONFIGOPTIONID_AMTIANALOG;
		}

		virtual ::DWORD buffersize() const
		{ return sizeof(dev); }

		virtual void* buffer()
		{ return &dev; }

		virtual const void* buffer() const
		{ return &dev; }

		virtual void ntoh(void* data) const
		{ codanet_hwconfig_amtianalog_ntoh((CODANET_HWCONFIG_AMTIANALOG*)data); }

		virtual void hton(void* data) const
		{ codanet_hwconfig_amtianalog_hton((CODANET_HWCONFIG_AMTIANALOG*)data); }

	public:
		CODANET_HWCONFIG_AMTIANALOG dev;
	};
}
