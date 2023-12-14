#include <stdexcept>
#include <utility>

class AttemptsExceeded : public std::exception {
public:
    explicit AttemptsExceeded() : std::exception() {}

};

