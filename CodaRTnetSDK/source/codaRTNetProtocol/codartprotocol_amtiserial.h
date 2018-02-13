#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "codartprotocol_forceplates.h"

/***********************************************************************
 * Device ID                                                           *
 ***********************************************************************/

#define DEVICEID_AMTISERIAL 2

/***********************************************************************
 * Network Protocol Command ID's                                       *
 ***********************************************************************/

#define CODANET_HWCONFIGOPTIONID_AMTISERIAL   2001
#define CODANET_DEVICEOPTIONID_AMTISERIAL_ADC 2001
#define CODANET_DEVICEINFOID_AMTISERIAL       2002

	/**********************************************************************/

#define CODANET_AMTISERIAL_MAXPORTS      1
#define CODANET_AMTISERIAL_MAXPORTPLATES 2
#define CODANET_AMTISERIAL_CALCFORCE 6

/**********************************************************************/

typedef struct CODANET_HWCONFIG_AMTISERIAL
{
	struct CODANET_HWCONFIG_BASE base;
	DWORD dwNumPorts;
	DWORD dwNotUsed;
	struct CODANET_HWCONFIG_AMTISERIAL_PORT
	{
		DWORD dwPortNumber;
		DWORD dwPortPlates;	
		struct CODANET_HWCONFIG_AMTISERIAL_PORT_PLATE
		{
			wchar_t modelname[CODANET_FORCEPLATE_MAX_MODELNAME_LENGTH];
			wchar_t serialnumber[CODANET_FORCEPLATE_MAX_SERIALNUMBER_LENGTH];
			double R[9];
			double t[3];
		} plate[CODANET_AMTISERIAL_MAXPORTPLATES];
	
	} port[CODANET_AMTISERIAL_MAXPORTS];

} tagCODANET_HWCONFIG_AMTISERIAL;

/**********************************************************************/

typedef struct CODANET_DEVICEOPTIONS_AMTISERIAL_ADC
{
	struct CODANET_DEVICEOPTIONS_BASE base;
	DWORD dwSampleRate;
	DWORD dwFilterNo;
} tagCODANET_DEVICEOPTIONS_AMTISERIAL_INPUTS;

/**********************************************************************/

typedef struct CODANET_DEVICEINFO_AMTISERIAL
{
	struct CODANET_DEVICEINFO_BASE base;
	DWORD dwSampleRate;
	DWORD dwFilterNo;
} tagCODANET_DEVICEINFO_AMTISERIAL;

/**********************************************************************/

void CODANET_API codanet_hwconfig_amtiserial_hton(struct CODANET_HWCONFIG_AMTISERIAL* config);
void CODANET_API codanet_hwconfig_amtiserial_ntoh(struct CODANET_HWCONFIG_AMTISERIAL* config);

void CODANET_API codanet_deviceoptions_amtiserial_adc_hton(struct CODANET_DEVICEOPTIONS_AMTISERIAL_ADC* opt);
void CODANET_API codanet_deviceoptions_amtiserial_adc_ntoh(struct CODANET_DEVICEOPTIONS_AMTISERIAL_ADC* opt);

void CODANET_API codanet_deviceinfo_amtiserial_hton(struct CODANET_DEVICEINFO_AMTISERIAL* info);
void CODANET_API codanet_deviceinfo_amtiserial_ntoh(struct CODANET_DEVICEINFO_AMTISERIAL* info);

#ifdef __cplusplus
}
#endif
