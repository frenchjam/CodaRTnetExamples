#include <iomanip>
#include "codaRTNetProtocolCPP/RTNetClient.h"
#include "StartSystem.h"

StartSystem::StartSystem()
{
	RegisterParameter("config_name", config_name);
}

void StartSystem::RunRTNet() throw(TracedException, codaRTNet::NetworkException, codaRTNet::DeviceStatusArray) 
{
		// get hardware config list
		codaRTNet::HWConfigEnum configs;
		Client().enumerateHWConfig(configs);

		// print config names
		Comment1("Found hardware configurations", configs.dwNumConfig);
		Comment1("Looking for configuration", config_name.Value());
		DWORD selectedconfig = 0xFFFFFFFFUL;
		for(DWORD iconfig = 0; iconfig < configs.dwNumConfig; iconfig++)
		{
			// basic transcoding (will truncate name if non-ASCII character found)
			std::string entryname;
			for (int ichar = 0; ichar < CODANET_HWCONFIG_MAXNAMELENGTH; ichar++)
			{
				wchar_t wch = configs.config[iconfig].strName[ichar];
				if (wch == L'\0' || wch > 255)
					break;
				char ch = (char)wch;
				entryname += ch;
			}

			// list names
			std::ostrstream formattedmessage;
			formattedmessage 
				<< "  [" << iconfig << "] address: " 
				<< std::hex << std::setw(8) << std::setfill('0') << configs.config[iconfig].dwAddressHandle
				<< " name: " << entryname;
			Comment(formattedmessage);

			// see if our name exists
			if (entryname == config_name.Value())
			{
				selectedconfig = iconfig;
			}
		}

		// must have at least one config set up
		if(selectedconfig == 0xFFFFFFFFUL)
		{
			COMMAND_STOP("specified hardware configuration not found");
		}

		// get enabled devices for selected hardware config
		CODANET_HWCONFIG_DEVICEENABLE devices;
		Client().getDeviceEnable(configs.config[selectedconfig].dwAddressHandle, devices);

		// print device IDs for some recognised devices
		Comment1("Found devices", devices.dwNumOptions);
		for(DWORD idevice = 0; idevice < devices.dwNumOptions; idevice++)
		{
			char buffer[256];
			WORD deviceID = (WORD)devices.option[idevice].wDevice;
      _snprintf(buffer, 255, "  ID[%u]: %04X [%s]", idevice, deviceID,
			        devices.option[idevice].wEnable ? "enabled" : "disabled");
			Comment(buffer);
		}

    // ensure that system is stopped (in case a client failed and left it open)
		Comment("Ensure Stopped");
    Client().stopSystem();

    // if system is already started, this does nothing
		// otherwise it will load the first configuration
		Comment("Start");
		Client().startSystem(configs.config[selectedconfig].dwAddressHandle);
}
