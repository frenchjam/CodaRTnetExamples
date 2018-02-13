# Microsoft Developer Studio Project File - Name="codaRTNetProtocolCPP_VS6" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=codaRTNetProtocolCPP_VS6 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "codaRTNetProtocolCPP_VS6.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "codaRTNetProtocolCPP_VS6.mak" CFG="codaRTNetProtocolCPP_VS6 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "codaRTNetProtocolCPP_VS6 - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "codaRTNetProtocolCPP_VS6 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "codaRTNetProtocolCPP_VS6 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Binaries\Release"
# PROP Intermediate_Dir "..\..\Temp\codaRTNetProtocolCPP_VS6\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "$(CODADEV_ROOT)\codaRTNet" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "NO64BIT" /YX /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\Binaries\Release\codaRTNetProtocolCPP_VS6SR.lib" /NODEFAULTLIB

!ELSEIF  "$(CFG)" == "codaRTNetProtocolCPP_VS6 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\Binaries\Debug"
# PROP Intermediate_Dir "..\..\Temp\codaRTNetProtocolCPP_VS6\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "$(CODADEV_ROOT)\codaRTNet" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "NO64BIT" /YX /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\Binaries\Debug\codaRTNetProtocolCPP_VS6SD.lib"

!ENDIF 

# Begin Target

# Name "codaRTNetProtocolCPP_VS6 - Win32 Release"
# Name "codaRTNetProtocolCPP_VS6 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\AutoDiscover.cpp
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\DataStream.cpp
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\DeviceInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\DeviceInfoAlignment.cpp
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\DeviceInfoAMTISerial.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceInfoCodaMode.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceInfoCodaPacketMode.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceInfoDI720USBInputs.cpp
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\DeviceInfoForcePlate.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceInfoGS16AIOInputs.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceInfoNI6221Inputs.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceInfoTeleMyoInputs.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceInfoUnitCoordSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceInfoVideo.cpp
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\DeviceOptions.cpp
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\DeviceOptionsAlignment.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceOptionsAMTISerial.cpp
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\DeviceOptionsCodaMode.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceOptionsCodaPacketMode.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceOptionsCodaStrobeOutput.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceOptionsDI720USBInputs.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceOptionsForcePlateChannelOffset.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceOptionsForcePlateLocation.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceOptionsGS16AIOInputs.cpp
# End Source File
# Begin Source File

SOURCE=.\DeviceOptionsTeleMyo.cpp
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\DeviceStatus.cpp
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\DeviceStatusArray.cpp
# End Source File
# Begin Source File

SOURCE=.\HWConfigOptions.cpp
# End Source File
# Begin Source File

SOURCE=.\HWConfigOptionsAMTIAnalog.cpp
# End Source File
# Begin Source File

SOURCE=.\HWConfigOptionsCX1MeasureParams.cpp
# End Source File
# Begin Source File

SOURCE=.\HWConfigOptionsCX1Ports.cpp
# End Source File
# Begin Source File

SOURCE=.\HWConfigOptionsKistler.cpp
# End Source File
# Begin Source File

SOURCE=.\HWConfigOptionsNI6221.cpp
# End Source File
# Begin Source File

SOURCE=.\HWConfigOptionsVideoInterfaceEnable.cpp
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\PacketDecode.cpp
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\PacketDecode3DResult.cpp
# End Source File
# Begin Source File

SOURCE=.\PacketDecode3DResultExt.cpp
# End Source File
# Begin Source File

SOURCE=.\PacketDecodeADC16.cpp
# End Source File
# Begin Source File

SOURCE=.\PacketDecodeCalcFloat32.cpp
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\RTNetClient.cpp
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\RTNetworkPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\Version.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\AutoDiscover.h
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\DataStream.h
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\DeviceEnum.h
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\DeviceInfo.h
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\DeviceInfoAlignment.h
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\DeviceInfoAMTISerial.h
# End Source File
# Begin Source File

SOURCE=.\DeviceInfoCodaMode.h
# End Source File
# Begin Source File

SOURCE=.\DeviceInfoCodaPacketMode.h
# End Source File
# Begin Source File

SOURCE=.\DeviceInfoDI720USBInputs.h
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\DeviceInfoForcePlate.h
# End Source File
# Begin Source File

SOURCE=.\DeviceInfoGS16AIOInputs.h
# End Source File
# Begin Source File

SOURCE=.\DeviceInfoNI6221Inputs.h
# End Source File
# Begin Source File

SOURCE=.\DeviceInfoTeleMyoInputs.h
# End Source File
# Begin Source File

SOURCE=.\DeviceInfoUnitCoordSystem.h
# End Source File
# Begin Source File

SOURCE=.\DeviceInfoVideo.h
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\DeviceOptions.h
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\DeviceOptionsAlignment.h
# End Source File
# Begin Source File

SOURCE=.\DeviceOptionsAMTISerial.h
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\DeviceOptionsCodaMode.h
# End Source File
# Begin Source File

SOURCE=.\DeviceOptionsCodaPacketMode.h
# End Source File
# Begin Source File

SOURCE=.\DeviceOptionsCodaStrobeOutput.h
# End Source File
# Begin Source File

SOURCE=.\DeviceOptionsDI720USBInputs.h
# End Source File
# Begin Source File

SOURCE=.\DeviceOptionsForcePlateChannelOffset.h
# End Source File
# Begin Source File

SOURCE=.\DeviceOptionsForcePlateLocation.h
# End Source File
# Begin Source File

SOURCE=.\DeviceOptionsGS16AIOInputs.h
# End Source File
# Begin Source File

SOURCE=.\DeviceOptionsTeleMyo.h
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\DeviceStatus.h
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\DeviceStatusArray.h
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\HWConfigEnum.h
# End Source File
# Begin Source File

SOURCE=.\HWConfigOptions.h
# End Source File
# Begin Source File

SOURCE=.\HWConfigOptionsAMTIAnalog.h
# End Source File
# Begin Source File

SOURCE=.\HWConfigOptionsCX1MeasureParams.h
# End Source File
# Begin Source File

SOURCE=.\HWConfigOptionsCX1Ports.h
# End Source File
# Begin Source File

SOURCE=.\HWConfigOptionsKistler.h
# End Source File
# Begin Source File

SOURCE=.\HWConfigOptionsNI6221.h
# End Source File
# Begin Source File

SOURCE=.\HWConfigOptionsVideoInterfaceEnable.h
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\NetworkException.h
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\PacketDecode.h
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\PacketDecode3DResult.h
# End Source File
# Begin Source File

SOURCE=.\PacketDecode3DResultExt.h
# End Source File
# Begin Source File

SOURCE=.\PacketDecodeADC16.h
# End Source File
# Begin Source File

SOURCE=.\PacketDecodeCalcFloat32.h
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\RTNetClient.h
# End Source File
# Begin Source File

SOURCE=..\codaRTNetProtocolCPP\RTNetworkPacket.h
# End Source File
# Begin Source File

SOURCE=.\Version.h
# End Source File
# End Group
# End Target
# End Project
