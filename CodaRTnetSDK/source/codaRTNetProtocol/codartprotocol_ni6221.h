#ifndef CODARTPROTOCOL_NI6221_H
#define CODARTPROTOCOL_NI6221_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 * Device ID                                                           *
 ***********************************************************************/

/** Device ID for NI6221 analog-to-digital converter system */
#define DEVICEID_NI6221 9

/***********************************************************************
 * Network Protocol Command ID's                                       *
 ***********************************************************************/

#define CODANET_DEVICEINFOID_CAMERAFRAME	    9001
#define CODANET_DEVICEINFOID_NI6221_INPUTS    9002
#define CODANET_HWCONFIGOPTIONID_NI6221       9003
#define CODANET_DEVICEOPTIONID_NI6221_INPUTS  9004

/***********************************************************************
 * Network Protocol Data Structures: Hardware Config                   *
 ***********************************************************************/

typedef struct CODANET_HWCONFIG_NI6221
{
	struct CODANET_HWCONFIG_BASE base;
	DWORD dwAnalogInputEnable;
	float fAnalogInputRateHz;
} tagCODANET_HWCONFIG_NI6221;

/**********************************************************************/

typedef struct CODANET_DEVICEINFO_NI6221_INPUTS
{
	struct CODANET_DEVICEINFO_BASE base;
	DWORD dwNumChannels;
	float fRateHz;
	DWORD dwMilliVoltsPer15Bits;
} tagCODANET_DEVICEINFO_NI6221_INPUTS;

/**********************************************************************/

typedef struct CODANET_DEVICEOPTIONS_NI6221_INPUTS
{
	struct CODANET_DEVICEOPTIONS_BASE base;
	float fBaseRateHz;
} tagCODANET_DEVICEOPTIONS_NI6221_INPUTS;

/**********************************************************************/

void CODANET_API codanet_hwconfig_ni6221_hton(struct CODANET_HWCONFIG_NI6221* dev);
void CODANET_API codanet_hwconfig_ni6221_ntoh(struct CODANET_HWCONFIG_NI6221* dev);
void CODANET_API codanet_deviceinfo_ni6221_inputs_hton(struct CODANET_DEVICEINFO_NI6221_INPUTS* info);
void CODANET_API codanet_deviceinfo_ni6221_inputs_ntoh(struct CODANET_DEVICEINFO_NI6221_INPUTS* info);
void CODANET_API codanet_deviceoptions_ni6221_inputs_hton(struct CODANET_DEVICEOPTIONS_NI6221_INPUTS* options);
void CODANET_API codanet_deviceoptions_ni6221_inputs_ntoh(struct CODANET_DEVICEOPTIONS_NI6221_INPUTS* options);

#ifdef __cplusplus
}
#endif

#endif
