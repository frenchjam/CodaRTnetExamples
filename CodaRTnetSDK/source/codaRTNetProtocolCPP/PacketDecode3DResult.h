#pragma once

#include "PacketDecode.h"

namespace codaRTNet
{
	/** Summary
			Original class to decode 3D position data from a data stream

			Description
			This was the original class used for 3D data streaming 
			from Codamotion CX1 systems but has
			been superceded by PacketDecode3DResultExt.  It remains in
			the API to allow the future possibility of reduced data sets
			for higher data throughput, but is not currently in use.
	  */
	class PacketDecode3DResult : public PacketDecode
	{
	public:
		PacketDecode3DResult()
		{
		}

		PacketDecode3DResult(const RTNetworkPacket& packet)
		{
			decode(packet);
		}

	public:

		DWORD getNumMarkers()
		{ return datasize / 13; }

		float* getPosition(DWORD ipoint)
		{ return (ipoint < getNumMarkers()) ? (float*)(data + 13*ipoint) : NULL; }

		BYTE getValid(DWORD ipoint)
		{ return (ipoint < getNumMarkers()) ? data[13*ipoint + 12] : 0; }

	protected:
		virtual bool canDecode(WORD wPacketID) const
		{ return (wPacketID == CODANET_PACKETID_3DRESULT); }

		virtual bool decodeData(const BYTE* input);
	};

}
