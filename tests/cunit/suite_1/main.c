#include "TcpIp.c"

#include "CUnit/Basic.h"
#include "CUnit/Automated.h"

TcpIp_ConfigType config = {

};


int suite_init(void)
{
    TcpIp_Init(&config);
    return 0;
}

int suite_clean(void)
{
    return 0;
}

void suite_tick(void)
{
}

void suite_test_1(void)
{
}

int main(void)
{
    CU_pSuite suite = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();


    /* add a suite to the registry */
    suite = CU_add_suite("Suite_Generic", suite_init, suite_clean);

    CU_add_test(suite, "test", suite_test_1);

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    /* Run results and output to files */
    CU_automated_run_tests();
    CU_list_tests_to_file();

    CU_cleanup_registry();
    return CU_get_error();
}
