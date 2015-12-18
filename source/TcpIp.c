#include "TcpIp.h"
#include "TcpIp_Cfg.h"
#include "SoAd_Cbk.h"

#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>

typedef int TcpIp_OsSocketType;

#define INVALID_SOCKET (TcpIp_OsSocketType)-1
#define closesocket(x) close(x)

const TcpIp_ConfigType* TcpIp_Config;


typedef enum {
    TCPIP_SOCKET_STATE_UNUSED,
    TCPIP_SOCKET_STATE_ALLOCATED,
    TCPIP_SOCKET_STATE_CONNECTING,
    TCPIP_SOCKET_STATE_CONNECTED,
    TCPIP_SOCKET_STATE_TCPCLOSE,
} TcpIp_SocketStateType;

typedef struct {
    TcpIp_ProtocolType    protocol;
    TcpIp_DomainType      domain;
    TcpIp_SocketStateType state;
    TcpIp_OsSocketType    fd;
} TcpIp_SocketType;

TcpIp_SocketType      TcpIp_Sockets[TCPIP_MAX_SOCKETS];
struct pollfd         TcpIp_PollFds[TCPIP_MAX_SOCKETS];

static void TcpIp_InitSocket(TcpIp_SocketType* s)
{
    memset(s, 0, sizeof(*s));
    s->state = TCPIP_SOCKET_STATE_UNUSED;
    s->fd = INVALID_SOCKET;
}

static void TcpIp_FreeSocket(TcpIp_SocketType* s)
{
    if (s->fd != INVALID_SOCKET) {
        closesocket(s->fd);
        s->fd = INVALID_SOCKET;
    }
    s->state = TCPIP_SOCKET_STATE_UNUSED;
}

/**
 * @brief This service initializes the TCP/IP Stack.
 *
 * TcpIp_Init may not block the start-up process for an indefinite amount of time.
 * @note The call of this service is mandatory before using the TcpIp instance for further processing.
 * @param[in] config Pointer to the configuration data of the TcpIp module
 */
void TcpIp_Init(const TcpIp_ConfigType* config)
{
    TcpIp_SocketIdType socket;
    TcpIp_Config = config;

    for (socket = 0u; socket < TCPIP_MAX_SOCKETS; ++socket) {
        TcpIp_InitSocket(&TcpIp_Sockets[socket]);
    }
}

/**
 * @brief By this API service the TCP/IP stack is requested to close the socket and release all related resources.
 * @param[in] Abort TRUE:  connection will immediately be terminated by sending a
 *                         RST-Segment and releasing all related resources.
 *                  FALSE: connection will be terminated after performing a regular
 *                         connection termination handshake and releasing all related
 *                         resources.
 * @return    E_OK:     The request has been accepted
 *            E_NOT_OK: The request has not been accepted.
 */
Std_ReturnType TcpIp_Close(
        TcpIp_SocketIdType          id,
        boolean                     abort
    )
{
    TcpIp_SocketType* s = &TcpIp_Sockets[id];
    Std_ReturnType   res;

    if (s->protocol == TCPIP_IPPROTO_TCP) {
        if (abort) {
            TcpIp_FreeSocket(s);
            SoAd_TcpIpEvent(id, TCPIP_TCP_RESET);
        } else {
            shutdown(s->fd, SHUT_RDWR);
            s->state = TCPIP_SOCKET_STATE_TCPCLOSE;
        }
        res = E_OK;
    } else if (s->protocol == TCPIP_IPPROTO_UDP) {
        TcpIp_FreeSocket(s);
        SoAd_TcpIpEvent(id, TCPIP_UDP_CLOSED);
        res = E_OK;
    } else {
        res = E_NOT_OK;
    }

    return res;
}

Std_ReturnType TcpIp_Bind(
        TcpIp_SocketIdType          id,
        TcpIp_LocalAddrIdType       local_addr,
        uint16*                     port
    )
{
    TcpIp_SocketType* s = &TcpIp_Sockets[id];
    Std_ReturnType    res;

    if (local_addr != TCPIP_LOCALADDRID_ANY) {
        /* TODO */
        return E_NOT_OK;
    }

    if (s->domain == TCPIP_AF_INET) {
        int v;
        struct sockaddr_in addr = {};
        addr.sin_family = AF_INET;
        addr.sin_port   = *port;
        addr.sin_len    = sizeof(addr);
        if (bind(s->fd, (const struct sockaddr*)&addr, sizeof(addr)) == 0) {
            *port = addr.sin_port;
            res = E_OK;
        } else {
            res = E_NOT_OK;
        }
    } else if (s->domain == TCPIP_AF_INET6) {
        /* TODO */
        res = E_NOT_OK;
    } else {
        res = E_NOT_OK;
    }
    return res;
}

Std_ReturnType TcpIp_TcpConnect(
        TcpIp_SocketIdType          SocketId,
        const TcpIp_SockAddrType*   RemoteAddrPtr
    )
{
    return E_NOT_OK;
}

Std_ReturnType TcpIp_TcpTransmit(
        TcpIp_SocketIdType  SocketId,
        const uint8*        DataPtr,
        uint32              AvailableLength,
        boolean             ForceRetrieve
    )
{
    return E_NOT_OK;
}

