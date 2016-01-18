#include "codaRTNetProtocolCPP/RTNetClient.h"
#include "codaRTNetProtocolCPP/DeviceOptionsCodaMode.h"
#include "codaRTNetProtocolCPP/DeviceInfoCodaMode.h"
#include "CodaMode.h"

CodaMode::CodaMode()
{
	RegisterParameter("mode", mode);
	RegisterParameter("decimation", decimation);
	RegisterParameter("external_sync", external_sync);
}

void CodaMode::RunRTNet() throw(TracedException, codaRTNet::NetworkException, codaRTNet::DeviceStatusArray)
{
	// decode the mode value from parameters into a mode code used by RTNet
	DWORD modecode(0);
	const char* modestring = "";
	switch (mode.Value())
	{
	case 100:
		modecode = CODANET_CODA_MODE_100;
		modestring = "100";
		break;
	case 200:
		modecode = CODANET_CODA_MODE_200;
		modestring = "200";
		break;
	case 400:
		modecode = CODANET_CODA_MODE_400;
		modestring = "400";
		break;
	case 800:
		modecode = CODANET_CODA_MODE_800;
		modestring = "800";
		break;
	default:
		COMMAND_STOP("Unrecognised mode code");
	}

	// build device options object
  codaRTNet::DeviceOptionsCodaMode mode_options(modecode, decimation.Value(), external_sync.Value());

	// log parameters
	Comment1("Mode", modestring);
	Comment1("Decimation", mode_options.opt.dwDecimation);
	Comment1("External Sync", mode_options.opt.dwExternalSync ? "on" : "off");

	// set options
	Comment("Sending mode change to server");
  Client().setDeviceOptions(mode_options);

	// check options ok
	Comment("Retrieving from server to verify result");
	codaRTNet::DeviceInfoCodaMode mode_info;
	Client().getDeviceInfo(mode_info);
	if (mode_info.dev.dwRateMode != modecode)
		COMMAND_STOP("Rate does not match the value set");
	if (mode_info.dev.dwDecimation != decimation.Value())
		COMMAND_STOP("Decimation does not match the value set");
	if ((external_sync.Value() && !mode_info.dev.dwExternalSync) || 
			(!external_sync.Value() && mode_info.dev.dwExternalSync))
		COMMAND_STOP("External sync setting does not match the value set");

	// all ok
	Comment("OK");
}
