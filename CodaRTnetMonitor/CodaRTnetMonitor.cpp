// 
// CodaRTnetMonitor.cpp : A simple command line program to monitor CODA measurements in real time.
//

#include "stdafx.h"

// Use main Codamotion RTNet namespace
using namespace codaRTNet;

// Include some common static variables.
#include "../CodaRTnetCommon/CodaRTnetCommon.h"

// We will interrogate the Coda system to fill in this value
// based on how many Coda units are used in the selected configuration.
int n_codas;

// Determines how many markers are to be included in the visibility string.
int maxDisplayMarkers = 28;

// The coordinates of one marker are displayed on the screen.
int displayMarker = 4;


bool confirmExit = false;

//* Generic data packet
RTNetworkPacket packet;

// client connection object
RTNetClient cl;

// decoder objects
PacketDecode3DResultExt	decode3D;	// 3D measurements (CX1)
PacketDecodeADC16		decodeADC;	// 16-bit ADC measurements (GS16AIO)

// Various objects
AutoDiscover discover;
HWConfigEnum configs;
CODANET_HWCONFIG_DEVICEENABLE devices;
DataStream stream;

//////////////////////////////////////////////////////////////////////////////////////////

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
	fprintf( stderr, "Press <RETURN> to continue. " );
	getchar();
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

	fprintf(stderr, "\nPress <RETURN> to continue.\n");
	getchar();
}

//////////////////////////////////////////////////////////////////////////////////////////

int Monitor( int coda_config ) {
	
	bool expect_cx1, expect_adc;
	
	// Define which subsystems will be used according to the selected configuration.
	// Limit acquisition to a set number of samples.
	
	switch ( coda_config ) {
	case ADConly:
		expect_cx1 = false;
		expect_adc = true;
		break;
	case CX1only:
		expect_cx1 = true;
		expect_adc = false;
		break;
	default:
		expect_cx1 = true;
		expect_adc = true;
		break;
	}
	
	
	// monitor
	while ( 1 ) // Will run until preset number of ticks are acquired.
	{
		// check for pending keystroke
		if (_kbhit() != 0)
		{
			int ch = _getch();
			break; 
		}
		
		// If the CODA is already acquiring, just read the latest data.
		// We assume that it is in buffered mode, so we have to ask for the latest frame.
		if ( cl.isAcqInProgress() ) {
			try
			{
				cl.monitorAcqBuffer();
			}
			catch(DeviceStatusArray& array)
			{
				print_devicestatusarray_errors(array);
				OutputDebugString( "Caught (DeviceStatusArray& array)\n" );
			}

		}

		// If not already acquiring, then request the frames one by one.
		else {
			// request a frame from all devices
			try
			{
				cl.doSingleShotAcq();
			}
			catch(DeviceStatusArray& array)
			{
				print_devicestatusarray_errors(array);
				OutputDebugString( "Caught (DeviceStatusArray& array)\n" );
			}
		}
		
		// process packets from all devices
		//* Charnwood version makes no assumptions about how many devices, and therefore how many packets
		//*  will arrive and need to be processed. So they try for packets until they get a timeout.
		//* In my version we assume that there will be 1 packets from each CX1 plus one combined, 
		//* and one packet from the ADC (if enabled).
		//* When both have been received, we can move on without waiting any longer.
		//* This should allow rapid responses to movements of the markers.
		//* Be careful, though, because if the loop runs fast enough you will probably get multiple 
		//*  packets from the same clock tick.
//		if ( cl.isAcqInProgress() ) {
		if ( 1 ) {

			float marker_position[3][28][3];
			bool  marker_visible[3][28];
			float residual;
			
			int received_cx1 = 0;
			bool received_adc = false;
			bool saw_page[3] = {false, false, false};
			int page[3] = {-9, -9, -9};
			do {
				// receive data - wait for max 50ms
				if ( stream.receivePacket(packet, 50000) == CODANET_STREAMTIMEOUT )
				{
					//* A timeout means that we didn't get one of the two packets that we expected.
					//* Just break out of the loop so as to request another pair.
					break;
				}
				
				// check result
				if ( packet.verifyCheckSum() )
				{
					
					// decode & print results
					if ( decode3D.decode(packet) )
					{
						float *pos;
						int unit = decode3D.getPage();
						saw_page[unit] = true;
						page[received_cx1] = unit;

						// find number of marker positions available
						int nMarkers = decode3D.getNumMarkers();
						
						// limit the number of markers 
						// (edit maxDisplayMarkers to change this)
						if (nMarkers > maxDisplayMarkers) nMarkers = maxDisplayMarkers;
						
						// loop through one or more markers (set maxDisplayMarkers to do more than one)
						char visibility_string[64] = "";
						for (int imarker = 0; imarker < nMarkers; imarker++)
						{
							BYTE  valid = decode3D.getValid(imarker);
							BYTE* intensity = decode3D.getIntensity(imarker);

							pos = decode3D.getPosition( imarker );
							if ( valid ) {
								strcat( visibility_string, "O" );
								for ( int j = 0; j < 3; j++ ) {
									marker_position[unit][imarker][j] = pos[j];
									marker_visible[unit][imarker] = true;
								}
							}
							else {
								strcat( visibility_string, "-" );
								marker_visible[unit][imarker] = false;
							}
							if ( !((imarker + 1) % 8) ) strcat( visibility_string, " " );

							
						}
						pos = decode3D.getPosition( displayMarker );
						fprintf( stderr, "%05u %02d: [%6.1f %6.1f %6.1f] %s\n", 	
							decode3D.getTick(), displayMarker, pos[0], pos[1], pos[2], visibility_string );
						//* Signal that we got the CX1 packet.
						received_cx1++;

					}
				}
				else
				{
					fprintf(stderr, "%08u: checksum failed", decodeADC.getTick());
				}
			} while ( expect_cx1 && ( received_cx1 <= n_codas ) ); //* If expected packets received, move on.
			if ( received_cx1 > n_codas ) {
				if ( marker_visible[1][displayMarker] && marker_visible[2][displayMarker] ) {
					float delta[3];
					for ( int j = 0; j < 3; j++ ) delta[j] = marker_position[1][displayMarker][j] - marker_position[2][displayMarker][j];
					residual = sqrt( delta[X] * delta[X] + delta[Y] * delta[Y] + delta[Z] * delta[Z] );
				}
				else residual = -999.999f;
				fprintf( stderr, "Residual: %.3f\n\n", residual );
			}
			// Flush any other packets that might come in.
			while ( stream.receivePacket(packet, 50000) != CODANET_STREAMTIMEOUT ); 
		}
	}
	return( 0 );
}

