#pragma once

#include "../codaRTNetProtocol/codartclient.h"
#include "../codaRTNetProtocol/codartprotocol.h"
#include "NetworkException.h"
#include "DeviceStatusArray.h"
#include "AutoDiscover.h"
#include "DeviceEnum.h"
#include "HWConfigEnum.h"
#include "DataStream.h"
#include "DeviceInfo.h"
#include "DeviceOptions.h"
#include "Version.h"

// Disable the following warning:
// warning C4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
#pragma warning ( disable : 4290 )


namespace codaRTNet
{

	/**codaRTNet::RTNetClient
	   Summary
	   Provides the main programming interface for commuication with
	   Codamotion RTNet Server.
	   
	   Description
	   Use doAutoDiscoverServer to discover available servers on the
	   network and connect to establish a connection. startSystem
	   initialises devices (start mode), and prepareForAcq prepares
	   devices for acquisition (prepared mode). Attach data streams
	   for receving data by using createDataStream. When in prepared
	   mode, a single frame of data can be requested from all
	   devices with doSingleShotAcq. To continuously stream data use
	   startAcqContinuous. To continously buffer data use
	   startAcqContinousBuffered. Whilst in buffered acquisition
	   mode the most recently acquired frame can be monitored using
	   requestAcqBufferPacket                                        */
	class RTNetClient
	{
	public:
		CODANET_SOCKET skt;
		DeviceStatusArray status;

	public:
		/** Summary
				Construct a disconnected RTNetClient object.
				Description
				Following construction, use connect to connect to a server.
		*/
		RTNetClient() :
			skt(CODANET_SOCKET_ERROR)
		{
			codanet_devicestatusarray_clear(&status);
		}

		/** Summary
				Construct an RTNetClient object and attempt to connect to server at specified IP address and port.
				
				Description
				Following construction, use connect to connect to a server.

				@param ip IP address of server expressed as 32-bit value
				@param port port number on which server is running
		*/
		RTNetClient(unsigned long ip, unsigned short port) :
			skt(CODANET_SOCKET_ERROR)
		{
			codanet_devicestatusarray_clear(&status);
			connect(ip, port);
		}

		/** Summary 
		    RTNetClient destructor - will disconnect from server if currently connected.
			*/
		~RTNetClient()
		{
			disconnect();
		}

		/** Summary
				Attempt to find Codamotion Realtime Network servers on the network.
			   
				Description
				This will use a UDP auto-discovery protocol to attempt to find all available
				Codamotion Realtime Network servers which are attached.  The
				results are placed in an AutoDiscover class.  If it failed to find any
				servers, the dwNumServers member of AutoDiscover will be
				zero.  If this happens it may be because servers are down or not connected.

				@param discover structure to be filled with array of available servers
			*/
		static void doAutoDiscoverServer(AutoDiscover& discover)
		{
			codanet_autodiscover(&discover);
		}

		/** Summary
				Connect to Codamotion RTNet Server using the specified IP address and port number.
				
				Description
				Suitable IP and port parameters can be retrieved using the doAutoDiscoverServer method,
				or can be specified explicitly.  Throws a NetworkException on failure.

				@param ip IP address of server expressed as 32-bit value
				@param port port number on which server is running
				@throws NetworkException
		*/
		void connect(unsigned long ip, unsigned short port) throw(NetworkException)
		{ 
			skt = codanet_connect(ip, port);
			if (skt == CODANET_SOCKET_ERROR)
				throw NetworkException(codanet_getlasterror());
		}

		/** Summary
		    Disconnect from Codamotion RTNet Server.
			*/
		void disconnect()
		{
			codanet_disconnect(skt);
			skt = CODANET_SOCKET_ERROR;
		}

		/** Summary
			  Determine whether currently connected to server.
				@return true if connected, false otherwise
				*/
		bool isConnected() const
		{ return (skt != CODANET_SOCKET_ERROR); }

	public:

	/** Summary
			Provide array of all attached devices.
  
			Description
			Provide array of all attached devices, identified by their device ID numbers. 
			The DeviceEnum class will be filled with an array of ID's for the attached
			devices.
			
			@param dev Reference to a DeviceEnum object to be filled with device information.
			@throws NetworkException, DeviceStatusArray 
		*/
		void enumerateDevices(DeviceEnum& dev) throw(NetworkException, DeviceStatusArray)
		{
			if (codanet_cmd_devices_enumerate(skt, &status, &dev) != CODANET_OK)
				throw NetworkException(codanet_getlasterror());
			if (status.numstatus)
				throw(status);
		}

