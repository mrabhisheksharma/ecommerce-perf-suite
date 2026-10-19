#include "web_api.h"

vuser_init()
{
    lr_output_message("Initializing VUser and performing pre-flight handshake...");

    web_set_sockets_option("SSL_VERSION", "TLS1.3");
    web_add_auto_header("Accept", "application/json");

    /* Register dynamic token correlation */
    web_reg_save_param_ex(
        "ParamName=c_authToken",
        "LB=\"token\":\"",
        "RB=\"",
        SEARCH_FILTERS,
        "Scope=Body",
        LAST);

    web_custom_request("00_Auth_Handshake",
        "URL=https://staging-perf.store.internal/api/v1/auth/session",
        "Method=POST",
        "Resource=0",
        "RecContentType=application/json",
        "Snapshot=t1.inf",
        "Mode=HTTP",
        "Body={\"clientType\":\"VUGEN_RUNNER\"}",
        LAST);

    return 0;
}
