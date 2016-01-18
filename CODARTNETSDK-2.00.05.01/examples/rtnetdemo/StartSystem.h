#ifndef _START_SYSTEM_H_
#define _START_SYSTEM_H_

#include "CommandRTNet.h"
#include "Framework/Command/ParameterString.h"

/** Demonstrate system start-up */
class StartSystem : public CommandRTNet
{
public:

	/** Constructor used by factory */
	StartSystem();

	/** Implementation */
	virtual void RunRTNet() throw(TracedException, codaRTNet::NetworkException, codaRTNet::DeviceStatusArray);

protected:

	/** Name of config to use */
	ParameterString config_name;
};

/** Factory to make start-system commands */
class StartSystemFactory : public CommandFactoryRTNet
{
public:
	StartSystemFactory(ResultLog& _results, codaRTNet::RTNetClient& _client) :
			CommandFactoryRTNet(_results, _client)
	{
	}

protected:

	virtual StartSystem* NewCommandInstance()
	{ return new StartSystem; }
};

#endif
