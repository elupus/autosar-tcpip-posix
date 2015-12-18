#include "TcpIp.c"

#include "CUnit/Basic.h"
#include "CUnit/Automated.h"


void SoAd_TcpConnected(
        TcpIp_SocketIdType SocketId
    )
{

}
struct suite_state {
    TcpIp_SocketIdType id;
    TcpIp_EventType    event;
};

struct suite_state suite_state;


void SoAd_TcpIpEvent(
        TcpIp_SocketIdType          id,
        TcpIp_EventType             event
    )
{
    CU_ASSERT_EQUAL(suite_state.id, id);
    suite_state.event = event;
}

TcpIp_ConfigType config = {

};

int suite_init(void)
{
	TcpIp_Init(&config);
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

void suite_test_simple_bind_tcp_v4(void)
{
    suite_test_simple_bind_x(TCPIP_AF_INET, TCPIP_IPPROTO_TCP);
}

void suite_test_simple_bind_udp_v4(void)
{
    suite_test_simple_bind_x(TCPIP_AF_INET, TCPIP_IPPROTO_UDP);
}

void suite_test_close_x(boolean abort, TcpIp_EventType event, int count)
{
    suite_state.event = -1;
    CU_ASSERT_EQUAL(TcpIp_Close(suite_state.id, abort), E_OK);

    for (int i; i < count && suite_state.event != event ; ++i) {
        TcpIp_MainFunction();
    }

    CU_ASSERT_EQUAL(suite_state.event, event);
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

int main(void)
{
    CU_pSuite suite = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();


    /* add a suite to the registry */
    suite = CU_add_suite("Suite_Generic", suite_init, suite_clean);

    CU_add_test(suite, "simple_bind_tcp_v4"             , suite_test_simple_bind_tcp_v4);
    CU_add_test(suite, "abort_tcp"                      , suite_test_abort_tcp);

    CU_add_test(suite, "simple_bind_tcp_v4"             , suite_test_simple_bind_tcp_v4);
    CU_add_test(suite, "close_tcp"                      , suite_test_close_tcp);

    CU_add_test(suite, "simple_bind_udp_v4"             , suite_test_simple_bind_udp_v4);
    CU_add_test(suite, "abort_udp"                      , suite_test_abort_udp);

    CU_add_test(suite, "simple_bind_udp_v4"             , suite_test_simple_bind_udp_v4);
    CU_add_test(suite, "close_udp"                      , suite_test_close_udp);

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    /* Run results and output to files */
    CU_automated_run_tests();
    CU_list_tests_to_file();

    CU_cleanup_registry();
    return CU_get_error();
}
