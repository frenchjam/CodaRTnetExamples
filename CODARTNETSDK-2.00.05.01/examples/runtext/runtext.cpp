/////////////////////////////////////////////////////////////////////////////
// Codamotion RTNet C++ Examples: runtext.cpp 
//
// Copyright (C) Charnwood Dynamics Ltd.
// All rights reserved.
//
// Connects to first available server and, if system is not already running,
// starts system using first available hardware configuration.  Optionally
// allows user to perform alignment and shows alignment diagnostics. Uses 
// buffered acquisition mode and monitor function for low-latency realtime 
// monitoring.  Prints realtime 3D and analog data to the screen for maximum
// of 2000 samples or until stopped by the user.
//
/////////////////////////////////////////////////////////////////////////////

// Standard library includes
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

// RTNet C++ includes
#include "codaRTNetProtocolCPP/RTNetClient.h"
#include "codaRTNetProtocolCPP/DeviceOptionsAlignment.h"
#include "codaRTNetProtocolCPP/DeviceInfoAlignment.h"
#include "codaRTNetProtocolCPP/PacketDecode3DResult.h"
#include "codaRTNetProtocolCPP/PacketDecode3DResultExt.h"
#include "codaRTNetProtocolCPP/PacketDecodeADC16.h"

// Use main Codamotion RTNet namespace
using namespace codaRTNet;

// Helper function to print network connection error codes (client-side errors)
void print_network_error(const NetworkException& exNet);

// Helper function to print device error codes (server-side errors)
void print_devicestatusarray_errors(const DeviceStatusArray& status);

// Helper function to print system alignment status
void print_alignment_status(const DWORD* marker_id_array, const DeviceInfoAlignment& info);

// constant for maximum number of marker positions to print
// (alter this constant to print more than one)
const DWORD maxprintmarkers = 1;

// constant for maximum number of analog channels to print
// (alter this constant to print more than one)
const DWORD maxprintanalog = 1;

