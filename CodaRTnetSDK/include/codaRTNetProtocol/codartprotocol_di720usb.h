#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 * Device ID                                                           *
 ***********************************************************************/

/** Device ID for DI720USB analog-to-digital converter system, comprising
    one or more sets of 32 analog input channels. */
#define DEVICEID_DI720USB 8

/***********************************************************************
 * Size Constants                                                      *
 ***********************************************************************/

#define CODANET_DI720USB_MAX_BOARDS 1UL
#define CODANET_DI720USB_BOARD_CHANNELS 32UL
#define CODANET_DI720USB_MAX_CHANNELS (CODANET_DI720USB_MAX_BOARDS*CODANET_DI720USB_BOARD_CHANNELS)
#define CODANET_DI720USB_VOLTRANGE_NONE 0xFFFFFFFFUL
#define CODANET_DI720USB_VOLTRANGE_02_5 0UL
#define CODANET_DI720USB_VOLTRANGE_05_0 1UL
#define CODANET_DI720USB_VOLTRANGE_10_0 2UL

#define CODANET_DI720USB_BASERATE_MIN 366.217f

/***********************************************************************
 * Network Protocol Command ID's                                       *
 ***********************************************************************/

#define CODANET_DEVICEOPTIONID_DI720USB_INPUTS     5001
#define CODANET_DEVICEINFOID_DI720USB_INPUTS       5002
#define CODANET_HWCONFIGOPTIONID_DI720USB          5003

/***********************************************************************
 * Network Protocol Data Structures: Hardware Config                   *
 ***********************************************************************/

	typedef struct CODANET_HWCONFIG_DI720USB
{
	struct CODANET_HWCONFIG_BASE base;
	DWORD dwNumVoltRanges;
	DWORD board_input_voltrange[CODANET_DI720USB_MAX_BOARDS];
} tagCODANET_HWCONFIG_DI720USB;

/***********************************************************************
 * Network Protocol Data Structures: Device Options                    *
 ***********************************************************************/

typedef struct CODANET_DEVICEOPTIONS_DI720USB_INPUTS
{
	struct CODANET_DEVICEOPTIONS_BASE base;
	float fBaseRateHz;
	DWORD channel_decimation[CODANET_DI720USB_MAX_CHANNELS];
} tagCODANET_DEVICEOPTIONS_DI720USB_INPUTS;

/***********************************************************************
 * Network Protocol Data Structures: Device Queries                    *
 ***********************************************************************/

typedef struct CODANET_DEVICEINFO_DI720USB_INPUTS
{
	struct CODANET_DEVICEINFO_BASE base;
	DWORD dwNumBoards;
	DWORD dwNumChannels;
	float fBaseRateHz;
	float board_voltsper15bits[CODANET_DI720USB_MAX_BOARDS];
	DWORD channel_decimation[CODANET_DI720USB_MAX_CHANNELS];
} tagCODANET_DEVICEINFO_DI720USB_INPUTS;

/**********************************************************************/

void CODANET_API codanet_hwconfig_DI720USB_hton(struct CODANET_HWCONFIG_DI720USB* dev);
void CODANET_API codanet_hwconfig_DI720USB_ntoh(struct CODANET_HWCONFIG_DI720USB* dev);
void CODANET_API codanet_deviceoptions_DI720USB_inputs_hton(struct CODANET_DEVICEOPTIONS_DI720USB_INPUTS* opt);
void CODANET_API codanet_deviceoptions_DI720USB_inputs_ntoh(struct CODANET_DEVICEOPTIONS_DI720USB_INPUTS* opt);
void CODANET_API codanet_deviceinfo_DI720USB_inputs_hton(struct CODANET_DEVICEINFO_DI720USB_INPUTS* info);
void CODANET_API codanet_deviceinfo_DI720USB_inputs_ntoh(struct CODANET_DEVICEINFO_DI720USB_INPUTS* info);

#ifdef __cplusplus
}
#endif
