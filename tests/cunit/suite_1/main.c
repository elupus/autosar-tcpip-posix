#include "TcpIp.c"

#include "CUnit/Basic.h"
#include "CUnit/Automated.h"
#include <arpa/inet.h>

struct suite_state {
    TcpIp_DomainType   domain;
    boolean            connected[TCPIP_MAX_SOCKETS];
    TcpIp_SocketIdType id;
    TcpIp_SocketIdType accept_id;
    TcpIp_EventType    events[TCPIP_MAX_SOCKETS];
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
    for(index = 0u; index < TCPIP_MAX_SOCKETS; ++index) {
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

void suite_test_loopback_connect(void)
{
    uint16 port;
    TcpIp_SocketIdType listen, connect;
    struct in_addr     loopback;

    if (suite_state.domain == TCPIP_AF_INET) {
        inet_pton(AF_INET, "127.0.0.1", &loopback);
    } else {
        inet_pton(AF_INET, "::1", &loopback);
    }

    CU_ASSERT_EQUAL_FATAL(TcpIp_SoAdGetSocket(suite_state.domain, TCPIP_IPPROTO_TCP, &listen), E_OK);

    port = TCPIP_PORT_ANY;
    CU_ASSERT_EQUAL_FATAL(TcpIp_Bind(listen, TCPIP_LOCALADDRID_ANY, &port)              , E_OK);
    CU_ASSERT_EQUAL_FATAL(TcpIp_TcpListen(listen, 100)                                  , E_OK);

    CU_ASSERT_EQUAL_FATAL(TcpIp_SoAdGetSocket(suite_state.domain, TCPIP_IPPROTO_TCP, &connect), E_OK);

    TcpIp_SockAddrInetType inet = {};
    inet.domain  = TCPIP_AF_INET;
    inet.port    = port;
    inet.addr[0] = loopback.s_addr;

    suite_state.connected[connect]        = FALSE;
    suite_state.events[connect]           = -1;
    suite_state.accept_id                 = listen;

    CU_ASSERT_EQUAL_FATAL(TcpIp_TcpConnect(connect, &inet.base), E_OK);

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
    CU_add_test(suite, "connect"                        , suite_test_loopback_connect);
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
