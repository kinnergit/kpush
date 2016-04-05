#ifndef KPUSH_APNS_TYPE_H
#define KPUSH_APNS_TYPE_H

#include <openssl/ssl.h>
#include <openssl/err.h>

#define APNS_COMMAND_PUSH   1
#define APNS_TEMPLATE_COUNT 1

#define DEVICE_BINARY_SIZE  32

#define MAX_PAYLOAD_SIZE    256
#define MAX_ALERT_SIZE      128

#define ERROR_RESPONSE_SIZE     6
#define ERROR_RESPONSE_COMMAND  8

typedef int apns_sock_fd_t;

typedef struct {
    apns_sock_fd_t sd;
    SSL_CTX * ctx;
    SSL * ssl;
} apns_t;

/**
 * APNs Notification Format: |COMMAND|ID|EXPIRY|TOKENLEN|TOKEN|PAYLOADLEN|PAYLOAD|
 */
typedef struct {
    uint8_t     command;
    uint32_t    identifier;
    uint32_t    expiry;
    uint16_t    token_len;
    uint8_t     device_token[DEVICE_BINARY_SIZE];
    uint16_t    payload_len;
    uint8_t     payload[MAX_PAYLOAD_SIZE];
} __attribute__ ((packed)) apns_ntfy_t;

/**
 * APNs Error Response Format: |COMMAND|STATUS|ID|
 */
typedef struct {
    uint8_t     command;
    uint8_t     status;
    uint32_t    identifier;
} __attribute__ ((packed)) apns_err_t;

typedef struct {
    uint8_t c1;
    uint8_t d1;
    uint8_t c2;
    uint8_t d2;
    uint8_t c3;
    uint8_t d3;
    uint8_t c4;
} ipv4_str_t;

#endif //KPUSH_APNS_TYPE_H
