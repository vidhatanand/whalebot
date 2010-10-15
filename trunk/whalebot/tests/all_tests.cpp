#include "generators_test.h"
#include "token_result_test.h"
#include "tokenize_ansi_test.h"
#include "tokenize_std_test.h"
#include "trim_test.h"
#include "cookie_pasre_test.h"
#include "host_tests.h"

int main ( int argc, char* argv[] )
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
