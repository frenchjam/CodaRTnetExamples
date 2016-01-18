/////////////////////////////////////////////////////////////////////////////
//
// RTnetAcquisition.cpp
// Copyright (c) PsyPhy Consulting
//
// Based on Codamotion RTNet C++ Examples: runtext.cpp 
// Copyright (C) Charnwood Dynamics Ltd.
//
// All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////

// Standard library includes
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <math.h>
#include <windows.h> // For the OutputDebugString routine.

// RTNet C++ includes
#define NO64BIT
#include "codaRTNetProtocolCPP/RTNetClient.h"
#include "codaRTNetProtocol/codartprotocol_gs16aio.h"
#include "codaRTNetProtocolCPP/DeviceOptionsAlignment.h"
#include "codaRTNetProtocolCPP/DeviceOptionsCodaMode.h"
#include "codaRTNetProtocolCPP/DeviceOptionsCodaPacketMode.h"
#include "codaRTNetProtocolCPP/DeviceOptionsGS16AIOInputs.h"
#include "codaRTNetProtocolCPP/DeviceInfoUnitCoordSystem.h"
#include "codaRTNetProtocolCPP/DeviceInfoAlignment.h"
#include "codaRTNetProtocolCPP/PacketDecode3DResult.h"
#include "codaRTNetProtocolCPP/PacketDecode3DResultExt.h"
#include "codaRTNetProtocolCPP/PacketDecodeADC16.h"

#define DEVICEID_ADC DEVICEID_GS16AIO

// These need to be set to correspond to the configurations that are 
// defined on whatever server you are using.
#define CX1andADC	4
#define CX1only		0
#define ADConly		8

int			codaConfig = CX1only;
double		acqDuration = 0.5;
bool		confirmExit = false;

// Parameterize the real-time display of data.
#define NBARS 5
int nBars = NBARS;
char *barString[NBARS * 2 + 1] = {
	"OOOOO|     ",		
	" OOOO|     ",
	"  OOO|     ",
	"   OO|     ",
	"    O|     ",
	"     |     ",
	"     |O    ",
	"     |OO   ",
	"     |OOO  ",
	"     |OOOO ", 
	"     |OOOOO" 

};

// Sets the full scale of the bar graphs.
int barRange = 1024;

// Which channels to display as bar graphs.
int maxDisplayChannels = 32;
int displayChannels = 2;
int displayChannelList[32] = { 6,7 };

int maxDisplayMarkers = 28;
int displayMarker = 4;

int maxSaveChannels = 32;
int maxSaveMarkers = 28;

// Freqencies in Hertz of each acqusition type.
// They could probably be read in from the Coda Server
//  but I just set them statically here for now.
double cx1Rate = 200.0;
double adcRate = 500.0;

// Hardwire the server IP address and port.
 char serverAddress[] = "192.168.200.133";
//char serverAddress[] = "192.168.1.1";
//char serverAddress[32] = "172.20.164.7";
unsigned int serverPort = 10111;

#define FRAMES_PER_DOT	1

// How many tries to get a data packet before giving up.
int maxRetries = 1;

// Use main Codamotion RTNet namespace
using namespace codaRTNet;

// Helper function to print network connection error codes (client-side errors)
void print_network_error(const NetworkException& exNet);

// Helper function to print device error codes (server-side errors)
void print_devicestatusarray_errors(const DeviceStatusArray& status);

// Helper function to print system alignment status
void print_alignment_status(const DWORD* marker_id_array, const DeviceInfoAlignment& info);

//* Generic data packet
RTNetworkPacket packet;

// client connection object
RTNetClient cl;

DeviceOptionsCodaMode			coda_mode( CODANET_CODA_MODE_200, 1, false );
//DeviceOptionsCodaPacketMode	packet_mode = CODANET_CODAPACKETMODE_SEPARATE_AND_COMBINED_COORD;
//int packetsPerFrame = 2;
DeviceOptionsCodaPacketMode		packet_mode = CODANET_CODAPACKETMODE_COMBINED_COORD;
int packetsPerFrame = 1;
//DeviceOptionsCodaPacketMode	packet_mode = CODANET_CODAPACKETMODE_SEPARATE_COORD;
//int packetsPerFrame = 2;
DeviceOptionsGS16AIOInputs		analog_mode( adcRate );

