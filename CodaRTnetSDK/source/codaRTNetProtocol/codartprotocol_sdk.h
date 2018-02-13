#pragma once

#include "codartprotocol.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 * Functions                                                           *
 ***********************************************************************/

/**codanet_cmd_hwconfig_remove
   Summary
   Remove the specified hardware configuration
   
   Description
	 Use with caution - this function permanently removes a set of hardware configuration
	 options.  For a list of all available configs, use codanet_hwconfig_enumerate.

   @param skt     Connection to server made using codanet_connect
   @param dwConfigHandle Handle of config to remove.
   
   @return Non-zero error code if a network error occured, zero
   \otherwise. Use codanet_getlasterror() to retrieve network
   error codes.                                                         */
int    CODANET_API codanet_cmd_hwconfig_remove(CODANET_SOCKET skt, struct CODANET_DEVICESTATUS_ARRAY* status, DWORD dwConfigHandle);

/**codanet_cmd_hwconfig_getdeviceenable
   Summary
   Retrieve enable/disable state information for all available devices for a 
	 specified hardware configuration
   
   Description
   Retrieve enable/disable state information for all available devices for a 
	 specified hardware configuration.  The opt->dwNumOptions will be set
	 equal to the total number of devices.

   @param skt     Connection to server made using codanet_connect
   @param status  Pointer to a structure which will be filled with
                  any error/status information
	 @param opt     Structure specifying enable/disable for one or more devices
   
   @return Non-zero error code if a network error occured, zero
   \otherwise. Use codanet_getlasterror() to retrieve network
   error codes.                                                         */
int    CODANET_API codanet_cmd_hwconfig_getdeviceenable(CODANET_SOCKET skt, struct CODANET_DEVICESTATUS_ARRAY* status, struct CODANET_HWCONFIG_DEVICEENABLE* opt);

/**codanet_cmd_hwconfig_setdeviceenable
   Summary
   Enable/Disable devices under a specified hardware configuration
   
   Description
	 The enable/disable state of devices which exist but are not listed
	 int the CODANET_HWCONFIG_DEVICEENABLE structure will not be changed.

   @param skt     Connection to server made using codanet_connect
   @param status  Pointer to a structure which will be filled with
                  any error/status information
	 @param opt     Structure specifying enable/disable for one or more devices
   
   @return Non-zero error code if a network error occured, zero
   \otherwise. Use codanet_getlasterror() to retrieve network
   error codes.                                                         */
int    CODANET_API codanet_cmd_hwconfig_setdeviceenable(CODANET_SOCKET skt, struct CODANET_DEVICESTATUS_ARRAY* status, const struct CODANET_HWCONFIG_DEVICEENABLE* opt);


/**codanet_cmd_startup
	 Summary
   Start hardware and perform initialisation
   
   Description
   This command is usually executed at the beginning of any
   program using the system. If the system has already been
   started then it will give a successful outcome and will <I>not</I>
   attempt a restart. If a restart is required (for example to
   clear some unresolved hardware errors on a device) then you
   should call codanet_cmd_shutdown followed by
   codanet_cmd_startup.
   
	 A valid hardware config handle must be passed to this function.
	 The list of all available hardware configs can be obtained
	 using codanet_hwconfig_enumerate.  

	 The purpose of a hardware config is that some devices may 
	 have pre-startup options which can be
   configured prior to startup, usually to enable or disable
   certain components. For example Codamotion CX1 units can be
   enabled or disabled, and different digital force plates
   selected.  
   
   After executing this function the status array should be
   checked for any errors. Note that the
   \return value of this function indicates only whether <I>network</I>
   errors have occured. It says nothing about command errors in
   the status structure.
   @param skt     Connection to server made using codanet_connect
   @param status  Pointer to a structure which will be filled with
                  any error/status information
	 @param confighandle Handle of hardware configuration to use.
   
   @return Non-zero error code if a network error occured, zero
   \otherwise. Use codanet_getlasterror() to retrieve network
   error codes.                                                         */
int    CODANET_API codanet_cmd_startup(CODANET_SOCKET skt, struct CODANET_DEVICESTATUS_ARRAY* status, DWORD confighandle);

