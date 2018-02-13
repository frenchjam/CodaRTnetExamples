#include "DeviceInfo.h"
#include "codaRTNetProtocol/codartprotocol_telemyo.h"

namespace codaRTNet {

class DeviceInfoTeleMyoInputs : public DeviceInfo
{

	public:

		/** Summary
			Construct TeleMyo information request ready to be filled by call to RTNetClient::getDeviceInfo 
			*/
		DeviceInfoTeleMyoInputs()
		{
			memset(&dev, 0, sizeof(dev));
			dev.base.wDevice = DEVICEID_TELEMYO;
			dev.base.wInfoID = CODANET_DEVICEINFOID_TELEMYO_INPUTS;
		}

		virtual ::DWORD buffersize() const
		{ return sizeof(dev); }

		virtual void* buffer()
		{ return &dev; }

		virtual const void* buffer() const
		{ return &dev; }

		virtual void ntoh(void* data) const
		{ codanet_deviceinfo_telemyo_inputs_ntoh((CODANET_DEVICEINFO_TELEMYO_INPUTS*)data); }

		virtual void hton(void* data) const
		{ codanet_deviceinfo_telemyo_inputs_hton((CODANET_DEVICEINFO_TELEMYO_INPUTS*)data); }

		/** Device information retrieved */
		CODANET_DEVICEINFO_TELEMYO_INPUTS dev;

	};

}

