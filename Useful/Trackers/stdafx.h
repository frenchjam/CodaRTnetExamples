// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

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

#include "../Useful/fMessageBox.h"

// RTNet C++ includes
#define NO64BIT
#include "../Include/codaRTNetProtocolCPP/RTNetClient.h"
#include "../Include/codaRTNetProtocol/codartprotocol_gs16aio.h"
#include "../Include/codaRTNetProtocolCPP/DeviceOptionsAlignment.h"
#include "../Include/codaRTNetProtocolCPP/DeviceOptionsCodaMode.h"
#include "../Include/codaRTNetProtocolCPP/DeviceOptionsCodaPacketMode.h"
#include "../Include/codaRTNetProtocolCPP/DeviceOptionsGS16AIOInputs.h"
#include "../Include/codaRTNetProtocolCPP/DeviceInfoUnitCoordSystem.h"
#include "../Include/codaRTNetProtocolCPP/DeviceInfoAlignment.h"
#include "../Include/codaRTNetProtocolCPP/PacketDecode3DResult.h"
#include "../Include/codaRTNetProtocolCPP/PacketDecode3DResultExt.h"
#include "../Include/codaRTNetProtocolCPP/PacketDecodeADC16.h"

