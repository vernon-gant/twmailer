#pragma once

#include <memory>
#include <ldap.h>
#include "CredentialsReader.h"

class LDAPConnector {

private:
    LDAP *_handle;

    static const std::string LDAP_SERVER_URI;

    static std::string get_bind_user_name(const std::string& user_name);
public:
    static std::unique_ptr<LDAPConnector> initialize_connection();

    bool valid_credentials(Credentials& credentials);

    explicit LDAPConnector(LDAP * handle);

    virtual ~LDAPConnector();


};
