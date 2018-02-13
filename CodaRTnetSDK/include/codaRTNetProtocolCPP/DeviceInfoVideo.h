#pragma once

#include "DeviceInfo.h"
#include "codaRTNetProtocol/codartprotocol_video.h"

namespace codaRTNet
{
	class DeviceInfoVideoFormat : public CODANET_VIDEO_FORMAT
	{
	public:
		DeviceInfoVideoFormat();
	};

	class DeviceInfoVideoInterfaceCaps : public CODANET_VIDEO_INTERFACE_CAPS
	{
	public:
		DeviceInfoVideoInterfaceCaps();

	public:
		bool AddFormat(::DWORD dwInputType, ::DWORD dwBitsPerPixel, ::DWORD dwWidth, ::DWORD dwHeight, float fRateHz);
	};

	class DeviceInfoVideoInterfaces : public DeviceInfo
	{
	public:
		CODANET_DEVICEINFO_VIDEO_INTERFACES dev;

	public:
		DeviceInfoVideoInterfaces(::WORD wDeviceID);

		virtual ::DWORD buffersize() const
		{ return sizeof(dev); }

		virtual void* buffer()
		{ return &dev; }

		virtual const void* buffer() const
		{ return &dev; }

		virtual void ntoh(void* data) const;

		virtual void hton(void* data) const;

	public:
		bool AddDeviceInfo(const DeviceInfoVideoInterfaceCaps& info);
	};
}
