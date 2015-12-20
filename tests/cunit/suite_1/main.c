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

#include "TcpIp.c"

#include "CUnit/Basic.h"
#include "CUnit/Automated.h"
#include <arpa/inet.h>
#include <netdb.h>

struct suite_state {
    TcpIp_DomainType   domain;
    boolean            connected[TCPIP_CFG_MAX_SOCKETS];
    TcpIp_SocketIdType id;
    TcpIp_SocketIdType accept_id;
    TcpIp_EventType    events[TCPIP_CFG_MAX_SOCKETS];
    uint32             received[TCPIP_CFG_MAX_SOCKETS];
};

struct suite_state suite_state;

void SoAd_TcpConnected(
        TcpIp_SocketIdType id
    )
{
    suite_state.connected[id] = TRUE;
}

void SoAd_TcpIpEvent(
        TcpIp_SocketIdType          id,
        TcpIp_EventType             event
    )
{
    suite_state.events[id] = event;
}


Std_ReturnType SoAd_TcpAccepted(
        TcpIp_SocketIdType          id,
        TcpIp_SocketIdType          id_connected,
        const TcpIp_SockAddrType*   remote
    )
{
    suite_state.accept_id               = id_connected;
    suite_state.connected[id_connected] = TRUE;
    return E_OK;
}

void SoAd_RxIndication(
        TcpIp_SocketIdType          id,
        const TcpIp_SockAddrType*   remote,
        uint8*                      buf,
        uint16                      len
    )
{
    suite_state.received[id] += len;
}

Std_ReturnType Det_ReportError(
        uint16 ModuleId,
        uint8 InstanceId,
        uint8 ApiId,
        uint8 ErrorId
    )
{
    return E_OK;
}

BufReq_ReturnType SoAd_CopyTxData(
        TcpIp_SocketIdType id,
        uint8*             buf,
        uint16             len
    )
{
    return BUFREQ_E_NOT_OK;
}


TcpIp_ConfigType config = {

};

int suite_init_v4(void)
{
	TcpIp_Init(&config);
	suite_state.domain = TCPIP_AF_INET;
    return 0;
}


int suite_init_v6(void)
{
    TcpIp_Init(&config);
    suite_state.domain = TCPIP_AF_INET6;
    return 0;
}

int suite_clean(void)
{
    TcpIp_SocketIdType index;
    for(index = 0u; index < TCPIP_CFG_MAX_SOCKETS; ++index) {
        TcpIp_SocketState_Enter(index, TCPIP_SOCKET_STATE_UNUSED);
    }
    return 0;
}

void suite_tick(void)
{
}

void suite_test_simple_bind_x(TcpIp_DomainType domain, TcpIp_ProtocolType protocol)
{
    uint16             port;
    CU_ASSERT_EQUAL_FATAL(TcpIp_SoAdGetSocket(domain, protocol, &suite_state.id), E_OK);

    port = TCPIP_PORT_ANY;
    CU_ASSERT_EQUAL(TcpIp_Bind(suite_state.id, TCPIP_LOCALADDRID_ANY, &port), E_OK);
    CU_ASSERT_NOT_EQUAL(port, TCPIP_PORT_ANY);
}

void suite_test_close_x(boolean abort, TcpIp_EventType event, int count)
{
    suite_state.events[suite_state.id] = -1;
    CU_ASSERT_EQUAL(TcpIp_Close(suite_state.id, abort), E_OK);

    for (int i; i < count && suite_state.events[suite_state.id] != event ; ++i) {
        TcpIp_MainFunction();
        usleep(1000);
    }

    CU_ASSERT_EQUAL(suite_state.events[suite_state.id], event);
}


void suite_test_simple_bind_tcp(void)
{
    suite_test_simple_bind_x(suite_state.domain, TCPIP_IPPROTO_TCP);
}

void suite_test_simple_bind_udp(void)
{
    suite_test_simple_bind_x(suite_state.domain, TCPIP_IPPROTO_UDP);
}

