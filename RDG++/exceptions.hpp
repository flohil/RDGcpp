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

class LoadingException : public std::exception {
private:

	std::string message_;

public:

	explicit LoadingException(const std::string& message);
	virtual const char* what() const throw();
};

class ConfigParseException : public std::exception {
private:

	std::string message_;

public:

	explicit ConfigParseException(const std::string& message);
	virtual const char* what() const throw();
};

class EnumMappingException : public std::exception {
private:

	std::string message_;

public:

	explicit EnumMappingException(const std::string& input, const std::string& enumName);
	virtual const char* what() const throw();
};

#endif // EXCEPTIONS_INCLUDE