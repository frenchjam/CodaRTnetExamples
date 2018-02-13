#pragma once

#include "../codaRTNetProtocol/codartprotocol.h"

namespace codaRTNet
{
	/** Summary
	    Base class for setting runtime device parameters or performing runtime setup operation
			on an enabled device.

			Description
			Should not be used directly - see derived classes instead.
		*/
	class DeviceOptions
	{
	public:

		virtual int sendto(CODANET_SOCKET skt, CODANET_DEVICESTATUS_ARRAY* status) const;

	protected:
		virtual ::DWORD buffersize() const = 0;

		virtual void* buffer() = 0;

		virtual const void* buffer() const = 0;

		virtual void hton(void* data) const = 0;

		virtual void ntoh(void* data) const = 0;
	};

}
