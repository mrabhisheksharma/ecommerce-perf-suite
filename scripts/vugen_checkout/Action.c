#include "web_api.h"

Action()
{
    /* -------------------------------------------------------------------------
       Transaction 01: Launch Homepage
       ------------------------------------------------------------------------- */
    lr_start_transaction("01_Launch_Homepage");

    web_custom_request("01_Launch",
        "URL=https://staging-perf.store.internal/api/v1/health",
        "Method=GET",
        "Resource=0",
        "RecContentType=application/json",
        "Mode=HTTP",
        LAST);

    lr_end_transaction("01_Launch_Homepage", LR_AUTO);

    /* Pacing Think Time: 2.0 seconds */
    lr_think_time(2.0);

    /* -------------------------------------------------------------------------
       Transaction 02: Search Catalog
       ------------------------------------------------------------------------- */
    lr_start_transaction("02_Search_Catalog");

    web_custom_request("02_Search",
        "URL=https://staging-perf.store.internal/api/v1/products/search?category=electronics&limit=20",
        "Method=GET",
        "Resource=0",
        "RecContentType=application/json",
        "Mode=HTTP",
        LAST);

    lr_end_transaction("02_Search_Catalog", LR_AUTO);

    lr_think_time(0.5); /* Aggressive think time for stress burst testing */

    /* -------------------------------------------------------------------------
       Transaction 03: Add To Cart
       ------------------------------------------------------------------------- */
    web_reg_save_param_json(
        "ParamName=c_cartToken",
        "QueryString=$.cartToken",
        SEARCH_FILTERS,
        "Scope=Body",
        LAST);

    lr_start_transaction("03_Add_To_Cart");

    web_custom_request("03_AddToCart",
        "URL=https://staging-perf.store.internal/api/v1/cart/items",
        "Method=POST",
        "Resource=0",
        "RecContentType=application/json",
        "Mode=HTTP",
        "Body={\"userId\": \"{p_userId}\", \"sku\": \"SKU-4921\", \"qty\": 1}",
        LAST);

    lr_end_transaction("03_Add_To_Cart", LR_AUTO);

    lr_think_time(2.0);

    /* -------------------------------------------------------------------------
       Transaction 04: Checkout Payment (SLA Target: < 2.0s)
       ------------------------------------------------------------------------- */
    lr_start_transaction("04_Checkout_Payment");

    web_custom_request("04_Payment",
        "URL=https://staging-perf.store.internal/api/v1/checkout/pay",
        "Method=POST",
        "Resource=0",
        "RecContentType=application/json",
        "Mode=HTTP",
        "Body={\"cartToken\": \"{c_cartToken}\", \"paymentMethod\": \"TOKENIZED_CARD\", \"amount\": 129.99}",
        LAST);

    lr_end_transaction("04_Checkout_Payment", LR_AUTO);

    lr_think_time(3.0);

    return 0;
}
