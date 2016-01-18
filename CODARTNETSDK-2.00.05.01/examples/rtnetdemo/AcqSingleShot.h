#ifndef _ACQ_SINGLE_SHOT_H_
#define _ACQ_SINGLE_SHOT_H_

#include "CommandRTNetData.h"
#include "Framework/Command/ParameterInteger.h"
#include "Framework/Command/ParameterString.h"

/** Demonstrate single-shot acquisition command */
class AcqSingleShot : public CommandRTNetData
{
public:

	/** Constructor used by factory */
	AcqSingleShot();

	/** Implementation */
	virtual void RunRTNet() throw(TracedException, codaRTNet::NetworkException, codaRTNet::DeviceStatusArray);

protected:
	/** Total number of single-shot samples to take */
	ParameterInteger num_samples;

	/** Delay period (milliseconds) between samples */
	ParameterInteger period;

	/** Title to put in data output */
	ParameterString title;
};

/** Factory for single-shot acquisition commands */
class AcqSingleShotFactory : public CommandFactoryRTNetData
{
public:
	AcqSingleShotFactory(ResultLog& _results, codaRTNet::RTNetClient& _client, DataWriter& _dataoutput) :
			CommandFactoryRTNetData(_results, _client, _dataoutput)
	{
	}

protected:
	virtual AcqSingleShot* NewCommandInstance()
	{ return new AcqSingleShot; }
};

#endif