// Function main - the main program entry point
int main(int, char**)
{
	// client connection object
	RTNetClient cl;

	// attempt auto-discover server
	AutoDiscover discover;
	cl.doAutoDiscoverServer(discover);

	// need at least one server to connect to
	if (discover.dwNumServers == 0)
	{
		fprintf(stderr, "ERROR: no servers found using auto-discovery");
		return 1;
	}

	// list all servers
	fprintf(stderr, "Found %u Codamotion Realtime Network servers: \n", discover.dwNumServers);
	for (DWORD iserver = 0; iserver < discover.dwNumServers; iserver++)
	{
		fprintf(stderr, "[%u]: %s IP: %u.%u.%u.%u port: %u\n",
			iserver,
			discover.server[iserver].hostname,
			(discover.server[iserver].ip & 0xFF000000) >> 24,
			(discover.server[iserver].ip & 0x00FF0000) >> 16,
			(discover.server[iserver].ip & 0x0000FF00) >>  8,
			(discover.server[iserver].ip & 0x000000FF)      ,
			discover.server[iserver].port);
	}

	try
	{
		// connect to first available server
		cl.connect(discover.server[0].ip, discover.server[0].port);

		// get config list
		HWConfigEnum configs;
		cl.enumerateHWConfig(configs);

		// print config names
		fprintf(stderr, "Found %u hardware configurations:\n", configs.dwNumConfig);
		for (DWORD iconfig = 0; iconfig < configs.dwNumConfig; iconfig++)
			fwprintf(stderr, L"  [%u] address: %08X  name: %s\n", 
			iconfig, configs.config[iconfig].dwAddressHandle, configs.config[iconfig].strName);

		// must have at least one config set up
		if (configs.dwNumConfig == 0)
		{
			fprintf(stderr, "ERROR: no hardware configurations are available\n");
			return 1;
		}

		// get enabled devices for first hardware config
		CODANET_HWCONFIG_DEVICEENABLE devices;
		cl.getDeviceEnable(configs.config[0].dwAddressHandle, devices);

		// print device ID's
		fprintf(stderr, "Found %u Available Devices:\n", devices.dwNumOptions);
		for (DWORD idevice = 0; idevice < devices.dwNumOptions; idevice++)
		{
			fprintf(stderr, "  ID[%u]: %04X [%s]\n", idevice, 
			(DWORD)devices.option[idevice].wDevice,
			devices.option[idevice].wEnable ? "enabled" : "disabled");
		}

		// if system is already started, this does nothing
		// otherwise it will load the first configuration
		cl.startSystem(configs.config[0].dwAddressHandle);

		// limit acquisition to 2000 samples of CX1 data
		// (this assumes a cx1 device in the chosen hardware configuration)
		cl.setAcqMaxTicks(DEVICEID_CX1, 2000);

		// perform alignment if requested
		fprintf(stdout, "Perform alignment? (Y/N)");
		int ch = getch();
		fprintf(stdout, "\n");
		if (ch == 'y' || ch == 'Y')
		{
			// scan alignment marker numbers from user
			DWORD dwOrigin(0), dwX(0), dwXY(0); 
			fprintf(stdout, "Enter three alignment marker numbers [1-56] separated by spaces: ");
			while (fscanf(stdin, " %u %u %u", &dwOrigin, &dwX, &dwXY) != 3 ||
						 (dwOrigin < 1 || dwOrigin > 56) ||
						 (dwX < 1 || dwX > 56) ||
						 (dwXY < 1 || dwXY > 56))
			{
				fprintf(stdout, "Error in numbers, please enter three numbers [1-56], separated by spaces: ");
			}

			// alignment request structure
			DeviceOptionsAlignment align(dwOrigin, dwOrigin, dwX, dwOrigin, dwXY);

			// perform alignment
			// this will cause system to look for alignment markers
			// - they must be in position by this time
			cl.setDeviceOptions(align);

			// retrieve information
			DeviceInfoAlignment info;
			cl.getDeviceInfo(info);

			// print alignment diagnostics
			DWORD marker_id_array[5] = { dwOrigin, dwOrigin, dwX, dwOrigin, dwXY };
			print_alignment_status(marker_id_array, info);

			// exit early if alignment failed
			if (info.dev.dwStatus != 0)
			{
				fprintf(stderr, "Exiting due to alignment errors\n");
				codanet_sleep(500);
				exit(-1);
			}
		}

		// prepare for acquisition
		cl.prepareForAcq();

		// create data stream
		DataStream stream;
		cl.createDataStream(stream, 7000);

		// ensure socket is up before starting acquisition
		codanet_sleep(50);

		// start acq (and buffer)
		cl.startAcqContinuousBuffered();

		// monitor
		while (1)
		{
			// check for pending keystroke
			if (_kbhit() != 0)
			{
				int ch = getch();
				if (ch == 'q' || ch == 'Q' || ch == 27)
					break;
			}

			// request latest frame from all devices
			try
			{
				cl.monitorAcqBuffer();
			}
			catch(DeviceStatusArray&)
			{
				/*... ignore status information from codanet_cmd_acqbuffermonitor -
							errors will just cause codanet_packet_receive to time out... */
			}

			// process packets from all devices
			while (true)
			{
				// receive data - wait for max 50ms - finish loop when no more packets
				RTNetworkPacket packet;
				if (stream.receivePacket(packet, 50000) == CODANET_STREAMTIMEOUT)
				{
					// timed out since no more packets are available -
					// this usually means that packets from all devices have been received
					// so we can now break to allow the next monitor request
					break;
				}

				// check result
				if (packet.verifyCheckSum())
				{
					// decoder objects
					PacketDecode3DResultExt decode3D;	// 3D measurements (CX1)
					PacketDecodeADC16 decodeADC;			// 16-bit ADC measurements (GS16AIO)
				
					// decode & print results
					if (decode3D.decode(packet))
					{
						// find number of marker positions available
						DWORD nmarkers = decode3D.getNumMarkers();

						// limit the number of markers 
						// (edit the maxprintmarkers to change this)
						if (nmarkers > maxprintmarkers)
							nmarkers = maxprintmarkers;

						// loop through one or more markers (set maxprintmarkers to do more than one)
						for (DWORD imarker = 0; imarker < nmarkers; imarker++)
						{
							float* pos = decode3D.getPosition(imarker);
							BYTE valid = decode3D.getValid(imarker);
							BYTE* intensity = decode3D.getIntensity(imarker);

							fprintf(stderr, "%05u COORDS: %05.1f %05.1f %05.1f VALID: %03d I: ",
								decode3D.getTick(), pos[0], pos[1], pos[2], (int)valid);
							for (DWORD iI = 0; iI < decode3D.getNumCamerasPerMarker(); iI++)
								fprintf(stderr, "%03d ", (int)intensity[iI]);
							fprintf(stderr, "\n");

						}
					}
					else if (decodeADC.decode(packet))
					{
						// get total analog values available
						DWORD nchannels = decodeADC.getNumValues();

						// limit the number of analog channels
						// (edit the maxprintanalog to change this)
						if (nchannels > maxprintanalog)
							nchannels = maxprintanalog;

						// loop through one or more analog channels and print
						for (DWORD ichannel = 0; ichannel < nchannels; ichannel++)
						{
							const short* value = decodeADC.getValues();
							fprintf(stderr, "%05u ADC: %d\n",
								decodeADC.getTick(), (int)value[ichannel]);
						}
					}
				}
				else
				{
					fprintf(stderr, "checksum failed\n");
				}
			}
		}

		// stop acquisition if still in progress
		if (cl.isAcqInProgress())
			cl.stopAcq();
	}
	catch(NetworkException& exNet)
	{
		print_network_error(exNet);
	}
	catch(DeviceStatusArray& array)
	{
		print_devicestatusarray_errors(array);
	}

	// pause before exiting
	codanet_sleep(500);

	return 0;
}

