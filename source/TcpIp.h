/* Copyright (C) 2015 Joakim Plate
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

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
 * @brief Development Errors
 * @req SWS_TCPIP_00042
 * @{
 */

/** @brief API service called before initializing the module */
#define TCPIP_E_NOTINIT        0x01u

/** @brief API service called with NULL pointer */
#define TCPIP_E_PARAM_POINTER  0x02u

/** @brief Invalid argument */
#define TCPIP_E_INV_ARG        0x03u

/** @brief no buffer space available */
#define TCPIP_E_NOBUFS         0x04u

/** @brief Message to long */
#define TCPIP_E_MSGSIZE        0x07u

/** @brief Protocol wrong type for socket */
#define TCPIP_E_PROTOCOL       0x08u

/** @brief Compatibility with AUTOSAR invalid doc */
#define TCPIP_E_PROTOTYPE      TCPIP_E_PROTOCOL

/** @brief Address already in use */
#define TCPIP_E_ADDRINUSE      0x09u

/** @brief Can't assignt requested address */
#define TCPIP_E_ADDRNOTAVAIL   0x0Au

/** @brief Socket is already connected */
#define TCPIP_E_ISCONN         0x0Bu

/** @brief Socket is not connected */
#define TCPIP_E_NOTCONN        0x0Cu

/** @brief Protocol is not available */
#define TCPIP_E_NOPROTOOPT     0x0Du

/** @brief Address family not supported by protocol family */
#define TCPIP_E_AFNOSUPPORT    0x0Eu

/** @brief Invalid configuration set selection */
#define TCPIP_E_INIT_FAILED    0x0Fu

/**
 * @}
 */

/**
 * @brief Service identifier
 * @{
 */
#define TCPIP_API_INIT                         0x01u
#define TCPIP_API_GETVERSIONINFO               0x02u
#define TCPIP_API_BIND                         0x05u
#define TCPIP_API_TCPCONNECT                   0x06u
#define TCPIP_API_TCPLISTEN                    0x07u
#define TCPIP_API_TCPRECEIVED                  0x08u
#define TCPIP_API_REQUESTCOMMODE               0x09u
#define TCPIP_API_REQUESTADDRASSIGNMENT        0x0Au
#define TCPIP_API_RELEASEIPADDRASSIGNMENT      0x0Bu
#define TCPIP_API_RESETIPASSIGNMENT            0x1Bu
#define TCPIP_API_ICMPTRANSMIT                 0x0Cu
#define TCPIP_API_ICMPV6TRANSMIT               0x18u
#define TCPIP_API_DHCPREADOPTION               0x0Du
#define TCPIP_API_DHCPV6READOPTION             0x19u
#define TCPIP_API_DHCPWRITEOPTION              0x0Eu
#define TCPIP_API_DHCPV6WRITEOPTION            0x1Au
#define TCPIP_API_CHANGEPARAMETER              0x0Fu
#define TCPIP_API_GETIPADDR                    0x10u
#define TCPIP_API_GETPHYSADDR                  0x01u
#define TCPIP_API_GETREMOTEPHYSADDR            0x16u
#define TCPIP_API_UDPTRANSMIT                  0x00u
#define TCPIP_API_TCPTRANSMIT                  0x13u
#define TCPIP_API_RXINDICATION                 0x04u
#define TCPIP_API_MAINFUNCTION                 0x15u
#define TCPIP_API_GETSOCKET                    0x03u
/**
 * @}
 */

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
    uint32           addr[4];
} TcpIp_SockAddrInet6Type;

typedef union {
    TcpIp_SockAddrType      base;
    TcpIp_SockAddrInetType  inet;
    TcpIp_SockAddrInet6Type inet6;
} TcpIp_SockAddrStorageType;

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
        TcpIp_DomainType            domain,
        TcpIp_ProtocolType          protocol,
        TcpIp_SocketIdType*         id
    );

void TcpIp_Init(
        const TcpIp_ConfigType*     config
    );


Std_ReturnType TcpIp_Bind(
        TcpIp_SocketIdType          id,
        TcpIp_LocalAddrIdType       local,
        uint16* port
    );

Std_ReturnType TcpIp_TcpListen(
        TcpIp_SocketIdType id,
        uint16 channels
    );

Std_ReturnType TcpIp_TcpConnect(
        TcpIp_SocketIdType          id,
        const TcpIp_SockAddrType*   remote
    );

Std_ReturnType TcpIp_UdpTransmit(
        TcpIp_SocketIdType          id,
        const uint8*                data,
        const TcpIp_SockAddrType*   remote,
        uint16                      len
    );

Std_ReturnType TcpIp_TcpTransmit(
        TcpIp_SocketIdType  id,
        const uint8*        data,
        uint32              aailable,
        boolean             force
    );


#endif /* TCPIP_H_ */
