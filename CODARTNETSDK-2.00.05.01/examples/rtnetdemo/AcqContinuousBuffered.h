#ifndef _ACQ_CONTINUOUS_BUFFERED_H_
#define _ACQ_CONTINUOUS_BUFFERED_H_

#include "CommandRTNetData.h"
#include "Framework/Command/ParameterInteger.h"
#include "Framework/Command/ParameterString.h"

/** Demonstrate use of continuous buffered acquisition */
class AcqContinuousBuffered : public CommandRTNetData
{
public:

	/** Constructor used by factory */
	AcqContinuousBuffered();

	/** Implementation */
	virtual void RunRTNet() throw(TracedException, codaRTNet::NetworkException, codaRTNet::DeviceStatusArray);

protected:

	/** Device to use to set frame count */
	ParameterInteger frame_counter_device;

	/** Frame count limit to set, or zero to set unlimited */
	ParameterInteger frames;

	/** Delay between requests for frames during real-time monitoring */
	ParameterInteger monitor_period;

	/** Title to put in output of packets from real-time monitoring, or empty to avoid writing during monitoring */
	ParameterString monitor_title;

	/** Title to put in output of packets downloaded post-acquisition, or empty to skip download */
	ParameterString download_title;
};

/** Factory for continuous buffered acquisition commands */
class AcqContinuousBufferedFactory : public CommandFactoryRTNetData
{
public:
	AcqContinuousBufferedFactory(ResultLog& _results, codaRTNet::RTNetClient& _client, DataWriter& _dataoutput) :
			CommandFactoryRTNetData(_results, _client, _dataoutput)
	{
	}

protected:
	virtual AcqContinuousBuffered* NewCommandInstance()
	{ return new AcqContinuousBuffered; }
};

#endif
