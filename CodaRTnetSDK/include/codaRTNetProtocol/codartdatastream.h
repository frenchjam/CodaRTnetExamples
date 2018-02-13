/***********************************************************************
 * codaRTNetDataStream                                                 *
 * network interface definitions for realtime comms                    *
 ***********************************************************************/

#ifndef CODARTDATASTREAM_H
#define CODARTDATASTREAM_H

#include "codartapi.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 * Packet Constants                                                    *
 ***********************************************************************/

#define CODANET_PACKET_CHECKSUM_SIZE 4
#define CODANET_PACKET_HEADER_SIZE 12
#define CODANET_PACKET_RECV_FLAGS  0
#define CODANET_PACKET_SEND_FLAGS  0
#define CODANET_PACKET_MAXSIZE 4096
#define CODANET_PACKET_WAIT_INDEFINITELY 0xFFFFFFFFUL	/** Indicates that function will not timeout but will block indefinitely waiting for data */

/***********************************************************************
 * Packet Data Structures                                              *
 ***********************************************************************/

typedef struct CODANET_PACKET_HEADER
{
	WORD wDeviceID;
	WORD wPacketID;
	DWORD dwTick;
	DWORD dwPage;
} tagCODANET_PACKET_HEADER;

typedef struct CODANET_PACKET
{
	DWORD dwTransmitSize;
	BYTE* data;
} tagCODANET_PACKET;

/***********************************************************************
 * Packet data ID's                                                    *
 * There are a finite number of these and clients may choose which     *
 * ones they are able to interpret                                     *
 ***********************************************************************/

/* CODANET_PACKETID_3DRESULT
	*
	* for each marker
	*   float x
	*   float y
	*   float z
	*   byte  inview
	* end for
	*/
#define CODANET_PACKETID_3DRESULT 1

/* CODANET_PACKETID_3DRESULT_EXT
	*
	* WORD num markers
	* BYTE num cameras
	* BYTE num residuals
	* for each marker
	*   float x
	*   float y
	*   float z
	*   byte  inview
	*   for each camera
	*     BYTE intensity
	*   end for
	*   for each residual
	*     WORD microns
	*   end for
	* end for
	*/
#define CODANET_PACKETID_3DRESULT_EXT 2


/* CODANET_PACKETID_ADC16
	*
	* for each channel
	*   short x
	* end for
	*/
#define CODANET_PACKETID_ADC16 10


/* CODANET_PACKETID_CALCFLOAT32
	*
	* for each channel
	*   float x
	* end for
	*/
#define CODANET_PACKETID_CALCFLOAT32 21

/***********************************************************************
 * Data Streaming (packet) Functions API                               *
 ***********************************************************************/

/* codanet_packet_create
   Summary
	 Create network packet for realtime stream */
void   CODANET_API codanet_packet_create(struct CODANET_PACKET* packet, WORD wDeviceID, WORD wPacketID, DWORD dwTick, DWORD dwPage, DWORD dwDataSize);


/**codanet_packet_receive
   Summary
   Receive data on a realtime data stream
   
   Description
	 Wait for and receive incoming realtime data on the specified data stream.
	 See codanet_datastream_create for more information about creating data streams
	 and sending information to them.

	 codanet_packet_free should be called to free packet memory resources 
	 after the resulting packet has been used.

   @param packet  Pointer to packet structure which will be filled with the
	                received data.
   @param sktUDP  A valid data stream socket created using codanet_datastream_create
                  any error/status information
	 @param timeoutusec Maximum time to wait for new data (microseconds), 
	                    or CODANET_PACKET_WAIT_INDEFINITELY to wait indefinitely (blocking)
   
   @retval CODANET_OK Packet successfully received
	 @retval CODANET_SOCKETERROR_BROKEN An error occured when receiving data
	 @retval CODANET_STREAMTIMEOUT Timed out waiting for new data
 */
int    CODANET_API codanet_packet_receive(struct CODANET_PACKET* packet, CODANET_SOCKET sktUDP, DWORD timeoutusec);

/**codanet_packet_free
   Summary
   Free memory resources and destroy packet data
   
   Description
   Free memory resources and destroy packet data.  Used to free memory 
	 for the packets generated by codanet_packet_receive.

   @param packet  Pointer to packet structure to be freed
   
 */
void   CODANET_API codanet_packet_free(struct CODANET_PACKET* packet);

/* codanet_packet_create
   Summary
	 Send network packet on realtime stream */
int    CODANET_API codanet_packet_send(const struct CODANET_PACKET* packet, CODANET_SOCKET sktUDP, const struct sockaddr_in* addr);

/* codanet_packet_create
   Summary
	 Compute and set checksum value for packet before sending */
int    CODANET_API codanet_packet_checksum_set(struct CODANET_PACKET* packet);

/**codanet_packet_checksum_diff
   Summary
   Perform checksum for consistency check on incoming packet data
   
   Description
	 Packets received on a data stream using codanet_packet_receive are
	 not guaranteed to be free of errors.  Use this function to perform
	 a consistency check on incoming data.

   @param packet  Pointer to packet structure to be checked

	 @return Zero if packet is valid (zero checksum difference) non-zero otherwise.
   
 */
DWORD  CODANET_API codanet_packet_checksum_diff(const struct CODANET_PACKET* packet);

/**codanet_packet_datasize
   Summary
   Find size of data associated with received network packet.
   
   Description
   Find size of data associated with received network packet. 
	 The data may be retrieved using codanet_packet_datacontent.  
	 This value does not include the size of header information in the packet.

   @param packet  Pointer to packet structure

	 @return Size of data in the packet expressed in bytes
   
 */
DWORD  CODANET_API codanet_packet_datasize(const struct CODANET_PACKET* packet);

/**codanet_packet_datacontent
   Summary
   Retrieve pointer to data contained in a packet
   
   Description
   Retrieve pointer to data contained in a packet.  This
	 does not include header information.  The total size of 
	 data (in bytes) can be found using codanet_packet_datasize.  

   @param packet  Pointer to packet structure

	 @return Size of data in the packet.
   
 */
LPBYTE CODANET_API codanet_packet_datacontent(struct CODANET_PACKET* packet);

/**codanet_packet_readheader
   Summary
   Retrieve header information from a received network packet
   
   Description
   Retrieve header information from a received network packet.
   @param packet  Pointer to packet structure
   @param header  Pointer to structure to receive header
                  information
   
   @return Zero if successful, non-zero if packet header
   information was missing or empty.                           */

int    CODANET_API codanet_packet_readheader(const struct CODANET_PACKET* packet, struct CODANET_PACKET_HEADER* header);


#ifdef __cplusplus
}
#endif

#endif