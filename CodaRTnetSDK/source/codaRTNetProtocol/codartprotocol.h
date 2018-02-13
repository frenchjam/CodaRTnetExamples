/***********************************************************************
 * codaRTNetProtocol                                                   *
 * network interface definitions for realtime comms                    *
 ***********************************************************************/

#ifndef CODARTPROTOCOL_H
#define CODARTPROTOCOL_H


#include "codartapi.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 * Network Protocol Commands                                           *
 * Numeric values for service requests to the server                   *
 ***********************************************************************/

#define CODANET_DEVICESENUMERATE         1
#define CODANET_STARTUP                  2
#define CODANET_SHUTDOWN                 3
#define CODANET_GETDEVICEINFO            4
#define CODANET_SETDEVICEOPTIONS         5

#define CODANET_HWCONFIGCREATE           6
#define CODANET_HWCONFIGREMOVE           7
#define CODANET_HWCONFIGSETOPTIONS       8
#define CODANET_HWCONFIGGETOPTIONS       9
#define CODANET_HWCONFIGENUMERATE       10
#define CODANET_HWCONFIGSETDEVICEENABLE 11
#define CODANET_HWCONFIGGETDEVICEENABLE 12
#define CODANET_GETRUNNINGHWCONFIG      13

#define CODANET_GETSERVERVERSION       101

#define CODANET_ACQPREPARE             201
#define CODANET_ACQSINGLESHOT          202
#define CODANET_ACQCONTINUOUS          203
#define CODANET_ACQCONTINUOUSBUFFERED  204
#define CODANET_ACQBUFFERMONITOR       205
#define CODANET_ACQSTOP                206
#define CODANET_ACQINPROGRESS          207
#define CODANET_DEVICETICKSECONDS      208
#define CODANET_ACQBUFFERNUMPACKETS    209
#define CODANET_ACQBUFFERREQUESTPACKET 210
#define CODANET_ACQSETMAXTICKS         211
#define CODANET_ACQGETMAXTICKS         212

#define CODANET_DATASTREAM_ADD        1001

#define CODANET_RESERVED_0            6666

/***********************************************************************
 * Network Protocol Constants                                          *
 ***********************************************************************/

#define CODANET_DEVICESTATUS_MAXITEMS 256    /** Maximum device status return codes after request to RTNet server. */
#define CODANET_DEVICEENUM_MAXITEMS 256			 /** Maximum number of devices. */
#define CODANET_HWCONFIG_MAXITEMS 256				 /** Maximum number of hardware configurations. */
#define CODANET_HWCONFIG_MAXNAMELENGTH 255   /** Maximum characters in name of hardware configuration, excluding null terminator. */
#define CODANET_ACQ_UNLIMITED 0xFFFFFFFFUL   /** Constant used to indicate unlimited acquisition time */

/***********************************************************************
 * Network Protocol Data Structures                                    *
 ***********************************************************************/

typedef struct CODANET_DEVICEENUM
{
	DWORD dwNumDevices;
	WORD wDeviceID[CODANET_DEVICEENUM_MAXITEMS];
} tagCODANET_DEVICEENUM;

typedef struct CODANET_HWCONFIGENUM
{
	DWORD dwNumConfig;
	struct
	{
		DWORD dwAddressHandle;
		wchar_t strName[CODANET_HWCONFIG_MAXNAMELENGTH+1];
	} config[CODANET_HWCONFIG_MAXITEMS];
} tagCODANET_HWCONFIGENUM;

/***********************************************************************/

typedef struct CODANET_DEVICEOPTIONS_BASE
{
	DWORD dwSize;
	DWORD dwCommand;
	WORD wDevice;
	WORD wOptionID;
} tagCODANET_DEVICEOPTIONS_BASE;

/***********************************************************************/

typedef struct CODANET_HWCONFIG_BASE
{
	DWORD dwSize;
	DWORD dwCommand;
	WORD wDevice;
	WORD wOptionID;
	DWORD dwConfig;
} tagCODANET_HWCONFIG_BASE;