		/** Summary
		    Enumerate all available hardware configurations and their corresponding names

				Description
				Available hardware configurations are returned in a HWConfigEnum class.
				Each configuration has an associated address handle and an associated 
				name.  The address handle can be used to select a specific configuration 
				in a call to startSystem, or to query the configuration using getDeviceEnable.

				@param config Reference to a HWConfigEnum to be filled with configuration information. 
				@throws NetworkException, DeviceStatusArray
				*/
		void enumerateHWConfig(HWConfigEnum& config) throw(NetworkException, DeviceStatusArray)
		{
			if (codanet_cmd_hwconfig_enumerate(skt, &status, &config) != CODANET_OK)
				throw NetworkException(codanet_getlasterror());
			if (status.numstatus)
				throw(status);
		}

		/** Summary
		    Get the address handle of the currently running hardware configuration,
				or zero if system not started.

				Description
				This can be used to determine whether the system has been started (using
				startSystem), and if so which hardware configuration was used.  See enumerateHWConfig
				for more information about hardware configurations.

				@return Address handle of the currently running hardware configuration,
				or zero if system not started.
				@throws NetworkException, DeviceStatusArray
				*/
		DWORD getRunningHWConfig() throw(NetworkException, DeviceStatusArray)
		{
			DWORD confighandle(0UL);
			CommandQuery(CODANET_GETRUNNINGHWCONFIG, sizeof(DWORD), &confighandle);
			confighandle = codanet_ntohl(confighandle);
			return confighandle;
		}

		/** Summary
		    Create a data stream object for receiving measurement data from the server.

				Description
				Creates a UDP socket on the client and identifies this socket to the server for
				subsequent streaming of data.  To request that packets of data are send to the
				stream use doSingleShotAcq, acqBufferMonitor, or requestAcqBufferPacket.

				@param stream Reference to a DataStream object which holds this stream's details. 
				@throws NetworkException, DeviceStatusArray
				*/
		void createDataStream(DataStream& stream, ::WORD port) throw(NetworkException, DeviceStatusArray)
		{
				CODANET_SOCKET sktstream = codanet_datastream_create(skt, &status, port);
				if (sktstream == CODANET_SOCKET_ERROR)
				{
					if (status.numstatus)
						throw (status);
					else 
						throw (NetworkException(codanet_getlasterror()));
				}
				stream = DataStream(sktstream);
		}

		/* Summary
		   Close the specified data stream port

			 Description
			 The specified data stream will be closed and no longer receive data packets from
			 the server.
			 @param The stream to close
		 */
		void closeDataStream(DataStream& stream)
		{
			// TODO: full close like this
			// codanet_datastream_close(stream.skt, &status);

			// TEMP: disconnect socket
			// note this won't remove the reference from the server so this machine
			// may continue to receive packets at a non-existent port
			codanet_disconnect(stream.skt);
			stream.skt = CODANET_SOCKET_ERROR;
		}

		/* Editing of Hardware Configs not available in RTNet SDK
		 *
     *
		DWORD createHWConfig(const wchar_t* name) throw(NetworkException, DeviceStatusArray)
		{
			DWORD newconfighandle = 0;
			if (codanet_cmd_hwconfig_create(skt, &status, name, &newconfighandle) != CODANET_OK)
				throw (NetworkException(codanet_getlasterror()));
			if (status.numstatus)
				throw (status);
			return newconfighandle;
		}

		void removeHWConfig(DWORD confighandle) throw(NetworkException, DeviceStatusArray)
		{
			Command(CODANET_HWCONFIGREMOVE, confighandle);
		}

		void setDeviceEnable(CODANET_HWCONFIG_DEVICEENABLE& opt) throw(NetworkException, DeviceStatusArray)
		{
			opt.dwSize = codanet_htonl(sizeof(CODANET_HWCONFIG_DEVICEENABLE));
			opt.dwCommand = codanet_htonl(CODANET_HWCONFIGSETDEVICEENABLE);
			CommandStruct(sizeof(CODANET_HWCONFIG_DEVICEENABLE), &opt);
		}
		*
		*
		*/

		/** Summary
		    Retrieve device enable/disable flags for the specified hardware configuration.

				@param confighandle Address handle for an existing hardware configuration. A list of all such handles
				                    can be retrieved using enumerateDevices.
        @param opt Reference to a CODANET_HWCONFIG_DEVICEENABLE structure to be filled with enable/disable information
				@throws NetworkException, DeviceStatusArray
				*/

