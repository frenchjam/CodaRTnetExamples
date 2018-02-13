#ifndef CODARTPROTOCOL_GS16AIO6_H
#define CODARTPROTOCOL_GS16AIO6_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 * Device ID                                                           *
 ***********************************************************************/

/** Device ID for GS16AIO analog-to-digital converter system, comprising
    one or more sets of 32 analog input channels. */
#define DEVICEID_GS16AIO 3

/***********************************************************************
 * Size Constants                                                      *
 ***********************************************************************/

#define CODANET_GS16AIO_MAX_BOARDS 4UL
#define CODANET_GS16AIO_BOARD_CHANNELS 32UL
#define CODANET_GS16AIO_MAX_CHANNELS (CODANET_GS16AIO_MAX_BOARDS*CODANET_GS16AIO_BOARD_CHANNELS)
#define CODANET_GS16AIO_VOLTRANGE_NONE 0xFFFFFFFFUL
#define CODANET_GS16AIO_VOLTRANGE_02_5 0UL
#define CODANET_GS16AIO_VOLTRANGE_05_0 1UL
#define CODANET_GS16AIO_VOLTRANGE_10_0 2UL

#define CODANET_GS16AIO_BASERATE_MIN 366.217f

/***********************************************************************
 * Network Protocol Command ID's                                       *
 ***********************************************************************/

#define CODANET_DEVICEOPTIONID_GS16AIO_INPUTS     2001
#define CODANET_DEVICEINFOID_GS16AIO_INPUTS       2002
#define CODANET_HWCONFIGOPTIONID_GS16AIO          2003

/***********************************************************************
 * Network Protocol Data Structures: Hardware Config                   *
 ***********************************************************************/

	typedef struct CODANET_HWCONFIG_GS16AIO
{
	struct CODANET_HWCONFIG_BASE base;
	DWORD dwNumVoltRanges;
	DWORD board_input_voltrange[CODANET_GS16AIO_MAX_BOARDS];
} tagCODANET_HWCONFIG_GS16AIO;

/***********************************************************************
 * Network Protocol Data Structures: Device Options                    *
 ***********************************************************************/

typedef struct CODANET_DEVICEOPTIONS_GS16AIO_INPUTS
{
	struct CODANET_DEVICEOPTIONS_BASE base;
	float fBaseRateHz;
	DWORD channel_decimation[CODANET_GS16AIO_MAX_CHANNELS];
} tagCODANET_DEVICEOPTIONS_GS16AIO_INPUTS;

/***********************************************************************
 * Network Protocol Data Structures: Device Queries                    *
 ***********************************************************************/

typedef struct CODANET_DEVICEINFO_GS16AIO_INPUTS
{
	struct CODANET_DEVICEINFO_BASE base;
	DWORD dwNumBoards;
	DWORD dwNumChannels;
	float fBaseRateHz;
	float board_voltsper15bits[CODANET_GS16AIO_MAX_BOARDS];
	DWORD channel_decimation[CODANET_GS16AIO_MAX_CHANNELS];
	DWORD channel_time_offset[CODANET_GS16AIO_MAX_CHANNELS];
} tagCODANET_DEVICEINFO_GS16AIO_INPUTS;

/**********************************************************************/

void CODANET_API codanet_hwconfig_gs16aio_hton(struct CODANET_HWCONFIG_GS16AIO* dev);
void CODANET_API codanet_hwconfig_gs16aio_ntoh(struct CODANET_HWCONFIG_GS16AIO* dev);
void CODANET_API codanet_deviceoptions_gs16aio_inputs_hton(struct CODANET_DEVICEOPTIONS_GS16AIO_INPUTS* opt);
void CODANET_API codanet_deviceoptions_gs16aio_inputs_ntoh(struct CODANET_DEVICEOPTIONS_GS16AIO_INPUTS* opt);
void CODANET_API codanet_deviceinfo_gs16aio_inputs_hton(struct CODANET_DEVICEINFO_GS16AIO_INPUTS* info);
void CODANET_API codanet_deviceinfo_gs16aio_inputs_ntoh(struct CODANET_DEVICEINFO_GS16AIO_INPUTS* info);

#ifdef __cplusplus
}
#endif

#endif