void suite_test_simple_listen_tcp(void)
{
    suite_test_simple_bind_x(suite_state.domain, TCPIP_IPPROTO_TCP);
    CU_ASSERT_EQUAL(TcpIp_TcpListen(suite_state.id, 100), E_OK);
}

void suite_test_abort_tcp(void)
{
    suite_test_close_x(TRUE, TCPIP_TCP_RESET, 0);
}

void suite_test_close_tcp(void)
{
    suite_test_close_x(FALSE, TCPIP_TCP_CLOSED, 100);
}

void suite_test_abort_udp(void)
{
    suite_test_close_x(TRUE, TCPIP_UDP_CLOSED, 0);
}

void suite_test_close_udp(void)
{
    suite_test_close_x(FALSE, TCPIP_UDP_CLOSED, 0);
}

void suite_test_fill_sockaddr(TcpIp_SockAddrStorageType* addr, const char* node, uint16 port)
{
    struct addrinfo  *result;
    int v;
    v = getaddrinfo(node, "", NULL,  &result);
    CU_ASSERT_EQUAL_FATAL(v, 0);

    if (result->ai_family == AF_INET) {
        struct sockaddr_in *in  = (struct sockaddr_in *)result->ai_addr;
        addr->inet.domain  = TCPIP_AF_INET;
        addr->inet.port    = port;
        addr->inet.addr[0] = in->sin_addr.s_addr;
    } else if (result->ai_family == AF_INET6) {
        struct sockaddr_in6 *in  = (struct sockaddr_in6 *)result->ai_addr;
        addr->inet6.domain  = TCPIP_AF_INET6;
        addr->inet6.port    = port;
        addr->inet6.addr[0] = in->sin6_addr.__u6_addr.__u6_addr32[0];
        addr->inet6.addr[1] = in->sin6_addr.__u6_addr.__u6_addr32[1];
        addr->inet6.addr[2] = in->sin6_addr.__u6_addr.__u6_addr32[2];
        addr->inet6.addr[3] = in->sin6_addr.__u6_addr.__u6_addr32[3];
    }

    freeaddrinfo(result);
}

void suite_test_loopback_connect_tcp(void)
{
    uint16 port;
    TcpIp_SocketIdType listen, connect;

    CU_ASSERT_EQUAL_FATAL(TcpIp_SoAdGetSocket(suite_state.domain, TCPIP_IPPROTO_TCP, &listen), E_OK);

    port = TCPIP_PORT_ANY;
    CU_ASSERT_EQUAL_FATAL(TcpIp_Bind(listen, TCPIP_LOCALADDRID_ANY, &port)              , E_OK);
    CU_ASSERT_EQUAL_FATAL(TcpIp_TcpListen(listen, 100)                                  , E_OK);

    CU_ASSERT_EQUAL_FATAL(TcpIp_SoAdGetSocket(suite_state.domain, TCPIP_IPPROTO_TCP, &connect), E_OK);

    TcpIp_SockAddrStorageType data;
    if (suite_state.domain == TCPIP_AF_INET) {
        suite_test_fill_sockaddr(&data, "127.0.0.1", port);
    } else {
        suite_test_fill_sockaddr(&data, "::1", port);
    }

    suite_state.connected[connect]        = FALSE;
    suite_state.events[connect]           = -1;
    suite_state.accept_id                 = listen;

    CU_ASSERT_EQUAL_FATAL(TcpIp_TcpConnect(connect, &data.base), E_OK);

    for (int i = 0; i < 1000 && ( (suite_state.connected[connect]  != TRUE)
                             ||   (suite_state.accept_id == listen)); ++i) {
        TcpIp_MainFunction();
        usleep(1000);
    }
    CU_ASSERT_NOT_EQUAL_FATAL(suite_state.accept_id, connect);
    CU_ASSERT_NOT_EQUAL_FATAL(suite_state.accept_id, listen);

    CU_ASSERT_EQUAL_FATAL(suite_state.connected[connect]              , TRUE);
    CU_ASSERT_EQUAL_FATAL(suite_state.connected[suite_state.accept_id], TRUE);
}


