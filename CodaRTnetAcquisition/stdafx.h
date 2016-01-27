// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// TODO: reference additional headers your program requires here
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
#include "../CODARTNETSDK-2.00.05.01/include/codaRTNetProtocol/joe.h"
#include "../CODARTNETSDK-2.00.05.01/include/codaRTNetProtocolCPP/RTNetClient.h"
#include "../CODARTNETSDK-2.00.05.01/include/codaRTNetProtocol/codartprotocol_gs16aio.h"
#include "../CODARTNETSDK-2.00.05.01/include/codaRTNetProtocolCPP/DeviceOptionsAlignment.h"
#include "../CODARTNETSDK-2.00.05.01/include/codaRTNetProtocolCPP/DeviceOptionsCodaMode.h"
#include "../CODARTNETSDK-2.00.05.01/include/codaRTNetProtocolCPP/DeviceOptionsCodaPacketMode.h"
#include "../CODARTNETSDK-2.00.05.01/include/codaRTNetProtocolCPP/DeviceOptionsGS16AIOInputs.h"
#include "../CODARTNETSDK-2.00.05.01/include/codaRTNetProtocolCPP/DeviceInfoUnitCoordSystem.h"
#include "../CODARTNETSDK-2.00.05.01/include/codaRTNetProtocolCPP/DeviceInfoAlignment.h"
#include "../CODARTNETSDK-2.00.05.01/include/codaRTNetProtocolCPP/PacketDecode3DResult.h"
#include "../CODARTNETSDK-2.00.05.01/include/codaRTNetProtocolCPP/PacketDecode3DResultExt.h"
#include "../CODARTNETSDK-2.00.05.01/include/codaRTNetProtocolCPP/PacketDecodeADC16.h"


