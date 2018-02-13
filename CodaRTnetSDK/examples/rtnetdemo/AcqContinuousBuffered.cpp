#include <conio.h>
#include <iostream>
#include <set>
#include <strstream>
#include <map>
#include "codaRTNetProtocolCPP/RTNetClient.h"
#include "codaRTNetProtocolCPP/DeviceInfoCodaMode.h"
#include "AcqContinuousBuffered.h"
#include "Framework/IO/DataWriter.h"

AcqContinuousBuffered::AcqContinuousBuffered()
{
	RegisterParameter("frame_counter_device", frame_counter_device);
	RegisterParameter("frames", frames);
	RegisterParameter("monitor_period", monitor_period);
	RegisterParameter("monitor_title", monitor_title);
	RegisterParameter("download_title", download_title);
}

void AcqContinuousBuffered::RunRTNet() throw(TracedException, codaRTNet::NetworkException, codaRTNet::DeviceStatusArray)
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

  // log samples where sync stopped
	std::set<DWORD> coda_sync_stop_sample;
	DWORD previous_coda_sample_frame(0xFFFFFFFFUL);

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
	bool store_monitor_data = (monitor_title.Value().size() > 0);
	if (store_monitor_data)
	{
		Comment1("Storing monitor data with title", monitor_title.Value());
		DataOutput().BeginAcquisition(monitor_title.Value(), device_sample_rate);
	}
	else
	{
		Comment("Storage of real-time monitoring data is disabled (no monitor_title specified)");
	}

	// start buffered acquisition
	Comment("Start");
  Client().startAcqContinuousBuffered();
	Comment("Running");

	// real-time loop
  // breakout by keyboard 'Q' 'S' or ESC, or when MaxSamples have been acquired (!cl.isAcqInProgress())
	do
	{
    // reduce loop rate to avoid calling cl.monitorAcqBuffer() too rapidly
    // which will cause it to receive duplicate data packets
		codanet_sleep(monitor_period.Value());  //ms

    // request latest frame (packet) from all devices
		try
		{
			Client().monitorAcqBuffer();
		}
		catch(codaRTNet::DeviceStatusArray&)
		{
			/*... ignore status information from codanet_cmd_acqbuffermonitor -
						errors will just cause codanet_packet_receive to time out... */
		}

    // receive monitor data - wait for N us - finish loop when no more packets
		codaRTNet::RTNetworkPacket packet;
		while (stream.receivePacket(packet, packet_timeout_microseconds) != CODANET_STREAMTIMEOUT)
		{
      // check result
			if(packet.verifyCheckSum())
			{				
				// examine packet header
				CODANET_PACKET_HEADER header;
				packet.readHeader(&header);

				// special processing for CODA devices to show diagnostics about external sync
				if (header.wDeviceID == DEVICEID_CX1 && header.dwPage == 0)
				{
          // get the cx1 sample number:
					DWORD coda_sample_frame = header.dwTick;

          // if SampleNum hasn't changed when running with ext.sync, don't re-print visibility data:
          if(coda_external_sync && (coda_sample_frame == previous_coda_sample_frame))
          {
            // inform user that we're waiting for sync
            std::cerr << "Waiting for external sync...\r";  //use \r to prevent scrolling this message 
            
						// add to list of sync-stop sample numbers, for indication in buffer download data:
						coda_sync_stop_sample.insert(coda_sample_frame);

						// ignore and continue to process next packet
						continue;
          }
          else
          {
            // remember this (new) sample number:
            previous_coda_sample_frame = coda_sample_frame;
					}
				}

				// log this packet if logging is enabled
				if (store_monitor_data)
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
	if (store_monitor_data)
	{
		DataOutput().EndAcquisition();
	}

	Comment("Acquisition finished");
	
	// download data if a download name was specified
	if (download_title.Value().size() > 0UL)
	{
		// begin data for download section
		Comment1("Storing post-acquisition download with title", download_title.Value());
		DataOutput().BeginAcquisition(download_title.Value(), device_sample_rate);

		// download loop
		// retrieve packets from each device
		Comment("Downloading");
		for(std::map<WORD, float>::const_iterator iter_device( device_sample_rate.begin() ); iter_device != device_sample_rate.end(); iter_device++)
		{
			// get device id
			WORD device = iter_device->first;

			// despite the name, this actually returns the number of frames
			// there may be more than one packet per frame
			DWORD numframes = Client().getAcqBufferNumPackets(device);

			// get all frames
			for (DWORD iframe = 0; iframe < numframes; iframe++)
			{
				// request all packets for this frame
				Client().requestAcqBufferPacket(device, iframe);

				for (DWORD ipacket = 0; ; ipacket++)
				{
					// assume at least one packet per-frame
					// all packets arrive at once, so only need a longer timeout on the first one
					DWORD timeout = (ipacket == 0) ? packet_timeout_microseconds : 0;

					// retrieve until timeout
					codaRTNet::RTNetworkPacket packet;
					if (stream.receivePacket(packet, timeout) != CODANET_OK)
						break;

					if (!packet.verifyCheckSum())
						COMMAND_STOP("Packet checksum failed");

					// external sync diagnostics
					bool sync_stop = 
						(coda_external_sync && 
						 (device == DEVICEID_CX1) && 
						 (coda_sync_stop_sample.find(iframe) != coda_sync_stop_sample.end()));

					// log the packet
					DataOutput().AddAcquisitionPacket(packet, sync_stop);
				}
			}
		}	// end of acquisition download loop

		// end of acquisition object
		DataOutput().EndAcquisition();
		Comment("Download complete");
	}
	else
	{
		Comment("Post-acquisition download is disabled (no download_title specified)");
	}

	// close socket to allow reuse of same socket port
	Client().closeDataStream(stream);
}
