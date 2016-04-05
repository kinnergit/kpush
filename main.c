#include <stdio.h>
#include <memory.h>
#include <netdb.h>
#include <signal.h>
#include <unistd.h>

#include "provider.h"
#include "config.h"

extern const char * apns_template[APNS_TEMPLATE_COUNT];

apns_t * apns = NULL;
apns_err_t err;
jmp_buf jmp;
int last_err_id = 0;

void sigpipe_handler(int signo)
{
    try_ecp() {
        //读取错误
        apns_recv(apns, &err, ERROR_RESPONSE_SIZE);
        //close apns connection
        APNs_close(apns);
        last_err_id = ntohl(err.identifier);
        throw_ecp(jmp, 1);
    }
}

int main (int argc, const char **argv) {
    //注册SIGPIPE(写端关闭信号)
    signal(SIGPIPE, sigpipe_handler);

    //读取配置文件
    config_t config;
    read_config(&config, "./conf/config.lua");

    char payload[MAX_PAYLOAD_SIZE];
    char bf[256];
    apns_ntfy_t n;

    const char * device_token = config.test_devtoken;
    const char * apns_tmp_sml = apns_template[0];

    int batch_send_count = config.apns_batch_send_count;
    int apns_batch_buffer = config.apns_batch_buffer;
    int sendNum = config.apns_send_total;

    char batch_buffer[apns_batch_buffer];

    catch_ecp(jmp)
    {
        if (ecp_code == 1) {
            printf("command: %d, status: %d, id: %d, err: %s\n",
                   err.command,
                   err.status,
                   ntohl(err.identifier),
                   get_apns_error(err.status));
        }

        printf("host:port %s:%d\npem_file: %s\npass_phrase: %s\ntimeout: %d\n",
               config.apns_push_host,
               config.apns_push_port,
               config.pem_file,
               config.pass_phrase,
               config.apns_connect_timeout
        );

        //连接APNs
        apns = apns_connect(config.apns_push_host, config.apns_push_port, config.pem_file, config.pass_phrase);

        int batch_len = 0;
        int count = 0;

        printf("last_err_id: %d\n", last_err_id);

        time_t expire = 30;

        for (int i = last_err_id; i < sendNum; i++) {
            expire += time(NULL);
            snprintf(bf, MAX_PAYLOAD_SIZE, config.test_msg, i+1);
            NTFY_PAYLOAD(payload, apns_tmp_sml, bf, i+1);
            printf("len: %ld, %s\n", strlen(payload), payload);
            NTFY(n, i+1, expire, device_token, payload);
            memcpy(batch_buffer + batch_len, &n, NTFY_LEN(n));
            batch_len += NTFY_LEN(n);

            count++;

            if (count >= batch_send_count) {
                apns_send(apns, batch_buffer, batch_len);
                batch_len = 0;
                count = 0;
                sleep(1);
            }
        }

        if (batch_len) {
            apns_send(apns, batch_buffer, batch_len);
        }

        sleep(2);

        free_config(&config);
        APNs_close(apns);
    }

    return 0;
}