		void getDeviceEnable(DWORD confighandle, CODANET_HWCONFIG_DEVICEENABLE& opt) throw(NetworkException, DeviceStatusArray)
		{	
			DWORD cmdstruct[3] =
			{
				2*sizeof(DWORD),
				codanet_htonl(CODANET_HWCONFIGGETDEVICEENABLE),
				codanet_htonl(confighandle)
			};

			CommandStructQuery(sizeof(cmdstruct), cmdstruct, sizeof(struct CODANET_HWCONFIG_DEVICEENABLE), &opt);
			codanet_hwconfig_deviceenable_ntoh(&opt);
		}

		/** Summary
		    Get runtime device information.  
				
				Description
				Typically used to retrieve those system paramters which may be subject to 
				change after a device has been started. Examples include sample rate or alignment data.

				The info parameter should be a reference to an instance of a class derived from 
				DeviceInfo.  The overridden class will determine the type of data being retrieved and 
				will be filled with that data on successful completion. For example DeviceInfoAlignment 
				retrieves information about the most recent alignment of a Codamotion CX1 system.

				@param info Reference to an instance of a class derived from DeviceInfo
				@throws NetworkException, DeviceStatusArray
			*/
		void getDeviceInfo(DeviceInfo& info) throw(NetworkException, DeviceStatusArray)
		{
			if (info.readfrom(skt, &status) != CODANET_OK)
				throw NetworkException(codanet_getlasterror());
			if (status.numstatus)
				throw (status);
		}

		/** Summary
		    Set runtime device information or perform runtime device setup operation.  
				
				Description
				Typically used to set those system paramters which can be changed after a device 
				has been started, or to perform setup operations on a running device 
				such as system alignment of a CX1 system.

				The info parameter should be a reference to an instance of a class derived from 
				DeviceOptions.  The overridden class will determine the type of data being set or 
				setup operation being performed. For example DeviceOptionsAlignment will
				cause a CX1 alignment procedure to be executed.

				@param info Reference to an instance of a class derived from DeviceOptions
				@throws NetworkException, DeviceStatusArray
			*/
		void setDeviceOptions(DeviceOptions& options)
			throw(NetworkException, DeviceStatusArray)
		{
			if (options.sendto(skt, &status) != CODANET_OK)
				throw NetworkException(codanet_getlasterror());
			if (status.numstatus)
				throw (status);
		}

		/** Summary 
		    Attempt to start the system with a specified hardware configuration.

				Description				
				If the system is currently in a shut-down state, this will attempt to start the system
				by initialising all devices specified in the hardware config identified by the 
				confighandle parameter.  On successful	completion the system will be in started mode.  
				An array of all available config address handle values can be found using enumerateHWConfig, 
				and the devices available	in any given config can be found using enumerateDevices.

				If the system has already been started, this function does nothing. 
				If shutdown-restart is required, call stopSystem first and then call this function.

				@param confighandle Address handle of hardware configuration to use
				@throws NetworkException, DeviceStatusArray
			*/
		 
		void startSystem(DWORD confighandle) throw(NetworkException, DeviceStatusArray)
		{
			Command(CODANET_STARTUP, confighandle);
		}

		/** Summary
		    If the system has been started, this will cause it to shut down.
				
				Description
				Shuts system down and enters shut-down state.  This will generate an error only 
				if the system was already shut down when it was called.

				@throws NetworkException, DeviceStatusArray 
			*/		 
		void stopSystem() throw(NetworkException, DeviceStatusArray)
		{
			Command(CODANET_SHUTDOWN);
		}

		/** Summary
		    Prepare for acquisition.
				
				Description
				After the system has been started, this function should be called to enter 
				a prepared state, ready for acquisition.  This performs any pre-acquisition
				configuration such as taking diode offset levels in a CX1 system, or resetting
				force platforms.  It may take several seconds to execute.

				Once the system is prepared for acquisition, the following functions become
				available for use: doSingleShotAcq, startAcqContinuous, startAcqContinousBuffered.

				@throws NetworkException, DeviceStatusArray 
			*/		 
		void prepareForAcq() throw(NetworkException, DeviceStatusArray)
		{
			Command(CODANET_ACQPREPARE);
		}

