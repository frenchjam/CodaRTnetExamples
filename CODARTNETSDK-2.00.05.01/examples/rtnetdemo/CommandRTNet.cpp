#include "CommandRTNet.h"
#include "Framework/TracedException.h"
#include "codaRTNetProtocolCPP/DeviceStatusArray.h"
#include "codaRTNetProtocolCPP/NetworkException.h"

CommandRTNet::CommandRTNet() :
	client(NULL)
{
}

void CommandRTNet::SetClient(codaRTNet::RTNetClient* _client)
{
	client = _client;
}

void CommandRTNet::Run() throw(TracedException)
{
	try
	{
		RunRTNet();
	}
	catch (const codaRTNet::DeviceStatusArray& )
	{
		STOP(Name(), "Exception received from server");
	}
	catch (const codaRTNet::NetworkException& )
	{
		STOP(Name(), "Network exception");
	}
}

CommandFactoryRTNet::CommandFactoryRTNet(ResultLog& _results, codaRTNet::RTNetClient& _client) :
	CommandFactory(_results),
	client(&_client)
{
}

CommandRTNet* CommandFactoryRTNet::Build(const std::string& name)
{
	CommandRTNet* command = static_cast<CommandRTNet*>( CommandFactory::Build(name) );
	command->SetClient(client);
	return command;
}
