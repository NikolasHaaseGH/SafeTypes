module;

#include "core/macros.hpp"

#include <functional>

export module helium.types:boolean;

import :base;

export namespace HE_NAMESPACE {

enum class BooleanValue : bool {
    False = 0,
    True = 1
};

struct Bool {
    template <typename> friend struct FastInt;
    template <typename> friend struct SafeInt;
    template <typename> friend struct FastDecimal;
    template <typename> friend struct SafeDecimal;

    public:

        HE_INLINE HE_CONSTEXPR Bool(bool value) HE_NOEXCEPT : m_value(static_cast<BooleanValue>(value)) {}

        static constexpr BooleanValue True = BooleanValue::True;
        static constexpr BooleanValue False = BooleanValue::False;

        HE_INLINE HE_CONSTEXPR Bool(const BooleanValue value = False) HE_NOEXCEPT : m_value(value) {}

        HE_INLINE HE_CONSTEXPR Bool & operator &=(Bool rhs) HE_NOEXCEPT { m_value = static_cast<BooleanValue>(static_cast<bool>(m_value) & static_cast<bool>(rhs.m_value)); return *this; }
        HE_INLINE HE_CONSTEXPR Bool & operator |=(Bool rhs) HE_NOEXCEPT { m_value = static_cast<BooleanValue>(static_cast<bool>(m_value) | static_cast<bool>(rhs.m_value)); return *this; }
        HE_INLINE HE_CONSTEXPR Bool & operator ^=(Bool rhs) HE_NOEXCEPT { m_value = static_cast<BooleanValue>(static_cast<bool>(m_value) ^ static_cast<bool>(rhs.m_value)); return *this; }
        
        friend HE_INLINE HE_CONSTEXPR Bool operator==(Bool lhs, Bool rhs)  HE_NOEXCEPT { return static_cast<BooleanValue>(+lhs == +rhs); }
        friend HE_INLINE HE_CONSTEXPR Bool operator!=(Bool lhs, Bool rhs) HE_NOEXCEPT { return static_cast<BooleanValue>(+lhs != +rhs); }

        HE_INLINE HE_CONSTEXPR Bool operator !() HE_NOEXCEPT const { return static_cast<BooleanValue>(!static_cast<bool>(m_value)); }

        explicit HE_INLINE HE_CONSTEXPR operator bool() const HE_NOEXCEPT { return static_cast<bool>(m_value); }
        HE_INLINE HE_CONSTEXPR bool operator+() const HE_NOEXCEPT { return static_cast<bool>(m_value); }
        
        HE_INLINE HE_CONSTEXPR Bool & operator +=(Bool rhs) HE_NOEXCEPT = delete;
        HE_INLINE HE_CONSTEXPR Bool & operator -=(Bool rhs) HE_NOEXCEPT = delete;
        HE_INLINE HE_CONSTEXPR Bool & operator *=(Bool rhs) HE_NOEXCEPT = delete;
        HE_INLINE HE_CONSTEXPR Bool & operator /=(Bool rhs) HE_NOEXCEPT = delete;
        HE_INLINE HE_CONSTEXPR Bool & operator %=(Bool rhs) HE_NOEXCEPT = delete;
        
        HE_INLINE HE_CONSTEXPR Bool & operator +(Bool rhs) HE_NOEXCEPT = delete;
        HE_INLINE HE_CONSTEXPR Bool & operator -(Bool rhs) HE_NOEXCEPT = delete;
        HE_INLINE HE_CONSTEXPR Bool & operator *(Bool rhs) HE_NOEXCEPT = delete;
        HE_INLINE HE_CONSTEXPR Bool & operator /(Bool rhs) HE_NOEXCEPT = delete;
        HE_INLINE HE_CONSTEXPR Bool & operator %(Bool rhs) HE_NOEXCEPT = delete;
        
        HE_INLINE HE_CONSTEXPR Bool & operator <<=(Bool rhs) HE_NOEXCEPT = delete;
        HE_INLINE HE_CONSTEXPR Bool & operator >>=(Bool rhs) HE_NOEXCEPT = delete;
        HE_INLINE HE_CONSTEXPR Bool & operator <<(Bool rhs) HE_NOEXCEPT = delete;
        HE_INLINE HE_CONSTEXPR Bool & operator >>(Bool rhs) HE_NOEXCEPT = delete;

        HE_INLINE HE_CONSTEXPR Bool operator >(Bool rhs) HE_NOEXCEPT = delete;
        HE_INLINE HE_CONSTEXPR Bool operator <(Bool rhs) HE_NOEXCEPT = delete;
        HE_INLINE HE_CONSTEXPR Bool operator >=(Bool rhs) HE_NOEXCEPT = delete;
        HE_INLINE HE_CONSTEXPR Bool operator <=(Bool rhs) HE_NOEXCEPT = delete;
        HE_INLINE HE_CONSTEXPR auto operator <=>(Bool rhs) HE_NOEXCEPT = delete;        

    private: 
        BooleanValue m_value;
    };

    //HE_INLINE HE_CONSTEXPR Bool operator&(Bool lhs, Bool rhs) HE_NOEXCEPT { return lhs &= rhs; }
    //HE_INLINE HE_CONSTEXPR Bool operator|(Bool lhs, Bool rhs) HE_NOEXCEPT { return lhs |= rhs; }
    //HE_INLINE HE_CONSTEXPR Bool operator^(Bool lhs, Bool rhs) HE_NOEXCEPT { return lhs ^= rhs; }
}

export namespace std {
    template <>
    struct hash<HE_NAMESPACE::Bool> {
        size_t operator()(const HE_NAMESPACE::Bool & value) const {
            return +value;
        }
    };
}