		/** Summary
		    Request that a single frame of measurements be gathered on all active devices
		    and sent to all connected data streams.

				Description
				This will only succeed if the system has been started (using startSystem)
				and prepared for acquisition (using prepareForAcq).  A single frame of data
				will be requested and, provided there are no network errors, will arrive 
				on all connected data stream sockets.

				@throws NetworkException, DeviceStatusArray 
				*/
		void doSingleShotAcq() throw(NetworkException, DeviceStatusArray)
		{
			Command(CODANET_ACQSINGLESHOT);
		}

		/** Summary
		    Begin continuous acquisition and continual streaming of measurements
				to all connected data stream sockets.

				Description
				Puts the system into continuous (non-buffered) acquisition mode.
				This will only succeed if the system has been started (using startSystem)
				and prepared for acquisition (using prepareForAcq).  Data will be acquired
				continuously until either stopAcq is called or the limit set by setAcqMaxTicks
				is reached.  All acquired data from all enabled devices is continually sent
				over the netork to those data streams which were connected at the time
				this function was called.

				@throws NetworkException, DeviceStatusArray 
				*/
		void startAcqContinuous() throw(NetworkException, DeviceStatusArray)
		{
			Command(CODANET_ACQCONTINUOUS);
		}

		/** Summary
		    Begin continuous acquisition with server-side buffering of 
				all incoming data.

				Description
				Puts the system into continuous buffered acquisition mode.
				This will only succeed if the system has been started (using startSystem)
				and prepared for acquisition (using prepareForAcq).  Data will be acquired
				continuously until either stopAcq is called or the limit set by setAcqMaxTicks
				is reached. All acquired data from all enabled devices is stored on the server 
				(or stored in on-board memory on connected devices) for later retrieval using 
				requestAcqBufferPacket.  To determine the total number of packets available,
				use getAcqBufferNumPackets.  During continuous buffered acquisition, the most recent
				available frame of data can be retrieved by calling monitorAcqBuffer.
				This allows a buffered acquisition to be monitored in realtime.

				Following end of acquistion (using stopAcq or when max frames has been reached),
				buffered data persists until the next acquisition function is called
				(one of prepareForAcq, doSingleShotAcq, startAcqContinuous, or startAcqContinuousBuffered), or 
				until the system is stopped using stopSystem.

				@throws NetworkException, DeviceStatusArray 
				*/
		void startAcqContinuousBuffered() throw(NetworkException, DeviceStatusArray)
		{
			Command(CODANET_ACQCONTINUOUSBUFFERED);
		}

		/** Summary
		    Stop a continuous or continuous buffered acquistion.

				Description
				If the system is currently acquiring data (following a call to startAcqContinuous or
				startAcqContinuousBuffered), this will put it back into a prepared but non-acquiring state.  

				@throws NetworkException, DeviceStatusArray
		  */
		void stopAcq() throw(NetworkException, DeviceStatusArray)
		{
			Command(CODANET_ACQSTOP);
		}

		/** Summary
				Request the most recent frame of data for monitoring during
				buffered acquisition.

				Description
				This method works only during buffered acquisition mode
				(acquisitions started using startAcqContinuousBuffered).
				It causes the most recent frame of data from all enabled
				devices to be sent to all data streams which were connected
				at the time at acquisition start.

				Note that the monitoring process runs asynchronously from
				any acquisition process.  As such it is not guaranteed that 
				successive calls to monitorAcqBuffer give successive frames.
				For example, two consecutive calls may cause the same frame
				to be transmitted twice, or may contain different frames
				separated by several frame sampling periods.

				@throws NetworkException, DeviceStatusArray
      */
		void monitorAcqBuffer() throw(NetworkException, DeviceStatusArray)
		{
			Command(CODANET_ACQBUFFERMONITOR);
		}

		/** Summary
				Request a specific packet of data from a buffered acquisition
				for the specified device.

				Description
				This method works during or immeidately after buffered acquisition mode
				(acquisitions started using startAcqContinuousBuffered).
				If used after acquisition mode, it must be called before 
				any of the following acquisition and system functions: doSingleShotAcq, 
				prepareForAcq, startAcqContinuous, startAcqContinuousBuffered,
				stopSystem.

				It causes the specified packet of data to be sent to all currently
				connected data streams.  To determine which devices are available,
				use getDeviceEnable.  To determine the total number of packets
				available for a specific device, use getAcqBufferNumPackets.

				@param the device for which to get data
				@packetindex index of packet to get in range 0 to (getAcqBufferNumPackets()-1)
				@throws NetworkException, DeviceStatusArray
      */
		void requestAcqBufferPacket(WORD device, DWORD packetindex) throw(NetworkException, DeviceStatusArray)
		{
			CODANET_PACKET_REQUEST req;
			req.dwSize = codanet_htonl( sizeof(CODANET_PACKET_REQUEST) );
			req.dwCommand = codanet_htonl( CODANET_ACQBUFFERREQUESTPACKET );
			req.wDevice = codanet_htons( device );
			req.wReserved = 0;
			req.dwPacketIndex = codanet_htonl( packetindex );
			CommandStruct(sizeof(req), &req);
		}

