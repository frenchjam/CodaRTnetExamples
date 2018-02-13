#ifndef _COMMAND_RT_NET_DATA_H_
#define _COMMAND_RT_NET_DATA_H_

#include "CommandRTNet.h"

class DataWriter;

/** Extend abstract RTNet command class also to allow a data output writer to be set by factory */
class CommandRTNetData : public CommandRTNet
{
	friend class CommandFactoryRTNetData;

protected:
	
	/** Constructor to be used by factory */
	CommandRTNetData();

	/** Allow factory to set data writer object */
	void SetDataOutput(DataWriter* _dataoutput);

public:

	/** Get data writer as set by factory just after construction */
	DataWriter& DataOutput()
	{ return *dataoutput; }

private:
	DataWriter* dataoutput;
};

/** Extend general RT Net command factory to allow data output writer to be set in the commands we build */
class CommandFactoryRTNetData : public CommandFactoryRTNet
{
public:
	/** Construct knowing the log, client, and data output which we will give to the commands we build
			@param _results Results log to give to commands
			@param _client RT Net SDK client to give to commands
		*/
	CommandFactoryRTNetData(ResultLog& _results, codaRTNet::RTNetClient& _client, DataWriter& _dataoutput);

	/** Build new command and set data writer 
			@param name Name to use for command module */
	virtual CommandRTNetData* Build(const std::string& name);

private:
	DataWriter* dataoutput;
};

#endif