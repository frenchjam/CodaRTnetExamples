#pragma once

#include "codartprotocol_cx1.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 * Functions                                                           *
 ***********************************************************************/

/**codanet_deviceoptions_codamode
   Summary
   Set acquisition rate, decimation factor, and internal/external sync options
	 for CX1 system.
   
   Description
	 Use this prior to acquisition to configure options for a CX1 system.
	 Note that choice of rate affects the maximum number of markers which
	 can be acquired.

	 After executing this function the status array should be
   checked for any errors, and codanet_devicestatusarray_free
   should be called to free memory resources. 

   @param skt     Connection to server made using codanet_connect
   @param status  Pointer to a structure which will be filled with
                  any error/status information
	 @param wDevice Identity of CX1 device.  Should be CX1_DEVICE_ID.
	 @param dwRateMode The frame rate to use for acquisitions.  Must be one of the following:
											CODANET_CODA_MODE_100 
											CODANET_CODA_MODE_120 
											CODANET_CODA_MODE_200 
											CODANET_CODA_MODE_400 
											CODANET_CODA_MODE_800 

	 @param dwDecimation Decimation factor.  Actual frame rate is the rate set by dwRateMode
	                     divided by this decimation factor.
	 @param dwExternalSync Set to zero to use normal (internal sync), or 1 to use
	                       an external sync source.
   
   @return Non-zero error code if a network error occured, zero
   \otherwise. Use codanet_getlasterror() to retrieve network
   error codes.                                                         */
int    CODANET_API codanet_deviceoptions_codamode(CODANET_SOCKET skt, struct CODANET_DEVICESTATUS_ARRAY* status, WORD wDevice, DWORD dwRateMode, DWORD dwDecimation, DWORD dwExternalSync);

/**codanet_deviceoptions_alignment
   Summary
   Instruct CX1 system to perform an alignment.
   
   Description
	 Markers must be placed in the field of view to define the XY coordinate plane.
	 This function will acquire data and attempt to use the results to 
	 compute coordinate transformations between each CX1's reference frame
	 and the room coordinate system defined by the markers.

	 Acquisition mode will temporarily be set to 100Hz using internal sync.
	 This allows the maximum number of marker identities to be used.

	 After executing this function the status array should be
   checked for any errors, and codanet_devicestatusarray_free
   should be called to free memory resources. 

	 For diagnostic information in case of alignment failure, 
	 use codanet_deviceinfo_get_alignment.

   @param skt     Connection to server made using codanet_connect
   @param status  Pointer to a structure which will be filled with
                  any error/status information
	 @param wDevice Identity of CX1 device.  Should be CX1_DEVICE_ID.

	 @param dwMarkerOrigin Identity of origin marker (1-based)
	 @param dwMarkerX0 Identity of marker defining start of X-Axis vector (1-based)
	 @param dwMarkerX1 Identity of marker defining end of X-Axis vector (1-based)
	 @param dwMarkerXY0 Identity of marker defining start of XY-Plane vector (1-based)
	 @param dwMarkerXY1 Identity of marker defining end of XY-Plane vector (1-based)
   
   @return Non-zero error code if a network error occured, zero
   \otherwise. Use codanet_getlasterror() to retrieve network
   error codes.                                                         */
int    CODANET_API codanet_deviceoptions_alignment(CODANET_SOCKET skt, struct CODANET_DEVICESTATUS_ARRAY* status, WORD wDevice, DWORD dwMarkerOrigin, DWORD dwMarkerX0, DWORD dwMarkerX1, DWORD dwMarkerXY0, DWORD dwMarkerXY1);


/* 
   Description
	 When the Codamotion Realtime Server starts, the registry is scanned to determine
	 all possible CX1 connection ports on the system.  However it may be that not all of these
	 are connected to CX1 units, or that only a subset of the connected units are to be used.

	 The entire range of available ports can be retrieved using codanet_deviceinfo_cx1ports,
	 and this function used to set which ports to enable/disable.

	 After executing this function the status array should be
   checked for any errors, and codanet_devicestatusarray_free
   should be called to free memory resources. 

   @param skt     Connection to server made using codanet_connect
   @param status  Pointer to a structure which will be filled with
                  any error/status information
	 @param wDevice Identity of CX1 device.  Should be CX1_DEVICE_ID.
	 @param dwConfigHandle Hardware configuration handle.  Must be one from the list
	                       obtained used codanet_hwconfig_enumerate.

	 @param enable  Array of DWORD values.  The number of elements in the array
	                must be equal to the num ber of ports (dwNumPorts) as
									retrieved using codanet_deviceinfo_cx1ports.  Set to zero
									to disable or 1 to enable the corresponding port.
   
   @return Non-zero error code if a network error occured, zero
   \otherwise. Use codanet_getlasterror() to retrieve network
   error codes.                                                         */
/* int    CODANET_API codanet_hwconfig_cx1ports(CODANET_SOCKET skt, struct CODANET_DEVICESTATUS_ARRAY* status, WORD wDevice, DWORD dwConfigHandle, DWORD* enable);*/

/**codanet_deviceinfo_get_alignment
   Summary
   Retrieve diagnostic information for recent alignment procedure.
   
   Description
	 Use this function after a call to codanet_deviceoptions_alignment,
	 to retrieve diagnostic information in a CODANET_DEVICEINFO_ALIGNMENT
	 structure.

	 After executing this function the status array should be
   checked for any errors, and codanet_devicestatusarray_free
   should be called to free memory resources. 

   @param skt     Connection to server made using codanet_connect
   @param status  Pointer to a structure which will be filled with
                  any error/status information
	 @param wDevice Identity of CX1 device.  Should be CX1_DEVICE_ID.

	 @param info  Pointer to CODANET_DEVICEINFO_ALIGNMENT structure
	                to be filled with diagnostic information.
   
   @return Non-zero error code if a network error occured, zero
   \otherwise. Use codanet_getlasterror() to retrieve network
   error codes.                                                         */
int    CODANET_API codanet_deviceinfo_get_alignment(CODANET_SOCKET skt, struct CODANET_DEVICESTATUS_ARRAY* status, WORD device, struct CODANET_DEVICEINFO_ALIGNMENT* info);

#ifdef __cplusplus
}
#endif