// decoder objects
PacketDecode3DResultExt		decode3D;	// 3D measurements (CX1)
PacketDecodeADC16			decodeADC;	// 16-bit ADC measurements (GS16AIO)

// Various objects
AutoDiscover discover;
HWConfigEnum configs;
CODANET_HWCONFIG_DEVICEENABLE devices;
DataStream stream;

//////////////////////////////////////////////////////////////////////////////////////////

// Local Routines

int Acquire( int coda_config, double duration, const char *fileroot );

 
//////////////////////////////////////////////////////////////////////////////////////////

// Function main - the main program entry point

int main(int argc, char** argv) {

	char *fileroot = "RTnetAcquisition";

	fprintf( stderr, "%s\n", argv[0] );

	for ( int arg = 1; arg < argc; arg++ ) {

		if ( !strcmp( argv[arg], "-confirm" ) ) confirmExit = true;
		else if ( !strcmp( argv[arg], "-cx1" ) ) codaConfig = CX1only;
		else if ( !strcmp( argv[arg], "-adc" ) ) codaConfig = ADConly;
		else if ( !strcmp( argv[arg], "-both" ) ) codaConfig = CX1andADC;

		else if ( !strcmp( argv[arg], "-ip" ) ) {
			arg++;
			if ( arg < argc ) strncpy( serverAddress, argv[arg], sizeof(serverAddress)  );
		}

		// Argument is either the duration or the filename.
		else if ( 1 != sscanf( argv[arg], "%lf", &acqDuration ) ) fileroot = argv[arg];

	}

	try {

		// Decode the IP address string.
		unsigned int p, q, r, s;
		fprintf(stderr, "RTnet Server: %s   Port: %d  Configuration: %d\n", serverAddress, serverPort, codaConfig );
		sscanf( serverAddress, "%d.%d.%d.%d", &p, &q, &r, &s );
		// Connect to the server.
		cl.connect( (p << 24) + (q << 16) + (r << 8) + s, serverPort );

		// Check that the server has at least the expected number of configurations.
		cl.enumerateHWConfig(configs);

		// Print config names.
		fprintf(stderr, "Found %u hardware configurations:\n", configs.dwNumConfig);
		for(DWORD iconfig = 0; iconfig < configs.dwNumConfig; iconfig++)
			fwprintf(stderr, L"  [%u] address: %08X  name: %s\n", 
			iconfig, configs.config[iconfig].dwAddressHandle, configs.config[iconfig].strName);

		if (configs.dwNumConfig < codaConfig)
		{
			fprintf(stderr, "ERROR: specified hardware configuration %d not available.\n", codaConfig);
			return 1;
		}

		// Show which configuration we are trying to use.
		fwprintf( stderr, L"RTnet Configuration: %s\n", configs.config[codaConfig].strName );

		// If system is already started, this does nothing.
		// Otherwise it will load the specified configuration.
		// We assume that the configuration constants defined here correspond
		//  to the actual configurations defined on the server..
		cl.startSystem( configs.config[codaConfig].dwAddressHandle );
		// These sets the acquisition frequence, the decimation and the external sync.
		if ( codaConfig != ADConly ) {
			cl.setDeviceOptions( coda_mode );
			// This is supposed to say whether we get one packet of combined data,
			// one packet from each coda, or both.
			cl.setDeviceOptions( packet_mode );
			// Find out how many Coda units are actually in use.
			// I don't really need the alignment information, but that structure
			// includes the number of Codas specified in the configuration that is in use.
			// So we do a bogus alignment.
			DeviceOptionsAlignment align(1, 1, 1, 1, 1);
			cl.setDeviceOptions( align );
			// Then this is what tells us how many units are there.
			DeviceInfoAlignment align_info;
			cl.getDeviceInfo( align_info );
			fprintf( stderr, "Number of connected cx1 units: %d\n", align_info.dev.dwNumUnits );
		}
		if ( codaConfig != CX1only ) cl.setDeviceOptions( analog_mode );

		



		// create data stream
		cl.createDataStream(stream, 7000);

		// ensure socket is up before starting acquisition
		codanet_sleep(50);

		Acquire( codaConfig, acqDuration, fileroot );

	}
	catch(NetworkException& exNet)
	{
		print_network_error(exNet);
		OutputDebugString( "Caught (NetworkException& exNet)\n" );
		if ( !confirmExit ) getchar();
	}
	catch(DeviceStatusArray& array)
	{
		print_devicestatusarray_errors(array);
		OutputDebugString( "Caught (DeviceStatusArray& array)\n" );
		if ( !confirmExit ) getchar();
	}

	// pause before exiting
	if ( confirmExit ) {
		fprintf( stderr, "\nPress <RETURN> to exit. ");
		getchar();
	}

	return( 0 );

}

