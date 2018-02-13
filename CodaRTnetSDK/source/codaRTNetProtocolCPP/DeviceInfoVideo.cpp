#include "DeviceInfoVideo.h"

namespace codaRTNet
{
	DeviceInfoVideoFormat::DeviceInfoVideoFormat()
	{
		CODANET_VIDEO_FORMAT* base = dynamic_cast<CODANET_VIDEO_FORMAT*> (this);
		memset(base, 0, sizeof(CODANET_VIDEO_FORMAT));
	}

	DeviceInfoVideoInterfaceCaps::DeviceInfoVideoInterfaceCaps()
	{
		CODANET_VIDEO_INTERFACE_CAPS* base = dynamic_cast<CODANET_VIDEO_INTERFACE_CAPS*> (this);
		memset(base, 0, sizeof(CODANET_VIDEO_INTERFACE_CAPS));
	}

	bool DeviceInfoVideoInterfaceCaps::AddFormat(DWORD dwInputType, DWORD dwBitsPerPixel, DWORD dwWidth, DWORD dwHeight, float fRateHz)
	{
		if (dwNumFormats >= CODANET_VIDEO_DEVICE_MAX_FORMATS)
			return false;
		format[dwNumFormats].dwInputType = dwInputType;
		format[dwNumFormats].dwBitsPerPixel = dwBitsPerPixel;
		format[dwNumFormats].dwWidth = dwWidth;
		format[dwNumFormats].dwHeight = dwHeight;
		format[dwNumFormats].fRateHz = fRateHz;
		++dwNumFormats;
		return true;
	}


	DeviceInfoVideoInterfaces::DeviceInfoVideoInterfaces(WORD wDeviceID)
	{
		memset(&dev, 0, sizeof(CODANET_DEVICEINFO_VIDEO_INTERFACES));
		dev.base.wDevice = wDeviceID;
		dev.base.wInfoID = CODANET_DEVICEINFOID_VIDEO_INTERFACES;
	}

	void DeviceInfoVideoInterfaces::ntoh(void* data) const
	{ 
		codanet_deviceinfo_video_interfaces_ntoh((CODANET_DEVICEINFO_VIDEO_INTERFACES*)data); 
	}

	void DeviceInfoVideoInterfaces::hton(void* data) const
	{ 
		codanet_deviceinfo_video_interfaces_hton((CODANET_DEVICEINFO_VIDEO_INTERFACES*)data); 
	}

	bool DeviceInfoVideoInterfaces::AddDeviceInfo(const DeviceInfoVideoInterfaceCaps& info)
	{
		if (dev.dwNumVideo >= CODANET_VIDEO_MAX_INTERFACES)
			return false;
		dev.video[dev.dwNumVideo++] = info;
		return true;
	}

}