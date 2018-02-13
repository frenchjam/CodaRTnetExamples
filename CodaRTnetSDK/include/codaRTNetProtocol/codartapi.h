#ifndef CODARTAPI_H
#define CODARTAPI_H

#ifdef __cplusplus
extern "C" {
#endif

/* MS Windows DLL Import / Export */
#ifdef WIN32
#ifdef CODARTCLIENT_EXPORTS
	#define CODANET_API __declspec(dllexport) __stdcall
#else
	#define CODANET_API __declspec(dllimport) __stdcall
#endif
#else
  #define CODANET_API
#endif

#ifndef _WCHAR_T_DEFINED
typedef unsigned short wchar_t;	/** Wide character type */
#define _WCHAR_T_DEFINED
#endif

#ifndef LPWCHAR
	typedef wchar_t* LPWCHAR;	/** Pointer to wide character string */
#endif

  
/** Represents a network connection. This may be to the
   Codamotion Realtime Server or a data stream for receiving
   data.
   
   \NOTE: this matches the SOCKET type in the sockets library of
   host OS.                                                      */
#ifdef WIN32
#ifdef NO64BIT
  #define __w64
#endif
  typedef __w64 unsigned int CODANET_SOCKET;
#else
  typedef int CODANET_SOCKET;
#endif

/* must match sizes in host OS */
typedef unsigned char BYTE;		/** 8-bit unsigned value */
typedef unsigned short WORD;	/** 16-bit unsigned value */
typedef unsigned long DWORD;  /** 32-bit unsigned value */

typedef BYTE* LPBYTE;					/** Pointer to 8-bit unsigned value */
typedef WORD* LPWORD;					/** Pointer to 16-bit unsigned value */
typedef DWORD* LPDWORD;				/** Pointer to 32-bit unsigned value */

/** Indicates invalid CODANET_SOCKET value. 

	\NOTE: this matches the SOCKET_INVALID value in sockets library of the host OS */
#ifdef WIN32
  #define CODANET_SOCKET_ERROR 0xFFFFFFFFUL
#else
  #define CODANET_SOCKET_ERROR -1
#endif

#ifdef __cplusplus
}
#endif

#endif