//////////////////////////////////////////////////////////////////////////////////////////

int Acquire( int coda_config, double duration, const char *fileroot ) {
	
	bool expect_cx1, expect_adc;
	int nTimeouts, nChecksumErrors, nUnexpectedPackets, nFailedPackets;
	int idevice;
	
	char mrk_line[256] = "";
	
	bool skip_retrieval = false;
	
	static int repetition = 0;
	
	// Define which subsystems will be used according to the selected configuration.
	// Limit acquisition to a set number of samples.
	
	switch ( coda_config ) {
	case ADConly:
		cl.setAcqMaxTicks(DEVICEID_ADC, floor( duration * adcRate ) );
		expect_cx1 = false;
		expect_adc = true;
		break;
	case CX1only:
		cl.setAcqMaxTicks(DEVICEID_CX1, floor( duration * cx1Rate ));
		expect_cx1 = true;
		expect_adc = false;
		break;
	default:
		cl.setAcqMaxTicks(DEVICEID_CX1, floor( duration * cx1Rate ));
		expect_cx1 = true;
		expect_adc = true;
		break;
	}
	
	OutputDebugString( "Start Acquire\n" );
	
	// prepare for acquisition
	OutputDebugString( "cl.prepareForAcq()\n" );
	cl.prepareForAcq();
	
	// start acq (and buffer)
	OutputDebugString( "cl.startAcqContinuousBuffered()\n" );
	cl.startAcqContinuousBuffered();
	fprintf( stderr, "Acquisition %d started.  Duration: %.2f\n", ++repetition, duration );
	
	// monitor
	while ( cl.isAcqInProgress() ) // Will run until preset numbe of ticks are acquired.
	{
		// check for pending keystroke
		if (_kbhit() != 0)
		{
			int ch = getch();
			// Stop acquisition early on any key.
			// If it is one of the escape keys, skip the subseqent retrieval.
			if (ch == 'q' || ch == 'Q' || ch == 27) skip_retrieval = true;
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
			OutputDebugString( "Caught error from cl.monitorAcqBuffer() - Ignored\n" );
		}
		
		// process packets from all devices
		//* Charnwood version makes no assumptions about how many devices, and therefore how many packets
		//*  will arrive and need to be processed. So they try for packets until they get a timeout.
		//* In my version we assume that there will be 2 packets, one from the CX1, one from the ADC.
		//* When both have been received, we can move on without waiting any longer.
		//* This should allow rapid responses to movements of the markers.
		//* Be careful, though, because if the loop runs fast enough you will probably get multiple 
		//*  packets from the same clock tick.
		
		bool received_cx1 = false;
		bool received_adc = false;
		do {
			// receive data - wait for max 50ms
			if ( stream.receivePacket(packet, 50000) == CODANET_STREAMTIMEOUT )
			{
				//* A timeout means that we didn't get one of the two packets that we expected.
				//* Just break out of the loop so as to request another pair.
				break;
			}
			
			// check result
			if ( decode3D.decode(packet) )
			{
				float* pos;
				
				// find number of marker positions available
				DWORD nMarkers = decode3D.getNumMarkers();
				
				// limit the number of markers 
				// (edit maxDisplayMarkers to change this)
				if (nMarkers > maxDisplayMarkers) nMarkers = maxDisplayMarkers;
				
				// loop through one or more markers (set maxDisplayMarkers to do more than one)
				char visibility_string[64] = "";
				for ( int imarker = 0; imarker < nMarkers; imarker++ )
				{
					pos = decode3D.getPosition(imarker);
					BYTE valid = decode3D.getValid(imarker);
					BYTE* intensity = decode3D.getIntensity(imarker);
					
					if ( valid ) strcat( visibility_string, "O" );
					else strcat( visibility_string, "-" );
					
					if ( !((imarker + 1) % 8) ) strcat( visibility_string, " " );
					
				}
				pos = decode3D.getPosition( displayMarker );
				sprintf( mrk_line, "%05u %02d: [%6.0f %6.0f %6.0f] %s", 	
					decode3D.getTick(), displayMarker, pos[0], pos[1], pos[2], visibility_string );
				
				//* Signal that we got the CX1 packet.
				received_cx1 = true;
			}
			else if ( packet.verifyCheckSum() )
			{
				
				// decode & print results
				if ( decodeADC.decode(packet) )
				{
					// get total analog values available
					int nChannels = decodeADC.getNumValues();
					
					// limit the number of analog channels
					// (edit the maxDisplayChannels to change this)
					if (nChannels > maxDisplayChannels) nChannels = maxDisplayChannels;
					
					// loop through one or more analog channels and display as bar meter.
					//					if ( !expect_cx1 || !received_cx1 ) fprintf(stderr, "\r                                                    / " );
					fprintf(stderr, "\r%s    %08u ADC: ", mrk_line, decodeADC.getTick() );
					for (DWORD ichannel = 0; ichannel < displayChannels; ichannel++)
					{
						const short* value = decodeADC.getValues();
						int bars = value[ displayChannelList[ichannel] ] * nBars / barRange + nBars;
						if ( bars < 0 ) bars = 0;
						if ( bars > 2 * nBars ) bars = 2 * nBars;
						fprintf(stderr, " %2d [%s]  ", displayChannelList[ichannel], barString[bars] );
					}
					//* Signal that we received the ADC packet.
					received_adc = true;
				}
			}
			else
			{
				fprintf(stderr, "%08u ADC: checksum failed", decodeADC.getTick());
			}
		} while( 1 );
		//		} while ( (expect_cx1 && !received_cx1) || (expect_adc && !received_adc) ); //* If expected packets received, move on.
		//		// Flush any other packets that might come in.
		//		while ( stream.receivePacket(packet, 1000) != CODANET_STREAMTIMEOUT ) fprintf( stderr, "Flush %8d.\n", nFlushed++ ); 
	}
	
	// stop acquisition if still in progress
	OutputDebugString( "Stop acquisition\n" );
	if ( cl.isAcqInProgress() ) cl.stopAcq();
	
	// If the acquisition was terminated with 'q', 'Q' or <ESC>, just exit.
	if ( skip_retrieval ) {
		OutputDebugString( "Skipping Retrieval\n" );
		return( -1 );
	}
	
	OutputDebugString( "Start Retrieval\n" );
	int nFrames;
	int nSamples; 
	int frame_index;
	
	if ( expect_cx1 ) {
		char filename[1024];
		FILE *fp;
		sprintf( filename, "%s.mrk", fileroot );
		if ( !(fp = fopen( filename, "w" )) ) {
			char message[2048];
			sprintf( message, "Error opening file for writing:\n %s", filename );
			MessageBox( NULL, message, "RTnetAcquisition", MB_OK | MB_ICONEXCLAMATION );
			exit( -1 );
		}
		else {
			fprintf( stderr, "\nFile %s open for writing.", filename );
		}
		
		
		nChecksumErrors = 0;
		nTimeouts = 0;
		nUnexpectedPackets = 0;
		nFailedPackets = 0;
		
		//*
		//* Retrieve the stored CX1 data.
		//*
		idevice = 1;	// Should be the CX1
		//* Find out how many ticks worth of data are stored for this device.
		nFrames= cl.getAcqBufferNumPackets( idevice );
		fprintf( stderr, "\nStart retrieval of Marker data (%d packets) .", nFrames );
		//* Loop through and get them all.
		for ( frame_index = 0; frame_index < nFrames; frame_index ++ ) {

			for ( int pkt = 0; pkt < packetsPerFrame; pkt++ ) {

				//* Try to read the packet. Normally it should get here the first try.
				//* But theoretically, it could get lost. So if we get a time out, we 
				//*  should try again.
				for ( int retry = 0; retry < maxRetries; retry++ ) {
					//* Tell the server to send the specified packet.
					try
					{
						cl.requestAcqBufferPacket( idevice, frame_index );
					}
					catch(DeviceStatusArray&)
					{
						OutputDebugString( "Caught error from cl.requestAcqBufferPacket()\n" );
					}
					//* Try to read it in. If it is good, break out of the retry loop.
					if ( stream.receivePacket(packet, 50000) != CODANET_STREAMTIMEOUT ) {
						if ( packet.verifyCheckSum() ) {
							if ( decode3D.decode( packet ) ) break;
							else nUnexpectedPackets++;
						}
						else nChecksumErrors++;
					}
					else nTimeouts++;
				}
				if ( retry >= maxRetries ) {
					nFailedPackets++;
					fprintf(fp, "%08u\tfailed\n", frame_index );
				}
				else {

					//* Process the frame full of data.
					
					// find number of marker positions available
					DWORD nMarkers = decode3D.getNumMarkers();
					
					// limit the number of markers 
					// (edit maxDisplayMarkers to change this)
					if (nMarkers > maxSaveMarkers) nMarkers = maxSaveMarkers;
					
					fprintf(fp, "%08u\t%08u", frame_index, decode3D.getTick() );
					for (int imarker = 0; imarker < nMarkers; imarker++)
					{
						float* pos = decode3D.getPosition(imarker);
						BYTE valid = decode3D.getValid(imarker);
						BYTE* intensity = decode3D.getIntensity(imarker);
						fprintf( fp, " %1d %6.2f %6.2f %6.2f", valid, pos[0], pos[1], pos[2] );
						
					}
					fprintf( fp, "\n" );
				}
				if ( !( frame_index % FRAMES_PER_DOT ) ) fprintf( stderr, "." );
			}
		}
		fclose( fp );
#if 1
		// Check if more packets are there. If so, then something is wrong.
		// This is here just for debugging at the moment.
		int nFlushed = 0;
		while ( stream.receivePacket(packet, 50000) != CODANET_STREAMTIMEOUT ) {
			decode3D.decode( packet );
			fprintf( stderr, "\nFlushed Packet: %8d. Count: %8d\n", decode3D.getTick(), nFlushed++ );
		}
#endif


		fprintf( stderr, "\nFile %s closed.\n", filename );
		fprintf( stderr, "Frames: %d  Failed Packets: %d  Timeouts: %d  Checksum Errors: %d  Unexpected Packets: %d\n",
			nFrames, nFailedPackets, nTimeouts, nChecksumErrors, nUnexpectedPackets );
		if ( nFailedPackets || nTimeouts || nChecksumErrors || nUnexpectedPackets ) {
			char message[2048];
			sprintf( message, "Packet Errors:\n\nPackets: %d\nFailed Packets: %d\nTimeouts: %d\nChecksum Errors: %d\nUnexpected Packets: %d",
				nFrames, nFailedPackets, nTimeouts, nChecksumErrors, nUnexpectedPackets );
			MessageBox( NULL, message, "RTnetAcquisition", MB_OK | MB_ICONEXCLAMATION );
		}
	}
	
	
	if ( expect_adc ) {
		//*
		//* Retrieve the stored ADC data.
		//*
		char filename[1024];
		FILE *fp;
		sprintf( filename, "%s.adc", fileroot );
		if ( !(fp = fopen( filename, "w" )) ) {
			char message[2048];
			sprintf( message, "Error opening file for writing:\n %s", filename );
			MessageBox( NULL, message, "RTnetAcquisition", MB_OK | MB_ICONEXCLAMATION );
			exit( -1 );
		}
		else {
			fprintf( stderr, "\nFile %s open for writing.", filename );
		}
		
		
		fprintf( stderr, "\nStart retrieval of ADC data ." );
		nChecksumErrors = 0;
		nTimeouts = 0;
		nUnexpectedPackets = 0;
		nFailedPackets = 0;
		idevice = 3;	// Should be the ADC
		//* Find out how many ticks worth of data are stored for this device.
		nSamples = cl.getAcqBufferNumPackets( idevice );
		//* Loop through and get them all.
		for ( int packetindex = 0; packetindex < nSamples; packetindex ++ ) {
			//* Try to read the packet. Normally it should get here the first try.
			//* But theoretically, it could get lost. So if we get a time out, we 
			//*  should try again.
			for ( int retry = 0; retry < maxRetries; retry++ ) {
				//* Tell the server to send the specified packet.
				//					cl.requestAcqBufferPacket( idevice, packetindex);
				try
				{
					cl.requestAcqBufferPacket( idevice, packetindex);
				}
				catch(DeviceStatusArray&)
				{
				/*... ignore status information 
					errors will just cause codanet_packet_receive to time out... */
					OutputDebugString( "Caught error from cl.requestAcqBufferPacket()\n" );
				}
				//* Try to read it in. If it is good, break out of the retry loop.
				if ( stream.receivePacket(packet, 50000) != CODANET_STREAMTIMEOUT ) {
					if ( packet.verifyCheckSum() ) {
						if ( decodeADC.decode( packet ) ) break;
						else nUnexpectedPackets++;
					}
					else nChecksumErrors++;
				}
				else nTimeouts++;
			}
			if ( retry >= maxRetries ) {
				nFailedPackets++;
				fprintf(fp, "%08u\tfailed\n", packetindex );
			}
			else {
				//* Process the frame full of data.
				//* One could write the data directly to a file, for instance.
				// get total analog values available
				const short* value = decodeADC.getValues();
				int nChannels = decodeADC.getNumValues();
				if ( nChannels > maxSaveChannels ) nChannels = maxSaveChannels;
				
				fprintf(fp, "%08u\t%08u", packetindex, decodeADC.getTick() );
				
				for ( int channel = 0; channel < nChannels; channel++ ) {
					fprintf( fp, " %6d", value[channel] );
				}
				fprintf( fp, "\n" );
			}
			if ( !( packetindex % FRAMES_PER_DOT ) ) fprintf( stderr, "." );
		}
		fclose( fp );
		fprintf( stderr, "\nFile %s closed.\n", filename );
		fprintf( stderr, "Packets: %d  Failed Packets: %d  Timeouts: %d  Checksum Errors: %d  Unexpected Packets: %d\n",
			nSamples, nFailedPackets, nTimeouts, nChecksumErrors, nUnexpectedPackets );
		if ( nFailedPackets || nTimeouts || nChecksumErrors || nUnexpectedPackets ) {
			char message[2048];
			sprintf( message, "Packet Errors:\n\nSamples: %d\nFailed Packets: %d\nTimeouts: %d\nChecksum Errors: %d\nUnexpected Packets: %d",
				nSamples, nFailedPackets, nTimeouts, nChecksumErrors, nUnexpectedPackets );
			MessageBox( NULL, message, "RTnetAcquisition", MB_OK | MB_ICONEXCLAMATION );
		}
	}
	OutputDebugString( "Stop Retrieval\n" );
	return( 0 );
}

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


		

		