// print_devicestatusarray_errors
// Helper function to print device errors (server-side errors)
// @param array Reference to DeviceStatusArray object thrown as an exception by RTNetClient
void print_devicestatusarray_errors(const DeviceStatusArray& array)
{
	for (DWORD idev = 0; idev < array.numstatus; idev++)
	{
		if (array.status[idev].error)
		{
			fprintf(stderr, "DEVICE %u SUBSYSTEM %u ERROR: %u\n", 
				(DWORD)array.status[idev].deviceID, 
				(DWORD)array.status[idev].subsystemID,
				(DWORD)array.status[idev].error);
		}
	}
}

// print_network_error
// Helper function to network errors (client-side errors)
// @param exNet Network exception object thrown from RTNetClient or DataStream
void print_network_error(const NetworkException& exNet)
{
	fprintf(stderr, "Network error: ");
	switch (exNet.errorcode)
	{
	case CODANET_OK:
		fprintf(stderr, "CODANET_OK: NO ERROR CODE WAS PRODUCED");
		break;

	case CODANET_SOCKETERROR_BROKEN:
		fprintf(stderr, "CODANET_SOCKETERROR_BROKEN");
		break;

	case CODANET_SOCKETERROR_WINDOWSDLL:
		fprintf(stderr, "CODANET_SOCKETERROR_WINDOWSDLL");
		break;

	case CODANET_SOCKETERROR_CREATE:
		fprintf(stderr, "CODANET_SOCKETERROR_CREATE");
		break;

	case CODANET_SOCKETERROR_HOSTNAME:
		fprintf(stderr, "CODANET_SOCKETERROR_HOSTNAME");
		break;

	case CODANET_SOCKETERROR_CONNECT:
		fprintf(stderr, "CODANET_SOCKETERROR_CONNECT");
		break;

	case CODANET_SOCKETERROR_TCPOPTIONS:
		fprintf(stderr, "CODANET_SOCKETERROR_TCPOPTIONS");
		break;

	case CODANET_CLIENTPROTOCOLERROR_BADNUMSTATUS:
		fprintf(stderr, "CODANET_CLIENTPROTOCOLERROR_BADNUMSTATUS");
		break;

	case CODANET_SOCKETERROR_STREAMCREATE:
		fprintf(stderr, "CODANET_SOCKETERROR_STREAMCREATE");
		break;

	case CODANET_SOCKETERROR_STREAMPORT:
		fprintf(stderr, "CODANET_SOCKETERROR_STREAMPORT");
		break;
	
	case CODANET_CLIENTPROTOCOLERROR_TOOBIG:
		fprintf(stderr, "CODANET_CLIENTPROTOCOLERROR_TOOBIG");
		break;

	default:
		fprintf(stderr, "UNKNOWN ERROR CODE %u", codanet_getlasterror());
		break;
	}

	fprintf(stderr, "\n");
}

