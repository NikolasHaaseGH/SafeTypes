module;

#include <limits> 
#include <type_traits>

export module nhtypes:integers;

import :common;
import :boolean;

#define ENABLE_IF_UNSIGNED(Type) requires(std::is_unsigned_v<Type>)
#define ENABLE_IF_SIGNED(Type) requires(std::is_signed_v<Type>)

namespace nh {

#define FRIEND_COMPARISON_OPERATORS(Type)                                         \
    friend constexpr bool operator==(Type lhs, Type rhs) { return +lhs == +rhs; } \
    friend constexpr bool operator!=(Type lhs, Type rhs) { return +lhs != +rhs; } \
    friend constexpr bool operator<(Type lhs, Type rhs) { return +lhs < +rhs; }   \
    friend constexpr bool operator>(Type lhs, Type rhs) { return +lhs > +rhs; }   \
    friend constexpr bool operator>=(Type lhs, Type rhs) { return +lhs >= +rhs; } \
    friend constexpr bool operator<=(Type lhs, Type rhs) { return +lhs <= +rhs; }

#define FRIEND_BITWISE_OPERATORS(Type, CType)                                                              \
    friend constexpr Type operator&(Type lhs, Type rhs) ENABLE_IF_UNSIGNED(CType) { return lhs &= rhs; }   \
    friend constexpr Type operator|(Type lhs, Type rhs) ENABLE_IF_UNSIGNED(CType) { return lhs |= rhs; }   \
    friend constexpr Type operator^(Type lhs, Type rhs) ENABLE_IF_UNSIGNED(CType) { return lhs ^= rhs; }   \
    friend constexpr Type operator<<(Type lhs, Type rhs) ENABLE_IF_UNSIGNED(CType) { return lhs <<= rhs; } \
    friend constexpr Type operator>>(Type lhs, Type rhs) ENABLE_IF_UNSIGNED(CType) { return lhs >>= rhs; }  

#define FRIEND_ARITHMETIC_OPERATORS(Type)                                      \
    friend constexpr Type operator+(Type lhs, Type rhs) { return lhs += rhs; } \
    friend constexpr Type operator-(Type lhs, Type rhs) { return lhs -= rhs; } \
    friend constexpr Type operator/(Type lhs, Type rhs) { return lhs /= rhs; } \
    friend constexpr Type operator*(Type lhs, Type rhs) { return lhs *= rhs; } \
    friend constexpr Type operator%(Type lhs, Type rhs) { return lhs %= rhs; }

    template <typename IntType>
    struct IntBase {
        constexpr inline IntBase(IntType value = 0) : m_value(value) {}

        constexpr inline IntType operator+() const { return m_value; }
        explicit constexpr inline operator IntType() const { return m_value; }

        static constexpr IntType Max = std::numeric_limits<IntType>::max();
        static constexpr IntType Min = std::numeric_limits<IntType>::min();
    public:
        IntType m_value;
    };

    template <typename IntType>
    struct SafeInt : public IntBase<IntType> 
    {
    private:
        typedef IntBase<IntType> Base;
        using Base::m_value;
        using CType = IntType;
    
    public:      
        constexpr inline SafeInt(int64_t value = 0) ENABLE_IF_SIGNED(IntType) : Base(value) {
            Assert(value >= Min && value <= Max);
        }

        constexpr inline SafeInt(uint64_t value = 0) ENABLE_IF_UNSIGNED(IntType) : Base(value) {
            Assert(value <= Max);
        }

        template <typename Other>
        requires(sizeof(Other) >= sizeof(IntType) && std::is_signed_v<IntType> == std::is_signed_v<Other>)
        constexpr operator SafeInt<Other>() const { return static_cast<Other>(m_value); }

        template <typename Other>
        requires(sizeof(Other) > sizeof(IntType) && std::is_unsigned_v<IntType> && std::is_signed_v<Other>)
        constexpr operator SafeInt<Other>() const { return static_cast<Other>(m_value); }

        constexpr SafeInt & operator++() {
            Assert(m_value != Max);
            ++m_value; return *this; 
        }

        constexpr SafeInt & operator--() { 
            Assert(m_value != Min);
            --m_value; return *this; 
        }

        constexpr SafeInt operator++(auto) { 
            Assert(m_value != Max);
            return m_value++; 
        }

        constexpr SafeInt operator--(auto) { 
            Assert(m_value != Min);
            return m_value--; 
        }

        constexpr SafeInt & operator%=(SafeInt rhs) { m_value %= rhs.m_value; return *this;}

        constexpr SafeInt operator-() const ENABLE_IF_SIGNED(IntType) {
            Assert(m_value != Min);
            return -m_value; 
        }

