#include <check.h>
#include <stdlib.h>
#include <stdint.h>
// asi se puede probar las funciones internas
#include "../src/galois_8.c"


START_TEST (test_galois_sum) {
    uint8_t a = 84;  // 0101 0100
    uint8_t b = 13;  // 0000 1101
    
    uint8_t expected = 89;
    ck_assert_uint_eq(expected, g_sum(a,b));
}
END_TEST

START_TEST (test_galois_mult) {
    uint8_t a = 84;  // 0101 0100
    uint8_t b = 13;  // 0000 1101

    uint8_t expected = 1;
    ck_assert_uint_eq(expected, g_mult(a,b));
}
END_TEST

START_TEST (test_galois_pol) {
    uint8_t coefs[4] = {1, 6, 0, 3};

    uint8_t expected = 26;
    ck_assert_uint_eq(expected, g_evaluatePolinomial(3,coefs,4));
}
END_TEST

Suite *
suite(void) {
    Suite *s   = suite_create("galois_8");

    // Sum test case
    TCase *tc_sum = tcase_create("galois_8_sum");
    tcase_add_test(tc_sum, test_galois_sum);
    suite_add_tcase(s, tc_sum);

    // Mult test case
    TCase *tc_mult = tcase_create("galois_8_mult");
    tcase_add_test(tc_mult, test_galois_mult);
    suite_add_tcase(s, tc_mult);

    // evaluate polinomial test case
    TCase *tc_pol = tcase_create("galois_8_pol");
    tcase_add_test(tc_pol, test_galois_pol);
    suite_add_tcase(s, tc_pol);
    return s;
}

int
main(void) {
    SRunner *sr  = srunner_create(suite());
    int number_failed;

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