void suite_test_loopback_send_udp(void)
{
    uint16 port;
    TcpIp_SocketIdType listen, connect;

    CU_ASSERT_EQUAL_FATAL(TcpIp_SoAdGetSocket(suite_state.domain, TCPIP_IPPROTO_UDP, &listen), E_OK);

    port = TCPIP_PORT_ANY;
    CU_ASSERT_EQUAL_FATAL(TcpIp_Bind(listen, TCPIP_LOCALADDRID_ANY, &port)              , E_OK);

    CU_ASSERT_EQUAL_FATAL(TcpIp_SoAdGetSocket(suite_state.domain, TCPIP_IPPROTO_UDP, &connect), E_OK);

    TcpIp_SockAddrStorageType remote;
    if (suite_state.domain == TCPIP_AF_INET) {
        suite_test_fill_sockaddr(&remote, "127.0.0.1", port);
    } else {
        suite_test_fill_sockaddr(&remote, "::1", port);
    }

    suite_state.events[connect]           = -1;
    suite_state.received[connect]         =  0;
    suite_state.events[listen]            = -1;
    suite_state.received[listen]          =  0;


    uint8 data[256] = {0};
    CU_ASSERT_EQUAL(TcpIp_UdpTransmit(connect, data, &remote.base, sizeof(data)), E_OK);

    for (int i = 0; i < 100; ++i) {
        TcpIp_MainFunction();
        usleep(1000);
    }

    CU_ASSERT_EQUAL_FATAL(suite_state.received[listen]               , sizeof(data));

    CU_ASSERT_EQUAL_FATAL(suite_state.events[connect]              , (TcpIp_EventType)-1);
    CU_ASSERT_EQUAL_FATAL(suite_state.events[listen]               , (TcpIp_EventType)-1);
}

void main_add_generic_suite(CU_pSuite suite)
{

    CU_add_test(suite, "simple_bind_tcp"             , suite_test_simple_bind_tcp);
    CU_add_test(suite, "abort_tcp"                      , suite_test_abort_tcp);

    CU_add_test(suite, "simple_bind_tcp"             , suite_test_simple_bind_tcp);
    CU_add_test(suite, "close_tcp"                      , suite_test_close_tcp);

    CU_add_test(suite, "simple_bind_udp"             , suite_test_simple_bind_udp);
    CU_add_test(suite, "abort_udp"                      , suite_test_abort_udp);

    CU_add_test(suite, "simple_bind_udp"             , suite_test_simple_bind_udp);
    CU_add_test(suite, "close_udp"                      , suite_test_close_udp);

    CU_add_test(suite, "simple_listen_tcp"           , suite_test_simple_listen_tcp);
    CU_add_test(suite, "close_tcp"                      , suite_test_close_tcp);
}

void main_add_loopback_suite(CU_pSuite suite)
{
    CU_add_test(suite, "connect_tcp"                 , suite_test_loopback_connect_tcp);
    CU_add_test(suite, "send_udp"                    , suite_test_loopback_send_udp);
}

int main(void)
{
    CU_pSuite suite = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

    /* add a suite to the registry */
    suite = CU_add_suite("Suite_Generic V4", suite_init_v4, suite_clean);
    main_add_generic_suite(suite);

    suite = CU_add_suite("Suite_Generic V6", suite_init_v6, suite_clean);
    main_add_generic_suite(suite);

    /* add a suite to the registry */
    suite = CU_add_suite("Suite_Loopback V4", suite_init_v4, suite_clean);
    main_add_loopback_suite(suite);

    suite = CU_add_suite("Suite_Loopback V6", suite_init_v6, suite_clean);
    main_add_loopback_suite(suite);


    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    /* Run results and output to files */
    CU_automated_run_tests();
    CU_list_tests_to_file();

    CU_cleanup_registry();
    return CU_get_error();
}
