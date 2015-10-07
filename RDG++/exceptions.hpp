#ifndef EXCEPTIONS_INCLUDE
#define EXCEPTIONS_INCLUDE

#include <exception>
#include <iostream>

class CustomException : public std::exception 
{
public:

	explicit CustomException(const std::string& message_) : message(message_) {};
	virtual const char* what() const throw() { return message.c_str(); };

private:

	std::string message;
};

class LocationException : public CustomException
{
public:

	explicit LocationException(const std::string& message) : CustomException("Unable to locate " + message) {};
};

class LoadingException : public CustomException
{
public:

	explicit LoadingException(const std::string& message) : CustomException("Unable to load " + message) {};
};

class ConfigParseException : public CustomException 
{
public:

	explicit ConfigParseException(const std::string& message) : CustomException("Unable to parse " + message) {};
};

class EnumMappingException : public CustomException
{
public:

	explicit EnumMappingException(const std::string& input, const std::string& enumName) : CustomException("Unable to map " + input + " to Enum " + enumName) {};
};

#endif // EXCEPTIONS_INCLUDE