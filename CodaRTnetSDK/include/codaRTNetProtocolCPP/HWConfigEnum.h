#pragma once

#include "../codaRTNetProtocol/codartprotocol.h"

namespace codaRTNet
{
	/** Summary
			Array of all available hardware configurations as returned by RTNetClient::enumerateHWConfig. 

			Description
			See the underlying CODANET_HWCONFIGENUM structure for details.
		*/
	class HWConfigEnum : public CODANET_HWCONFIGENUM
	{
	};

}
