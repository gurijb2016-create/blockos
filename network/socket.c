#include "../network/socket.h"
#include "../kernel/types.h"

#define MAX_SOCKETS 16

static Socket sockets[MAX_SOCKETS];
static int socket_count = 0;

/* Create socket */
int socket_create(int family, int type, int protocol) {
    if (socket_count >= MAX_SOCKETS) return -1;
    
    Socket* sock = &sockets[socket_count];
    sock->fd = socket_count;
    sock->family = family;
    sock->type = type;
    sock->protocol = protocol;
    sock->state = SOCKET_STATE_CLOSED;
    sock->remote_ip = 0;
    sock->remote_port = 0;
    sock->local_port = 0;
    
    socket_count++;
    return sock->fd;
}

/* Bind socket to address */
int socket_bind(int fd, SocketAddress* addr) {
    if (fd >= socket_count) return -1;
    
    Socket* sock = &sockets[fd];
    sock->local_port = addr->port;
    
    return 0;
}

/* Listen for connections */
int socket_listen(int fd, int backlog) {
    if (fd >= socket_count) return -1;
    
    Socket* sock = &sockets[fd];
    sock->state = SOCKET_STATE_LISTEN;
    
    return 0;
}

/* Accept connection */
int socket_accept(int fd, SocketAddress* addr) {
    if (fd >= socket_count) return -1;
    
    /* TODO: Accept incoming connection */
    
    return -1;
}

/* Connect to remote address */
int socket_connect(int fd, SocketAddress* addr) {
    if (fd >= socket_count) return -1;
    
    Socket* sock = &sockets[fd];
    sock->remote_ip = addr->address;
    sock->remote_port = addr->port;
    sock->state = SOCKET_STATE_ESTABLISHED;
    
    /* TODO: Send TCP SYN or UDP handshake */
    
    return 0;
}

/* Send data */
int socket_send(int fd, uint8_t* data, uint16_t length) {
    if (fd >= socket_count) return -1;
    
    Socket* sock = &sockets[fd];
    
    if (sock->state != SOCKET_STATE_ESTABLISHED) return -1;
    
    /* TODO: Send data via TCP or UDP */
    
    return length;
}

/* Receive data */
int socket_recv(int fd, uint8_t* buffer, uint16_t max_length) {
    if (fd >= socket_count) return -1;
    
    Socket* sock = &sockets[fd];
    
    /* TODO: Receive data from socket buffer */
    
    return 0;
}

/* Close socket */
int socket_close(int fd) {
    if (fd >= socket_count) return -1;
    
    Socket* sock = &sockets[fd];
    sock->state = SOCKET_STATE_CLOSED;
    
    return 0;
}
