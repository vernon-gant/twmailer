#pragma once

#include <memory>
#include <ldap.h>
#include "CredentialsReader.h"

class LDAPConnector {

private:
    LDAP *_handle;

    static const std::string LDAP_SERVER_URI;
public:
    static std::unique_ptr<LDAPConnector> initialize();

    bool valid_credentials(Credentials& credentials);

    explicit LDAPConnector(LDAP * handle);

    virtual ~LDAPConnector();


};
