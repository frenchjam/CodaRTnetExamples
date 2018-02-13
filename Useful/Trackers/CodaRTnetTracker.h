/*********************************************************************************/
/*                                                                               */
/*                                  CodaRTnetTracker.h                           */
/*                                                                               */
/*********************************************************************************/

// Interface to the CodaRTnet hardware.

#pragma once

// Disable warnings about "unsafe" functions.
#define _CRT_SECURE_NO_WARNINGS

#include "../Useful/fOutputDebugString.h"
#include "../Useful/fMessageBox.h"
#include "Trackers.h"

// RTNet C++ includes
#define NO64BIT
#include "../Include/codaRTNetProtocolCPP/RTNetClient.h"
#include "../Include/codaRTNetProtocolCPP/DeviceOptionsAlignment.h"
#include "../Include/codaRTNetProtocolCPP/DeviceOptionsCodaPacketMode.h"
#include "../Include/codaRTNetProtocolCPP/DeviceOptionsCodaMode.h"
#include "../Include/codaRTNetProtocolCPP/DeviceInfoAlignment.h"
#include "../Include/codaRTNetProtocolCPP/PacketDecode3DResult.h"
#include "../Include/codaRTNetProtocolCPP/PacketDecode3DResultExt.h"
#include "../Include/codaRTNetProtocolCPP/PacketDecodeADC16.h"
#include "../Include/codaRTNetProtocolCPP/DeviceInfoUnitCoordSystem.h"

// Exit codes for abort on errors.
// To avoid confusion with more generic errors, all are set to -2 at the moment.
// I don't yet have a nice way to coordinate exit codes across applications.
#define RTNET_CONFIGERROR	-2
#define RTNET_INITERROR		-2
#define RTNET_RETRIEVEERROR	-2

namespace PsyPhy {

class CodaRTnetTracker : public Tracker {

protected:

	// Hardwire the server IP address and port.
	char serverAddress[64];
	unsigned int serverPort;

	// Information to allow FTP to the server.
	// We use this to manipulate the alignment transformations.
	char executablesPath[FILENAME_MAX];				// Path on the local machine to executables such as WinSCP.exe.
	char serverLogonID[64];				// Login ID and password for FTP to the CodaRTNet server.
	char serverPassword[64];
	char codaCalDirectory[FILENAME_MAX];				// Location and filename of where the alignment resides on the server.
	char codaAlignmentFilename[FILENAME_MAX];
	char codaSerialNumber[MAX_UNITS][16];			// Serial numbers of the CODA cx1 units. These are hard coded for now but we should read them somehow.

	// Marker tracker device.
	const int cx1Device;	// Should be the CX1

	// How many tries to get a data packet before giving up.
	int maxRetries;

	// Flag to keep track of overrunning the acquisition time.
	bool overrun;

	// Helper function to print network connection error codes (client-side errors)
	void print_network_error(const codaRTNet::NetworkException& exNet);

	// Helper function to print device error codes (server-side errors)
	void print_devicestatusarray_errors(const codaRTNet::DeviceStatusArray& status);

	// Helper function to print system alignment status
	int print_alignment_status(const DWORD* marker_id_array, const codaRTNet::DeviceInfoAlignment& info, DWORD MB );

	//* Generic data packet
	codaRTNet::RTNetworkPacket packet;

	// client connection object
	codaRTNet::RTNetClient cl;

	int codaConfig;
	codaRTNet::DeviceOptionsCodaMode coda_mode;
	codaRTNet::DeviceOptionsCodaPacketMode packet_mode;
	int packetsPerFrame;

	// decoder objects
	codaRTNet::PacketDecode3DResultExt	decode3D;		// 3D measurements (CX1)
	codaRTNet::PacketDecodeADC16		decodeADC;		// 16-bit ADC measurements (GS16AIO)

	// Various objects
	codaRTNet::AutoDiscover	discover;


  // Holds information about the different configurations defined on the CODA system.
  // I am guessing that there will only be one. Ideally, though, one might define
  // three different configurations, one with both Coda units active, the other 
  // two with each of the Codas working in isolation.
	codaRTNet::HWConfigEnum	configs;
	codaRTNet::DataStream		stream;
	CODANET_HWCONFIG_DEVICEENABLE devices;

protected:

public:

	CodaRTnetTracker( void ) : 

	  	// Marker acquistion rate (200Hz), down sampling (none) and external sync (no).
		coda_mode( CODANET_CODA_MODE_200, 1, false ), 

		// Request marker data from each Coda unit separately.
		packet_mode( CODANET_CODAPACKETMODE_SEPARATE_COORD ),	

		// Selects which of the configurations, as defined on the server, to be used.
		// This has to be set up as a cx1 only configuration on the server.
		// Someday I will think about how to allow for other devices as well, e.g. the ADC.
		codaConfig(0), 
		// A Coda RTnet configuration can include cx1 devices, ADC, force platforms, etc.
		// This is just a constant specifying the cx1 device.
		cx1Device(1),
		// This determines how many times we try to get a failed packet before giving up.
		maxRetries(5)
	
