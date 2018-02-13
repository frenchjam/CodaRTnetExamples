#pragma once

#include "DeviceOptions.h"
#include "../codaRTNetProtocol/codartprotocol_cx1.h"

namespace codaRTNet
{
	/** Summary
			Use as argument to RTNetClient::setDeviceOptions to set options on which
			packets are transmitted from the CX1 system.

			Description
			The constructor allows mode information to be specified.  Once constructed,
			use RTNetClient::setDeviceOptions to set the new mode.
		*/
	class DeviceOptionsCodaPacketMode : public DeviceOptions
	{
	public:
		/** Summary 
		    Underlying CX1 mode options request

				Description
				This is usually set via the DeviceOptionsCodaMode::DeviceOptionsCX1DataMode
				constructor rather than manipulated directly.
			*/
		CODANET_DEVICEOPTIONS_CODAPACKETMODE opt;

		/** Summary
		   Construct specified mode options ready to be set using
		   RTNetClient::setDeviceOptions
		   
		   Description
		   Set specified data streaming mode information. Possible values for the 
			 dwPacketMode parameter are as follows:

			 <TABLE>
		   dwRateMode                                          Description
		   ==================================================  =============
		   CODANET_CODAPACKETMODE_COMBINED_COORD							 Single packet of 3D coords combined from all units
		   CODANET_CODAPACKETMODE_SEPARATE_COORD							 Separate packets of 3D coords from each unit
		   CODANET_CODAPACKETMODE_SEPARATE_AND_COMBINED_COORD	 One packet of combined 3D coords and separate packets of 3D coords from each unit
		   </TABLE>

			 @param dwPacketMode The data mode (see table above)
		                                                                 */
		DeviceOptionsCodaPacketMode(DWORD dwPacketMode)
		{
			opt.base.dwSize = sizeof(opt);
			opt.base.dwCommand = CODANET_SETDEVICEOPTIONS;
			opt.base.wDevice = DEVICEID_CX1;
			opt.base.wOptionID = CODANET_DEVICEOPTIONID_CODAPACKETMODE;
			opt.dwPacketMode = dwPacketMode;
		}

	protected:
		virtual ::DWORD buffersize() const
		{ return (::DWORD)sizeof(opt); }

		virtual void* buffer()
		{ return &opt; }

		virtual const void* buffer() const
		{ return &opt; }

		virtual void hton(void* data) const
		{ codanet_deviceoptions_codapacketmode_hton((CODANET_DEVICEOPTIONS_CODAPACKETMODE*)data); }

		virtual void ntoh(void* data) const
		{ codanet_deviceoptions_codapacketmode_ntoh((CODANET_DEVICEOPTIONS_CODAPACKETMODE*)data); }
	};

}
