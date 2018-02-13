#pragma once

#include "DeviceOptions.h"
#include "codaRTNetProtocol/codartprotocol_forceplates.h"

namespace codaRTNet
{
	/** Summary
	    Use with RTNetClient::setDeviceInfo to set force plate rotation and translation 
			matrices

			Description
			In case more than one type of digital interface is available, the device number must
			be specified in the constructor.  On successful completion of RTNetClient::setDeviceInfo,
			the dev and program will use spplied infomation to do force calculations.
		*/
	class DeviceOptionsForcePlateLocation: public DeviceOptions
	{
	public:
		/** Underlying device options structure */
		CODANET_DEVICEOPTIONS_FORCEPLATE_LOCATION opt;

		/** Summary
				Construct with the specified rotation and translation on all forceplate.

				Description
				All forceplate will have their calibration and translation infomation 
				pass to the codaRTNet. The order of R matrix elements is first row first,
				second row second, and third row third.
				
				@params R 9 doubles and t 3 doubles.
			*/
		DeviceOptionsForcePlateLocation(WORD dwDevice, DWORD dwPlateIndex, const double* R, const double* t)
		{
			opt.base.dwSize = sizeof(opt);
			opt.base.dwCommand = CODANET_SETDEVICEOPTIONS;
			opt.base.wDevice = dwDevice;
			opt.base.wOptionID = CODANET_DEVICEOPTIONID_FORCEPLATE_LOCATION;
			opt.dwPlateIndex = dwPlateIndex;
			DWORD i;
			for (i = 0; i < 9; i++)
				opt.R[i] = R[i];
			for (i = 0; i < 3; i++)
				opt.t[i] = t[i];
		}

	protected:

		virtual ::DWORD buffersize() const
		{ return sizeof(opt); }

		virtual void* buffer()
		{ return &opt; }

		virtual const void* buffer() const
		{ return &opt; }

		virtual void ntoh(void* data) const
		{ codanet_deviceoptions_forceplate_location_ntoh((CODANET_DEVICEOPTIONS_FORCEPLATE_LOCATION*)data); }

		virtual void hton(void* data) const
		{ codanet_deviceoptions_forceplate_location_hton((CODANET_DEVICEOPTIONS_FORCEPLATE_LOCATION*)data); }
		  
	public:
	
	};
}
