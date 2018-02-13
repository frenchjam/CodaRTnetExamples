#pragma once

#include "DeviceOptions.h"
#include "../codaRTNetProtocol/codartprotocol_cx1.h"

namespace codaRTNet
{
	/** Summary
			Use as argument to RTNetClient::setDeviceOptions to perform a CX1 system alignment operation

			Description
			The constructor allows a set of alignment marker identities to be specified.  Once constructed,
			use RTNetClient::setDeviceOptions to perform the alignment operation.
			On completion, diagnostic information about the alignment can be retrieved using
			RTNetClient::getDeviceInfo with a DeviceInfoAlignment structure as the argument.
		*/
	class DeviceOptionsAlignment : public DeviceOptions
	{
	public:
		/** Summary 
		    Underlying alignment options request

				Description
				This is usually set via the DeviceOptionsAlignment::DeviceOptionsAlignment
				constructor rather than manipulated directly.
			*/
		CODANET_DEVICEOPTIONS_ALIGNMENT opt;

	public:
		/** Summary
				Construct an alignment request with the specified marker identities

				Description
				Marker identities are one-based.  Following construction with an appropriate
				set of identities, use with RTNetClient::setDeviceOptions to perform 
				the alignment procedure.

				To perform an alignment, markers with know identities must be placed in
				an approximate right-angle configuration in the field of view.  A minimum
				of three markers are required.  One marker defines the origin, two markers
				define an X-axis, and another two markers define an XY-axis.  The X-axis
				vector must be sufficiently perpendicular to the XY-plane vector 
				(at least 60 degrees included angle), and the pairs of markers defining the vectors 
				must be separated by sufficient distance (greater than 300mm).

				To use the minimum number of three markers, the same marker can be used for
				the origin as for the X0 and XY0 points.  One further marker is then required
				for the X1 point and another for the XY1 point (see parameters below).

				@param dwMarkerOrigin One-based identity of origin marker.
				@param dwMarkerX0 One-based identity of marker defining start of X-axis vector.
				@param dwMarkerX1 One-based identity of marker defining start of X-axis vector.
				@param dwMarkerXY0 One-based identity of marker on axis in XY-plane.
				@param dwMarkerXY1 One-based identity of marker on axis in XY-plane (can be same as origin but must differ from XY0)
			*/
		DeviceOptionsAlignment(DWORD dwMarkerOrigin, DWORD dwMarkerX0, DWORD dwMarkerX1, DWORD dwMarkerXY0, DWORD dwMarkerXY1)
		{
			opt.base.dwSize = sizeof(opt);
			opt.base.dwCommand = CODANET_SETDEVICEOPTIONS;
			opt.base.wDevice = DEVICEID_CX1;
			opt.base.wOptionID = CODANET_DEVICEOPTIONID_ALIGNMENT;
			opt.dwMarkerOrigin = dwMarkerOrigin;
			opt.dwMarkerX0 = dwMarkerX0;
			opt.dwMarkerX1 = dwMarkerX1;
			opt.dwMarkerXY0 = dwMarkerXY0;
			opt.dwMarkerXY1 = dwMarkerXY1;
		}


	protected:
		virtual ::DWORD buffersize() const
		{ return (::DWORD)sizeof(opt); }

		virtual void* buffer()
		{ return &opt; }

		virtual const void* buffer() const
		{ return &opt; }

		virtual void hton(void* data) const
		{ codanet_deviceoptions_alignment_hton((CODANET_DEVICEOPTIONS_ALIGNMENT*)data); }

		virtual void ntoh(void* data) const
		{ codanet_deviceoptions_alignment_ntoh((CODANET_DEVICEOPTIONS_ALIGNMENT*)data); }
	};

}
