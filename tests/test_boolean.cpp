module;

#include "catch2/catch_test_macros.hpp"
#include <limits>

export module test.boolean;

import nhtypes;

TEST_CASE("Test boolean"){
    SECTION("Create booleans") {
        nh::Bool a = 0;
        nh::Bool b = 1;
        nh::Bool c = 2;
    }
}
