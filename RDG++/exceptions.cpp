#include "exceptions.hpp"

LocationException::LocationException(const std::string& message) : message_("Unable to locate " + message) {

}

const char* LocationException::what() const throw() {
	return message_.c_str();
}

LoadingException::LoadingException(const std::string& message) : message_("Unable to load " + message) {

}

const char* LoadingException::what() const throw() {
	return message_.c_str();
}

ConfigParseException::ConfigParseException(const std::string& message) : message_("Unable to parse " + message) {

}

const char* ConfigParseException::what() const throw() {
	return message_.c_str();
}

EnumMappingException::EnumMappingException(const std::string& input, const std::string& enumName) : message_("Unable to map " + input + " to Enum " + enumName) {

}

const char* EnumMappingException::what() const throw() {
	return message_.c_str();
}