/**codanet_cmd_shutdown
   Summary
   Stop hardware devices and perform cleanup on server
   
   Description
	 Programs may choose to call this function just before they terminate.
	 Care should be taken if multiple programs are accessing the server
	 since no realtime data commands will work after shutdown has been performed.

	 This function may be used to attempt a hardware restart if it is followed by
	 a call to codanet_cmd_startup.
 
   After executing this function the status array should be
   checked for any errors. 

   @param skt     Connection to server made using codanet_connect
   @param status  Pointer to a structure which will be filled with
                  any error/status information
   
   @return Non-zero error code if a network error occured, zero
   \otherwise. Use codanet_getlasterror() to retrieve network
   error codes.                                                         */
int    CODANET_API codanet_cmd_shutdown(CODANET_SOCKET skt, struct CODANET_DEVICESTATUS_ARRAY* status);

/**codanet_cmd_acqmaxticks
   Summary
   Set or remove limit on duration of acquisition
   
   Description
	 This function affects the outcome of subsequent calls to begin acquisition in continuous mode,
	 using either codanet_cmd_acqcontinuous or codanet_cmd_acqcontinuousbuffered.  It can be used 
	 limit the duration of acquisition, or to instruct the system to
	 continue acquisition indefinitely (until codanet_cmd_acqstop is called).  
	 
	 Note that some devices may have a limit to the duration of data which can be acquired. 

	 The is specified in device ticks using a specified device to convert from ticks to actual time.
	 The duration in seconds of a single device tick on a given device can be obtained
	 using codanet_query_devicetickseconds.  The list of possible devices to use for this purpose
	 can be obtained using codanet_enumerate_devices.

   After executing this function the status array should be
   checked for any errors. 

   @param skt     Connection to server made using codanet_connect
   @param status  Pointer to a structure which will be filled with
                  any error/status information
	 @param wTickDevice ID of the device to use to convert ticks into a time duration
	 @param dwMaxTicks The maximum number of ticks to allow, or CODANET_ACQ_UNLIMITED
	                   to continue acquisition indefinitely untile coda_cmd_acqstop is called.
   
   @return Non-zero error code if a network error occured, zero
   \otherwise. Use codanet_getlasterror() to retrieve network
   error codes.                                                         */
int    CODANET_API codanet_cmd_acqmaxticks(CODANET_SOCKET skt, struct CODANET_DEVICESTATUS_ARRAY*, WORD wTickDevice, DWORD dwMaxTicks);

/**codanet_cmd_acqprepare
   Summary
   Perform necessary initialisation on all devices prior to acquisition
   
   Description
	 Must be called after codanet_cmd_startup and prior to any of the following acquisition commands:
	 codanet_cmd_acqsingleshot, codanet_cmd_acqcontinuous, codanet_cmd_acqcontinuousbuffered.

   After executing this function the status array should be
   checked for any errors. 

   @param skt     Connection to server made using codanet_connect
   @param status  Pointer to a structure which will be filled with
                  any error/status information
   
   @return Non-zero error code if a network error occured, zero
   \otherwise. Use codanet_getlasterror() to retrieve network
   error codes.                                                         */
int    CODANET_API codanet_cmd_acqprepare(CODANET_SOCKET skt, struct CODANET_DEVICESTATUS_ARRAY* status);

/**codanet_cmd_acqsingleshot
   Summary
   Request all devices to acquire a single frame of data
   
   Description
	 A single frame of data is acquired and transmitted to any available realtime streams.
	 The system must be ready to acquire, meaning that codanet_cmd_startup and 
	 codanet_cmd_acqprepare have both completed successfully.

	 To receive the data produced by this function, a realtime data stream must be connected
	 to the server.  See codanet_datastream_create for more information.

   After executing this function the status array should be
   checked for any errors. 

   @param skt     Connection to server made using codanet_connect
   @param status  Pointer to a structure which will be filled with
                  any error/status information
   
   @return Non-zero error code if a network error occured, zero
   \otherwise. Use codanet_getlasterror() to retrieve network
   error codes.                                                         */
int    CODANET_API codanet_cmd_acqsingleshot(CODANET_SOCKET skt, struct CODANET_DEVICESTATUS_ARRAY* status);

