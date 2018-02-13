#ifndef _PARAMETER_STRING_H_
#define _PARAMETER_STRING_H_

#include "Parameter.h"

/** Text string command parameter */
class ParameterString : public Parameter
{
public:

	/** Construct empty */
	ParameterString();

	/** Destructor */
	virtual ~ParameterString();

	/** Parse from string (just copies)
			@param str String to copy
		*/
	virtual void Parse(const std::string& str);

	/** Get value
			@return text value
		*/
	const std::string& Value() const
	{ return value; }

protected:

	std::string value;
};

#endif