typedef struct CODANET_HWCONFIG_REQUEST
{
	DWORD dwSize;
	DWORD dwCommand;
	WORD wDevice;
	WORD wOptionID;
	DWORD dwConfig;
} tagCODANET_HWCONFIG_REQUEST;

/***********************************************************************/

typedef struct CODANET_HWCONFIG_DEVICEENABLE
{
	DWORD dwSize;
	DWORD dwCommand;
	DWORD dwConfig;
	DWORD dwNumOptions;
	struct 
	{
		WORD wDevice;
		WORD wEnable;
	} option[CODANET_DEVICEENUM_MAXITEMS];
} tagCODANET_HWCONFIG_DEVICEENABLE;

/***********************************************************************/

typedef struct CODANET_PACKET_REQUEST
{
	DWORD dwSize;
	DWORD dwCommand;
	WORD wDevice;
	WORD wReserved;
	DWORD dwPacketIndex;
} tagCODANET_PACKET_REQUEST;

/***********************************************************************/

typedef struct CODANET_MAXTICKS_REQUEST
{
	DWORD dwSize;
	DWORD dwCommand;
	WORD wTickDevice;
	WORD wReserved;
	DWORD dwMaxTicks;
} tagCODANET_MAXTICKS_REQUEST;

/***********************************************************************/

typedef struct CODANET_ACQINFO_REQUEST
{
	DWORD dwSize;
	DWORD dwCommand;
	WORD wDevice;
	WORD wReserved;
} tagCODANET_ACQINFO_REQUEST;

/***********************************************************************/

typedef struct CODANET_DEVICEINFO_REQUEST
{
	DWORD dwSize;
	DWORD dwCommand;
	WORD wDevice;
	WORD wInfoID;
} tagCODANET_DEVICEINFO_REQUEST;

typedef struct CODANET_DEVICEINFO_BASE
{
	WORD wDevice;
	WORD wInfoID;
} tagCODANET_DEVICEINFO_BASE;

typedef struct CODANET_DEVICEINFO_FLOAT
{
	struct CODANET_DEVICEINFO_BASE base;
	float x;
} tagCODANET_DEVICEINFO_FLOAT;

typedef struct CODANET_DEVICEINFO_DWORD
{
	struct CODANET_DEVICEINFO_BASE base;
	DWORD n;
} tagCODANET_DEVICEINFO_DWORD;

/** Summary
		Represents RTNet server-side error state on a particular device or device subsystem

		Description
		If an error occurs on any device or subsystem during an interaction with the RTNet server,
		a structure of this type will be created and returned to the client as part of a
		CODANET_DEVICESTATUS_ARRAY structure.
  */
typedef struct CODANET_DEVICESTATUS
{
	/** The device on which the error occured, or zero to indicate an error
	    which applies to all devices. */
	WORD deviceID;

	/** The subsystem of the device on which the error occured if known, or zero otherwise.
	    The meaning of the subsystemID number is device-dependent and state-dependent.
			Prior to being started, Codamotion CX1 systems give a subsystem ID which refers to 
			the serial port on which the error occured.  Once started the subsystem ID can be used
			to indicate the serial number of a CX1 unit.
			*/
	WORD subsystemID;

	/** Error code produced - see device error codes */
	DWORD error;

} tagCODANET_RT_DEVICESTATUS;

/** Summary
		Represents RTNet server-side error state on one or more devices.

		Description
		A structure of this type is retrieved from the server after every 
		interaction between server and client. It is possible for a single 
		interaction to generate more than one	error - for example an
		attempted system start may fail on more than one device.
		This structure therefore provides for an array of error information
		in the form of multiple CODANET_DEVICESTATUS structures.
	*/
typedef struct CODANET_DEVICESTATUS_ARRAY
{
	/** The number of error codes contained in this object, or zero if no error codes
	    were generated */
	DWORD numstatus;

	/** Array of CODANET_DEVICESTATUS objects.  The number of these which
	    contain any valid information is determined by the numstatus
			value. */
	struct CODANET_DEVICESTATUS status[CODANET_DEVICESTATUS_MAXITEMS];
} tagCODANET_DEVICESTATUS_ARRAY;

