
#include "codaRTNetProtocolCPP/RTNetClient.h"
#include "codaRTNetProtocolCPP/DeviceOptionsCodaPacketMode.h"
#include "codaRTNetProtocolCPP/DeviceInfoCodaPacketMode.h"
#include "CodaPacketMode.h"

CodaPacketMode::CodaPacketMode()
{
	RegisterParameter("coord_mode", coord_mode);
}

void CodaPacketMode::RunRTNet() throw(TracedException, codaRTNet::NetworkException, codaRTNet::DeviceStatusArray)
{
	// decode the mode value from parameters into a mode code used by RTNet
	DWORD modecode(0);
	if (coord_mode.Value() == "combined")
	{
		modecode = CODANET_CODAPACKETMODE_COMBINED_COORD;
	}
	else if (coord_mode.Value() == "separate")
	{
		modecode = CODANET_CODAPACKETMODE_SEPARATE_COORD;
	}
	else if (coord_mode.Value() == "combined-and-separate")
	{
		modecode = CODANET_CODAPACKETMODE_SEPARATE_AND_COMBINED_COORD;
	}
	else
	{
		COMMAND_STOP2("Unrecognised coord mode", coord_mode.Value());
	}

	// build device options object
  codaRTNet::DeviceOptionsCodaPacketMode packet_mode_options(modecode);

	// log parameters
	Comment1("Coord Mode", coord_mode.Value());

	// set options
	Comment("Sending mode change to server");
  Client().setDeviceOptions(packet_mode_options);

	// retrieve and check correct
	Comment("Retrieving from server to verify result");
	codaRTNet::DeviceInfoCodaPacketMode packet_mode_info;
	Client().getDeviceInfo(packet_mode_info);
	if (packet_mode_info.dev.dwPacketMode != modecode)
		COMMAND_STOP("Retrieved mode did not match the mode set");

	// done ok
	Comment("OK");
}
