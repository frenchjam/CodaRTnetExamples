/***********************************************************************
 * codarterrorcodes                                                    *
 * device status error codes for realtime comms                        *
 ***********************************************************************/

#ifndef CODARTERRORCODES_H
#define CODARTERRORCODES_H

#ifdef __cplusplus
extern "C" {
#endif

#define CODANET_DS_OK                                                  0

#define CODANET_DS_MISSING_DEVICE                                      1
#define CODANET_DS_OS_NOTIMER                                          2
#define CODANET_DS_DEVICE_DISABLED                                     4

#define CODANET_PROTOCOL_STRUCTURE_SIZE_INCONSISTENT                1001
#define CODANET_PROTOCOL_INPUT_SIZE_UNEXPECTED                      1002
#define CODANET_PROTOCOL_MESSAGE_LESSTHANDWORD                      1003
#define CODANET_PROTOCOL_MESSAGE_UNRECOGNISED                       1004
#define CODANET_PROTOCOL_STRINGTOOLONG                              1005

#define CODANET_HWCONFIGERROR_DUPLICATENAME                         1506
#define CODANET_HWCONFIGERROR_REMOVEFAILED                          1507
#define CODANET_HWCONFIGERROR_NOTFOUND                              1508
#define CODANET_HWCONFIGERROR_CHANGEOVER                            1509

#define CODANET_DS_ACQBUFFERMONITOR_NOACQBUFFERMODE                 2001
/*
#define CODANET_DS_SINGLESHOT_WHILST_ACQUIRING                      2002
#define CODANET_DS_ACQSTOP_NOACQMODE                                2003
*/
#define CODANET_DS_ACQBUFFER_NOSUCHPACKET                           2004
#define CODANET_DS_ACQBUFFERMONITOR_NOTICKSCOMPLETE                 2005
/*
#define CODANET_DS_ACQPREPARE_WHILST_ACQUIRING                      2006
#define CODANET_DS_ACQPREPARE_WHILST_SINGLESHOT                     2007
#define CODANET_DS_ACQCONTINUOUS_WHILST_SINGLESHOT                  2008
#define CODANET_DS_SINGLESHOT_WHILST_ACQPREPARING                   2009
#define CODANET_DS_ACQCONTINUOUS_WHILST_ACQPREPARING								2010
*/

#define CODANET_DS_STATEERROR_TRANSITIONINPROGRESS                  2051
#define CODANET_DS_STATEERROR_NOCHANGESTARTED                       2052
#define CODANET_DS_STATEERROR_ILLEGALCHANGE                         2053

#define CODANET_DS_SIMULATOR_FILE_MISSING                           9001
#define CODANET_DS_SIMULATOR_FILE_NOMARKERS                         9002
#define CODANET_DS_SIMULATOR_FILE_NOANALOG                          9003

// failure messages associated with CX1 system startup
#define CODANET_DS_CX1_PORT_OPEN_FAILED                            10001
#define CODANET_DS_CX1_COMMS_LORATE_FAILED                         10002
#define CODANET_DS_CX1_COMMS_SETHIRATE_FAILED                      10003
#define CODANET_DS_CX1_COMMS_VERIFYHIRATE_FAILED                   10004
#define CODANET_DS_CX1_DATAUPLOADFAILED                            10005
#define CODANET_DS_CX1_RESETDSP_FAILED                             10006
#define CODANET_DS_CX1_STARTDSP_FAILED                             10007
#define CODANET_DS_CX1_COMMS_VERIFYRESETDSP_FAILED                 10008
#define CODANET_DS_CX1_COMMS_VERIFYSTARTDSP_FAILED                 10009

// more general errors
#define CODANET_DS_CX1_COMMS_GENERALFAILURE                        10020
#define CODANET_DS_CX1_SEMANTIC_EXCEPTION                          10021

// failures associated with offset calibration
#define CODANET_DS_CX1_OFFSETCAL_SHARCFAILED                       10022

// timing info errors
#define CODANET_DS_CX1_COORDPTRZERO                                10023
#define CODANET_DS_CX1_COORDPTROUTOFRANGE                          10024
#define CODANET_DS_CX1_STROBEENDTIMEOUT                            10025

// sync chip errors
#define CODANET_DS_CX1_SYNCCHIP_OPENFAILED                         10030
#define CODANET_DS_CX1_SYNCCHIP_CX1RESETFAILED                     10031
#define CODANET_DS_CX1_SYNCCHIP_ENABLECX1IN_FAILED                 10032
#define CODANET_DS_CX1_SYNCCHIP_ENABLEEXTIN_FAILED                 10033
#define CODANET_DS_CX1_SYNCCHIP_SETACTIVEHI_FAILED                 10034
#define CODANET_DS_CX1_SYNCCHIP_STARTFAILED                        10035
#define CODANET_DS_CX1_SYNCCHIP_STOPFAILED                         10036
#define CODANET_DS_CX1_SYNCCHIP_SETFREQFAILED                      10037

// cal file errors
#define CODANET_DS_CX1_FILE_PROGRAM_LOADFAILED                     10051
#define CODANET_DS_CX1_FILE_PROGRAM_BADFORMAT                      10052
#define CODANET_DS_CX1_FILE_TEMPL_LOADFAILED                       10053
#define CODANET_DS_CX1_FILE_TEMPL_BADFORMAT                        10054
#define CODANET_DS_CX1_FILE_TNORM_LOADFAILED                       10055
#define CODANET_DS_CX1_FILE_TNORM_BADFORMAT                        10056
#define CODANET_DS_CX1_FILE_TEMPLMID_LOADFAILED                    10057
#define CODANET_DS_CX1_FILE_TEMPLMID_BADFORMAT                     10058
#define CODANET_DS_CX1_FILE_GAINS_LOADFAILED                       10059
#define CODANET_DS_CX1_FILE_GAINS_BADFORMAT                        10060
#define CODANET_DS_CX1_FILE_GAINS_SERIALMISMATCH                   10061
#define CODANET_DS_CX1_FILE_CALGEOM_LOADFAILED                     10062
#define CODANET_DS_CX1_FILE_CALGEOM_BADFORMAT                      10064
#define CODANET_DS_CX1_FILE_CALGEOM_SERIALMISMATCH                 10065
#define CODANET_DS_CX1_FILE_PEAKLOOKUP_LOADFAILED                  10066
#define CODANET_DS_CX1_FILE_PEAKLOOKUP_BADFORMAT                   10067
#define CODANET_DS_CX1_FILE_PEAKLOOKUP_SERIALMISMATCH              10068

// gains calibration errors
#define CODANET_DS_CX1_GAINS_ACQ_OVERWRITE                         10101
#define CODANET_DS_CX1_GAINS_ACQ_FILEOPEN_FAILED                   10102
#define CODANET_DS_CX1_GAINS_DELETE_FAILED                         10103

	// registry errors
#define CODANET_DS_CX1_REGISTRYFORMATBAD                           10120
#define CODANET_DS_CX1_NOT_STARTED                                 10121

// additional verification
#define CODANET_DS_CX1_MODEVERIFYFAILED                            10150
#define CODANET_DS_CX1_EPOCHSYNCTIMEOUT                            10151
#define CODANET_DS_CX1_CONTROLSETTIMEOUT                           10152

// serial comms errors for AMTI serial devices
#define CODANET_DS_AMTI_COMMS_PORT_NOT_OPEN                        20001
#define CODANET_DS_AMTI_COMMS_NOT_READ                             20002
#define CODANET_DS_AMTI_SERIALNUMBER_MISMATCH                      20003
#define CODANET_DS_AMTI_SETRATE_FAILED                             20004
#define CODANET_DS_AMTI_SETFILTER_FAILED                           20005
#define CODANET_DS_AMTI_AUTOZERO_FAILED_SINGLE                     20006
#define CODANET_DS_AMTI_AUTOZERO_FAILED_PAIR                       20007

#define CODANET_DS_FP_CALFILE_NOT_FOUND									           21001
#define CODANET_DS_FP_CALFILE_SERIALNUMBER_MISMATCH                21003
#define CODANET_DS_FP_CALFILE_FORMAT_MISSINGFORCEPLATE             21004
#define CODANET_DS_FP_CALFILE_MISSINGSIZE                          21005
#define CODANET_DS_FP_CALFILE_SIZEFORMAT                           21006
#define CODANET_DS_FP_CALFILE_MISSINGCALPARAM                      21007
#define CODANET_DS_FP_CALFILE_CALPARAMFORMAT                       21008

// 1000 error codes allocated to GS16AIO
#define CODANET_DS_GS16AIO_FIRST_ERROR_CODE                        11513
#define CODANET_DS_GS16AIO_LAST_ERROR_CODE                         12512

#define CODANET_DS_NI6221_ERROR 90001

// #define CODANET_DS_CX1_SHARC_EPOCHREQUEST_FAILED                   10021
// #define CODANET_DS_CX1_GAINS_SET_NOTEXPLICIT                       10023
// #define CODANET_DS_CX1_ACQ_TIMEOUT                                 10024

#define CODANET_DIGITAL_NO_OUTPUT_DEVICE_FOUND                        11513
#define CODANET_DIGITAL_CHANNEL_CONFLICTS_FOUND                         12512


#ifdef __cplusplus
}
#endif

#endif
