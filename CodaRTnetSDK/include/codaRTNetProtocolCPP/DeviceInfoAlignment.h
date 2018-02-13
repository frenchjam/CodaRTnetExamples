
#pragma once

#include "DeviceInfo.h"
#include "../codaRTNetProtocol/codartprotocol_cx1.h"

namespace codaRTNet
{
	/** Summary
	    Use with RTNetClient::getDeviceInfo to retrieve diagnostic information about most recent 
			Codamotion CX1 system alignment.

			Description
			On successful completion, the dev member will be filled with information about
			the most recent system alignment.  To perform system alignments, use
			RTNetClient::setDeviceOptions with a DeviceOptionsAlignment class.

		*/
	class DeviceInfoAlignment : public DeviceInfo
	{
	public:
		/** Construct alignment request ready to be filled by call to RTNetClient::getDeviceInfo */
		DeviceInfoAlignment()
		{
			memset(&dev, 0, sizeof(dev));
			dev.base.wDevice = DEVICEID_CX1;
			dev.base.wInfoID = CODANET_DEVICEINFOID_ALIGNMENT;
		}

		virtual ::DWORD buffersize() const
		{ return sizeof(dev); }

		virtual void* buffer()
		{ return &dev; }

		virtual const void* buffer() const
		{ return &dev; }

		virtual void ntoh(void* data) const
		{ codanet_deviceinfo_alignment_ntoh((CODANET_DEVICEINFO_ALIGNMENT*)data); }

		virtual void hton(void* data) const
		{ codanet_deviceinfo_alignment_hton((CODANET_DEVICEINFO_ALIGNMENT*)data); }

	public:
		/** Device information retrieved */
		CODANET_DEVICEINFO_ALIGNMENT dev;
	};
}
