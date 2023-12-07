#include "Socket.h"
#include <unistd.h>

void Socket::close() const {
    ::close(_socket_descriptor);
}