        constexpr SafeInt & operator+=(SafeInt rhs) ENABLE_IF_UNSIGNED(IntType) { 
            const IntType result = m_value + rhs.m_value;
            Assert(result >= m_value);        
            m_value = result; 
            return *this; 
        }

        constexpr SafeInt & operator+=(SafeInt rhs) ENABLE_IF_SIGNED(IntType) { 
            const bool didOverflow {
                (rhs.m_value > 0 && m_value > Max - rhs.m_value) ||
                (rhs.m_value < 0 && m_value < Min - rhs.m_value)
            };
            Assert(!didOverflow);
            m_value += rhs.m_value; 
            return *this; 
        }

        constexpr SafeInt & operator-=(SafeInt rhs) ENABLE_IF_UNSIGNED(IntType) { 
            const IntType result = m_value - rhs.m_value; 
            Assert(result <= m_value); // result > m_value -> overflow
            m_value = result; 
            return *this;
        }

        constexpr SafeInt & operator-=(SafeInt rhs) ENABLE_IF_SIGNED(IntType) { 
            const bool didOverflow {
                (rhs.m_value < 0 && m_value > Max + rhs.m_value) ||
                (rhs.m_value > 0 && m_value < Min + rhs.m_value)
            };
            Assert(!didOverflow);
            m_value -= rhs.m_value;  
            return *this;
        }

        constexpr SafeInt & operator/=(SafeInt rhs) ENABLE_IF_UNSIGNED(IntType) {
            Assert(rhs.m_value != 0);
            m_value = m_value / rhs.m_value; 
            return *this;
        }

        constexpr SafeInt & operator/=(SafeInt rhs) ENABLE_IF_SIGNED(IntType) {
            const bool didOverflow {
                rhs.m_value == 0 || (rhs.m_value == -1 && m_value == Min)
            };
            Assert(!didOverflow);
            m_value *= rhs.m_value;
            return *this; 
        }

        constexpr SafeInt & operator*=(SafeInt rhs) ENABLE_IF_UNSIGNED(IntType) {
            const IntType result = m_value * rhs.m_value;
            const bool didOverflow = result < m_value && rhs.m_value != 0;
            Assert(!didOverflow);
            m_value = result; 
            return *this;
        }

        constexpr SafeInt & operator*=(SafeInt rhs) ENABLE_IF_SIGNED(IntType) {
            const bool didOverflow {
                (m_value == -1 && rhs.m_value == Min) ||
                (rhs.m_value == -1 && m_value == Min) ||
                (rhs.m_value != 0 && m_value > Max / rhs.m_value) ||
                (rhs.m_value != 0 && m_value < Min / rhs.m_value)
            };
            Assert(!didOverflow);
            m_value *= rhs.m_value; 
            return *this;
        }

        constexpr SafeInt operator~() ENABLE_IF_UNSIGNED(IntType) { return ~m_value; }
        constexpr SafeInt & operator&=(SafeInt rhs) ENABLE_IF_UNSIGNED(IntType) { m_value &= rhs.m_value; return *this; }
        constexpr SafeInt & operator|=(SafeInt rhs) ENABLE_IF_UNSIGNED(IntType) { m_value |= rhs.m_value; return *this; }
        constexpr SafeInt & operator^=(SafeInt rhs) ENABLE_IF_UNSIGNED(IntType) { m_value ^= rhs.m_value; return *this; }

        constexpr SafeInt & operator<<=(SafeInt rhs) ENABLE_IF_UNSIGNED(IntType) { 
            const bool didOverflow = rhs.m_value >= sizeof(IntType)*8;
            Assert(!didOverflow);
            m_value <<= rhs.m_value;
            return *this;
        }

        constexpr SafeInt & operator>>=(SafeInt rhs) ENABLE_IF_UNSIGNED(IntType) { 
            const bool didOverflow = rhs.m_value >= sizeof(IntType)*8;
            Assert(!didOverflow);
            m_value >>= rhs.m_value;
            return *this;
        }

        FRIEND_COMPARISON_OPERATORS(SafeInt)
        FRIEND_ARITHMETIC_OPERATORS(SafeInt)
        FRIEND_BITWISE_OPERATORS(SafeInt, IntType)

        public:
            using Base::Max;
            using Base::Min;
    };

    template <typename IntType>
    struct FastInt : public IntBase<IntType> 
    {
    private:
        typedef IntBase<IntType> Base;
        using Base::m_value;
        using CType = IntType;

    public:
        constexpr FastInt(uint64_t value = 0) ENABLE_IF_UNSIGNED(IntType) : Base(value) {
        }
        constexpr FastInt(int64_t value = 0) ENABLE_IF_SIGNED(IntType) : Base(value) {
        }

