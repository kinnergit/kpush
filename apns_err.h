#ifndef KPUSH_ERROR_RESPONSE_H
#define KPUSH_ERROR_RESPONSE_H

#include <stdio.h>
#include <setjmp.h>

#include "apns_type.h"

#define throw_ecp(env, code) longjmp((env), (code))
#define try_ecp(e)
#define catch_ecp(e) int ecp_code = setjmp((e));

static const char * apns_error[] = {
    "No errors encountered",
    "Processing error",
    "Missing device token",
    "Missing topic",
    "Missing payload",
    "Invalid token size",
    "Invalid topic size",
    "Invalid payload size",
    "Invalid token",
    "Shutdown",
    "None (unknown)",
    NULL
};

static inline int get_apns_code(int code)
{
    switch (code)
    {
        case 10 :
            code = 9;
            break;
        case 255 :
            code = 10;
            break;
        default:
            break;
    }

    return code;
}

static inline const char * get_apns_error(int code)
{
    return apns_error[get_apns_code(code)];
}

#endif //KPUSH_ERROR_RESPONSE_H
