#pragma once

#include "codartprotocol.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 * Device ID                                                           *
 ***********************************************************************/

/** Device ID for direct connection to video: 
    Maximises efficiency when client, server, and video camera 
		are all on same machine */
#define DEVICEID_VIDEO_DIRECTCONNECT 4

/** Device ID for video hosted on dedicated video server: 
    RTNet device communicates with video server via named pipe */
#define DEVICEID_VIDEO_REMOTESERVER 5

/***********************************************************************
 * Network Protocol Data Structures: Device Queries                    *
 ***********************************************************************/

#define CODANET_VIDEO_DEVICE_MAX_NAME_LENGTH 256
#define CODANET_VIDEO_DEVICE_MAX_FORMATS 32
#define CODANET_VIDEO_MAX_INTERFACES 16
#define CODANET_VIDEO_FORMAT_INPUT_DIRECTSHOW_WEBCAM 1
#define CODANET_VIDEO_FORMAT_INPUT_DIRECTSHOW_DV     2
#define CODANET_VIDEO_FORMAT_INPUT_DIRECTSHOW_BASLER 3

typedef struct CODANET_VIDEO_FORMAT
{
	DWORD dwInputType;
	DWORD dwBitsPerPixel;
	DWORD dwWidth;
	DWORD dwHeight;
	float fRateHz;
} tagCODANET_VIDEO_FORMAT;

typedef struct CODANET_VIDEO_INTERFACE_CAPS
{
	wchar_t friendlyname[CODANET_VIDEO_DEVICE_MAX_NAME_LENGTH];
	wchar_t systemname[CODANET_VIDEO_DEVICE_MAX_NAME_LENGTH];
	DWORD dwNumFormats;
  struct CODANET_VIDEO_FORMAT format[CODANET_VIDEO_DEVICE_MAX_FORMATS];
} tagCODANET_VIDEO_INTERFACE_CAPS;

typedef struct CODANET_DEVICEINFO_VIDEO_INTERFACES
{
	struct CODANET_DEVICEINFO_BASE base;
	DWORD dwNumVideo;
	struct CODANET_VIDEO_INTERFACE_CAPS video[CODANET_VIDEO_MAX_INTERFACES];
} tagCODANET_DEVICEINFO_VIDEO_INTERFACES;

typedef struct CODANET_HWCONFIG_VIDEO_INTERFACE_ENABLE
{
	struct CODANET_HWCONFIG_BASE base;
	DWORD dwNumEnabled;
	struct
	{
		wchar_t systemname[CODANET_VIDEO_DEVICE_MAX_NAME_LENGTH];
		struct CODANET_VIDEO_FORMAT format;
	}
	video[CODANET_VIDEO_MAX_INTERFACES];
} tagCODANET_HWCONFIG_VIDEO_INTERFACE_ENABLE;

/***********************************************************************
 * Network Protocol Command ID's                                       *
 ***********************************************************************/

#define CODANET_DEVICEINFOID_VIDEO_INTERFACES           4001
#define CODANET_HWCONFIGOPTIONID_VIDEO_INTERFACE_ENABLE 4002

/***********************************************************************
 * Network Conversion Utilities                                        *
 ***********************************************************************/

void CODANET_API codanet_video_format_ntoh(struct CODANET_VIDEO_FORMAT* format);
void CODANET_API codanet_video_format_hton(struct CODANET_VIDEO_FORMAT* format);
void CODANET_API codanet_deviceinfo_video_interfaces_ntoh(struct CODANET_DEVICEINFO_VIDEO_INTERFACES* info);
void CODANET_API codanet_deviceinfo_video_interfaces_hton(struct CODANET_DEVICEINFO_VIDEO_INTERFACES* info);
int CODANET_API codanet_hwconfig_video_interface_enable_ntoh(struct CODANET_HWCONFIG_VIDEO_INTERFACE_ENABLE* dev);
int CODANET_API codanet_hwconfig_video_interface_enable_hton(struct CODANET_HWCONFIG_VIDEO_INTERFACE_ENABLE* dev);

#ifdef __cplusplus
}
#endif
