#pragma once

#include "DeviceOptions.h"
#include "codaRTNetProtocol/codartprotocol_amtiserial.h"

namespace codaRTNet
{
	/** Summary
	    Use with RTNetClient::getDeviceInfo to retrieve information about AMTI serial force platform
			interface

			Description
			In case more than one type of digital interface is available, the device number must
			be specified in the constructor.  On successful completion of RTNetClient::getDeviceInfo,
			the dev member will be filled with information about the specified AMTI serial device.
		*/
	class DeviceOptionsAMTISerial: public DeviceOptions
	{
	public:
		/** Underlying device options structure */
		CODANET_DEVICEOPTIONS_AMTISERIAL_ADC opt;

		/** Summary
				Construct with the specified sample rate on all forceplates

				Description
				Constructed like this, all forceplates will have the port number and sample rate.
				
				@params fBaseRateHz The base sample rate for all channels, in Hertz.
			*/
		DeviceOptionsAMTISerial(DWORD samplerate, DWORD filterno)
		{
			opt.base.dwSize = sizeof(opt);
			opt.base.dwCommand = CODANET_SETDEVICEOPTIONS;
			opt.base.wDevice = DEVICEID_AMTISERIAL;
			opt.base.wOptionID = CODANET_DEVICEOPTIONID_AMTISERIAL_ADC;
			opt.dwSampleRate = samplerate;
			opt.dwFilterNo = filterno;
		}

	protected:

		virtual ::DWORD buffersize() const
		{ return sizeof(opt); }

		virtual void* buffer()
		{ return &opt; }

		virtual const void* buffer() const
		{ return &opt; }

		virtual void ntoh(void* data) const
		{ codanet_deviceoptions_amtiserial_adc_ntoh((CODANET_DEVICEOPTIONS_AMTISERIAL_ADC*)data); }

		virtual void hton(void* data) const
		{ codanet_deviceoptions_amtiserial_adc_hton((CODANET_DEVICEOPTIONS_AMTISERIAL_ADC*)data); }
		  
	};
}
