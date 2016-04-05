#include "apns_type.h"

const char * apns_template[APNS_TEMPLATE_COUNT] = {
        "{\"aps\":{\"alert\":\"%s\",\"sound\":\"default\",\"badge\":%d}}",
};
