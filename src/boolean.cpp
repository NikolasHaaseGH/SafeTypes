module;

#include <functional>

export module nhtypes:boolean;

import :common;

export namespace NH_NAMESPACE {

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

        inline constexpr Bool(bool value) noexcept : m_value(static_cast<BooleanValue>(value)) {}

        static constexpr BooleanValue True = BooleanValue::True;
        static constexpr BooleanValue False = BooleanValue::False;

        inline constexpr Bool(const BooleanValue value = False) noexcept : m_value(value) {}

        inline constexpr Bool & operator &=(Bool rhs) noexcept { m_value = static_cast<BooleanValue>(static_cast<bool>(m_value) & static_cast<bool>(rhs.m_value)); return *this; }
        inline constexpr Bool & operator |=(Bool rhs) noexcept { m_value = static_cast<BooleanValue>(static_cast<bool>(m_value) | static_cast<bool>(rhs.m_value)); return *this; }
        inline constexpr Bool & operator ^=(Bool rhs) noexcept { m_value = static_cast<BooleanValue>(static_cast<bool>(m_value) ^ static_cast<bool>(rhs.m_value)); return *this; }
        
        friend inline constexpr Bool operator==(Bool lhs, Bool rhs)  noexcept { return static_cast<BooleanValue>(+lhs == +rhs); }
        friend inline constexpr Bool operator!=(Bool lhs, Bool rhs) noexcept { return static_cast<BooleanValue>(+lhs != +rhs); }

        inline constexpr Bool operator!() const noexcept { return static_cast<BooleanValue>(!static_cast<bool>(m_value)); }

        explicit inline constexpr operator bool() const noexcept { return static_cast<bool>(m_value); }
        inline constexpr bool operator+() const noexcept { return static_cast<bool>(m_value); }
        
        Bool & operator +=(Bool rhs) = delete;
        Bool & operator -=(Bool rhs) = delete;
        Bool & operator *=(Bool rhs) = delete;
        Bool & operator /=(Bool rhs) = delete;
        Bool & operator %=(Bool rhs) = delete;
        
        Bool & operator +(Bool rhs) = delete;
        Bool & operator -(Bool rhs) = delete;
        Bool & operator *(Bool rhs) = delete;
        Bool & operator /(Bool rhs) = delete;
        Bool & operator %(Bool rhs) = delete;
        
        Bool & operator <<=(Bool rhs) = delete;
        Bool & operator >>=(Bool rhs) = delete;
        Bool & operator <<(Bool rhs) = delete;
        Bool & operator >>(Bool rhs) = delete;

        Bool operator >(Bool rhs) = delete;
        Bool operator <(Bool rhs) = delete;
        Bool operator >=(Bool rhs) = delete;
        Bool operator <=(Bool rhs) = delete;
        auto operator <=>(Bool rhs) = delete;        

    private: 
        BooleanValue m_value;
    };

    //inline constexpr Bool operator&(Bool lhs, Bool rhs) noexcept { return lhs &= rhs; }
    //inline constexpr Bool operator|(Bool lhs, Bool rhs) noexcept { return lhs |= rhs; }
    //inline constexpr Bool operator^(Bool lhs, Bool rhs) noexcept { return lhs ^= rhs; }
}

export namespace std {
    template <>
    struct hash<NH_NAMESPACE::Bool> {
        size_t operator()(const NH_NAMESPACE::Bool & value) const {
            return +value;
        }
    };
}
