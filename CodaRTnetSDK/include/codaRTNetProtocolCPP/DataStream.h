#pragma once

#include "RTNetworkPacket.h"

// Disable the following warning:
// warning C4290: C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
#pragma warning ( disable : 4290 )


namespace codaRTNet
{
	/** Summary
			Represents a data stream for receiving measurement data from RTNet Server (a UDP socket)

			Description
			To create a data stream use RTNetClient::createDataStream.  Packets can
			then be requested using the RTNetClient methods (doSingleShotAcq, monitorAcqBuffer, 
			startAcqContinuous, requestAcqBufferPacket).  To receive requested packets on
			the data stream use the receivePacket method.
		*/
	class DataStream
	{
		friend class RTNetClient;

	protected:
		 CODANET_SOCKET skt;

	public:
		/** Summary
				Construct a data stream object in a disconnected state
			*/
		DataStream() :
				skt(CODANET_SOCKET_ERROR)
		{
		}

	protected:
		/** Summary
				Construct a data stream object from an existing UDP socket

				Description
				This function is documented for completeness and is not 
				usually called directly.  To create a connected data stream
				first create an unconnected one using the 
				DataStream::DataStream() constructor, and then connect it
				using RTNetClient::createDataStream.

				@params nskt existing UDP socket
			*/
		DataStream(CODANET_SOCKET nskt) :
			skt(nskt)
		{
		}

	public:

		/** Summary
				Receive network packets on this data stream

				Description
				Packets will arrive in the data stream after they have
				been requested using one of the RTNetClient request
				methods (doSingleShotAcq, monitorAcqBuffer, 
			  startAcqContinuous, requestAcqBufferPacket).

				Packets received on the stream and not yet retrieved using
				this function are buffered up to the system default
				UDP receive-buffer limit.  On windows XP systems that default 
				limit is 8192 bytes.  Once this buffer is full, any additional
				packets received will be lost.

				If there are no packets available when this function is called,
				it will block whilst waiting for new data, for up to the 
				maximum number of microseconds specified in the timeoutusec parameter.
				To block indefinitely use the symbolic constant
				CODANET_PACKET_WAIT_INDEFINITELY.

				@param packet Reference to an RTNetworkPacket object which will
				              be filled with new data on successful completion.
				@param timeoutusec Maximum time to block whilst waiting for a new
				                   data packet (microseconds), or CODANET_PACKET_WAIT_INDEFINITELY to
													 wait indefinitely.
			  @retval CODANET_OK Packet was received ok .
				@retval CODANET_STREAMTIMEOUT timeoutusec was reached before any data packets were received.
				@retval CODANET_SOCKETERROR_BROKEN Stream connection was broken.
			*/
		int receivePacket(RTNetworkPacket& packet, DWORD timeoutusec)
		{
			return packet.recvFrom(skt, timeoutusec);
		}
	};

}