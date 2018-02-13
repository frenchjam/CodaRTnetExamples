#ifndef _SELF_TEST_
#define _SELF_TEST_

class TracedException;
class ResultLog;

/** Run all self-tests
    @param results Log list of tests to this log
		@throws TracedException
*/
void SelfTest(ResultLog& results) throw(TracedException);

#endif
