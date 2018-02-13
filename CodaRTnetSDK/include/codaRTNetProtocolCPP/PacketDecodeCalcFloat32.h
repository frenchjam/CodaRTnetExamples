#pragma once

#include "PacketDecode.h"

namespace codaRTNet
{
	/** Summary
	    Decodes 32-bit measurement data received from a data stream.

			Description
			Use this class to decode network packets which are in the <b>CalcFloat32</b> 
			format.  This is the format used to transmit measurements 32-bit float data such as 
			the AMTI force plate calculated force and moment data.  They contain
			six 32-bit values per channel, for several channels.  
		*/
	class PacketDecodeCalcFloat32 : public PacketDecode
	{
	public:
		/** Summary
				Construct a decoder object with no associated data 

				Description
				When constructed in this way, use the PacketDecode::decode method in the base class 
				to attempt to decode a network packet.
			*/
		PacketDecodeCalcFloat32()
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
		PacketDecodeCalcFloat32(const RTNetworkPacket& packet)
		{
			decode(packet);
		}

	public:

		/** Number of 16-bit values contained in the decoded packet */
		DWORD getNumValues()
		{ return datasize / sizeof(float); }

		/** Pointer to array of 16-bit values whose array length is given by getNumValues */
		float* getValues()
		{ return (float*)data; }

	protected:
		virtual bool canDecode(WORD wPacketID) const
		{ return (wPacketID == CODANET_PACKETID_CALCFLOAT32); }
		//{ return (wPacketID == CODANET_PACKETID_ADC16); }
		
		virtual bool decodeData(const BYTE* input);
	};

}
