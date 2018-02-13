#pragma once

#include "../codaRTNetProtocol/codartprotocol.h"

namespace codaRTNet
{
	/** Summary
	    An array of available system devices as found by RTNetClient::enumerateDevices.

			Description
			For details see the underlying C-style structure, CODANET_DEVICEENUM.
		*/
	class DeviceEnum : public CODANET_DEVICEENUM
	{
	};

}