/**codanet_cmd_acqcontinuous
   Summary
   Request all devices to acquire continuously and stream data
   
   Description
	 All devices will begin realtime streaming on successful completion of this function.
	 Acquisition will continue until either codanet_cmd_acqstop is called, or until
	 a time limit specified by codanet_cmd_acqmaxticks is reached.
	 
	 For this to work, the system must be ready to acquire, meaning that codanet_cmd_startup and 
	 codanet_cmd_acqprepare have both completed successfully.	 To receive the realtime data 
	 produced by this function, a realtime data stream must be connected
	 to the server.  See codanet_datastream_create for more information.

	 This function is used in the case that all data must be extracted from the server in
	 realtime and with lowest latency.  If latency is non-critical, the alternative 
	 function codanet_cmd_acqcontinuousbuffered may be more appropriate.

	 After executing this function the status array should be
   checked for any errors. 

   @param skt     Connection to server made using codanet_connect
   @param status  Pointer to a structure which will be filled with
                  any error/status information
   
   @return Non-zero error code if a network error occured, zero
   \otherwise. Use codanet_getlasterror() to retrieve network
   error codes.                                                         */
int    CODANET_API codanet_cmd_acqcontinuous(CODANET_SOCKET skt, struct CODANET_DEVICESTATUS_ARRAY* status);

/**codanet_cmd_acqcontinuousbuffered
   Summary
   Request all devices to acquire continuously and buffer data internally
   
   Description
	 All devices will begin acquisition to internal buffers. This will continue 
	 until either codanet_cmd_acqstop is called, or until
	 a time limit specified by codanet_cmd_acqmaxticks is reached.
	 
	 For this to work, the system must be ready to acquire, meaning that codanet_cmd_startup and 
	 codanet_cmd_acqprepare have both completed successfully.	 

	 The data produced by this function can be retrieved using codanet_cmd_acqbufferrequestpacket.
	 To get a realtime monitor of the acquisition buffer, use codanet_cmd_acqbuffermonitor.

	 This function is used in the case that all data must be extracted from the server in
	 realtime and with lowest latency.  If latency is non-critical, the alternative 
	 function codanet_cmd_acqcontinuousbuffered may be more appropriate.

	 After executing this function the status array should be
   checked for any errors. 

   @param skt     Connection to server made using codanet_connect
   @param status  Pointer to a structure which will be filled with
                  any error/status information
   
   @return Non-zero error code if a network error occured, zero
   \otherwise. Use codanet_getlasterror() to retrieve network
   error codes.                                                         */
int    CODANET_API codanet_cmd_acqcontinuousbuffered(CODANET_SOCKET skt, struct CODANET_DEVICESTATUS_ARRAY* status);

/**codanet_cmd_acqstop
   Summary
   Request all devices to stop acquisition
   
   Description
	 Stops an acquisition which had previously been started using codanet_cmd_acqcontinuous
	 or codanet_cmd_acqcontinuousbuffered.

	 In the case of acquisitions which were started using codanet_cmd_acqcontinuousbuffered,
	 the acquisition data which was acquired will continue to be available for download using
	 codanet_cmd_acqbufferrequestpacket, until the next acquisition is performed.

	 After executing this function the status array should be
   checked for any errors. 

   @param skt     Connection to server made using codanet_connect
   @param status  Pointer to a structure which will be filled with
                  any error/status information
   
   @return Non-zero error code if a network error occured, zero
   \otherwise. Use codanet_getlasterror() to retrieve network
   error codes.                                                         */
int    CODANET_API codanet_cmd_acqstop(CODANET_SOCKET skt, struct CODANET_DEVICESTATUS_ARRAY* status);

/**codanet_cmd_acqbufferrequestpacket
   Summary
   Retrieve acquired data from internal buffers
   
   Description
	 During buffered acquisition, acquired data is stored in internal buffers 
	 on the server or on the connected hardware.  Use this function to
	 download that data for later offline use.  

	 Data will be sent to any realtime streams connected using 
	 codanet_datastream_create.

	 This function is for use after a buffered acquisition has completed.  
	 Buffered acquisitions are started using codanet_cmd_acqcontinuousbuffered and
	 considered to be completed when either codanet_cmd_acqstop is called or when the
	 maximum duration specified using codanet_cmd_acqmaxticks is reached.

	 Note that this function is not intended to work during acquisition, 
	 only after it has completed.  If realtime monitoring is needed for data in the 
	 internal buffers, then use codanet_acqbuffermonitor instead.  If low-latency
	 access to all realtime data is needed then the use of acquisition buffers
	 is not recommended - use codanet_cmd_acqcontinuous instead.

	 After executing this function the status array should be
   checked for any errors. 

   @param skt     Connection to server made using codanet_connect
   @param status  Pointer to a structure which will be filled with
                  any error/status information
	 @param device  ID of the device from which to get data.  A list of all 
	                available devices can be obtained using codanet_enumerate_devices.
   
   @return Non-zero error code if a network error occured, zero
   \otherwise. Use codanet_getlasterror() to retrieve network
   error codes.                                                         */
