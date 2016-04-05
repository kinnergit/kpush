#ifndef KPUSH_APNS_STRUCT_H
#define KPUSH_APNS_STRUCT_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>
#include <netdb.h>

#include "apns_type.h"

#define NTFY_ENTITY(ntfy, c, id, e, tlen, tok, pldlen, pld) \
do {                                                        \
    (ntfy).command        = (c);                            \
    (ntfy).identifier     = htonl((id));                    \
    (ntfy).expiry         = htonl((e));                     \
    (ntfy).token_len      = htons((tlen));                  \
    (ntfy).payload_len    = htons((pldlen));                \
    memcpy((ntfy).device_token, (tok), (tlen));             \
    memcpy((ntfy).payload, (pld), (pldlen));                \
} while(0)

#define PUSH_NTFY_ENTITY(ntfy, id, e, tlen, tok, pldlen, pld) \
NTFY_ENTITY(ntfy, APNS_COMMAND_PUSH, (id), (e), (tlen), (tok), (pldlen), (pld))

#define NTFY(ntfy, id, e, tok, pld)                                                 \
do {                                                                                \
    uint8_t tokbin[DEVICE_BINARY_SIZE];                                             \
    devtok2bin(tokbin, (tok));                                                      \
    size_t pldlen = strlen((pld));                                                  \
    PUSH_NTFY_ENTITY((ntfy), (id), (e), DEVICE_BINARY_SIZE, tokbin, pldlen, (pld)); \
} while(0)

#define NTFY_LEN(ntfy) ((int)(sizeof(n) - (MAX_PAYLOAD_SIZE - ntohs((ntfy).payload_len))))

#define NTFY_PAYLOAD(payload, apns_tmp, alt, badge) \
snprintf((payload), sizeof((payload)), (apns_tmp), (alt), (badge))

static inline void devtok2bin(uint8_t binary_token[DEVICE_BINARY_SIZE], const char * restrict device_token)
{
    uint32_t val;

    while (*device_token)
    {
        sscanf(device_token, "%2x", &val);

        *(binary_token++) = (uint8_t)val;
        device_token += 2;
    }
}

static inline int is_ipv4_str(const char * restrict ipv4_str)
{
    int ret = 0;

    ipv4_str_t ipv4;

    sscanf(ipv4_str, "%d%c%d%c%d%c%d", (int*)&ipv4.c1, &ipv4.d1,
           (int*)&ipv4.c2, &ipv4.d2, (int*)&ipv4.c3, &ipv4.d3, (int*)&ipv4.c4);

    ret = (ipv4.d1 == '.' && ipv4.d2 == '.' && ipv4.d3 == '.')
            && (0 <= ipv4.c1 && ipv4.c1 <= 255)
            && (0 <= ipv4.c2 && ipv4.c2 <= 255)
            && (0 <= ipv4.c3 && ipv4.c3 <= 255)
            && (0 <= ipv4.c4 && ipv4.c4 <= 255);

    return ret;
}

#endif //KPUSH_APNS_STRUCT_H
