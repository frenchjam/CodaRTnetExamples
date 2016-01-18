#include <conio.h>
#include <iostream>
#include <set>
#include <strstream>
#include <map>
#include "codaRTNetProtocolCPP/RTNetClient.h"
#include "codaRTNetProtocolCPP/DeviceInfoCodaMode.h"
#include "Framework/IO/DataWriter.h"
#include "AcqContinuous.h"

AcqContinuous::AcqContinuous()
{
	RegisterParameter("frame_counter_device", frame_counter_device);
	RegisterParameter("frames", frames);
	RegisterParameter("title", title);
}

void AcqContinuous::RunRTNet() throw(TracedException, codaRTNet::NetworkException, codaRTNet::DeviceStatusArray)
{
	// retrieve sample rates for all enabled devices
	std::map<WORD, float> device_sample_rate;
	CODANET_HWCONFIG_DEVICEENABLE devices;
	Client().getDeviceEnable(Client().getRunningHWConfig(), devices);
	for (DWORD ioption = 0; ioption < devices.dwNumOptions; ioption++)
	{
		if (devices.option[ioption].wEnable)
		{
			// put into map
			WORD device = devices.option[ioption].wDevice;
			float sample_rate = 1.0f / Client().getDeviceTickSeconds(device);
			device_sample_rate[device] = sample_rate;

			// log message
			std::ostrstream formattedmessage;
			formattedmessage << "Device " << device << " sample rate: " << sample_rate;
			Comment(formattedmessage);
		}
	}

	// allow some extra diagnostics for external sync of CODA systems
	// this get set true if CODA device detected and external sync is enabled on that device
	bool coda_external_sync = false;

	// special info retieval for CODA system
	if (device_sample_rate.find(DEVICEID_CX1) != device_sample_rate.end())
	{
		// look to see if using external sync
		Comment("Retrieving mode information");
		codaRTNet::DeviceInfoCodaMode mode;
		Client().getDeviceInfo(mode);
		if (mode.dev.dwExternalSync)
			coda_external_sync = true;
	
		// log mode
		const char* modestring = "";
		switch (mode.dev.dwRateMode)
		{
		case CODANET_CODA_MODE_100:
			modestring = "100";
			break;
		case CODANET_CODA_MODE_200:
			modestring = "200";
			break;
		case CODANET_CODA_MODE_400:
			modestring = "400";
			break;
		case CODANET_CODA_MODE_800:
			modestring = "800";
			break;
		}
		Comment1("CODA mode", modestring);

		// log decimation
		Comment1("CODA Decimation", mode.dev.dwDecimation);

		// log use of external sync
		Comment1("CODA External sync",  coda_external_sync ? "on" : "off");
	}
		
	// put into prepared mode	
	Comment("Preparing");
	Client().prepareForAcq();

	// create socket for receiving acquisition
	Comment("Creating socket");
	codaRTNet::DataStream stream;
	Client().createDataStream(stream, 11012);
	Comment("Socket created");

	// ensure socket is up before starting acquisition
	codanet_sleep(50);

	// timeout for receiving packets
	// 1ms should be fine on local network
  const DWORD packet_timeout_microseconds = 1000UL;

	// Set time
	if (frames.Value() <= 0)
	{
		Comment1("Setting frames", "unlimited");
		WORD first_device = device_sample_rate.begin()->first;
		Client().setAcqMaxTicks(first_device, CODANET_ACQ_UNLIMITED);
	}
	else
	{
		Comment1("Setting frames", frames.Value());
		Comment1("Using device for frame counter", frame_counter_device.Value());
		Client().setAcqMaxTicks((WORD)frame_counter_device.Value(), frames.Value());
	}

	// begin a new acquisition of monitoring data if non-empty title specified
	bool store_data = (title.Value().size() > 0);
	if (store_data)
	{
		Comment1("Storing data with title", title.Value());
		DataOutput().BeginAcquisition(title.Value(), device_sample_rate);
	}
	else
	{
		Comment("Storage of data is disabled (no title specified)");
	}

	// start buffered acquisition
	Comment("Start");
  Client().startAcqContinuous();
	Comment("Running");

	// real-time loop
  // breakout by keyboard 'Q' 'S' or ESC, or when MaxSamples have been acquired (!cl.isAcqInProgress())
	do
	{
    // receive monitor data - wait for N us - finish loop when no more packets
		codaRTNet::RTNetworkPacket packet;
		while (stream.receivePacket(packet, packet_timeout_microseconds) != CODANET_STREAMTIMEOUT)
		{
      // check result
			if(packet.verifyCheckSum())
			{				
				// log this packet if logging is enabled
				if (store_data)
				{
					DataOutput().AddAcquisitionPacket( packet );
				}
			}
			else
			{
				COMMAND_STOP("Packet checksum failed");
			}
    }

		// check for pending keystroke to cancel acquisition
		if (_kbhit() != 0)
		{
			int ch = _getch();
			if (ch == 'q' || ch == 'Q' || ch == 27 || ch == 'S' || ch == 's')  //Quit, Esc, Stop
      {
				// stop acquisition:
				Comment("Stopped by user");
        Client().stopAcq();
      }
		}
	}
	while(Client().isAcqInProgress());	// end of real-time loop

	// put end-of-acquisition marker in data output
	if (store_data)
	{
		DataOutput().EndAcquisition();
	}

	// close socket to allow reuse of same socket port
	Client().closeDataStream(stream);

	Comment("Acquisition finished");	
}
