module;

#include <limits>

export module safe_types:type_traits;

import :integers;
import :boolean;
import :decimals;

#define CREATE_TYPE_TRAITS_TEMPLATE_FOR_HELIUM_TYPE(HeType)                                    \
    template<typename T>                                                                       \
    class numeric_limits<HeType<T>> {                                                          \
    public:                                                                                    \
        static constexpr HeType<T> min() { return std::numeric_limits<T>::min(); }             \
        static constexpr HeType<T> max() { return std::numeric_limits<T>::max(); }             \
\
        static constexpr bool is_real = std::numeric_limits<T>::is_real;  \
        static constexpr bool is_signed = std::numeric_limits<T>::is_signed; \
        static constexpr bool is_integer = std::numeric_limits<T>::is_integer; \
                                                                                               \
        static bool is_specialized() { return true; }                                          \
    };  \

export namespace std {

//CREATE_TYPE_TRAITS_TEMPLATE_FOR_HELIUM_TYPE(he::SafeSignedInt)
//CREATE_TYPE_TRAITS_TEMPLATE_FOR_HELIUM_TYPE(he::FastInt)
}
