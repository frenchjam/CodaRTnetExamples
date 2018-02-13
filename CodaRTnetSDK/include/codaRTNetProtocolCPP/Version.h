#pragma once

#include "../codaRTNetProtocol/codartapi.h"
#include "../codaRTNetProtocol/codartprotocol.h"

namespace codaRTNet
{
	/** Summary
			Version information structure retrieved from RTNet server
		*/
	class Version : public CODANET_VERSION
	{
	public:
		/** Summary
				Construct version structure and initialise version numbers to zero
			*/
		Version()
		{
			wMajor = 0;
			wMinor = 0;
			wBuild = 0;
			wRevision = 0;
		}
	};

}
