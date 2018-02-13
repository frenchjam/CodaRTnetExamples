#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "codartprotocol_forceplates.h"

/***********************************************************************
 * Device ID                                                           *
 ***********************************************************************/

#define DEVICEID_KISTLER 6

/***********************************************************************
 * Network Protocol Command ID's                                       *
 ***********************************************************************/

#define CODANET_HWCONFIGOPTIONID_KISTLER 6001
#define CODANET_DEVICEINFOID_KISTLER     6002
/**********************************************************************/

#define CODANET_KISTLER_MAXPORTS 32
#define CODANET_KISTLER_CALCFORCE 6

/**********************************************************************/

typedef struct CODANET_HWCONFIG_KISTLER
{
	struct CODANET_HWCONFIG_BASE base;
	DWORD dwNumPlates;
	DWORD notused1;
	struct CODANET_HWCONFIG_KISTLER_FORCEPLATE
	{
		wchar_t modelname[CODANET_FORCEPLATE_MAX_MODELNAME_LENGTH];
		wchar_t serialnumber[CODANET_FORCEPLATE_MAX_SERIALNUMBER_LENGTH];
		DWORD ondevice;
		DWORD channelstart;
		double R[9];  // 3x3 matrix count order in 11, 12, 13, 21 22 23 31 32 33
		double t[3]; 
		DWORD selrange_xy;
		DWORD selrange_z;
	} plate[CODANET_FORCEPLATE_MAX];
} tagCODANET_HWCONFIG_KISTLER;

/**********************************************************************/

void CODANET_API codanet_hwconfig_kistler_hton(struct CODANET_HWCONFIG_KISTLER* config);
void CODANET_API codanet_hwconfig_kistler_ntoh(struct CODANET_HWCONFIG_KISTLER* config);

#ifdef __cplusplus
}
#endif
