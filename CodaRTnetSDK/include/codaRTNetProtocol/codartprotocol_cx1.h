#ifndef CODARTPROTOCOL_CX1_H
#define CODARTPROTOCOL_CX1_H

#include "codartprotocol.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 * Device ID                                                           *
 ***********************************************************************/

/** Device ID for Codamotion CX1 System device which represents 
    one or more CX1 measurement units */
#define DEVICEID_CX1 1

/***********************************************************************
 * Size Constants                                                      *
 ***********************************************************************/

#define CODANET_CODA_MAXUNITS 8
#define CODANET_CODA_MODE_100 100
#define CODANET_CODA_MODE_120 120
#define CODANET_CODA_MODE_200 200
#define CODANET_CODA_MODE_400 400
#define CODANET_CODA_MODE_800 800

#define CODANET_DEVICEINFO_ALIGNMENT_MAXFRAMES 200

#define CODANET_DEVICEINFO_CX1PORTS_AH           1
#define CODANET_DEVICEINFO_CX1PORTS_MH_BB_PCI    2
#define CODANET_DEVICEINFO_CX1PORTS_MH_BB_PCMCIA 3
#define CODANET_DEVICEINFO_CX1PORTS_MH_ES_USB 4

#define CODANET_ALIGNMENTERROR_NOTDONE                           1
#define CODANET_ALIGNMENTERROR_SYSTEM                            2
#define CODANET_ALIGNMENTERROR_ALREADY_ACQUIRING                 3
#define CODANET_ALIGNMENTERROR_OCCLUSIONS                        4
#define CODANET_ALIGNMENTERROR_XTOOCLOSE                         5
#define CODANET_ALIGNMENTERROR_XYTOOCLOSE                        6
#define CODANET_ALIGNMENTERROR_NOTPERP                           7

#define CODANET_CODAPACKETMODE_COMBINED_COORD              1
#define CODANET_CODAPACKETMODE_SEPARATE_COORD              2
#define CODANET_CODAPACKETMODE_SEPARATE_AND_COMBINED_COORD 3

/***********************************************************************
 * Network Protocol Data Structures: Device Options                    *
 ***********************************************************************/

typedef struct CODANET_HWCONFIG_CX1PORTS
{
	struct CODANET_HWCONFIG_BASE base;
	DWORD dwNumPorts;
	struct
	{
		DWORD number;
		DWORD type;
		DWORD enable;
	} port[CODANET_CODA_MAXUNITS];
} tagCODANET_DEVICEOPTIONS_CX1PORTS;

typedef struct CODANET_HWCONFIG_CX1MEASUREPARAMS
{
	struct CODANET_HWCONFIG_BASE base;
	DWORD corrmin_x1000000;
} tagCODANET_HWCONFIG_CX1MEASUREPARAMS;

typedef struct CODANET_DEVICEOPTIONS_CODAMODE
{
	struct CODANET_DEVICEOPTIONS_BASE base;
	DWORD dwRateMode;
	DWORD dwDecimation;
	DWORD dwExternalSync;
} tagCODANET_DEVICEOPTIONS_CODAMODE;

typedef struct CODANET_DEVICEOPTIONS_ALIGNMENT
{
	struct CODANET_DEVICEOPTIONS_BASE base;
	DWORD dwMarkerOrigin;
	DWORD dwMarkerX0;
	DWORD dwMarkerX1;
	DWORD dwMarkerXY0;
	DWORD dwMarkerXY1;
} tagCODANET_DEVICEOPTIONS_ALIGNMENT;

typedef struct CODANET_DEVICEOPTIONS_ALIGNMENT_EXT
{
	struct CODANET_DEVICEOPTIONS_ALIGNMENT basic;
	BYTE enable[CODANET_CODA_MAXUNITS]; 
	double fOriginOffset[3];
} tagCODANET_DEVICEOPTIONS_ALIGNMENT_EXT;

typedef struct CODANET_DEVICEOPTIONS_STROBEOUTPUT
{
	struct CODANET_DEVICEOPTIONS_BASE base;
	DWORD dwStrobeOutput;
} tagCODANET_DEVICEOPTIONS_STROBEOUTPUT;

typedef struct CODANET_DEVICEOPTIONS_CODAPACKETMODE
{
	struct CODANET_DEVICEOPTIONS_BASE base;
	DWORD dwPacketMode;
} tagCODANET_DEVICEOPTIONS_CODAPACKETMODE;

/***********************************************************************
 * Network Protocol Data Structures: Device Queries                    *
 ***********************************************************************/

typedef struct CODANET_DEVICEINFO_ALIGNMENT
{
	struct CODANET_DEVICEINFO_BASE base;
	DWORD dwStatus;
	DWORD dwUTC_MS;
	DWORD dwUTC_LS;
	WORD  wMilliseconds;
	WORD  wReserved;
	DWORD dwNumUnits;
	DWORD dwNumFrames;
	BYTE camera_flag[3*5*CODANET_CODA_MAXUNITS*CODANET_DEVICEINFO_ALIGNMENT_MAXFRAMES];
} tagCODANET_DEVICEINFO_ALIGNMENT;

