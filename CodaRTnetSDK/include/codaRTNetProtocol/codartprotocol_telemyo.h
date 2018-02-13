#ifndef CODARTPROTOCOL_TELEMYO_H
#define CODARTPROTOCOL_TELEMYO_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 * Device ID                                                           *
 ***********************************************************************/

/* Device ID for TELEMYO EMG System */
#define DEVICEID_TELEMYO 10

/***********************************************************************
 * Size Constants                                                      *
 ***********************************************************************/

#define CODANET_TELEMYO_MAX_BOARDS 4UL
#define CODANET_TELEMYO_BOARD_CHANNELS 32UL
#define CODANET_TELEMYO_MAX_CHANNELS (CODANET_TELEMYO_MAX_BOARDS*CODANET_TELEMYO_BOARD_CHANNELS)
/*
#define CODANET_TELEMYO_VOLTRANGE_NONE 0xFFFFFFFFUL
#define CODANET_TELEMYO_VOLTRANGE_02_5 0UL
#define CODANET_TELEMYO_VOLTRANGE_05_0 1UL
#define CODANET_TELEMYO_VOLTRANGE_10_0 2UL

#define CODANET_TELEMYO_BASERATE_MIN 366.217f
*/
/***********************************************************************
 * Network Protocol Command ID's                                       *
 ***********************************************************************/

#define CODANET_DEVICEOPTIONID_TELEMYO_INPUTS     2001
#define CODANET_DEVICEINFOID_TELEMYO_INPUTS       2002
#define CODANET_HWCONFIGOPTIONID_TELEMYO          2003

/***********************************************************************
 * Network Protocol Data Structures: Hardware Config                   *
 ***********************************************************************/

typedef struct CODANET_HWCONFIG_TELEMYO
{
	struct CODANET_HWCONFIG_BASE base;
	DWORD dwNumVoltRanges;
	DWORD board_input_voltrange[CODANET_TELEMYO_MAX_BOARDS];
} tagCODANET_HWCONFIG_TELEMYO;

/***********************************************************************
 * Network Protocol Data Structures: Device Options                    *
 ***********************************************************************/

typedef struct CODANET_DEVICEOPTIONS_TELEMYO_INPUTS
{
	struct CODANET_DEVICEOPTIONS_BASE base;
	float fBaseRateHz;
	DWORD channel_decimation[CODANET_TELEMYO_MAX_CHANNELS];
	DWORD channel_enabled[CODANET_TELEMYO_MAX_CHANNELS];
	DWORD channel_offset[CODANET_TELEMYO_MAX_CHANNELS];
} tagCODANET_DEVICEOPTIONS_TELEMYO_INPUTS;

/***********************************************************************
 * Network Protocol Data Structures: Device Queries                    *
 ***********************************************************************/

typedef struct CODANET_DEVICEINFO_TELEMYO_INPUTS
{
	struct CODANET_DEVICEINFO_BASE base;
	DWORD dwNumBoards;
	DWORD dwNumChannels;
	float fBaseRateHz;
	float board_voltsper15bits[CODANET_TELEMYO_MAX_BOARDS];
	DWORD channel_decimation[CODANET_TELEMYO_MAX_CHANNELS];
	DWORD channel_time_offset[CODANET_TELEMYO_MAX_CHANNELS];
} tagCODANET_DEVICEINFO_TELEMYO_INPUTS;

/**********************************************************************/

void CODANET_API codanet_hwconfig_telemyo_hton(struct CODANET_HWCONFIG_TELEMYO* dev);
void CODANET_API codanet_hwconfig_telemyo_ntoh(struct CODANET_HWCONFIG_TELEMYO* dev);
void CODANET_API codanet_deviceoptions_telemyo_inputs_hton(struct CODANET_DEVICEOPTIONS_TELEMYO_INPUTS* opt);
void CODANET_API codanet_deviceoptions_telemyo_inputs_ntoh(struct CODANET_DEVICEOPTIONS_TELEMYO_INPUTS* opt);
void CODANET_API codanet_deviceinfo_telemyo_inputs_hton(struct CODANET_DEVICEINFO_TELEMYO_INPUTS* info);
void CODANET_API codanet_deviceinfo_telemyo_inputs_ntoh(struct CODANET_DEVICEINFO_TELEMYO_INPUTS* info);

#ifdef __cplusplus
}
#endif

#endif
