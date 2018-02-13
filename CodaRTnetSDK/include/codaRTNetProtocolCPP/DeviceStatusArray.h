#pragma once

#include "../codaRTNetProtocol/codartapi.h"
#include "../codaRTNetProtocol/codartprotocol.h"

namespace codaRTNet
{
	/** Summary
			Provides error state information for multiple RTNet devices

			Description
			Objects of this class are thrown as exceptions from the RTNetClient class
			if a server-side error is generated during a function call.  It is
			possible for a single interaction with the server to generate more than one
			error (for example the startSystem command may fail on multiple devices).
			This class therefore provides for an array of error information. 
			See the underlying CODANET_DEVICESTATUS_ARRAY structure for details.
		*/
	class DeviceStatusArray : public CODANET_DEVICESTATUS_ARRAY
	{
	public:
		/** Summary
				Construct an empty device status array (indicating no errors)
			*/
		DeviceStatusArray()
		{
			codanet_devicestatusarray_clear(this);
		}

		~DeviceStatusArray()
		{
		}
	};
}