        template <typename Other>
        requires(sizeof(Other) >= sizeof(IntType) && std::is_signed_v<IntType> == std::is_signed_v<Other>)
        constexpr operator FastInt<Other>() const { return static_cast<Other>(m_value); }

        template <typename Other>
        requires(sizeof(Other) > sizeof(IntType) && std::is_unsigned_v<IntType> && std::is_signed_v<Other>)
        constexpr operator FastInt<Other>() const { return static_cast<Other>(m_value); }

        template <typename Other>
        requires(sizeof(Other) >= sizeof(IntType) && std::is_signed_v<IntType> == std::is_signed_v<Other>)
        constexpr operator SafeInt<Other>() const { return static_cast<Other>(m_value); }

        template <typename Other>
        requires(sizeof(Other) > sizeof(IntType) && std::is_unsigned_v<IntType> && std::is_signed_v<Other>)
        constexpr operator SafeInt<Other>() const { return static_cast<Other>(m_value); }

        constexpr FastInt & operator+=(FastInt rhs) { m_value += rhs.m_value; return *this;}
        constexpr FastInt & operator-=(FastInt rhs) { m_value -= rhs.m_value; return *this;}
        constexpr FastInt & operator/=(FastInt rhs) { m_value /= rhs.m_value; return *this;}
        constexpr FastInt & operator*=(FastInt rhs) { m_value *= rhs.m_value; return *this;}
        constexpr FastInt & operator%=(FastInt rhs) { m_value %= rhs.m_value; return *this;}

        constexpr FastInt & operator++() { ++m_value; return *this; }
        constexpr FastInt & operator--() { --m_value; return *this; }
        constexpr FastInt operator++(auto) { return m_value++; }
        constexpr FastInt operator--(auto) { return m_value--; }

        constexpr FastInt operator~() ENABLE_IF_UNSIGNED(IntType) { return ~m_value; }
        constexpr FastInt & operator&=(FastInt rhs) ENABLE_IF_UNSIGNED(IntType) { m_value &= rhs.m_value; return *this; }
        constexpr FastInt & operator|=(FastInt rhs) ENABLE_IF_UNSIGNED(IntType) { m_value |= rhs.m_value; return *this; }
        constexpr FastInt & operator^=(FastInt rhs) ENABLE_IF_UNSIGNED(IntType) { m_value ^= rhs.m_value; return *this; }
        constexpr FastInt & operator<<=(FastInt rhs) ENABLE_IF_UNSIGNED(IntType) { m_value <<= rhs.m_value; return *this; }
        constexpr FastInt & operator>>=(FastInt rhs) ENABLE_IF_UNSIGNED(IntType) { m_value >>= rhs.m_value; return *this; }

        FRIEND_COMPARISON_OPERATORS(FastInt)
        FRIEND_ARITHMETIC_OPERATORS(FastInt)
        FRIEND_BITWISE_OPERATORS(FastInt, IntType)

        public:
            using Base::Max;
            using Base::Min;
    };

}

export namespace nh 
{
    using FastI8 = FastInt<int8_t>;
    using FastI16 = FastInt<int16_t>;
    using FastI32 = FastInt<int32_t>;
    using FastI64 = FastInt<int64_t>;
    using FastU8 = FastInt<uint8_t>;
    using FastU16 = FastInt<uint16_t>;
    using FastU32 = FastInt<uint32_t>;
    using FastU64 = FastInt<uint64_t>;

    using SafeI8 = SafeInt<int8_t>;
    using SafeI16 = SafeInt<int16_t>;
    using SafeI32 = SafeInt<int32_t>;
    using SafeI64 = SafeInt<int64_t>;
    using SafeU8 = SafeInt<uint8_t>;
    using SafeU16 = SafeInt<uint16_t>;
    using SafeU32 = SafeInt<uint32_t>;
    using SafeU64 = SafeInt<uint64_t>;

#if HE_USE_64_BIT_PTR_DEFINES
    using FastSize = FastU64;
    using SafeSize = SafeU64;
#else 
    using FastSize = FastU32;
    using SafeSize = SafeU32;
#endif

    //auto* operator+(auto * lhs, FastSize rhs) { return lhs + +rhs; }   \
    //auto* operator-(auto * lhs, FastSize rhs) { return lhs - +rhs; }   
    auto* operator+(auto * lhs, SafeSize rhs) { return lhs + +rhs; }   \
    auto* operator-(auto * lhs, SafeSize rhs) { return lhs - +rhs; }   
}

export namespace std {
    template <typename T>
    struct hash<nh::FastInt<T>> {
        size_t operator()(const nh::FastInt<T>& value) const noexcept {
            return +value;
        }
    };

    template <typename T>
    struct hash<nh::SafeInt<T>> {
        size_t operator()(const nh::SafeInt<T>& value) const noexcept {
            return +value;
        }
    };
}
