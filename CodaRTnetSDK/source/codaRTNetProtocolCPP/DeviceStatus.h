#pragma once

#include "codaRTNetProtocol/codartapi.h"
#include "codaRTNetProtocol/codartprotocol.h"

namespace codaRTNet
{
	/** Summary
			Error state information for a single RTNet device
		*/
	class DeviceStatus : public CODANET_DEVICESTATUS
	{
	public:
		DeviceStatus()
		{
			deviceID = 0;
			subsystemID = 0;
			error = CODANET_DS_OK;
		}

		DeviceStatus(WORD device, WORD subsystem, DWORD nerror)
		{
			deviceID = device;
			subsystemID = subsystem;
			error = nerror;
		}
	};

	class DeviceStatus_Missing : public DeviceStatus
	{
	public:
		DeviceStatus_Missing(WORD ndevice) :
				DeviceStatus(ndevice, 0, CODANET_DS_MISSING_DEVICE)
		{
		}
	};
}
