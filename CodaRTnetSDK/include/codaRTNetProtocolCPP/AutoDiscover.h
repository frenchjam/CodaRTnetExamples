#pragma once

#include "../codaRTNetProtocol/codartclient.h"

namespace codaRTNet
{
	/** Summary
	    An array of available Codamotion RTNet servers as found by RTNetClient::doAutoDiscoverServer

			Description
			For details see the underlying C-style structure, CODANET_AUTODISCOVER.
		*/
	class AutoDiscover : public CODANET_AUTODISCOVER
	{
	};

}
