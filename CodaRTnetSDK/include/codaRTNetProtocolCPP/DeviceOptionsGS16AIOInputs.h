#pragma once

#include "DeviceOptions.h"
#include "../codaRTNetProtocol/codartprotocol_gs16aio.h"

namespace codaRTNet
{
	/** Summary
			Use as argument to RTNetClient::setDeviceOptions to set GS16AIO analog
			input options (sample rates and decimation).

			Description
			The constructor allows sample rate and decimation information to be specified.  
			Once constructed,	use RTNetClient::setDeviceOptions to set the new rates.
		*/
	class DeviceOptionsGS16AIOInputs : public DeviceOptions
	{
	public:
		/** Underlying device options structure */
		CODANET_DEVICEOPTIONS_GS16AIO_INPUTS opt;

		/** Summary
				Construct with the specified sample rate on all channels.

				Description
				Constructed like this, all GS16AIO channels will have the same
				sample rate once this object is passed to RTNetClient::setDeviceOptions.
				To apply lower sample rates for certain channels, a decimation factor
				can be applied.  Use the opt.channel_decimation member to set individual
				channel decimation factors relative to the base rate.

				The actual base sample rate used will be the nearest whole divisor of the 
				GS16AIO hardware clock rate which is 24MHz.

				The GS16AIO device sends measurement data as ADC16 packets for receipt by the
				DataStream::receivePacket method and decoding by PacketDecodeADC16.  When
				decimation is applied, a decimated channel will be absent from all packets
				whose tick value is not a whole multiple of the decimation value.  The received
				packet tick number must be checked to determine which channels are present.

				@params fBaseRateHz The base sample rate for all channels, in Hertz.
			*/
		DeviceOptionsGS16AIOInputs(float fBaseRateHz)
		{
			opt.base.dwSize = sizeof(opt);
			opt.base.dwCommand = CODANET_SETDEVICEOPTIONS;
			opt.base.wDevice = DEVICEID_GS16AIO;
			opt.base.wOptionID = CODANET_DEVICEOPTIONID_GS16AIO_INPUTS;
			opt.fBaseRateHz = fBaseRateHz;
			for (DWORD i = 0; i < CODANET_GS16AIO_MAX_CHANNELS; i++)
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
		{ codanet_deviceoptions_gs16aio_inputs_hton((CODANET_DEVICEOPTIONS_GS16AIO_INPUTS*)data); }

		virtual void ntoh(void* data) const
		{ codanet_deviceoptions_gs16aio_inputs_ntoh((CODANET_DEVICEOPTIONS_GS16AIO_INPUTS*)data); }
	};

}
