module;

#include "catch2/catch_test_macros.hpp"
#include <limits>

export module test.boolean;

import helium.types;

TEST_CASE("Test boolean"){
    SECTION("Create booleans") {
        he::Bool a = 0;
        he::Bool b = 1;
        he::Bool c = 2;
    }
}
