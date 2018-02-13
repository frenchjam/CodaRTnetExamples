#pragma once

#include "RTNetworkPacket.h"

namespace codaRTNet
{
	/** Summary
			Abstract base class for decoding packet information received on a data stream.

			Description
			Provides the base class for decoding of incoming data packets (RTNetworkPacket objects)
			received on a connected data stream (DataStream object).

			Data packets may contain different data structures depending on the device used
			and the device options which have been set.  Derived classes contain specific
			methods to decode the different types of packet data.  For Codamotion CX1 data packets use
			the PacketDecode3DResultExt and for 16-bit analog data packets use PacketDecodeADC16.

			All network packets received via a DataStream object will contain a numeric identifier
			for the device which created them, a device tick number indicating the time at which the
			measurements were taken, and an optional page number in the case that multiple packets
			(pages) are sent during the same tick.

			The decode method will attempt to decode an	incoming packet in a format determined 
			by the derived class.  To establish whether the packet is in a format which 
			an instance of a derived class can decode 
			(but without actually decoding it), use the canDecodePacket method.

			Decoded data can be cleared using the clear method.  This should be done if
			the RTNetworkPacket object used is destroyed before the PacketDecode object
			to avoid stale pointers being used.

			@see PacketDecode3DResultExt, PacketDecodeADC16
		*/
	class PacketDecode
	{
	protected:
		CODANET_PACKET_HEADER header;
		BYTE* data;
		DWORD datasize;

	public:
		PacketDecode() :
			data(NULL),
			datasize(0)
		{
		}

		virtual ~PacketDecode()
		{
			clear();
		}

		/** Dissocates the decoder object from any incoming packet data and reset to an undecoded state */
		void clear();

		/** Summary
				Determine whether the specified packet is in a format which 
		    the instantiated decoder object can decode.  
				
				Description
				This method does not actually perform any decoding operation
				but merely determines whether it is possible.  Use decode to
				perform the decoding operation.

				@param packet The packet to inspect.
				@return true if the packet can be decoded by this object, false otherwise.
				@see decode
			*/
		bool canDecodePacket(const RTNetworkPacket& packet) const;

		/** Summary
				Attempt to decode the specified packet for retrieval via the methods
				of a derived class

				Description
				The nature of the decoding which is done will depend upon the derived class.
				After decoding the derived class will have a variety of <b>get</b> methods
				to retrieve the decoded data.  See for example PacketDecode3DResultExt::getNumMarkers.
				If the packet is not in a form which can be decoded by the derived class, 
				this method returns false.

				@param packet The network packet to decode.
				@return true if packet was successfully decoded, false otherwise.
			*/
		bool decode(const RTNetworkPacket& packet);

		/** Gets the numeric identifier for device from which the decoded data packet originates.
		    @return Numeric identifier for device or zero if no packet has been decoded.
			*/
		WORD getDeviceID() const
		{ return header.wDeviceID; }

		/** Summary
				Gets the clock tick time at which this packet was generated.
				Description
				The actual time interval corresponding to one tick will depend
				on the settings of the device from which this packet came.
				See RTNetClient::getDeviceTickSeconds to determine tick
				time in seconds.
		    @return Integer tick value. Will be zero if no packet has been decoded.
			*/
		DWORD getTick() const
		{ return header.dwTick; }

		/** Summary
				Gets the page (packet per device clock tick) of this packet.
				Description
				This is only used for devices which send multiple packets 
				per clock tick.
				@return Zero-based page number. Will be zero if no packet has been decoded.
				*/
		DWORD getPage() const
		{ return header.dwPage; }

	protected:

		virtual bool canDecode(WORD wPacketID) const = 0;

		virtual bool decodeData(const BYTE* input) = 0;
	};
}
