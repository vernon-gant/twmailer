#include <string>
#include <iostream>
#include "LDAPConnector.h"
#include "errors/ConfigurationError.h"
#include "errors/InternalServerError.h"

const std::string LDAPConnector::LDAP_SERVER_URI = "ldap://ldap.technikum-wien.at:389";

LDAPConnector::LDAPConnector(LDAP *handle) : _handle(handle) {}

LDAPConnector::~LDAPConnector() {
    ldap_unbind_ext_s(_handle, nullptr, nullptr);
}

std::unique_ptr<LDAPConnector> LDAPConnector::initialize() {
    LDAP *ldap_handle;

    int return_code;

    return_code = ldap_initialize(&ldap_handle, LDAP_SERVER_URI.c_str());
    if (return_code != LDAP_SUCCESS) {
        std::cout << ldap_err2string(return_code) << std::endl;
        throw ConfigurationError("Configuration Error : failed to initialize LDAP connection");
    }

    int ldap_version = LDAP_VERSION3;

    return_code = ldap_set_option(ldap_handle, LDAP_OPT_PROTOCOL_VERSION, &ldap_version);
    if (return_code != LDAP_OPT_SUCCESS) {
        std::cout << ldap_err2string(return_code) << std::endl;
        ldap_unbind_ext_s(ldap_handle, nullptr, nullptr);
        throw ConfigurationError("Configuration Error : failed to configure LDAP options");
    }

    /*return_code = ldap_start_tls_s(ldap_handle, nullptr, nullptr);
    if (return_code != LDAP_SUCCESS) {
        std::cout << ldap_err2string(return_code) << std::endl;
        ldap_unbind_ext_s(ldap_handle, nullptr, nullptr);
        throw ConfigurationError("Configuration Error : failed to initialize a TLS session");
    }*/

    return std::make_unique<LDAPConnector>(ldap_handle);
}

bool LDAPConnector::valid_credentials(Credentials &credentials) {
    BerValue bind_credentials;
    bind_credentials.bv_val = credentials.password.data();
    bind_credentials.bv_len = credentials.password.length();
    BerValue *server_credentials;

    int return_code = ldap_sasl_bind_s(
            _handle,
            credentials.user_name.c_str(),
            LDAP_SASL_SIMPLE,
            &bind_credentials,
            nullptr,
            nullptr,
            &server_credentials);

    if (server_credentials) ber_bvfree(server_credentials);

    if (return_code == LDAP_INVALID_CREDENTIALS) return false;
    if (return_code != LDAP_SUCCESS) throw InternalServerError("Internal Server Error : failed to bind with LDAP server");

    return true;
}