int    CODANET_API codanet_cmd_acqbufferrequestpacket(CODANET_SOCKET skt, struct CODANET_DEVICESTATUS_ARRAY* status, WORD device, DWORD packetindex);

/**codanet_cmd_acqbufferrequestpacket
   Summary
   Retrieve acquired data from internal buffers
   
   Description
	 During buffered acquisition, acquired data is stored in internal buffers 
	 on the server or on the connected hardware.  Use this function to
	 receive the most recent frame acquired (for realtime monitoring purposes).

	 Data will be sent to any realtime streams connected using 
	 codanet_datastream_create.

	 After executing this function the status array should be
   checked for any errors. 

   @param skt     Connection to server made using codanet_connect
   @param status  Pointer to a structure which will be filled with
                  any error/status information
   
   @return Non-zero error code if a network error occured, zero
   \otherwise. Use codanet_getlasterror() to retrieve network
   error codes.                                                         */
int    CODANET_API codanet_cmd_acqbuffermonitor(CODANET_SOCKET skt, struct CODANET_DEVICESTATUS_ARRAY* status);

/**codanet_query_acqinprogress
   Summary
   Determine whether continuous data acquisition is currently in progress
   
   Description
	 Use this to determine whether an acquisition previously started using
	 codanet_acq_continuous or codanet_cmd_acqcontinuousbuffered is still in progress.
	 Acquisitions can terminate if codanet_cmd_acqstop is called or if the time
	 limit set by codanet_cmd_acqmaxticks is reached.

	 After executing this function the status array should be
   checked for any errors. 

   @param skt     Connection to server made using codanet_connect
   @param status  Pointer to a structure which will be filled with
                  any error/status information
	 @param inprogress  Pointer to a single DWORD value which will be
	                    set to non-zero if acquisition is currently in progress
											and zero otherwise.
   
   @return Non-zero error code if a network error occured, zero
   \otherwise. Use codanet_getlasterror() to retrieve network
   error codes.                                                         */
int    CODANET_API codanet_query_acqinprogress(CODANET_SOCKET skt, struct CODANET_DEVICESTATUS_ARRAY* status, DWORD* inprogress);

/**codanet_query_devicetickseconds
   Summary
   Determine the time in seconds corresponding to one tick on the specified device
   
   Description
	 When requesting data from a device or specifying acquisition time, the unit 
	 of measure for time is a <I><B>device tick</I></B>.  This is usually equivalent to frame
	 time on the device (1 / acquisition rate).  Use this function to find the
	 equivalent duration of a tick (in seconds) on the specified device.

	 After executing this function the status array should be
   checked for any errors. 

   @param skt     Connection to server made using codanet_connect
   @param status  Pointer to a structure which will be filled with
                  any error/status information
	 @param seconds Pointer to a single float value which will be
	                filled with the value of 1 tick in seconds.
   
   @return Non-zero error code if a network error occured, zero
   \otherwise. Use codanet_getlasterror() to retrieve network
   error codes.                                                         */
int    CODANET_API codanet_query_devicetickseconds(CODANET_SOCKET skt, struct CODANET_DEVICESTATUS_ARRAY* status, WORD device, float* seconds);

/**codanet_query_deviceacqnumpackets
   Summary
   Determine the number of packets of data acquired using codanet_cmd_acqcontinuousbuffered
   
   Description
   Determine the number of packets of data acquired using codanet_cmd_acqcontinuousbuffered,
	 on a specified device.
	 Packets can be downloaded using codanet_cmd_acqbufferrequestpacket.

	 After executing this function the status array should be
   checked for any errors. 

   @param skt     Connection to server made using codanet_connect
   @param status  Pointer to a structure which will be filled with
                  any error/status information
	 @param device  The device to query
	 @param numpackets Pointer to a single DWORD value which will be
	                set to indicate the number of packets available
									on the specified device.
   
   @return Non-zero error code if a network error occured, zero
   \otherwise. Use codanet_getlasterror() to retrieve network
   error codes.                                                         */
int    CODANET_API codanet_query_deviceacqnumpackets(CODANET_SOCKET skt, struct CODANET_DEVICESTATUS_ARRAY* status, WORD device, DWORD* numpackets);

#ifdef __cplusplus
}
#endif
