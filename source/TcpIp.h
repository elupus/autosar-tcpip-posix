
#ifndef TCPIP_H_
#define TCPIP_H_

#include "Std_Types.h"

/**
 * @brief Protocol type used by a socket.
 * @req   SWS_TCPIP_00010
 */
typedef enum {
    TCPIP_IPPROTO_TCP = 0x06,
    TCPIP_IPPROTO_UDP = 0x11,
} TcpIp_ProtocolType;

/**
 * @brief TcpIp address families.
 * @req   SWS_TCPIP_00009
 */
typedef enum {
    TCPIP_AF_INET     = 0x02,
    TCPIP_AF_INET6    = 0x1c,
} TcpIp_DomainType;

/**
 * @brief Specifies the TcpIp state for a specific EthIf controller.
 * @req   SWS_TCPIP_00073
 */
typedef enum {
    /**
     * TCP/IP stack state for a specific EthIf controller is
     * ONLINE, i.e. communication via at least one IP address is
     * possible.
     */
    TCPIP_STATE_ONLINE,

    /**
     * TCP/IP stack state for a specific EthIf controller is
     * ONHOLD, i.e. no communication is currently possible
     * (e.g. link down).
     */
    TCPIP_STATE_ONHOLD,

    /**
     * TCP/IP stack state for a specific EthIf controller is
     * OFFLINE, i.e. no communication is possible.
     */
    TCPIP_STATE_OFFLINE,

    /**
     * TCP/IP stack state for a specific EthIf controller is
     * STARTUP, i.e. IP address assignment in progress or
     * ready for manual start, communication is currently not
     * possible.
     */
    TCPIP_STATE_STARTUP,

    /* TCP/IP stack state for a specific EthIf controller is
     * SHUTDOWN, i.e. release of resources using the EthIf
     * controller, release of IP address assignment.
     */
    TCPIP_STATE_SHUTDOWN,
} TcpIp_StateType;

/**
 * @brief Specifies the state of local IP address assignment
 * @req   SWS_TCPIP_00082
 */
typedef enum {
    /**
     * local IP address is assigned
     */
    TCPIP_IPADDR_STATE_ASSIGNED,

    /**
     * local IP address is assigned, but cannot be used as the network is not active
     */
    TCPIP_IPADDR_STATE_ONHOLD,

    /**
     * local IP address is unassigned
     */
    TCPIP_IPADDR_STATE_UNASSIGNED,
} TcpIp_IpAddrStateType;

/**
 * @brief Events reported by TcpIp.
 * @req   SWS_TCPIP_00031
 */
typedef enum {
    /**
     * TCP connection was reset, TCP socket and all related resources have been released.
     */
    TCPIP_TCP_RESET,

    /**
     * TCP connection was closed successfully, TCP socket and all related resources have been released.
     */
    TCPIP_TCP_CLOSED,

    /**
     * A FIN signal was received on the TCP connection, TCP socket is still valid.
     */
    TCPIP_TCP_FIN_RECEIVED,

    /**
     * UDP socket and all related resources have been released.
     */
    TCPIP_UDP_CLOSED,
} TcpIp_EventType;

typedef enum {
    /**
     * operation completed successfully.
     */
    TCPIP_OK,

    /**
     * operation failed.
     */
    TCPIP_E_NOT_OK,

    /**
     * operation failed because of an ARP/NDP cache miss.
     */
    TCPIP_E_PHYS_ADDR_MISS,
} TcpIp_ReturnType;

/**
 * @brief Configuration data structure of the TcpIp module.
 * @req   SWS_TCPIP_00067
 */
typedef struct {
    uint8 dummy;
} TcpIp_ConfigType;

/**
 * @brief Generic structure used by APIs to specify an IP address. (A specific address
 *        type can be derived from this structure via a cast to the specific struct type.)
 * @req   SWS_TCPIP_00012
 */
typedef struct {
    TcpIp_DomainType domain;
} TcpIp_SockAddrType;

/**
 * @brief This structure defines an IPv4 address type which can be derived from the generic
 *        address structure via cast.
 * @req   SWS_TCPIP_00013
 */
typedef struct {
    union {
        TcpIp_SockAddrType base;
        TcpIp_DomainType   domain;
    };

    uint16           port;
    uint32           addr[1];
} TcpIp_SockAddrInetType;

/**
 * @brief socket identifier type for unique identification of a TcpIp stack socket.
 *        TCPIP_SOCKETID_INVALID shall specify an invalid socket handle.
 * @req   SWS_TCPIP_00038
 */
typedef uint16 TcpIp_SocketIdType;

/**
 * @brief Address identification type for unique identification of a local IP address and
 *        EthIf Controller configured in the TcpIp module.
 * @req   SWS_TCPIP_00030
 */
typedef uint8  TcpIp_LocalAddrIdType;

#define TCPIP_IPADDR_ANY       0xffffffffu
#define TCPIP_PORT_ANY         0x0u
#define TCPIP_SOCKETID_INVALID (TcpIp_SocketIdType)0xffffu
#define TCPIP_LOCALADDRID_ANY  (TcpIp_LocalAddrIdType)0xffu
/**
 * @brief By this API service the TCP/IP stack is requested to allocate a new socket.
 *        Note: Each accepted incoming TCP connection also allocates a socket resource.
 */
Std_ReturnType TcpIp_SoAdGetSocket(
        TcpIp_DomainType    Domain,
        TcpIp_ProtocolType  Protocol,
        TcpIp_SocketIdType* SocketIdPtr
    );

void TcpIp_Init(const TcpIp_ConfigType* config);


Std_ReturnType TcpIp_Bind(TcpIp_SocketIdType       id, TcpIp_LocalAddrIdType local, uint16* port);
Std_ReturnType TcpIp_TcpListen (TcpIp_SocketIdType id, uint16 channels);
Std_ReturnType TcpIp_TcpConnect(TcpIp_SocketIdType id, const TcpIp_SockAddrType* remote);


#endif /* TCPIP_H_ */
