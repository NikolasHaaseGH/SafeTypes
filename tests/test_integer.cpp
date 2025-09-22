module;

#include "catch2/catch_test_macros.hpp"
#include <limits>

export module test.integer;

import helium.types;

using namespace he;

TEST_CASE("Convert between integer types")
{
    SECTION("Signed to signed")
    {
        FastI8  fi8   = FastI8();
        FastI16 fi16  = FastI8();
        FastI32 fi32  = FastI16();
        FastI64 fi64  = FastI32();
        FastI64 fi64_ = FastI64();

        SafeI8  si8   = FastI8();
        SafeI16 si16  = FastI8();
        SafeI32 si32  = FastI16();
        SafeI64 si64  = FastI32();
        SafeI64 si64_ = FastI64();

        SafeI8  si8_    = SafeI8();
        SafeI16 si16_   = SafeI8();
        SafeI32 si32_   = SafeI16();
        SafeI64 si64__  = SafeI32();
        SafeI64 si64___ = SafeI64();
    }

    SECTION("Unsigned to unsigned")
    {
        // Same classes as signed counterparts(only internal type is different)
        // and therefore identical behaviour as above.
    }

    SECTION("Unsigned to signed")
    {
        FastI16 fi16 = FastU8();
        FastI32 fi32 = FastU8();
        FastI64 fi64 = FastU32();

        SafeI16 si16 = FastU8();
        SafeI32 si32 = FastU16();
        SafeI64 si64 = FastU32();

        SafeI16 si16_ = SafeU8();
        SafeI32 si32_ = SafeU16();
        SafeI64 si64_ = SafeU32();
    }
}

#define Integer_Arithmetic_Tests(Type, CType) \
                                              \
    REQUIRE_NOTHROW(Type(Min));               \
    REQUIRE_NOTHROW(Type(Max));               \
                                              \
    REQUIRE_THROWS(Type(Min) -= 1);           \
    REQUIRE_THROWS(Type(Max) += 1);           \
    REQUIRE_THROWS(Type(Max) *= 2);           \
    REQUIRE_THROWS(Type(Max) /= 0);           \
                                              \
    REQUIRE_NOTHROW(Type(Min) += 1);          \
    REQUIRE_NOTHROW(Type(Max) -= 1);          \
    REQUIRE_NOTHROW(Type(1) *= 2);            \
    REQUIRE_NOTHROW(Type(Max) /= 1);          \
                                              \
    REQUIRE_NOTHROW(Type(Min)++);             \
    REQUIRE_NOTHROW(++Type(Min));             \
    REQUIRE_NOTHROW(Type(Max)--);             \
    REQUIRE_NOTHROW(--Type(Max));             \
                                              \
    REQUIRE_THROWS(Type(Max)++);              \
    REQUIRE_THROWS(++Type(Max));              \
    REQUIRE_THROWS(Type(Min)--);              \
    REQUIRE_THROWS(--Type(Min));              \
                                              \
    REQUIRE(+Type(1));                        \
    REQUIRE_FALSE(+Type(0));

#define Integer_Bitwise_Tests(Type, CType)                \
    REQUIRE_THROWS(Type(0) <<= (sizeof(CType) * 8));      \
    REQUIRE_THROWS(Type(0) >>= (sizeof(CType) * 8));      \
                                                          \
    REQUIRE_NOTHROW(Type(0) <<= (sizeof(CType) * 8) - 1); \
    REQUIRE_NOTHROW(Type(0) <<= 0);                       \
    REQUIRE_NOTHROW(Type(0) >>= (sizeof(CType) * 8) - 1); \
    REQUIRE_NOTHROW(Type(0) >>= 0);

#define Make_Signed_Integer_Tests(Type, CType)                   \
    TEST_CASE("Test " #Type)                                     \
    {                                                            \
        constexpr CType Min = std::numeric_limits<CType>::min(); \
        constexpr CType Max = std::numeric_limits<CType>::max(); \
                                                                 \
        Integer_Arithmetic_Tests(Type, CType);                   \
                                                                 \
        REQUIRE_THROWS(Type(Min) *= 2);                          \
        REQUIRE_THROWS(Type(Max) -= (-1));                       \
        REQUIRE_THROWS(-Type(Min));                              \
    }

#define Make_Unsigned_Integer_Tests(Type, CType)                 \
    TEST_CASE("Test " #Type)                                     \
    {                                                            \
        constexpr CType Min = std::numeric_limits<CType>::min(); \
        constexpr CType Max = std::numeric_limits<CType>::max(); \
                                                                 \
        Integer_Arithmetic_Tests(Type, CType);                   \
                                                                 \
        REQUIRE_NOTHROW(Type(Min) *= 2);                         \
    }

#define Make_Index_Integer_Tests(Type, CType)                    \
    TEST_CASE("Test " #Type)                                     \
    {                                                            \
        constexpr CType Min = std::numeric_limits<CType>::min(); \
        constexpr CType Max = std::numeric_limits<CType>::max(); \
                                                                 \
        Integer_Arithmetic_Tests(Type, CType);                   \
        Integer_Bitwise_Tests(Type, CType);                      \
                                                                 \
        REQUIRE_NOTHROW(Type(Min) *= 2);                         \
        REQUIRE(Type(1));                                        \
        REQUIRE_FALSE(Type(0));                                  \
    }

    Make_Unsigned_Integer_Tests(SafeU8, uint8_t);
    Make_Unsigned_Integer_Tests(SafeU16, uint16_t);
    Make_Unsigned_Integer_Tests(SafeU32, uint32_t);
    Make_Unsigned_Integer_Tests(SafeU64, uint64_t);

    Make_Signed_Integer_Tests(SafeI8, int8_t);
    Make_Signed_Integer_Tests(SafeI16, int16_t);
    Make_Signed_Integer_Tests(SafeI32, int32_t);
    Make_Signed_Integer_Tests(SafeI64, int64_t);