//////////////////////////////////////////////////////////////////////////////////////////

int _tmain(int argc, _TCHAR* argv[])
{
	// Show what program is running.	
	fprintf( stderr, "%s\n\n", argv[0] );

	try {

		// Decode the IP address string.
		unsigned int p, q, r, s;
		fprintf(stderr, "RTnet Server: %s   Port: %d\n", serverAddress, serverPort );
		sscanf( serverAddress, "%d.%d.%d.%d", &p, &q, &r, &s );

		// Connect to the server.
		cl.connect( (p << 24) + (q << 16) + (r << 8) + s, serverPort );

		// Check that the server has at least the expected number of configurations.
		cl.enumerateHWConfig(configs);

		// print config names
		fprintf(stderr, "Found %u hardware configurations:\n", configs.dwNumConfig);
		for(DWORD iconfig = 0; iconfig < configs.dwNumConfig; iconfig++)
			fwprintf(stderr, L"  [%u] address: %08X  name: %s\n", 
			iconfig, configs.config[iconfig].dwAddressHandle, configs.config[iconfig].strName);


		if ( configs.dwNumConfig < codaConfig )
		{
			fprintf(stderr, "ERROR: specified hardware configuration %d not available.\n", codaConfig);
			return 1;
		}

		// Show which configuration we are trying to use.
		fwprintf( stderr, L"RTnet Configuration: %s\n", configs.config[codaConfig].strName );

		// get enabled devices for selected hardware config
		CODANET_HWCONFIG_DEVICEENABLE devices;
		cl.getDeviceEnable(configs.config[codaConfig].dwAddressHandle, devices);

		// print device ID's
		fprintf(stderr, "Found %u Available Devices:\n", devices.dwNumOptions);
		for (DWORD idevice = 0; idevice < devices.dwNumOptions; idevice++)
		{
			fprintf(stderr, "  ID[%u]: %04X [%s]\n", idevice, 
			(DWORD)devices.option[idevice].wDevice,
			devices.option[idevice].wEnable ? "enabled" : "disabled");
		}

		// If it's already running but not using the right config,
		// shutdown and start over.

		DWORD	running_config = cl.getRunningHWConfig();
		if ( running_config != configs.config[codaConfig].dwAddressHandle ) {
			OutputDebugString( "*** Not running the right configuration! ***" );
			cl.stopSystem();
			running_config = NULL;
		}

		if ( !running_config ) {

			OutputDebugString( "*** Starting up Coda system! ***" );

			// If system is already started, this does nothing.
			// Otherwise it will load the specified configuration.
			// We assume that the configuration constants defined here correspond
			//  to the actual configurations defined on the server..
			cl.startSystem( configs.config[codaConfig].dwAddressHandle );

			cl.setDeviceOptions( coda_mode );

			// prepare for acquisition
			OutputDebugString( "cl.prepareForAcq()\n" );
			cl.prepareForAcq();

		}

		// create data stream
		cl.createDataStream(stream, 7001);
		// ensure socket is up before starting acquisition
		codanet_sleep(50);

		DeviceInfoAlignment align_info;
		cl.getDeviceInfo( align_info );
		n_codas = align_info.dev.dwNumUnits;

			
		Monitor( codaConfig );



	}
	catch(NetworkException& exNet)
	{
		print_network_error(exNet);
		OutputDebugString( "Caught (NetworkException& exNet)\n" );
	}
	catch(DeviceStatusArray& array)
	{

		print_devicestatusarray_errors(array);
		OutputDebugString( "Caught (DeviceStatusArray& array)\n" );
	}

	// pause before exiting
	if ( confirmExit ) {
		fprintf( stderr, "\nPress <RETURN> to exit. ");
		getchar();
	}

	return 0;
}

