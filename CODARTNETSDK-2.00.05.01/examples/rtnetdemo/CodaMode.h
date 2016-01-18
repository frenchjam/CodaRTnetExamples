#ifndef _CODA_MODE_H_
#define _CODA_MODE_H_

#include "CommandRTNet.h"
#include "Framework/Command/ParameterInteger.h"

/** Demonstrate setting of CODA mode */
class CodaMode : public CommandRTNet
{
public:

	/** Constructor used by factory */
	CodaMode();

	/** Implementation */
	virtual void RunRTNet() throw(TracedException, codaRTNet::NetworkException, codaRTNet::DeviceStatusArray);

protected:
	
	/** Mode value to set (rate in Hz)*/
	ParameterInteger mode;
	
	/** Decimation to set */
	ParameterInteger decimation;
	
	/** Non-zero to set external-sync mode */
	ParameterInteger external_sync;
};

/** Factory for code mode commands */
class CodaModeFactory : public CommandFactoryRTNet
{
public:
	CodaModeFactory(ResultLog& _results, codaRTNet::RTNetClient& _client) :
			CommandFactoryRTNet(_results, _client)
	{
	}

protected:
	virtual CodaMode* NewCommandInstance()
	{ return new CodaMode; }
};

#endif
