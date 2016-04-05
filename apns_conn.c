#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#include "apns_conn.h"
#include "kmalloc.h"

#define FAIL    -1

static apns_sock_fd_t apns_sock_connect(const char * host, in_port_t port);
static SSL_CTX * init_ssl_ctx(const char * pem_file, const char * pass_phrase);
static SSL * apns_ssl_connect(SSL_CTX * ctx, apns_sock_fd_t sd);

apns_t * apns_connect(const char * host,
                            in_port_t port,
                            const char * pem_file,
                            const char * pass_phrase)
{
    apns_sock_fd_t sd   = apns_sock_connect(host, port);

    if (sd <= 0) {
        return NULL;
    }

    SSL_CTX * ctx       = init_ssl_ctx(pem_file, pass_phrase);

    if ( ! ctx) {
        return NULL;
    }

    SSL * ssl           = apns_ssl_connect(ctx, sd);

    if ( ! ssl) {
        return NULL;
    }

    apns_t * apns = (apns_t *)kmalloc(sizeof(apns_t));

    if (apns) {
        apns->sd    = sd;
        apns->ctx   = ctx;
        apns->ssl   = ssl;
    }

    return apns;
}

int apns_send(apns_t * apns, void * send_buff, int send_len)
{
    int ret = SSL_write(apns->ssl, (const void *)send_buff, send_len);

    return ret;
}

int apns_recv(apns_t * apns, void * recv_buff, int recv_len)
{
    int ret = SSL_read(apns->ssl, recv_buff, recv_len);

    return ret;
}

int apns_close(apns_t * apns)
{
    if (apns) {
        close(apns->sd);
        SSL_free(apns->ssl);
        SSL_CTX_free(apns->ctx);

        kfree(apns);
    }

    return 0;
}

static apns_sock_fd_t apns_sock_connect(const char * host, in_port_t port)
{
    apns_sock_fd_t sd;
    struct hostent * hent;
    struct sockaddr_in saddr;
    in_addr_t ip;

    if (is_ipv4_str(host)) {
        ip = inet_addr(host);
    } else{
        hent = gethostbyname(host);

        if ( ! hent) {
            return FAIL;
        }

        ip = inet_addr(inet_ntoa(*(struct in_addr*)hent->h_addr_list[0]));
    }

    sd = socket(PF_INET, SOCK_STREAM, 0);

    memset(&saddr, 0, sizeof(saddr));

    saddr.sin_family        = AF_INET;
    saddr.sin_port          = htons(port);
    saddr.sin_addr.s_addr   = ip;

    int conn = connect(sd, (struct sockaddr *) &saddr, sizeof(saddr));

    if (conn) {
        close(sd);

        return FAIL;
    }

    return sd;
}

static SSL_CTX * init_ssl_ctx(const char * pem_file, const char * pass_phrase)
{
    SSL_METHOD * method;
    SSL_CTX * ctx;

    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();
    ERR_load_BIO_strings();

    method  = (SSL_METHOD *)SSLv23_client_method();
    ctx     = SSL_CTX_new(method);

    if (ctx == NULL) {
        return NULL;
    }

    SSL_CTX_set_verify_depth(ctx, 1);
    SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, NULL);
    SSL_CTX_set_default_passwd_cb_userdata(ctx, (void *)pass_phrase);
    SSL_CTX_use_certificate_chain_file(ctx, pem_file);
    SSL_CTX_use_PrivateKey_file(ctx, pem_file, SSL_FILETYPE_PEM);

    return ctx;
}

static SSL * apns_ssl_connect(SSL_CTX * ctx, apns_sock_fd_t sd)
{
    apns_sock_fd_t ret;
    SSL * ssl;

    ssl = SSL_new(ctx);

    SSL_set_verify(ssl, SSL_VERIFY_NONE, NULL);
    SSL_set_fd(ssl, sd);

    ret = SSL_connect(ssl);

    if (ret == FAIL) {
        return NULL;
    }

    return ssl;
}

void print_cert(SSL *ssl)
{
    X509 *cert;
    char *line;
    cert = SSL_get_peer_certificate(ssl);/* get the server's certificate */

    if (cert != NULL) {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        kfree(line);
        /* free the malloc'ed string */
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        kfree(line);
        /* free the malloc'ed string */
        X509_free(cert);    /* free the malloc'ed certificate copy */
    } else {
        printf("No certificates.\n");
    }
}
