
#ifndef SOAD_CBK_H_
#define SOAD_CBK_H_

#include "Std_Types.h"
#include "TcpIp.h"
#include "ComStack_Types.h"

void SoAd_RxIndication(
        TcpIp_SocketIdType          SocketId,
        const TcpIp_SockAddrType*   RemoteAddrPtr,
        uint8*                      BufPtr,
        uint16                      Length
    );

void SoAd_TcpIpEvent(
        TcpIp_SocketIdType          SocketId,
        TcpIp_EventType             Event
    );

void SoAd_TxConfirmation(
        TcpIp_SocketIdType          SocketId,
        uint16                      Length
    );

Std_ReturnType SoAd_TcpAccepted(
        TcpIp_SocketIdType          SocketId,
        TcpIp_SocketIdType          SocketIdConnected,
        const TcpIp_SockAddrType*   RemoteAddrPtr
    );

void SoAd_TcpConnected(
        TcpIp_SocketIdType SocketId
    );

BufReq_ReturnType SoAd_CopyTxData(
        TcpIp_SocketIdType SocketId,
        uint8* BufPtr,
        uint16 BufLength
    );

#endif /* SOAD_CBK_H_ */
