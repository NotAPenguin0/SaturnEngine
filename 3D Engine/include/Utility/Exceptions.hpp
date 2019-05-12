#ifndef MVG_EXCEPTIONS_HPP_
#define MVG_EXCEPTIONS_HPP_

#include <stdexcept>

namespace Saturn {

class InvalidViewportException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

}

#endif