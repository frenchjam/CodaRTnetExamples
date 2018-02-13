#pragma once

#include "DeviceOptions.h"
#include "../codaRTNetProtocol/codartprotocol_cx1.h"

namespace codaRTNet
{
	/** Summary
			Use as argument to RTNetClient::setDeviceOptions to set Coda mode information (
			CX1 sample rates and decimation).

			Description
			The constructor allows mode information to be specified.  Once constructed,
			use RTNetClient::setDeviceOptions to set the new mode.
		*/
	class DeviceOptionsCodaMode : public DeviceOptions
	{
	public:
		/** Summary 
		    Underlying CX1 mode options request

				Description
				This is usually set via the DeviceOptionsCodaMode::DeviceOptionsCodaMode
				constructor rather than manipulated directly.
			*/
		CODANET_DEVICEOPTIONS_CODAMODE opt;

		/**codaRTNet::DeviceOptionsCodaMode::DeviceOptionsCodaMode@DWORD@DWORD@DWORD
		   Summary
		   Construct specified mode options ready to be set using
		   RTNetClient::setDeviceOptions
		   
		   Description
		   Set specified mode information. In Codamotion systems the
		   mode determines not only the sample rate but also the maximum
		   number of markers. Possible values for the dwRateMode
		   parameter are as follows:

			 <TABLE>
		   dwRateMode              \Sample Rate   Number of
		                                           Markers
		   ======================  =============  ==========
		   CODANET_CODA_MODE_100   100Hz          56
		   CODANET_CODA_MODE_200   200Hz          28
		   CODANET_CODA_MODE_400   400Hz          12
		   CODANET_CODA_MODE_800   800Hz          6
		   </TABLE>
		   
		   The dwDecimation value reduces the bandwidth and system load
		   by measuring only every <I>N</I>th sample period at the given
		   rate. For example a dwRateMode of CODANET_CODA_MODE_200 with
		   dwDecimation of 4 would give an actual sample rate of 200Hz /
		   4 = 50Hz.  Most Codamotion systems use battery-powered drive
			 boxes and using lower sample rates where possible will
			 preserve battery life.
		   
		   Systems can be configured to accept external synchronisation
		   signals. In this case the dwRateMode should indicate a sample
		   rate which is not less than the input signal rate.

			 @param dwRateMode The sample rate mode (see table above)
			 @param dwDecimation Decimation factor to divide sample rate down and reduce system load.
			 @param dwExternalSync Non-zero to enable external sync, zero otherwise.
		                                                                 */
		DeviceOptionsCodaMode(DWORD dwRateMode, DWORD dwDecimation, DWORD dwExternalSync)
		{
			opt.base.dwSize = sizeof(opt);
			opt.base.dwCommand = CODANET_SETDEVICEOPTIONS;
			opt.base.wDevice = DEVICEID_CX1;
			opt.base.wOptionID = CODANET_DEVICEOPTIONID_CODAMODE;
			opt.dwRateMode = dwRateMode;
			opt.dwDecimation = dwDecimation;
			opt.dwExternalSync = dwExternalSync;
		}

	protected:
		virtual ::DWORD buffersize() const
		{ return (::DWORD)sizeof(opt); }

		virtual void* buffer()
		{ return &opt; }

		virtual const void* buffer() const
		{ return &opt; }

		virtual void hton(void* data) const
		{ codanet_deviceoptions_codamode_hton((CODANET_DEVICEOPTIONS_CODAMODE*)data); }

		virtual void ntoh(void* data) const
		{ codanet_deviceoptions_codamode_ntoh((CODANET_DEVICEOPTIONS_CODAMODE*)data); }
	};

}
