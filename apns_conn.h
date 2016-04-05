#ifndef KPUSH_CONNECTION_INTERNAL_H
#define KPUSH_CONNECTION_INTERNAL_H

#include <netdb.h>

#include "apns_struct.h"

#define APNs_close(apns) do{apns_close(apns);apns = NULL;}while(0);

apns_t * apns_connect(const char * host,
                            in_port_t port,
                            const char * pem_file,
                            const char * pass_phrase);

int apns_send(apns_t * apns, void * send_buff, int send_len);
int apns_recv(apns_t * apns, void * recv_buff, int recv_len);
int apns_close(apns_t * apns);

#endif //KPUSH_CONNECTION_INTERNAL_H