// print_alignment_status
// Helper function to show alignment status
// @param marker_id_array 5-element array containing one-based marker identities for alignment markers
// @param info Alignment info retrieved from server
void print_alignment_status(const DWORD* marker_id_array,  const DeviceInfoAlignment& info)
{
		// print alignment status value
		fprintf(stderr, "Alignment result: ");
		switch (info.dev.dwStatus)
		{
		case 0:
			fprintf(stderr, "success");
			break;
		case CODANET_ALIGNMENTERROR_SYSTEM:
			fprintf(stderr, "system error");
			break;
		case CODANET_ALIGNMENTERROR_ALREADY_ACQUIRING:
			fprintf(stderr, "already acquiring (is another program running?");
			break;
		case CODANET_ALIGNMENTERROR_OCCLUSIONS:
			fprintf(stderr, "occlusions");
			break;
		case CODANET_ALIGNMENTERROR_XTOOCLOSE:
			fprintf(stderr, "x-axis markers too close");
			break;
		case CODANET_ALIGNMENTERROR_XYTOOCLOSE:
			fprintf(stderr, "xy-plane markers too close");
			break;
		case CODANET_ALIGNMENTERROR_NOTPERP:
			fprintf(stderr, "marked axes not sufficiently perpendicular");
			break;
		default:
			fprintf(stderr, "unknown alignment status error code %d", info.dev.dwStatus);
			break;
		}
		fprintf(stderr, "\n");

		// number of CX1 units
		DWORD nunits = info.dev.dwNumUnits;

		// frame count
		DWORD nframes = info.dev.dwNumFrames;

		// print visibility information
		for (DWORD icoda = 0; icoda < nunits; icoda++)
		{
			// index of Codamotion CX1 unit
			fprintf(stderr, "Coda %d\n", icoda+1);

			// data from each marker
			for (DWORD imarker = 0; imarker < 5; imarker++)
			{
				// actual marker identity
				DWORD marker_identity = marker_id_array[imarker];

				for (DWORD icam = 0; icam < 3; icam++)
				{
					// label for this marker
					switch (imarker)
					{
					case 0:
						fprintf(stderr, "Org");
						break;
					case 1:
						fprintf(stderr, "X0 ");
						break;
					case 2:
						fprintf(stderr, "X1 ");
						break;
					case 3:
						fprintf(stderr, "XY0");
						break;
					case 4:
						fprintf(stderr, "XY1");
						break;
					}

					// print marker identity
					fprintf(stderr, " (Marker %02d) ", marker_identity);

					// camera letter for each coda camera
					switch (icam)
					{
					case 0:
						fprintf(stderr, "A");
						break;
					case 1:
						fprintf(stderr, "B");
						break;
					case 2:
						fprintf(stderr, "C");
						break;
					}

					// space
					fprintf(stderr, ": ");

					// print visibility graph for frames of data
					// show a 1 for visible and _ for occluded
					// (show alternate frames only to save screen space)
					for (DWORD iframe = 0; iframe < nframes; iframe+=2)
					{
						BYTE flag = info.dev.camera_flag[3*nframes*5*icoda + 3*nframes*imarker + 3*iframe + icam];
						if (flag <= 10)
							fprintf(stderr, "_");
						else
							fprintf(stderr, "1");
					}

					// new line
					fprintf(stderr, "\n");
				}
			}
		}
}
