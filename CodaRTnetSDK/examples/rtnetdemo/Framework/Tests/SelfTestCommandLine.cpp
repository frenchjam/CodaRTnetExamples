#include "../CommandLineOptions.h"
#include "../TracedException.h"
#include "SelfTestMacros.h"
#include "SelfTestCommandLine.h"

void SelfTestCommandLine() throw(TracedException)
{
	std::string datafile;
	CommandLineOptions testopt;
	testopt.RegisterOption("data-file", datafile, "");
	char* testargv[] = { "", "--data-file", "exampledatafile" };
	testopt.Parse(3, testargv);
	SELF_TEST_ASSERT(datafile == "exampledatafile");
}
