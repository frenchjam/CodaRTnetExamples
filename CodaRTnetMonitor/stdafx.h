// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// Disable warnings about unsafe char routines.
#define _CRT_SECURE_NO_WARNINGS

// Standard library includes
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <memory.h>
#include <math.h>
#include <windows.h> // For the OutputDebugString routine.

// RTNet C++ includes
#define NO64BIT
#include "../../CodaRTnetSDK/include/codaRTNetProtocolCPP/RTNetClient.h"
#include "../../CodaRTnetSDK/include/codaRTNetProtocol/codartprotocol_gs16aio.h"
#include "../../CodaRTnetSDK/include/codaRTNetProtocolCPP/DeviceOptionsAlignment.h"
#include "../../CodaRTnetSDK/include/codaRTNetProtocolCPP/DeviceOptionsCodaMode.h"
#include "../../CodaRTnetSDK/include/codaRTNetProtocolCPP/DeviceOptionsCodaPacketMode.h"
#include "../../CodaRTnetSDK/include/codaRTNetProtocolCPP/DeviceOptionsGS16AIOInputs.h"
#include "../../CodaRTnetSDK/include/codaRTNetProtocolCPP/DeviceInfoUnitCoordSystem.h"
#include "../../CodaRTnetSDK/include/codaRTNetProtocolCPP/DeviceInfoAlignment.h"
#include "../../CodaRTnetSDK/include/codaRTNetProtocolCPP/PacketDecode3DResult.h"
#include "../../CodaRTnetSDK/include/codaRTNetProtocolCPP/PacketDecode3DResultExt.h"
#include "../../CodaRTnetSDK/include/codaRTNetProtocolCPP/PacketDecodeADC16.h"