#include "exceptions.hpp"

LocationException::LocationException(const std::string& message) : message_("Unable to locate " + message) {

}

const char* LocationException::what() const throw() {
	return message_.c_str();
}