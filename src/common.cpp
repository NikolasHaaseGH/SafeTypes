module;

#include <cstdint>
#include <cstddef>
#include <cassert>

export module safe_types:common;

export namespace nh {

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

namespace nh {

    void DebugBreak()
    {
#if (__has_builtin(__builtin_debugtrap))
        __builtin_debugtrap();
#elif (defined(WIN32) || defined(_WIN32) || defined(__WIN32__))
        __debugbreak()
#elif (defined(__linux__) || defined(__gnu_linux__) || defined(__APPLE__))
        raise(SIGTRAP);
#else
        throw "Invalid Platform";
#endif
    }

    export inline void Assert(bool condition) { 
            if(!condition) [[unlikely]]{
#if ENABLE_TESTABLE_ASSERTIONS
                throw 1;
#elif( defined(_DEBUG))
                DebugBreak();
else
                std::exit(); 
#endif
            }
        }
}
