#include "SequentialHandler.h"

void SequentialHandler::handle(std::shared_ptr<ConnectionSocket> connection_socket) {
    process_connection(connection_socket);
}
