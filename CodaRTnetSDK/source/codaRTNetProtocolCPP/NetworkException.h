#pragma once

namespace codaRTNet
{
	/** Summary
			Contains error code for network connection errors

			Description
			Objects of this class are thrown by RTNetClient functions if 
			network connection problems occur.  The errorcode provides further
			information about the type of error
		*/
	class NetworkException
	{
	public:
		
		/**codaRTNet::NetworkException::errorcode
		   Summary
		   The error code
		   
		   Description
		   This will be one of the following symbolic constants:
		   <TABLE>
		   Symbolic Constant                             Meaning
		   --------------------------------------------  -------------------------------------------------
		   CODANET_OK                                    No error occured
		   CODANET_SOCKETERROR_BROKEN                    Network socket connection broken
		   CODANET_SOCKETERROR_WINDOWSDLL                Windows sockets DLL missing or
		                                                  unavailable
		   CODANET_SOCKETERROR_CREATE                    Socket could not be created
		   CODANET_SOCKETERROR_HOSTNAME                  Hostname could not be resolved
		   CODANET_SOCKETERROR_CONNECT                   Could not connect to specified host
		   CODANET_SOCKETERROR_TCPOPTIONS                Error occured whilst setting the
		                                                  required TCP options
		   CODANET_CLIENTPROTOCOLERROR_BADNUMSTATUS      Unexpected status results from server (possible
		                                                  data corruption or incorrect client
		                                                  \version)
		   CODANET_SOCKETERROR_STREAMCREATE              Unable to create new stream 
		   CODANET_SOCKETERROR_STREAMPORT                Unable to bind stream to the requested
		                                                  port - most commonly happens if there is
		                                                  an existing stream already created on
		                                                  that port
		   CODANET_CLIENTPROTOCOLERROR_TOOBIG            Unexpected return results from server
		                                                  (possible data corruption or incorrect
		                                                  client version)
		   CODANET_STREAMTIMEOUT                         Timed out whilst waiting for data
		   CODANET_CLIENTPROTOCOLERROR_BADNUMCONFIG      Unexpected return results from server retrieving
		                                                  hardware configuration (possible data
		                                                  corruption or incorrect client version)
		   CODANET_CLIENTPROTOCOLERROR_BADSTRINGLENGTH   Unexpected string length from server
		                                                  (possible data corruption or incorrect
		                                                  client version)
		   CODANET_CLIENTPROTOCOLERROR_BADNUMDEVICES     Unexpected number of devices from server
		                                                  (possible data corruption or incorrect
		                                                  client version)
		   CODANET_CLIENTPROTOCOLERROR_BADSTRUCTSIZE     Unexpected structure size from server
		                                                  (possible data corruption or incorrect
		                                                  client version)
		   </TABLE>                                                                                        */
		int errorcode;
		
		/** Construct with specified error code */
		NetworkException(int ncode) :
			errorcode(ncode)
		{
		}
	};
}
