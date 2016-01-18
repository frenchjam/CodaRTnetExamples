#include <conio.h>
#include <iostream>
#include <set>
#include <strstream>
#include <map>
#include "codaRTNetProtocolCPP/RTNetClient.h"
#include "codaRTNetProtocolCPP/DeviceInfoCodaMode.h"
#include "Framework/IO/DataWriter.h"
#include "AcqSingleShot.h"

AcqSingleShot::AcqSingleShot()
{
	RegisterParameter("period", period);
	RegisterParameter("num_samples", num_samples);
	RegisterParameter("title", title);
}

void AcqSingleShot::RunRTNet() throw(TracedException, codaRTNet::NetworkException, codaRTNet::DeviceStatusArray)
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

  // log samples where sync stopped
	std::set<DWORD> coda_sync_stop_sample;
	DWORD previous_coda_sample_frame(0xFFFFFFFFUL);

	// timeout for receiving packets
	// 1ms should be fine on local network
  const DWORD packet_timeout_microseconds = 1000UL;

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

	// real-time sampling loop
	for (long sample_count = 0; sample_count < num_samples.Value(); sample_count++)
	{
		// sleep for specified time
		codanet_sleep(period.Value());  //ms

		// request a single frame
		Client().doSingleShotAcq();

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
	}

	// put end-of-acquisition marker in data output
	if (store_data)
	{
		DataOutput().EndAcquisition();
	}

	// close socket to allow reuse of same socket port
	Client().closeDataStream(stream);

	Comment("Acquisition finished");
}
