module;

#include "core/macros.hpp"
#include <cstdint>
#include <cstddef>
#include <cassert>

export module helium.types:base;

export namespace HE_NAMESPACE {

    namespace detail {
        inline void Assert(bool condition) { 
            if(!condition){
                if constexpr (HE_TESTABLE_ASSERTIONS) {
                    throw 1;
                } else {
                    assert(condition); 
                }
            }
        }

        inline void DebugAssert(bool condition) { 
            if constexpr(HE_DEBUG) {
                if(!condition){
                    if constexpr (HE_TESTABLE_ASSERTIONS) {
                        throw 1;
                    } else {
                        assert(condition); 
                    }
                }
            }
        }

    }

    struct safe_type_trait {};

    using uint8_t = std::uint8_t; 
    using uint16_t = std::uint16_t;
    using uint32_t = std::uint32_t;
    using uint64_t = std::uint64_t;
    
    using int8_t = std::int8_t;
    using int16_t = std::int16_t;
    using int32_t = std::int32_t;
    using int64_t = std::int64_t;

    using size_t = std::size_t;
}

