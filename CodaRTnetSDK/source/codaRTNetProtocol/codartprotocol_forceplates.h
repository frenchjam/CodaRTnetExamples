#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 * Network Protocol Command ID's                                       *
 ***********************************************************************/

#define CODANET_DEVICEINFOID_FORCEPLATE   3005
#define CODANET_DEVICEOPTIONID_FORCEPLATE_LOCATION 3006
#define CODANET_DEVICEOPTIONID_FORCEPLATE_CHANNELOFFSET 3004

/**********************************************************************/

#define CODANET_FORCEPLATE_MAX 32
#define CODANET_FORCEPLATE_CALPARAMS_MAX 128
#define CODANET_FORCEPLATE_CHANNELS_MAX 12

/**********************************************************************/

#define CODANET_FORCEPLATE_CALCTYPE_GENERIC 0
#define CODANET_FORCEPLATE_CALCTYPE_AMTI12CHANNEL 2
#define CODANET_FORCEPLATE_CALCTYPE_KISTLER 6
#define CODANET_FORCEPLATE_CALCTYPE_AMTIANALOG 7
#define CODANET_FORCEPLATE_CALCTYPE_RSSCAN 8
#define CODANET_FORCEPLATE_CALCTYPE_ARSALIS 9

/**********************************************************************/

#define CODANET_FORCEPLATE_MAX_MODELNAME_LENGTH    32
#define CODANET_FORCEPLATE_MAX_SERIALNUMBER_LENGTH 32

/**********************************************************************/

typedef struct CODANET_DEVICEOPTIONS_FORCEPLATE_LOCATION
{
	struct CODANET_DEVICEOPTIONS_BASE base;
	DWORD dwPlateIndex;
	double R[9];  /* 3x3 matrix in column-major order */
	double t[3];  
} tagCODANET_DEVICEOPTIONS_FORCEPLATE_LOCATION;

/**********************************************************************/

typedef struct CODANET_DEVICEOPTIONS_FORCEPLATE_CHANNELOFFSET
{
	struct CODANET_DEVICEOPTIONS_BASE base;
	DWORD dwPlateIndex;
	float offsets[CODANET_FORCEPLATE_CHANNELS_MAX];
	float offsetstdev[CODANET_FORCEPLATE_CHANNELS_MAX];
} tagCODANET_DEVICEOPTIONS_FORCEPLATE_CHANNELOFFSET;

/**********************************************************************/

typedef struct CODANET_DEVICEINFO_FORCEPLATE_PLATE
{
	DWORD calctype;
	DWORD ondevice;
	DWORD channelstart;
	DWORD numchannels;
	float calparam[CODANET_FORCEPLATE_CALPARAMS_MAX];
	float outline[12];
	float offsets[CODANET_FORCEPLATE_CHANNELS_MAX];
	float offsetstdev[CODANET_FORCEPLATE_CHANNELS_MAX];
	double R[9];
	double t[3];
} tagCODANET_DEVICEINFO_FORCEPLATE_PLATE;

typedef struct CODANET_DEVICEINFO_FORCEPLATE
{
	struct CODANET_DEVICEINFO_BASE base;
	DWORD dwNumPlates;
	struct CODANET_DEVICEINFO_FORCEPLATE_PLATE plate[CODANET_FORCEPLATE_MAX];
} tagCODANET_DEVICEINFO_FORCEPLATE;

/**********************************************************************/

void CODANET_API codanet_deviceinfo_forceplate_hton(struct CODANET_DEVICEINFO_FORCEPLATE* info);
void CODANET_API codanet_deviceinfo_forceplate_ntoh(struct CODANET_DEVICEINFO_FORCEPLATE* info);

void CODANET_API codanet_deviceoptions_forceplate_location_hton(struct CODANET_DEVICEOPTIONS_FORCEPLATE_LOCATION* opt);
void CODANET_API codanet_deviceoptions_forceplate_location_ntoh(struct CODANET_DEVICEOPTIONS_FORCEPLATE_LOCATION* opt);

void CODANET_API codanet_deviceoptions_forceplate_channeloffset_hton(struct CODANET_DEVICEOPTIONS_FORCEPLATE_CHANNELOFFSET* opt);
void CODANET_API codanet_deviceoptions_forceplate_channeloffset_ntoh(struct CODANET_DEVICEOPTIONS_FORCEPLATE_CHANNELOFFSET* opt);

/**********************************************************************/

void CODANET_API codanet_forceplate_outline_global(float* outline, const double* R, const double* t, float sx, float sy);

#ifdef __cplusplus
}
#endif
