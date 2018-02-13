#pragma once

#include "DeviceOptions.h"
#include "codaRTNetProtocol/codartprotocol_cx1.h"

namespace codaRTNet
{
	/** Summary
			Use as argument to RTNetClient::setDeviceOptions to turn optical strobe on and off.

			Description
			The constructor allows the optical strobe settings to be specified.  Once constructed,
			use RTNetClient::setDeviceOptions to set the new strobe option.  This should only be
			done whilst the system is in a stopped state.
		*/
	class DeviceOptionsCodaStrobeOutput : public DeviceOptions
	{
	public:
		/** Summary 
		    Underlying CX1 mode options request

				Description
				This is usually set via the DeviceOptionsCodaMode::DeviceOptionsCodaStrobe
				constructor rather than manipulated directly.
			*/
		CODANET_DEVICEOPTIONS_STROBEOUTPUT opt;

		/** Summary
		    Construct specified optical strobe options ready to be set using
		    RTNetClient::setDeviceOptions
		   
			  @param dwStrobeOn Non-zero to enable strobe, zero to disable.
		                                                                 */
		DeviceOptionsCodaStrobeOutput(DWORD dwStrobeOutput)
		{
			opt.base.dwSize = sizeof(opt);
			opt.base.dwCommand = CODANET_SETDEVICEOPTIONS;
			opt.base.wDevice = DEVICEID_CX1;
			opt.base.wOptionID = CODANET_DEVICEOPTIONID_STROBEOUTPUT;
			opt.dwStrobeOutput = dwStrobeOutput;
		}

	protected:
		virtual ::DWORD buffersize() const
		{ return (::DWORD)sizeof(opt); }

		virtual void* buffer()
		{ return &opt; }

		virtual const void* buffer() const
		{ return &opt; }

		virtual void hton(void* data) const
		{ codanet_deviceoptions_strobeoutput_hton((CODANET_DEVICEOPTIONS_STROBEOUTPUT*)data); }

		virtual void ntoh(void* data) const
		{ codanet_deviceoptions_strobeoutput_ntoh((CODANET_DEVICEOPTIONS_STROBEOUTPUT*)data); }
	};

}
