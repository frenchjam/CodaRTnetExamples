#ifndef _PARAMETER_H
#define _PARAMETER_H

#include <string>

/** Base class for parameters which we can register with our command objects */
class Parameter
{
public:

	/** Construct empty / default value */
	Parameter();

	/** Destructor */
	virtual ~Parameter();

	/** Parse from string read from structured text 
			@param str Text to parse
		*/
	virtual void Parse(const std::string& str) = 0;

	/** See if successfully parsed 
			@return true if successfully parsed*/
	bool IsSet() const;

protected:

	/** Derived classes use this to set when successfully parsed */
	void Set();

private:
	Parameter(const Parameter& src);
	const Parameter& operator=(const Parameter& src);

private:
	bool isset;
};

#endif
