/**
* Copyright (C) Mellanox Technologies Ltd. 2001-2013.  ALL RIGHTS RESERVED.
* Copyright (C) UT-Battelle, LLC. 2014. ALL RIGHTS RESERVED.
* See file LICENSE for terms.
*/

#include <gtest/gtest.h>
extern "C" {
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#include <ucs/config/parser.h>
#include <ucs/sys/sys.h>
}
#include "test_helpers.h"

static int ucs_gtest_random_seed = -1;

void parse_test_opts(int argc, char **argv) {
    int c;
    while ((c = getopt(argc, argv, "s:")) != -1) {
        switch (c) {
        case 's':
            ucs_gtest_random_seed = atoi(optarg);
            break;
        default:
            fprintf(stderr, "Usage: gtest [ -s rand-seed ]\n");
            exit(1);
        }
    }
}

static void modify_config_for_valgrind(const char *name, const char *value)
{
    char full_name[128];

    snprintf(full_name, sizeof(full_name), "%s%s", UCS_CONFIG_PREFIX, name);

    if (getenv(full_name) == NULL) {
        UCS_TEST_MESSAGE << " Setting for valgrind: " << full_name << "=" << value;
        setenv(full_name, value, 1);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    parse_test_opts(argc, argv);
    if (ucs_gtest_random_seed == -1) {
        ucs_gtest_random_seed = time(NULL) % 32768;
    }
    UCS_TEST_MESSAGE << "Using random seed of " << ucs_gtest_random_seed;
    srand(ucs_gtest_random_seed);
    if (RUNNING_ON_VALGRIND) {
        modify_config_for_valgrind("IB_RX_QUEUE_LEN", "512");
        modify_config_for_valgrind("IB_RX_BUFS_GROW", "512");
        modify_config_for_valgrind("MM_RX_BUFS_GROW", "32");
        modify_config_for_valgrind("IB_TX_QUEUE_LEN", "128");
        modify_config_for_valgrind("IB_TX_BUFS_GROW", "64");
        modify_config_for_valgrind("RC_TX_CQ_LEN", "256");
    }
    return RUN_ALL_TESTS();
}
