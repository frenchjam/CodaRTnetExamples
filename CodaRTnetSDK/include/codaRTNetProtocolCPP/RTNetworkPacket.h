#pragma once

#include <stdlib.h>
#include "../codaRTNetProtocol/codartdatastream.h"

namespace codaRTNet
{
	/** Summary
			Holds the data from a single packet on a data stream

			Description
			Construct an instance of this object for use with
			DataStream::receivePacket.  Once received, packets can be
			decoded into measurement information using a class derived from
			PacketDecode (see PacketDecode3DResultExt and PacketDecodeADC16).

			On busy networks it is possible for packets to become split
			or corrupted. Use verifyCheckSum prior to decoding to ensure
			that no corrupt data is used.
			*/
	class RTNetworkPacket : protected CODANET_PACKET
	{
	public:
		RTNetworkPacket(::WORD wDeviceID, ::WORD wPacketID, ::DWORD dwTick, ::DWORD dwPage, ::DWORD dwDataSize);

		/** Construct an un-initialised packet object */
		RTNetworkPacket();

		~RTNetworkPacket();

		BYTE* getData() 
		{ return (data != NULL) ? (data + CODANET_PACKET_HEADER_SIZE) : NULL; };

		const BYTE* getData() const
		{ return (data != NULL) ? (data + CODANET_PACKET_HEADER_SIZE) : NULL; };

		DWORD getDataSize() const;

		DWORD transmitSize() const
		{ return dwTransmitSize; }

		void computeCheckSum();

		/** Provides a consistency check on incoming packets by comparing packet data
		    with a checksum value which was originally transmitted with the data.
				@return true if packet data is valid, false if corrupt.
			*/
		bool verifyCheckSum() const;

		bool readHeader(CODANET_PACKET_HEADER* header) const;

		int sendTo(CODANET_SOCKET sktUDP, const sockaddr_in* addr) const;

		int recvFrom(CODANET_SOCKET sktUDP, DWORD timeoutusec);
	};

}