typedef struct CODANET_DEVICEINFO_UNITCOORDSYSTEM
{
	struct CODANET_DEVICEINFO_BASE base;
	DWORD dwNumUnits;
	struct
	{
		double R[9];
		double t[3];

	} Rt[CODANET_CODA_MAXUNITS];
} tagCODANET_DEVICEINFO_UNITCOORDSYSTEM;

typedef struct CODANET_DEVICEINFO_CODAMODE
{
	struct CODANET_DEVICEINFO_BASE base;
	DWORD dwRateMode;
	DWORD dwDecimation;
	DWORD dwExternalSync;
} tagCODANET_DEVICEINFO_CODAMODE;

typedef struct CODANET_DEVICEINFO_CODAPACKETMODE
{
	struct CODANET_DEVICEINFO_BASE base;
	DWORD dwPacketMode;
} tagCODANET_DEVICEINFO_CODAPACKETMODE;

/***********************************************************************
 * Network Protocol Command ID's                                       *
 ***********************************************************************/

#define CODANET_HWCONFIGOPTIONID_CX1PORTS         1001
#define CODANET_DEVICEOPTIONID_CODAMODE           1002
#define CODANET_DEVICEOPTIONID_ALIGNMENT          1003
#define CODANET_DEVICEINFOID_ALIGNMENT            1004
#define CODANET_HWCONFIGOPTIONID_CX1MEASUREPARAMS 1005
#define CODANET_DEVICEOPTIONID_STROBEOUTPUT       1006
#define CODANET_DEVICEINFOID_UNITCOORDSYSTEM      1007
#define CODANET_DEVICEOPTIONID_CODAPACKETMODE     1008
#define CODANET_DEVICEINFOID_CODAMODE             1009
#define CODANET_DEVICEINFOID_CODAPACKETMODE       1010
#define CODANET_DEVICEOPTIONID_ALIGNMENT_EXT      1011

/***********************************************************************
 * Network Conversion Utilities                                        *
 ***********************************************************************/

void   CODANET_API codanet_deviceinfo_alignment_hton(struct CODANET_DEVICEINFO_ALIGNMENT* info);
void   CODANET_API codanet_deviceinfo_alignment_ntoh(struct CODANET_DEVICEINFO_ALIGNMENT* info);

int    CODANET_API codanet_deviceoptions_codamode_hton(struct CODANET_DEVICEOPTIONS_CODAMODE* dev);
int    CODANET_API codanet_deviceoptions_codamode_ntoh(struct CODANET_DEVICEOPTIONS_CODAMODE* dev);

int    CODANET_API codanet_deviceoptions_strobeoutput_hton(struct CODANET_DEVICEOPTIONS_STROBEOUTPUT* dev);
int    CODANET_API codanet_deviceoptions_strobeoutput_ntoh(struct CODANET_DEVICEOPTIONS_STROBEOUTPUT* dev);

int    CODANET_API codanet_deviceoptions_alignment_hton(struct CODANET_DEVICEOPTIONS_ALIGNMENT* dev);
int    CODANET_API codanet_deviceoptions_alignment_ntoh(struct CODANET_DEVICEOPTIONS_ALIGNMENT* dev);
int    CODANET_API codanet_deviceoptions_alignment_ext_hton(struct CODANET_DEVICEOPTIONS_ALIGNMENT_EXT* dev);
int    CODANET_API codanet_deviceoptions_alignment_ext_ntoh(struct CODANET_DEVICEOPTIONS_ALIGNMENT_EXT* dev);
int    CODANET_API codanet_hwconfig_cx1ports_hton(struct CODANET_HWCONFIG_CX1PORTS* dev);
int    CODANET_API codanet_hwconfig_cx1ports_ntoh(struct CODANET_HWCONFIG_CX1PORTS* dev);
int    CODANET_API codanet_hwconfig_cx1measureparams_hton(struct CODANET_HWCONFIG_CX1MEASUREPARAMS* dev);
int    CODANET_API codanet_hwconfig_cx1measureparams_ntoh(struct CODANET_HWCONFIG_CX1MEASUREPARAMS* dev);

void   CODANET_API codanet_deviceinfo_unitcoordsystem_hton(struct CODANET_DEVICEINFO_UNITCOORDSYSTEM* info);
void   CODANET_API codanet_deviceinfo_unitcoordsystem_ntoh(struct CODANET_DEVICEINFO_UNITCOORDSYSTEM* info);

int   CODANET_API codanet_deviceoptions_codapacketmode_hton(struct CODANET_DEVICEOPTIONS_CODAPACKETMODE* dev);
int   CODANET_API codanet_deviceoptions_codapacketmode_ntoh(struct CODANET_DEVICEOPTIONS_CODAPACKETMODE* dev);

int    CODANET_API codanet_deviceinfo_codamode_hton(struct CODANET_DEVICEINFO_CODAMODE* dev);
int    CODANET_API codanet_deviceinfo_codamode_ntoh(struct CODANET_DEVICEINFO_CODAMODE* dev);

int   CODANET_API codanet_deviceinfo_codapacketmode_hton(struct CODANET_DEVICEINFO_CODAPACKETMODE* dev);
int   CODANET_API codanet_deviceinfo_codapacketmode_ntoh(struct CODANET_DEVICEINFO_CODAPACKETMODE* dev);

#ifdef __cplusplus
}
#endif

#endif
