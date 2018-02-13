#pragma once

#include "DeviceOptions.h"
#include "codaRTNetProtocol/codartprotocol_forceplates.h"

namespace codaRTNet
{
	/** Summary
	    Use with RTNetClient::setDeviceInfo to pass force plate channel offset 
			calibration information to the device on codaRTnet.			
		*/
	class DeviceOptionsForceplateChannelOffset: public DeviceOptions
	{
	public:
		/** Underlying device options structure */
		CODANET_DEVICEOPTIONS_FORCEPLATE_CHANNELOFFSET opt;

		/** Summary
				Construct for specified plate, specified device, 


				@param wDevice Device ID
				@param dwPlateIndex zero-based index of plate on specified device
				@param dwNumChannels the number of channels to set
				@param offset the offset data (float array of dimension dwNumChannels)
			*/
		DeviceOptionsForceplateChannelOffset(WORD wDevice, DWORD dwPlateIndex, DWORD dwNumChannels, const float* offsets, const float* offsetstdev)
		{
			opt.base.dwSize = sizeof(opt);
			opt.base.dwCommand = CODANET_SETDEVICEOPTIONS;
			opt.base.wDevice = wDevice;
			opt.base.wOptionID = CODANET_DEVICEOPTIONID_FORCEPLATE_CHANNELOFFSET;
			opt.dwPlateIndex = dwPlateIndex;
			for (DWORD ichannel = 0; ichannel < dwNumChannels; ichannel++)
			{
				opt.offsets[ichannel] = offsets[ichannel];
				opt.offsetstdev[ichannel] = offsetstdev[ichannel];
			}
		}

	protected:

		virtual ::DWORD buffersize() const
		{ return sizeof(opt); }

		virtual void* buffer()
		{ return &opt; }

		virtual const void* buffer() const
		{ return &opt; }

		virtual void ntoh(void* data) const
		{ codanet_deviceoptions_forceplate_channeloffset_ntoh((CODANET_DEVICEOPTIONS_FORCEPLATE_CHANNELOFFSET*)data); }

		virtual void hton(void* data) const
		{ codanet_deviceoptions_forceplate_channeloffset_hton((CODANET_DEVICEOPTIONS_FORCEPLATE_CHANNELOFFSET*)data); }
		  
	public:

	};
}