static sint8 TcpIp_GetBsdTypeFromProtocol(TcpIp_ProtocolType  protocol)
{
    sint8 res;
    switch(protocol) {
        case TCPIP_IPPROTO_TCP:
            res = SOCK_STREAM;
            break;
        case TCPIP_IPPROTO_UDP:
            res = SOCK_DGRAM;
            break;
        default:
            res = 0;
    }
    return res;
}

static sint8 TcpIp_GetBsdDomainFromDomain(TcpIp_DomainType domain)
{
    return (sint8)domain;
}

static Std_ReturnType TcpIp_GetFreeSocket(TcpIp_SocketIdType* socketid)
{
    TcpIp_SocketIdType index;
    Std_ReturnType     res;

    for (index = 0u; index < TCPIP_MAX_SOCKETS; ++index) {
        if (TcpIp_Sockets[index].state == TCPIP_SOCKET_STATE_UNUSED) {
            break;
        }
    }
    if (index < TCPIP_MAX_SOCKETS) {
        *socketid = index;
        res = E_OK;
    } else {
        res = E_NOT_OK;
    }
    return res;
}

static Std_ReturnType Tcp_SetNonBlocking(TcpIp_OsSocketType fd)
{
    Std_ReturnType     res;
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) {
        res = E_NOT_OK;
    } else {
        flags = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
        if (flags < 0) {
            res = E_NOT_OK;
        } else {
            res = E_OK;
        }
    }
    return res;
}

Std_ReturnType TcpIp_SoAdGetSocket(
        TcpIp_DomainType    domain,
        TcpIp_ProtocolType  protocol,
        TcpIp_SocketIdType* socketid
    )
{
    TcpIp_SocketIdType index;
    Std_ReturnType     res;

    res = TcpIp_GetFreeSocket(socketid);
    if (res == E_OK) {
        TcpIp_SocketType*  s = &TcpIp_Sockets[index];
        TcpIp_OsSocketType fd;
        fd = socket( TcpIp_GetBsdDomainFromDomain(domain)
                   , TcpIp_GetBsdTypeFromProtocol(protocol)
                   , 0);

        if (fd != INVALID_SOCKET) {
            s->fd       = fd;
            s->state    = TCPIP_SOCKET_STATE_ALLOCATED;
            s->protocol = protocol;
            s->domain   = domain;
            res = Tcp_SetNonBlocking(s->fd);
        } else {
            res = E_NOT_OK;
        }
    }

    return res;
}

void TcpIp_SocketState_Connecting(TcpIp_SocketIdType index)
{
    TcpIp_SocketType* s = &TcpIp_Sockets[index];
    struct pollfd*    p = &TcpIp_PollFds[index];
    int res;

    if (p->revents & POLLOUT) {
        struct sockaddr_storage addr;
        socklen_t len = sizeof(addr);

        /* check if connect succeeded */
        res = getpeername(s->fd, (struct sockaddr*)&addr, &len);
        if (res == 0) {
            s->state = TCPIP_SOCKET_STATE_CONNECTED;
            SoAd_TcpConnected(index);
        } else {
            s->state = TCPIP_SOCKET_STATE_ALLOCATED; /* error */
        }
    }
}

void TcpIp_SocketState_TcpClose(TcpIp_SocketIdType index)
{
    TcpIp_SocketType* s = &TcpIp_Sockets[index];
    struct pollfd*    p = &TcpIp_PollFds[index];

    /* TODO - how to handle proper FIN */
    TcpIp_FreeSocket(s);
    SoAd_TcpIpEvent(index, TCPIP_TCP_CLOSED);
}

void TcpIp_SocketState_All(TcpIp_SocketIdType index)
{
    TcpIp_SocketType* s = &TcpIp_Sockets[index];
    struct pollfd*    p = &TcpIp_PollFds[index];
    int res;

    /* handle current state */
    switch (s->state) {
        case TCPIP_SOCKET_STATE_CONNECTING:
            TcpIp_SocketState_Connecting(index);
            break;
        case TCPIP_SOCKET_STATE_TCPCLOSE:
            TcpIp_SocketState_TcpClose(index);
            break;
        default:
            break;
    }

    /* what events are we listening on */
    switch (s->state) {
        case TCPIP_SOCKET_STATE_CONNECTING:
            p->events = POLLOUT;
            break;
        case TCPIP_SOCKET_STATE_TCPCLOSE:
            p->events = POLLIN;
            break;
        default:
            p->events = 0;
            break;

    }
}

void TcpIp_MainFunction(void)
{
    TcpIp_SocketIdType index;
    int                timeout = 1;
    int                res;

    for (index = 0u; index < TCPIP_MAX_SOCKETS; ++index) {
        TcpIp_PollFds[index].fd      = TcpIp_Sockets[index].fd;
        TcpIp_PollFds[index].revents = 0;
    }

    res = poll(TcpIp_PollFds, TCPIP_MAX_SOCKETS, timeout);
    if (res > 0) {
        /* something to do */
    } else if (res < 0) {
        /* error occured */
    } else {
        /* nothing to do */
    }
    for (index = 0u; index < TCPIP_MAX_SOCKETS; ++index) {
        TcpIp_SocketState_All(index);
    }
}

