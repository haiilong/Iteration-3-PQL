#include "BadTokenException.h"

BadTokenException::BadTokenException(const std::string & message) {
    this->message = message;
}
