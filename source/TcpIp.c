#include "TcpIp.h"
#include "TcpIp_Cfg.h"
#include <string.h>


const TcpIp_ConfigType* TcpIp_Config;

typedef struct {
    TcpIp_DomainType domain;
} TcpIp_SocketStateType;

TcpIp_SocketStateType TcpIp_Sockets[TCPIP_MAX_SOCKETS];

void TcpIp_InitSocketState(TcpIp_SocketStateType* state)
{
    memset(state, 0, sizeof(*state));
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
        TcpIp_InitSocketState(&TcpIp_Sockets[socket]);
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
        TcpIp_SocketIdType          SocketId,
        boolean                     Abort
    )
{
    return E_NOT_OK;
}

Std_ReturnType TcpIp_Bind(
        TcpIp_SocketIdType          SocketId,
        TcpIp_LocalAddrIdType       LocalAddrId,
        uint16* PortPtr
    )
{
    return E_NOT_OK;
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

Std_ReturnType TcpIp_SoAdGetSocket(
        TcpIp_DomainType    Domain,
        TcpIp_ProtocolType  Protocol,
        TcpIp_SocketIdType* SocketIdPtr
    )
{
    return E_NOT_OK;
}

void TcpIp_MainFunction(void)
{
}

