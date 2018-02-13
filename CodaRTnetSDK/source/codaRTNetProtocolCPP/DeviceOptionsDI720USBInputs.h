#pragma once

#include "DeviceOptions.h"
#include "codaRTNetProtocol/codartprotocol_DI720USB.h"

namespace codaRTNet
{
	/** Summary
			Use as argument to RTNetClient::setDeviceOptions to set DI720USB analog
			input options (sample rates and decimation).

			Description
			The constructor allows sample rate and decimation information to be specified.  
			Once constructed,	use RTNetClient::setDeviceOptions to set the new rates.
		*/
	class DeviceOptionsDI720USBInputs : public DeviceOptions
	{
	public:
		/** Underlying device options structure */
		CODANET_DEVICEOPTIONS_DI720USB_INPUTS opt;

		/** Summary
				Construct with the specified sample rate on all channels.

				Description
				Constructed like this, all DI720USB channels will have the same
				sample rate once this object is passed to RTNetClient::setDeviceOptions.
				To apply lower sample rates for certain channels, a decimation factor
				can be applied.  Use the opt.channel_decimation member to set individual
				channel decimation factors relative to the base rate.

				The actual base sample rate used will be the nearest whole divisor of the 
				DI720USB hardware clock rate which is 24MHz.

				The DI720USB device sends measurement data as ADC16 packets for receipt by the
				DataStream::receivePacket method and decoding by PacketDecodeADC16.  When
				decimation is applied, a decimated channel will be absent from all packets
				whose tick value is not a whole multiple of the decimation value.  The received
				packet tick number must be checked to determine which channels are present.

				@params fBaseRateHz The base sample rate for all channels, in Hertz.
			*/
		DeviceOptionsDI720USBInputs(float fBaseRateHz)
		{
			opt.base.dwSize = sizeof(opt);
			opt.base.dwCommand = CODANET_SETDEVICEOPTIONS;
			opt.base.wDevice = DEVICEID_DI720USB;
			opt.base.wOptionID = CODANET_DEVICEOPTIONID_DI720USB_INPUTS;
			opt.fBaseRateHz = fBaseRateHz;
			for (DWORD i = 0; i < CODANET_DI720USB_MAX_CHANNELS; i++)
				opt.channel_decimation[i] = 1;
		}

	protected:
		virtual ::DWORD buffersize() const
		{ return (::DWORD)sizeof(opt); }

		virtual void* buffer()
		{ return &opt; }

		virtual const void* buffer() const
		{ return &opt; }

		virtual void hton(void* data) const
		{ codanet_deviceoptions_DI720USB_inputs_hton((CODANET_DEVICEOPTIONS_DI720USB_INPUTS*)data); }

		virtual void ntoh(void* data) const
		{ codanet_deviceoptions_DI720USB_inputs_ntoh((CODANET_DEVICEOPTIONS_DI720USB_INPUTS*)data); }
	};

}
