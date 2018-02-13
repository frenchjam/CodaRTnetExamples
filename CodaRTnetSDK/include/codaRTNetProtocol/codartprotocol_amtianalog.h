#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "codartprotocol_forceplates.h"

/***********************************************************************
 * Device ID                                                           *
 ***********************************************************************/

#define DEVICEID_AMTIANALOG 7

/***********************************************************************
 * Network Protocol Command ID's                                       *
 ***********************************************************************/

#define CODANET_HWCONFIGOPTIONID_AMTIANALOG 7001
#define CODANET_DEVICEINFOID_AMTIANALOG     7002
/**********************************************************************/

#define CODANET_AMTIANALOG_MAXPLATES 32
#define CODANET_AMTIANALOG_CALCFORCE 6

/**********************************************************************/

typedef struct CODANET_HWCONFIG_AMTIANALOG
{
	struct CODANET_HWCONFIG_BASE base;
	DWORD dwNumPlates;
	DWORD notused1;
	struct CODANET_HWCONFIG_AMTIANALOG_FORCEPLATE
	{
		wchar_t modelname[CODANET_FORCEPLATE_MAX_MODELNAME_LENGTH];
		wchar_t serialnumber[CODANET_FORCEPLATE_MAX_SERIALNUMBER_LENGTH];
		DWORD ondevice;
		DWORD channelstart;
		double R[9];  // 3x3 matrix count order in 11, 12, 13, 21 22 23 31 32 33
		double t[3]; 
	} plate[CODANET_FORCEPLATE_MAX];
} tagCODANET_HWCONFIG_AMTIANALOG;


/**********************************************************************/

void CODANET_API codanet_hwconfig_amtianalog_hton(struct CODANET_HWCONFIG_AMTIANALOG* config);
void CODANET_API codanet_hwconfig_amtianalog_ntoh(struct CODANET_HWCONFIG_AMTIANALOG* config);

/**********************************************************************/

#ifdef __cplusplus
}
#endif
