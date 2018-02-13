#pragma once

#include "DeviceInfo.h"
#include "../codaRTNetProtocol/codartprotocol_cx1.h"

namespace codaRTNet
{
	/** Summary
	    Use with RTNetClient::getDeviceInfo to retrieve current CX1 unit coordinate systems.

			Description
			On successful completion, the dev member will be filled with coordinate system information,
			representing the transform from local unit coordinates to global room coordinates.  This
			is the information obtained during alignment.
		*/
	class DeviceInfoUnitCoordSystem : public DeviceInfo
	{
	public:
		/** Construct alignment request ready to be filled by call to RTNetClient::getDeviceInfo */
		DeviceInfoUnitCoordSystem()
		{
			memset(&dev, 0, sizeof(dev));
			dev.base.wDevice = DEVICEID_CX1;
			dev.base.wInfoID = CODANET_DEVICEINFOID_UNITCOORDSYSTEM;
		}

		virtual ::DWORD buffersize() const
		{ return sizeof(dev); }

		virtual void* buffer()
		{ return &dev; }

		virtual const void* buffer() const
		{ return &dev; }

		virtual void ntoh(void* data) const
		{ codanet_deviceinfo_unitcoordsystem_ntoh((CODANET_DEVICEINFO_UNITCOORDSYSTEM*)data); }

		virtual void hton(void* data) const
		{ codanet_deviceinfo_unitcoordsystem_hton((CODANET_DEVICEINFO_UNITCOORDSYSTEM*)data); }

	public:
		/** Device information retrieved */
		CODANET_DEVICEINFO_UNITCOORDSYSTEM dev;
	};
}
