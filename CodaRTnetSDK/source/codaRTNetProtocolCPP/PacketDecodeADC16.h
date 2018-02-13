#pragma once

#include "PacketDecode.h"

namespace codaRTNet
{
	/** Summary
	    Decodes 16-bit measurement data received from a data stream.

			Description
			Use this class to decode network packets which are in the <b>ADC16</b> 
			format.  This is the format used to transmit measurements from 16-bit analog-to-digital
			converters such as the GS16AIO on a Codamotion Active Hub.  They contain
			one 16-bit value per channel of analog data, for several channels.  However, devices may
			have elected to decimate one or more channels, in which case the values corresponding
			to those channels are ommitted for all packets whose device tick value is not a
			multiple of the decimation factor.  When using the results from this decoder,
			it is therefore important to know the channel decimation factors which were applied
			in order to correctly identify which channel is which.

			Note that decimation factors are only applied to packets received
			using RTNetClient::startAcqContinuous or RTNetClient::requestAcqBufferFrame
			and <I>not</I> when received using RTNetClient::doAcqSingleShot or
			RTNetClient::monitorAcqBuffer.
		*/
	class PacketDecodeADC16 : public PacketDecode
	{
	public:
		/** Summary
				Construct a decoder object with no associated data 

				Description
				When constructed in this way, use the PacketDecode::decode method in the base class 
				to attempt to decode a network packet.
			*/
		PacketDecodeADC16()
		{
		}

		/** Summary
				Construct a decoder object and attempt to decode a specified network packet

				Description
				This is the same as constructing using the default constructor and calling
				the decode method.  

				@param packet A network packet to decode.
				@see PacketDecode::decode
			*/
		PacketDecodeADC16(const RTNetworkPacket& packet)
		{
			decode(packet);
		}

	public:

		/** Number of 16-bit values contained in the decoded packet */
		DWORD getNumValues()
		{ return datasize / sizeof(short); }

		/** Pointer to array of 16-bit values whose array length is given by getNumValues */
		short* getValues()
		{ return (short*)data; }

	protected:
		virtual bool canDecode(WORD wPacketID) const
		{ return (wPacketID == CODANET_PACKETID_ADC16); }

		virtual bool decodeData(const BYTE* input);
	};

}
