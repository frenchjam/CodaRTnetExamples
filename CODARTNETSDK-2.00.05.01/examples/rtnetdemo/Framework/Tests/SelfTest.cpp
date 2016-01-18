#include <strstream>
#include "../TracedException.h"
#include "../ResultLog.h"
#include "SelfTestCommandLine.h"
#include "SelfTestScripts.h"
#include "SelfTestDataReader.h"
#include "SelfTestDataWriter.h"
#include "SelfTestMacros.h"
#include "SelfTest.h"

#define TEST_RUN(t) { results.Log1(SelfTestModuleName, #t); t(); }

void SelfTest(ResultLog& results) throw(TracedException)
{
	results.Log1(SelfTestModuleName, "Begin");
	TEST_RUN(SelfTestCommandLine)
	TEST_RUN(SelfTestScripts)
	TEST_RUN(SelfTestDataReader)
	TEST_RUN(SelfTestDataWriter)
	results.Log1(SelfTestModuleName, "Ok");
}
