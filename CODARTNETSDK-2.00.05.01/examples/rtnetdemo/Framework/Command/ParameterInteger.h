#ifndef _PARAMETER_INTEGER_H_
#define _PARAMETER_INTEGER_H_

#include "Parameter.h"

/** Integer command parameter */
class ParameterInteger : public Parameter
{
public:

	/** Construct and initialise to zero */
	ParameterInteger();

	/** Destructor */
	virtual ~ParameterInteger();

	/** Parse string to integer */
	virtual void Parse(const std::string& str);

	/** Get value of integer 
			@return value */
	const long Value() const
	{ return value; }

protected:

	long value;
};

#endif