		/** Summary
		    Set limit on duration of continuous acquisitions

				Description
				Following a call to startAcqContinuous or startAcqContinuousBuffered, 
				Acquisition will expire for all devices once the specified number
				of device ticks have elapsed.  To continue acquisition indefinitely 
				(until stopAcq is called), use the symbolic constant CODANET_ACQ_UNLIMITED.  
				
				The length of a device tick is usually (though not always) equal to the sample period on the device,
				for example the marker rate on Codamotion CX1 systems.  To determine
				the length of a device tick based on current device settings for a given
				device,	use getDeviceTickSeconds.  To get an array of all enabled devices,
				use getDeviceEnable.

				Note that the timing accuracy with which acquisition is stopped
				is not guaranteed.  All devices will acquire for <I>at least</I>
				the amount of time specified.  Most devices will stop within 0.1s
				after this, but please contact Charnwood Dynamics for more details
				if the length of this delay is critical for your application.

				@param tickdevice the device whose tick length to use
				@param maxticks the number of ticks after which acquisition will expire, or
				       CODANET_ACQ_UNLIMITED to continue until stopAcq is called
			  @throws etworkException, DeviceStatusArray

			*/
		void setAcqMaxTicks(WORD tickdevice, DWORD maxticks) throw(NetworkException, DeviceStatusArray)
		{
			CODANET_MAXTICKS_REQUEST req;
			req.dwSize = codanet_htonl( sizeof(CODANET_MAXTICKS_REQUEST) );
			req.dwCommand = codanet_htonl( CODANET_ACQSETMAXTICKS );
			req.wTickDevice = codanet_htons( tickdevice );
			req.wReserved = 0;
			req.dwMaxTicks = codanet_htonl( maxticks );
			CommandStruct(sizeof(req), &req);
		}

		/** Summary
		    Find acquisition time limit in terms of specified device ticks.
				Description
				This time limit is the one which would have been set by
				setAcqMaxTicks.
				@param device Device whose tick length to use as the time scale.
				@return time Time limit in device ticks or CODANET_ACQ_UNLIMITED if there is no time limit.
				@see setAcqMaxTicks
			*/
		DWORD getAcqMaxTicks(WORD device) throw(NetworkException, DeviceStatusArray)
		{
			CODANET_ACQINFO_REQUEST req;
			req.dwSize = codanet_htonl( sizeof(req) );
			req.dwCommand = codanet_htonl( CODANET_ACQGETMAXTICKS );
			req.wDevice = codanet_htons( device );
			req.wReserved = 0;

			// request
			DWORD ticks(0UL);
			CommandStructQuery(sizeof(req), &req, sizeof(DWORD), &ticks);
			
			// convert & return
			ticks = codanet_ntohl(ticks);
			return ticks;
		}

		/** Summary
				Establish whether system is currently in an acquisition mode

				Description
				The system will be in an acquisition mode if acqContinuous or 
				acqContinuousBuffered have successfully been called and
				the duration has not exceeded any limit set by setAcqMaxTicks

				*/
		bool isAcqInProgress() throw(NetworkException, DeviceStatusArray)
		{
			DWORD inprogress(0);
			CommandQuery(CODANET_ACQINPROGRESS, sizeof(DWORD), &inprogress);
			inprogress = codanet_ntohl(inprogress);
			return inprogress ? true : false;
		}

