#ifndef _COMMAND_RT_NET_H_
#define _COMMAND_RT_NET_H_

#include "Framework/ResultLog.h"
#include "Framework/TracedException.h"
#include "Framework/Command/Command.h"
#include "Framework/Command/CommandFactory.h"

// forward declaration
namespace codaRTNet { class RTNetClient; }

/** Extension of abstract Command class to allow global RT Net SDK client to be set from factory
		and RT Net SDK exceptions to be caught */
class CommandRTNet : public Command
{
	friend class CommandFactoryRTNet;

protected:

	/** Constructor to be called by factory */
	CommandRTNet();

	/** Allow factory to set RT Net Client
			@param _client Client to set */
	void SetClient(codaRTNet::RTNetClient* _client);

public:

	/** Get client which was set by factory just after construction */
	codaRTNet::RTNetClient& Client()
	{ return *client; }

	/** Implementation of abstract Run method to use new abstract RunRTNet method
			and catch RTNet exceptions */
	virtual void Run() throw(TracedException);

	/** Abstract method to be used by derived classes */
	virtual void RunRTNet() throw(codaRTNet::NetworkException, codaRTNet::DeviceStatusArray) = 0;

private:
	codaRTNet::RTNetClient* client;
};

/** Extension of command factory to allow RT Net SDK client to be set in the commands we build */
class CommandFactoryRTNet : public CommandFactory
{
public:
	/** Construct knowing the log and client which we will give to the commands we build
			@param _results Results log to give to commands
			@param _client RT Net SDK client to give to commands
		*/
	CommandFactoryRTNet(ResultLog& _results, codaRTNet::RTNetClient& _client);

	/** Build new command and set RT Net SDK client 
			@param name Name to use for command module */
	virtual CommandRTNet* Build(const std::string& name);

private:
	codaRTNet::RTNetClient* client;
};

#endif