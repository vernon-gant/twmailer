#include "SequentialHandler.h"

void SequentialHandler::handle(std::shared_ptr<ConnectionSocket> connection_socket) {
    process_request(connection_socket);
}