		/** Summary
		    Find the number of seconds corresponding to one device tick on
				the specified device

				Description
				A device tick is a native unit of measure used by hardware devices
				and usually (but not always) corresponds to one sample period.
				For example a Codamotion CX1 device sampling at 200Hz with decimation
				factor of 1 (no decimation) will have a device tick of 0.005 seconds.

				Device ticks are used when specifying the maximum acquisition length,
				and are embedded in measurement packets received on a data stream
				using classes derived from PacketDecode.

				@params device the device whose tick length to get
				@returns tick length in seconds (floating point)
				@throws NetworkException, DeviceStatusArray
		*/
		float getDeviceTickSeconds(WORD device) throw(NetworkException, DeviceStatusArray)
		{
			CODANET_ACQINFO_REQUEST req;
			req.dwSize = codanet_htonl( sizeof(req) );
			req.dwCommand = codanet_htonl( CODANET_DEVICETICKSECONDS );
			req.wDevice = codanet_htons( device );
			req.wReserved = 0;

			// request
			float seconds(0.0f);
			CommandStructQuery(sizeof(req), &req, sizeof(float), &seconds);
			
			// convert & return
			seconds = codanet_ntohf(seconds);
			return seconds;
		}

		/** Summary
		    Returns total data packets in the server-side buffer for the specified device 
				during or following a buffered acquisition

				Description
				Used in conjunction with requestAcqBufferPacket to retrieve data after or
				during a buffered acquisition.

				@params device the device of interest
				@returns the number of packets available in the buffer
				@throws NetworkException, DeviceStatusArray
				*/
		DWORD getAcqBufferNumPackets(WORD device)  throw(NetworkException, DeviceStatusArray)
		{
			CODANET_ACQINFO_REQUEST req;
			req.dwSize = codanet_htonl( sizeof(req) );
			req.dwCommand = codanet_htonl( CODANET_ACQBUFFERNUMPACKETS );
			req.wDevice = codanet_htons( device );
			req.wReserved = 0;
			DWORD numpackets(0UL);
			CommandStructQuery(sizeof(req), &req, sizeof(DWORD), &numpackets);
			numpackets = codanet_ntohl(numpackets);
			return numpackets;
		}

		/** Summary
			  Get server version number

				Description
				Retrieve the current server version number. 

				@param v Object to hold the retrieved version number
				@throws NetworkException, DeviceStatusArray
				*/
		void getServerVersion(Version& v) throw(NetworkException, DeviceStatusArray)
		{
			CODANET_VERSION* resultbuffer = &v;
			CommandQuery(CODANET_GETSERVERVERSION, sizeof(CODANET_VERSION), resultbuffer);
			codanet_ntoh_version(resultbuffer);
		}

	protected:

		// simple command (no associated data to send, no return result to read)
		void Command(DWORD dwCommand)
		{
			DWORD cmdstruct[2] = { codanet_htonl( 2*sizeof(DWORD) ), codanet_htonl(dwCommand) };
			if (codanet_commandstructquery(skt, &status, sizeof(cmdstruct), cmdstruct, 0UL, NULL) != CODANET_OK)
				throw NetworkException(codanet_getlasterror());
			if (status.numstatus)
				throw (status);
		}
		
		// command with one DWORD of associated data, no return result to read
		void Command(DWORD dwCommand, DWORD dwData)
		{
			DWORD cmdstruct[3] = { codanet_htonl( 3*sizeof(DWORD) ), codanet_htonl(dwCommand), codanet_htonl(dwData) };
			CommandStruct(sizeof(cmdstruct), cmdstruct);
		}

		// command (with data to send but no return result to read)
		void CommandStruct(DWORD dwCommandDataSize, const void* commanddata)
			throw(NetworkException, DeviceStatusArray)
		{ 
			if (codanet_commandstructquery(skt, &status, dwCommandDataSize, commanddata, 0UL, NULL) != CODANET_OK)
				throw NetworkException(codanet_getlasterror());
			if (status.numstatus)
				throw (status);
		}

		// simple query (no associated data to send, but a result to read)
		void CommandQuery(DWORD dwCommand, DWORD dwResultSize, void* result)
		{
			DWORD cmdstruct[2] = { codanet_htonl( 2*sizeof(DWORD) ), codanet_htonl(dwCommand) };
			if (codanet_commandstructquery(skt, &status, sizeof(cmdstruct), cmdstruct, dwResultSize, result) != CODANET_OK)
				throw NetworkException(codanet_getlasterror());
			if (status.numstatus)
				throw (status);
		}
		

		// query (data to send and result to read)
		void CommandStructQuery(DWORD dwCommandDataSize, const void* commanddata, DWORD dwResultSize, void* result)
			throw(NetworkException, DeviceStatusArray)
		{ 
			if (codanet_commandstructquery(skt, &status, dwCommandDataSize, commanddata, dwResultSize, result) != CODANET_OK)
				throw NetworkException(codanet_getlasterror());
			if (status.numstatus)
				throw (status);
		}
	};

}