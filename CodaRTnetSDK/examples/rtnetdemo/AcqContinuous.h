#ifndef _ACQ_CONTINUOUS_H_
#define _ACQ_CONTINUOUS_H_

#include "CommandRTNetData.h"
#include "Framework/Command/ParameterInteger.h"
#include "Framework/Command/ParameterString.h"

/** Demonstrate use of continuous acquisition */
class AcqContinuous : public CommandRTNetData
{
public:

	/** Constructor used by factory */
	AcqContinuous();

	/** Implementation */
	virtual void RunRTNet() throw(TracedException, codaRTNet::NetworkException, codaRTNet::DeviceStatusArray);

protected:
	/** Device to use to set frame count */
	ParameterInteger frame_counter_device;

	/** Frame count limit to set, or zero to set unlimited */
	ParameterInteger frames;

	/** Title to put in data output, or empty to skip writing output */
	ParameterString title;
};

/** Factory for continuous acquisition commands */
class AcqContinuousFactory : public CommandFactoryRTNetData
{
public:
	AcqContinuousFactory(ResultLog& _results, codaRTNet::RTNetClient& _client, DataWriter& _dataoutput) :
			CommandFactoryRTNetData(_results, _client, _dataoutput)
	{
	}

protected:
	virtual AcqContinuous* NewCommandInstance()
	{ return new AcqContinuous; }
};

#endif
