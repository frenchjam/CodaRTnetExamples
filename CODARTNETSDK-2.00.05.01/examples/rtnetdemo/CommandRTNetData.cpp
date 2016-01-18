#include "CommandRTNetData.h"
#include "Framework/TracedException.h"

CommandRTNetData::CommandRTNetData() :
	dataoutput(NULL)
{
}

void CommandRTNetData::SetDataOutput(DataWriter* _dataoutput)
{
	dataoutput = _dataoutput;
}

CommandFactoryRTNetData::CommandFactoryRTNetData(ResultLog& _results, codaRTNet::RTNetClient& _client, DataWriter& _dataoutput) :
	CommandFactoryRTNet(_results, _client),
	dataoutput(&_dataoutput)
{
}

CommandRTNetData* CommandFactoryRTNetData::Build(const std::string& name)
{
	CommandRTNetData* command = static_cast<CommandRTNetData*>( CommandFactoryRTNet::Build(name) );
	command->SetDataOutput(dataoutput);
	return command;
}