	{
		// Host address and UDP port for the Coda RTnet server.
	    // The following addresss is for the RTnet server on DEX via the ETD port.
		strncpy( serverAddress, "10.80.12.103", sizeof( serverAddress ) );
		serverPort = 10111;

		// FTP parameters
		strncpy( serverLogonID, "administrator", sizeof( serverLogonID ) );
		strncpy( serverPassword, "dex", sizeof( serverPassword ) );
		strncpy( codaCalDirectory, "CodaMotion\\RTNet\\Binaries\\", sizeof( codaCalDirectory ) );
		strncpy( codaAlignmentFilename, "codaRTModuleCX1-Alignment.dat", sizeof( codaAlignmentFilename ) );
		strncpy( executablesPath, "Utils\\", sizeof( executablesPath ) );

		// Define the CODA cx1 serial numbers.
		// I set them to "0000" here and check later to see if the correct values have been
		//  initialized by the presence of a .ini file.
		for ( int unit = 0; unit < MAX_UNITS; unit++ ) strncpy( codaSerialNumber[unit], "0000", sizeof( codaSerialNumber[unit] ) );
	}

protected: 

	// Provide the means to read a .ini file to set configuration parameters.
	// This is defined here as static because its address is sent as a callback to a parsing routine.
	static int iniHandler( void *which_instance, const char* section, const char* name, const char* value ) {
		CodaRTnetTracker *instance = (CodaRTnetTracker *) which_instance;
		for ( int unit = 0; unit < MAX_UNITS; unit ++ ) {
			char unit_name[32];
			sprintf( unit_name, "CX1SerialNumber%d", unit );
			if ( !strcmp( section, "CodaRTnet" ) && !strcmp( name, unit_name )) {
				strncpy( instance->codaSerialNumber[unit], value ,  sizeof( instance->codaSerialNumber[unit] ));
				fOutputDebugString( "CodaRTnet:  Serial Number %d = %s\n", unit, instance->codaSerialNumber[unit] );
			}
		}
		if ( !strcmp( section, "CodaRTnet" ) && !strcmp( name, "CodaServerHost" )) strncpy( instance->serverAddress, value, sizeof( instance->serverAddress ));
		if ( !strcmp( section, "CodaRTnet" ) && !strcmp( name, "CodaServerPort" )) instance->serverPort = atoi( value );
		if ( !strcmp( section, "CodaRTnet" ) && !strcmp( name, "CodaServerLogin" )) strncpy( instance->serverLogonID, value, sizeof( instance->serverLogonID ));
		if ( !strcmp( section, "CodaRTnet" ) && !strcmp( name, "CodaServerPassword" )) strncpy( instance->serverPassword, value, sizeof( instance->serverPassword ));
		if ( !strcmp( section, "CodaRTnet" ) && !strcmp( name, "LocalExecutablesPath  " )) strncpy( instance->executablesPath, value, sizeof( instance->executablesPath  ));

		return 1;
	}

public:
	virtual void Initialize( const char *ini_filename = "CodaRTnet.ini" );
	virtual int  Update( void );
	virtual void Quit( void );

	virtual void StartAcquisition( float max_duration );
	virtual void StopAcquisition( void );
	virtual void AbortAcquisition( void );
	virtual bool CheckAcquisitionOverrun( void );

	virtual bool GetAcquisitionState( void );
	virtual int  GetNumberOfCodas( void );

	virtual int		RetrieveMarkerFramesUnit( MarkerFrame frames[], int max_frames, int unit );
	virtual int		RetrieveMarkerFrames( MarkerFrame frames[], int max_frames );
	virtual bool	GetCurrentMarkerFrameUnit( MarkerFrame &frame, int unit );

	virtual int		PerformAlignment( int origin, int x_negative, int x_positive, int xy_negative, int xy_positive, bool force_show = true );
	virtual void	AnnulAlignment( const char *filename = nullptr );
	virtual void	GetAlignmentTransforms( Vector3 offset[MAX_UNITS], Matrix3x3 rotation[MAX_UNITS] );
	virtual void	SetAlignmentTransforms( Vector3 offset[MAX_UNITS], Matrix3x3 rotation[MAX_UNITS], const char *filename );
	virtual void	SetAlignmentTransforms( Vector3 offset[MAX_UNITS], Matrix3x3 rotation[MAX_UNITS] );
	
	// This is a little different from the above. If we have the pose of an object in the intrinsic frame,
	// the transformation has to be inverted before sending it to the CODA system.
	virtual void	SetAlignmentFromPoses( Pose pose[MAX_UNITS], const char *filename );

	virtual void	GetUnitPlacement( int unit, Vector3 &pos, Quaternion &ori ) {
		fMessageBox( MB_OK, "CodaRTnetTracker", "GetUnitPlacement() not yet implemented." );
	}
	virtual void	GetUnitTransform( int unit, Vector3 &offset, Matrix3x3 &rotation );
	virtual void	SetUnitTransform( int unit, Vector3 &offset, Matrix3x3 &rotation );

protected:
	// These are used internally to start and stop the CODA system, e.g. when setting a new alignment.
	virtual void	Startup( void );
	virtual void	Shutdown( void );

};

};
