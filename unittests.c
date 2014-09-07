#include <glib.h>
#include <stdlib.h>

#include "tcpbridge_options.h"
#include "freeatexit.h"

void test_no_options() {
    if (g_test_subprocess()) {
        char *test_argv[] = { "" };
        evaluate_options(1, test_argv);
    }

    g_test_trap_subprocess(NULL, 0, 0);
    g_test_trap_assert_failed();
}

void test_wrong_option() {
    if (g_test_subprocess()) {
        char *test_argv[] = { "", "--nonex-option" };
        evaluate_options(2, test_argv);
    }

    g_test_trap_subprocess(NULL, 0, 0);
    g_test_trap_assert_failed();
}

void test_help() {
    if (g_test_subprocess()) {
        char *test_argv[] = { "", "-h" };
        evaluate_options(2, test_argv);
    }

    g_test_trap_subprocess(NULL, 0, 0);
    g_test_trap_assert_passed();
    g_test_trap_assert_stdout(usage_text(""));
}

void test_default_values() {
    char *test_argv[] = {
        "",
        "-p", "6",
        "-q", "8",
    };
    int test_argc = 5;
    tcpbridge_options *result = evaluate_options(test_argc, test_argv);
    g_assert(result->use_ipv6 == false);
    g_assert_cmpstr(result->first_address_str, ==, DEFAULT_ADDRESS);
    g_assert_cmpstr(result->second_address_str, ==, DEFAULT_ADDRESS);
    g_assert(result->first_port == 6);
    g_assert(result->second_port == 8);
}

void test_wrong_port1() {
    if (g_test_subprocess()) {
        char *test_argv[] = {
            "",
            "-p", "ABC",
            "-q", "8",
        };
        int test_argc = 5;
        evaluate_options(test_argc, test_argv);
    }

    g_test_trap_subprocess(NULL, 0, 0);
    g_test_trap_assert_failed();
}

void test_wrong_port2() {
    if (g_test_subprocess()) {
        char *test_argv[] = {
            "",
            "-p", "6",
            "-q", "DEF",
        };
        int test_argc = 5;
        evaluate_options(test_argc, test_argv);
    }

    g_test_trap_subprocess(NULL, 0, 0);
    g_test_trap_assert_failed();
}

void test_all_values() {
    char *host1 = "nonexistinghost.nodomain";
    char *host2 = "otherhost.nodomain";
    char *test_argv[] = {
        "",
        "-6",
        "-a", host1,
        "-b", host2,
        "-p", "6",
        "-q", "8",
    };
    int test_argc = 10;
    tcpbridge_options *result = evaluate_options(test_argc, test_argv);
    g_assert(result->use_ipv6 == true);
    g_assert_cmpstr(result->first_address_str, ==, host1);
    g_assert_cmpstr(result->second_address_str, ==, host2);
    g_assert(result->first_port == 6);
    g_assert(result->second_port == 8);
}

int main(int argc, char *argv[]) {
    atexit(free_atexit);
    g_test_init(&argc, &argv, NULL);
    g_test_add_func("/options/no_options", test_no_options);
    g_test_add_func("/options/wrong_option", test_wrong_option);
    g_test_add_func("/options/help", test_help);
    g_test_add_func("/options/default_values", test_default_values);
    g_test_add_func("/options/wrong_port1", test_wrong_port1);
    g_test_add_func("/options/wrong_port2", test_wrong_port2);
    g_test_add_func("/options/all_values", test_all_values);
    return g_test_run();
}

