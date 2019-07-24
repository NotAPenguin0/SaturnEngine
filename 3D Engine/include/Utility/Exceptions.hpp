#ifndef SATURN_EXCEPTIONS_HPP_
#define SATURN_EXCEPTIONS_HPP_

#include <stdexcept>

namespace Saturn {

class InvalidViewportException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

}

#endif