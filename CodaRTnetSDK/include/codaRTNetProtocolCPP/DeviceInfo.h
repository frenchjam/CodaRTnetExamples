#pragma once

#include "../codaRTNetProtocol/codartprotocol.h"
#include <vector>

namespace codaRTNet
{
	/** Summary
			Base class for retrieving runtime information about an enabled device.

			Description
			Should not be used directly - see derived classes instead.
		*/
	class DeviceInfo
	{
	public:

		/** Serialisation function used on server side - do not call from client */
		virtual void serialise(::BYTE* info) const;

		/** Read function used by RTNetClient - do not call directly */
		virtual int readfrom(CODANET_SOCKET skt, CODANET_DEVICESTATUS_ARRAY* status);

		virtual ::DWORD buffersize() const = 0;

		virtual void* buffer() = 0;

		virtual const void* buffer() const = 0;

		virtual void hton(void* data) const = 0;

		virtual void ntoh(void* data) const = 0;
	};

}
