#ifndef _SELF_TEST_MACROS_
#define _SELF_TEST_MACROS_

/**  Indicate self-test failure with specified message 
     @param _message The message to log 
 */
#define SELF_TEST_FAIL(_message) STOP(SelfTestModuleName, _message)

/** Self-test failure if condition is false.  The code given for the condition will be logged
    as the failure message
		@param condition Condition to test
	*/
#define SELF_TEST_ASSERT(condition) { if (!(condition)) { SELF_TEST_FAIL(#condition); } }

/** Identifier to use for module name if an exception is thrown during self-test */
extern const char* SelfTestModuleName;

#endif
