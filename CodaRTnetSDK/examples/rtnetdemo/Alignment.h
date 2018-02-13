#ifndef _ALIGNMENT_H_
#define _ALIGNMENT_H_

#include "CommandRTNet.h"
#include "Framework/Command/ParameterInteger.h"

// forward declarations
namespace codaRTNet 
{ 
	class DeviceOptionsAlignment; 
	class DeviceInfoAlignment; 
}

/** Demonstrate alignment */
class Alignment : public CommandRTNet
{
public:

	/** Constructor used by factory */
	Alignment();

	/** Implementation */
	virtual void RunRTNet() throw(TracedException, codaRTNet::NetworkException, codaRTNet::DeviceStatusArray);

protected:

	void LogStatus(const codaRTNet::DeviceOptionsAlignment& align, const codaRTNet::DeviceInfoAlignment& info);

protected:

	/** Origin marker ID for alignment options */
	ParameterInteger origin;

	/** X0 marker ID for alignment options */
	ParameterInteger x0;

	/** X1 marker ID for alignment options */
	ParameterInteger x1;

	/** XY0 marker ID for alignment options */
	ParameterInteger xy0;

	/** XY1 marker ID for alignment options */
	ParameterInteger xy1;

	/** Expected result of alignment in this step */
	ParameterInteger expected_result;

	/** Non-zero means retry attempts will be allowed */
	ParameterInteger allow_retry;

	/** If non-zero it will not cause an abort error if user cancels alignment */
	ParameterInteger allow_cancel;
};

/** Factory for alignment commands */
class AlignmentFactory : public CommandFactoryRTNet
{
public:
	AlignmentFactory(ResultLog& _results, codaRTNet::RTNetClient& _client) :
			CommandFactoryRTNet(_results, _client)
	{
	}

protected:
	virtual Alignment* NewCommandInstance()
	{ return new Alignment; }
};

#endif
