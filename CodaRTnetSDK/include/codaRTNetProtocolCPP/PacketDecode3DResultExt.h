#pragma once

#include "PacketDecode.h"

namespace codaRTNet
{
	/** Summary
	    Decodes 3D measurement packets received on a data stream.

			Description
			Use this class to decode 3D measurement packets which are in the <b>3DRESULT_EXT</b> 
			format.  This is the format used to transmit measurements from Codamotion CX1
			systems.  Such packets are received using a DataStream object as RTNetworkPacket
			objects.  The 3DRESULT_EXT format includes 3-dimensional position measurements
			for one or more markers and visibility flag (visible/occluded) for each marker.
			It may also include intensity data for each marker (three or more cameras per marker), 
			and residuals (one or more residuals per marker).
		*/
	class PacketDecode3DResultExt : public PacketDecode
	{
	public:
		/** Summary
				Construct a decoder object with no associated data 

				Description
				When constructed in this way, use the PacketDecode::decode method in the base class 
				to attempt to decode a network packet.
			*/
		PacketDecode3DResultExt()
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
		PacketDecode3DResultExt(const RTNetworkPacket& packet)
		{
			decode(packet);
		}

	public:

		/** Number of markers */
		DWORD getNumMarkers()
		{ return *(WORD*)data; }

		/** Number of cameras per marker (indicates the number of intensity values present per marker) */
		BYTE getNumCamerasPerMarker()
		{ return data[2]; }

		/** Number of residual values per marker */
		BYTE getNumResidualsPerMarker()
		{ return data[3]; }

		/** Retrieves 3D vector position of decoded point, in millimetres
				@param ipoint Zero-based index of the marker to get, in range zero to (getNumMarkers()-1).
				@return Pointer to three-element array with X,Y and Z coordinate values for the specified marker, measured in millimetres
			*/
		float* getPosition(DWORD ipoint)
		{ return (ipoint < getNumMarkers()) ? (float*)(data + 4 + ipoint*(13+getNumCamerasPerMarker()+2*getNumResidualsPerMarker())) : NULL; }

		/** Retrieves validity flag for decoded marker
				@param ipoint Zero-based index of the flag to get, in range zero to (getNumMarkers()-1).
				@return Non-zero if marker was visible to at least one measurement unit, zero otherwise.
			*/
		BYTE getValid(DWORD ipoint)
		{ return (ipoint < getNumMarkers()) ? data[4 + ipoint*(13+getNumCamerasPerMarker()+2*getNumResidualsPerMarker()) + 12] : 0; }

		/** Summary
				Retrieves infra-red light intensity information for decoded point.
				
				Description
				Light intensity is measured on a scale 0 to 255 although values below 10 are error codes
				and therefore should not be used.  This function returns a pointer to an array of bytes, 
				one per intensity value.  The number of elements in the returned array
				is determined by getNumCamerasPerMarker.

				@param ipoint Zero-based index of the marker whose intensity to get, in range zero to (getNumMarkers()-1).
				@return Pointer to array of bytes
			*/
		BYTE* getIntensity(DWORD ipoint)
		{ return (ipoint < getNumMarkers()) ? data + 4 + ipoint*(13+getNumCamerasPerMarker()+2*getNumResidualsPerMarker()) + 13 : NULL; }

		/** getResiduals

				Summary
				Retrieves marker residual values in millimetres.
				
				Description
				The residual is the unweighted euclidean distance between the mean marker location
				reported by getPosition and that reported by an individual measurement unit, where
				two or more units are used.  It is represented here as an integer value of microns.
				If only one unit is used, no residual information is transmitted since it would always be zero.  Use getNumResidualsPerMarker to determine
				how many residual values are available per marker.

				@param ipoint Zero-based index of the marker whose residual to get, in range zero to (getNumMarkers()-1).
				@return Pointer to array of 16-bit unsigned values. Each element represents a residual in microns.
			*/
		WORD* getResiduals(DWORD ipoint)
		{ return (ipoint < getNumMarkers()) ? (WORD*)(data + 4 + ipoint*(13+getNumCamerasPerMarker()+2*getNumResidualsPerMarker()) + 13 + getNumCamerasPerMarker()) : NULL; }

	protected:
		virtual bool canDecode(WORD wPacketID) const
		{ return (wPacketID == CODANET_PACKETID_3DRESULT_EXT); }

		virtual bool decodeData(const BYTE* input);
	};

}
