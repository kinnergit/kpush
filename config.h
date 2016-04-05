#ifndef KPUSH_CONFIG_H
#define KPUSH_CONFIG_H

#include <netdb.h>

#define LUA_STACK_TOP -1
#define LUA_FUNCTION_RETURN_SUCCESS 0

typedef struct {
    const char * pem_file;
    const char * pass_phrase;
    const char * apns_push_host;
    const char * test_devtoken;
    const char * test_msg;
    int apns_connect_timeout;
    int apns_batch_send_count;
    int apns_batch_buffer;
    int apns_send_total;
    in_port_t apns_push_port;
} config_t;

void read_config(config_t * config, const char * config_file);
void free_config(config_t * config);

#endif //KPUSH_CONFIG_H