typedef struct CODANET_DATASTREAM_DEST
{
	DWORD dwSize;
	unsigned long dwCommand;
	unsigned long dwIP;
	unsigned short wPort;
} tagCODANET_DATASTREAM_DEST;

/** Summary
	Version information structure retrieved from RTNet server.
	
	Description
	The four members wMajor, wMinor, wBuild and wRevision
	constitute a value in the standard versioning system used
	throughout Codamotion software. They are usually presented to
	the user as a multi-digit version number of the form:
	[wMajor].[wMinor] Build [wBuild] Revision [wRevision], for
	\example: <C>Version 1.01 Build 3 Revision 1</C>.              */
typedef struct CODANET_VERSION
{
	/** Major version */
	WORD wMajor;

	/** Minor version */
	WORD wMinor;

	/** Build number */
	WORD wBuild;

	/** Revision number */
	WORD wRevision;

} tagCODANET_VERSION;

/***********************************************************************
 * Network Client Functions API                                        *
 ***********************************************************************/

/**codanet_deviceenum_clear
   Summary
   Clear CODANET_DEVICEENUM structure
   
   Description
	 The codanet_enumerate_devices function filles a CODANET_DEVICEENUM structure.
	 Use this function to clear the structure to indicate zero devices present.

   @param deviceenum     Pointer to structure for memory resources to be freed.
	 */
void   CODANET_API codanet_deviceenum_clear(struct CODANET_DEVICEENUM* deviceenum);

/**codanet_devicestatusarray_clear
   Summary
   Set device status array to zero information
   
   Description
   Several functions fill a CODANET_DEVICESTATUS_ARRAY with possible error information.
	 Use this to provide clear such a structure such that no errors are
	 flagged as present.

   @param array     Pointer to structure for memory resources to be initialised.
	 */
void   CODANET_API codanet_devicestatusarray_clear(struct CODANET_DEVICESTATUS_ARRAY* array);

/**codanet_commandstructquery
   Summary
   Send command (and optional data) to server and retrieve result.
   
   Description
	 This function should not be called directly but instead using C++ wrapper classes.
   This is the primary means of communication with the realtime server.
	 The data structure and return buffer must conform to the Codamotion RTNet protocol.

   @param skt     Connection to server made using codanet_connect
   @param status  Pointer to a structure which will be filled with
                  any error/status information
   @param dwDataSize Size of data to send (in bytes)
	 @param data    Data buffer to send (must be of at least dwDataSize bytes)
	 @param dwResultSize Maximum size for returned result (in bytes)
	 @param result  Data buffer for receiving result (must be of at least dwResultSize bytes)
   
   @return Non-zero error code if a network error occured, zero
   \otherwise. Use codanet_getlasterror() to retrieve network
   error codes.                                                         */
int    CODANET_API codanet_commandstructquery(CODANET_SOCKET skt, struct CODANET_DEVICESTATUS_ARRAY* status, DWORD dwDataSize, const void* data, DWORD dwResultSize, void* result);

/**codanet_cmd_devices_enumerate
   Summary
   Provide list of all attached device ID's
   
   Description
   Provide list of all attached device ID's. The deviceenum will be filled with
	 information corresponding to the list of devices which are accessible.

   @param skt     Connection to server made using codanet_connect
   @param deviceenum  Structure to be filled with device information
   
   @return Non-zero error code if a network error occured, zero
   \otherwise. Use codanet_getlasterror() to retrieve network
   error codes.                                                         */
int    CODANET_API codanet_cmd_devices_enumerate(CODANET_SOCKET skt, struct CODANET_DEVICESTATUS_ARRAY* status, struct CODANET_DEVICEENUM* dev);

/**codanet_cmd_hwconfig_enumerate
   Summary
   Get names of all pre-stored hardware configs available.
   
   Description
	 Get names of all pre-stored hardware configs available.
	 New items can be added with codanet_hwconfig_create.  Any one of these
	 can be selected for use using codanet_hwconfig_select, prior to startup.

   @param skt     Connection to server made using codanet_connect
   @param conf    Pointer to CODANET_HWCONFIGENUM structure to be filled with result
   
   @return Non-zero error code if a network error occured, zero
   \otherwise. Use codanet_getlasterror() to retrieve network
   error codes.                                                         */
