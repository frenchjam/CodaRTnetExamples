//
// CodaRTnetAlignment.cpp : Perform an alignment of multiple Coda units.
//

#include "stdafx.h"

// Use main Codamotion RTNet namespace
using namespace codaRTNet;

// Include some common static variables.
#include "../CodaRTnetCommon/CodaRTnetCommon.h"

// client connection object
RTNetClient cl;

#if 0
int origin = 3;
int x_negative = 3;
int x_positive = 8;
int xy_negative = 2;
int xy_positive = 4;
#else
int origin = 1;
int x_negative = 1;
int x_positive = 2;
int xy_negative = 3;
int xy_positive = 4;
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

	fprintf(stderr, "Press <RETURN> to continue.\n");
	getchar();
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
		fprintf(stderr, "\nCoda %d:\n", icoda+1);

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
					fprintf(stderr, "   Org");
					break;
				case 1:
					fprintf(stderr, "   X0 ");
					break;
				case 2:
					fprintf(stderr, "   X1 ");
					break;
				case 3:
					fprintf(stderr, "   XY0");
					break;
				case 4:
					fprintf(stderr, "   XY1");
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
	fprintf(stderr, "\n");
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
	fprintf( stderr, "Press <RETURN> to continue. " );
	getchar();
}

//* Perform a CODA alignment.

void perform_alignment( void ) {

	fprintf( stderr, "\nOrigin: %d\nX axis: %d -> %d\nY axis: %d -> %d\n\n",
		origin, x_negative, x_positive, xy_negative, xy_positive);

	// alignment request structure
	DeviceOptionsAlignment align(origin, x_negative, x_positive, xy_negative, xy_positive);

	// perform alignment
	// this will cause system to look for alignment markers
	// - they must be in position by this time
	cl.setDeviceOptions(align);

	// retrieve information
	DeviceInfoAlignment info;
	cl.getDeviceInfo(info);

	// print alignment diagnostics
	DWORD marker_id_array[5] = { origin, x_negative, x_positive, xy_negative, xy_positive };
	print_alignment_status(marker_id_array, info);

	// exit early if alignment failed
	if (info.dev.dwStatus != 0) {
		char message[2048];
		sprintf( message, "\nOrigin: %d\nX axis: %d -> %d\nY axis: %d -> %d\n\nAlignment errors.",
			origin, x_negative, x_positive, xy_negative, xy_positive);
		MessageBox( NULL, message, "RTnetDoAlignment", MB_OK | MB_ICONEXCLAMATION );
	}
	else {
		char message[2048];
		sprintf( message, "\nOrigin: %d\nX axis: %d -> %d\nY axis: %d -> %d\n\nSuccess.",
			origin, x_negative, x_positive, xy_negative, xy_positive);
		MessageBox( NULL, message, "RTnetDoAlignment", MB_OK | MB_ICONASTERISK );
	}

}


int _tmain(int argc, _TCHAR* argv[])
{

	AutoDiscover	discover;
	HWConfigEnum	configs;
	CODANET_HWCONFIG_DEVICEENABLE devices;

	// Show what program is running.	
	fprintf( stderr, "%s\n\n", argv[0] );
	
	// Unlike the acquisition programs, that use a fixed IP address, 
	// here we look for a CODA RTnet server using the autodiscover function.
	// There is no real reason to do that here. We could use the fixed address.
	// But I leave it here as an example of what to do.
	cl.doAutoDiscoverServer(discover);

	// need at least one server to connect to
	if (discover.dwNumServers == 0)
	{
		fprintf(stderr, "ERROR: no servers found using auto-discovery.\n");
		fprintf( stderr, "Press <RETURN> to continue." );
		getchar();
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

	try {
		// connect to first available server
		cl.connect( discover.server[0].ip, discover.server[0].port );

		// get config list
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
		if ( running_config && running_config != configs.config[codaConfig].dwAddressHandle ) {
			OutputDebugString( "*** Not running the right configuration! ***\n" );
			cl.stopSystem();
			running_config = NULL;
		}

		if ( !running_config ) {

			OutputDebugString( "*** Starting up Coda system! ***\n" );

			// If system is already started, this does nothing.
			// Otherwise it will load the specified configuration.
			// We assume that the configuration constants defined here correspond
			//  to the actual configurations defined on the server..
			cl.startSystem( configs.config[codaConfig].dwAddressHandle );

			cl.setDeviceOptions( coda_mode );
			cl.setDeviceOptions( packet_mode );


			// prepare for acquisition
			OutputDebugString( "*** cl.prepareForAcq() ***\n" );
			cl.prepareForAcq();

		}

		// Show what are the alignment transformations before doing the alignment.
		DeviceInfoUnitCoordSystem pre_xforms;
		cl.getDeviceInfo( pre_xforms );


		// perform alignment 
		perform_alignment();

		// Show what are the alignment transformations after doing the alignment.
		DeviceInfoUnitCoordSystem post_xforms;
		cl.getDeviceInfo( post_xforms );

		int i = 0;

		cl.stopSystem();
		cl.disconnect();

	
	}
	catch(NetworkException& exNet)
	{
		print_network_error(exNet);
		OutputDebugString( "Caught (NetworkException& exNet)\n" );
	}
	catch(DeviceStatusArray& array)
	{
		OutputDebugString( "Caught (DeviceStatusArray& array)\n" );
		print_devicestatusarray_errors(array);
	}

	return( 0 );

	return 0;
}

