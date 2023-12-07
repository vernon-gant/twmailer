#include "RequestHandler.h"
#include "exceptions/ValidationError.h"
#include "exceptions/InternalServerError.h"

std::string RequestHandler::handle(std::string &message, const UserContext &user_context) {
    try {
        std::unique_ptr<Command> command = _command_parser->create(message, user_context);
        _command_pipeline->process(*command);
        command->execute();
        return command->get_response();
    } catch (const ValidationError &validation_error) {
        return validation_error.what();
    } catch (const InternalServerError &internal_error) {
        return "ERR\n";
    }
}