int    CODANET_API codanet_cmd_hwconfig_enumerate(CODANET_SOCKET skt, struct CODANET_DEVICESTATUS_ARRAY* status, struct CODANET_HWCONFIGENUM* conf);

/**codanet_cmd_hwconfig_create
   Summary
   Create a new named hardware configuration
   
   Description
	 Create a new named hardware configuration.  
	 Multiple configurations can be created and stored.  Any one of these can be
	 selected for use prior to device startup.  To set the device options within
	 a particular config, use device-specific functions (e.g. codanet_hwconfig_cx1ports
	 for CX1 systems).

   @param skt     Connection to server made using codanet_connect
   @param strConfigName  Name for new configuration.  Must be unique.
   @param dwNewHandle DWORD value to be filled with handle ID of new config.
   
   @return Non-zero error code if a network error occured, zero
   \otherwise. Use codanet_getlasterror() to retrieve network
   error codes.                                                         */
int    CODANET_API codanet_cmd_hwconfig_create(CODANET_SOCKET skt, struct CODANET_DEVICESTATUS_ARRAY* status, const wchar_t* strConfigName, DWORD* dwNewHandle);

/**codanet_datastream_create
   Summary
   Create a stream to receive realtime data
   
   Description
	 Use this to create a network port on your computer which is used
	 to receive data from the server.  Once configured, data can be triggered
	 to be send to this port using one of the following:
	 codanet_cmd_acqsingleshot, codanet_cmd_acqcontinuous, or codanet_cmd_acqbufferrequestpacket.

	 After executing this function the status array should be
   checked for any errors. 

   @param sktClient Connection to server made using codanet_connect
   @param status  Pointer to a structure which will be filled with
                  any error/status information
	 @param port    Network port number for the data
   
   @return Non-zero error code if a network error occured, zero
   \otherwise. Use codanet_getlasterror() to retrieve network
   error codes.                                                         */
CODANET_SOCKET CODANET_API codanet_datastream_create(CODANET_SOCKET sktClient, struct CODANET_DEVICESTATUS_ARRAY* status, int port);


void CODANET_API codanet_hwconfig_deviceenable_hton(struct CODANET_HWCONFIG_DEVICEENABLE* opt);
void CODANET_API codanet_hwconfig_deviceenable_ntoh(struct CODANET_HWCONFIG_DEVICEENABLE* opt);

void CODANET_API codanet_hwconfig_base_hton(struct CODANET_HWCONFIG_BASE* dev);
void CODANET_API codanet_hwconfig_base_ntoh(struct CODANET_HWCONFIG_BASE* dev);

void   CODANET_API codanet_deviceinfo_dword_hton(struct CODANET_DEVICEINFO_DWORD* info);
void   CODANET_API codanet_deviceinfo_dword_ntoh(struct CODANET_DEVICEINFO_DWORD* info);
void   CODANET_API codanet_deviceinfo_float_hton(struct CODANET_DEVICEINFO_FLOAT* info);
void   CODANET_API codanet_deviceinfo_float_ntoh(struct CODANET_DEVICEINFO_FLOAT* info);

void CODANET_API codanet_deviceoptions_base_hton(struct CODANET_DEVICEOPTIONS_BASE* dev);
void CODANET_API codanet_deviceoptions_base_ntoh(struct CODANET_DEVICEOPTIONS_BASE* dev);
void CODANET_API codanet_deviceinfo_base_hton(struct CODANET_DEVICEINFO_BASE* info);
void CODANET_API codanet_deviceinfo_base_ntoh(struct CODANET_DEVICEINFO_BASE* info);

void CODANET_API codanet_hton_version(struct CODANET_VERSION* version);
void CODANET_API codanet_ntoh_version(struct CODANET_VERSION* version);

/***********************************************************************
 * codarterrorcodes                                                    *
 * device status error codes for realtime comms                        *
 ***********************************************************************/
#include "codarterrorcodes.h"

#ifdef __cplusplus
}
#endif

#endif
