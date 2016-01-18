#ifndef _CODA_PACKET_MODE_H_
#define _CODA_PACKET_MODE_H_

#include "CommandRTNet.h"
#include "Framework/Command/ParameterString.h"

/** Demonstrate setting of packet mode for combined coordinates or multi-coordinates */
class CodaPacketMode : public CommandRTNet
{
public:

	/** Constructor used by factory */
	CodaPacketMode();

	/** Implementation */
	virtual void RunRTNet() throw(TracedException, codaRTNet::NetworkException, codaRTNet::DeviceStatusArray);

protected:
	
	/** Coordiante mode - should be one of "combined", "separate", "combined-and-separate" */
	ParameterString coord_mode;	
};

/** Factory for code mode commands */
class CodaPacketModeFactory : public CommandFactoryRTNet
{
public:
	CodaPacketModeFactory(ResultLog& _results, codaRTNet::RTNetClient& _client) :
			CommandFactoryRTNet(_results, _client)
	{
	}

protected:
	virtual CodaPacketMode* NewCommandInstance()
	{ return new CodaPacketMode; }
};


#endif