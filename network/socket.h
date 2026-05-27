#ifndef SOCKET_H
#define SOCKET_H

#include "../kernel/types.h"
#include "tcp_ip.h"

/* Socket structure */
typedef struct {
    int fd;
    int family;          /* AF_INET */
    int type;            /* SOCK_STREAM, SOCK_DGRAM */
    int protocol;        /* IPPROTO_TCP, IPPROTO_UDP */
    uint32_t remote_ip;
    uint16_t remote_port;
    uint16_t local_port;
    int state;           /* Connection state */
} Socket;

/* Socket families */
#define AF_INET 2

/* Socket types */
#define SOCK_STREAM 1
#define SOCK_DGRAM 2

/* IP protocols */
#define IPPROTO_TCP 6
#define IPPROTO_UDP 17

/* Socket states */
#define SOCKET_STATE_CLOSED 0
#define SOCKET_STATE_LISTEN 1
#define SOCKET_STATE_ESTABLISHED 2
#define SOCKET_STATE_TIME_WAIT 3

/* Socket address structure */
typedef struct {
    uint16_t family;
    uint16_t port;
    uint32_t address;
    uint8_t zero[8];
} SocketAddress;

/* Function declarations */
int socket_create(int family, int type, int protocol);
int socket_bind(int fd, SocketAddress* addr);
int socket_listen(int fd, int backlog);
int socket_accept(int fd, SocketAddress* addr);
int socket_connect(int fd, SocketAddress* addr);
int socket_send(int fd, uint8_t* data, uint16_t length);
int socket_recv(int fd, uint8_t* buffer, uint16_t max_length);
int socket_close(int fd);

#endif
