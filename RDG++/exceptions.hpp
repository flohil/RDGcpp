#ifndef EXCEPTIONS_INCLUDE
#define EXCEPTIONS_INCLUDE

#include <exception>
#include <iostream>

class LocationException : public std::exception {
private:

	std::string message_;

public:

	explicit LocationException(const std::string& message);
	virtual const char* what() const throw();
};

#endif // EXCEPTIONS_INCLUDE