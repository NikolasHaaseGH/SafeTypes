module;

#include <functional>
#include <limits>
#include <cmath>

export module nhtypes:decimals;

import :common;
import :integers;
import :boolean;

namespace nh {

#define FRIEND_ARITHMETIC_OPERATORS(Type) \
    friend Type operator+(Type lhs, Type rhs) { lhs += rhs; return lhs; } \
    friend Type operator-(Type lhs, Type rhs) { lhs -= rhs; return lhs; } \
    friend Type operator*(Type lhs, Type rhs) { lhs *= rhs; return lhs; } \
    friend Type operator/(Type lhs, Type rhs) { lhs /= rhs; return lhs; } 


template <typename FloatingPointType>
constexpr inline bool isInfinityOrNan(FloatingPointType value) noexcept
{ 
    return (fabs(value) == std::numeric_limits<FloatingPointType>::infinity()) || std::isnan(value);
}

template <typename ValueType>
struct DecimalBase
{
    explicit operator ValueType() { return m_value; }
    constexpr ValueType operator+() const { return m_value; }

    ValueType m_value;

    static constexpr ValueType Max = std::numeric_limits<ValueType>::max();
    static constexpr ValueType Min = std::numeric_limits<ValueType>::min();

    DecimalBase(ValueType value = 0) : m_value(value) {}
};

template <typename ValueType>
struct SafeDecimal : public DecimalBase<ValueType>
{
    using CType = ValueType;

private:
    typedef DecimalBase<ValueType> Base;
    using Base::m_value;

public:
    constexpr inline SafeDecimal(ValueType value = 0) noexcept : Base(value) {
        Assert(!isInfinityOrNan(value));
    };

    template <typename Other>
    requires (sizeof(Other) >= sizeof(ValueType))
    inline constexpr operator SafeDecimal<Other>() { return m_value; }

    template <typename IntType>
    requires(sizeof(IntType) * 8 <= std::numeric_limits<ValueType>::digits)
    constexpr SafeDecimal(SafeInt<IntType> value) : Base(+value) {}

    template <typename IntType>
    requires(sizeof(IntType) * 8 >= std::numeric_limits<ValueType>::digits)
    constexpr operator SafeInt<IntType>() { return m_value; }

    // Assignemnt Operators
    constexpr inline Bool operator==(SafeDecimal const &rhs) const noexcept {
        return safeCompare(m_value, rhs.m_value);
    }

    constexpr inline Bool operator!=(SafeDecimal const &rhs) const noexcept {
        return !safeCompare(m_value, rhs.m_value);
    }

    constexpr inline Bool operator<(SafeDecimal const &rhs) const noexcept {
        return (m_value < rhs.m_value) && !safeCompare(m_value, rhs.m_value);
    }

    constexpr inline Bool operator>(SafeDecimal const &rhs) const noexcept {
        return (m_value > rhs.m_value) && !safeCompare(m_value, rhs.m_value);
    }

    constexpr inline Bool operator<=(SafeDecimal const &rhs) const noexcept {
        return (m_value <= rhs.m_value || safeCompare(m_value, rhs.m_value));
    }

    constexpr inline Bool operator>=(SafeDecimal const &rhs) const noexcept {
        return (m_value >= rhs.m_value || safeCompare(m_value, rhs.m_value));
    }

    // Assignemnt Operators
    constexpr inline SafeDecimal &operator%=(SafeDecimal const &rhs) noexcept {
        m_value = m_value % rhs.m_value;
        return *this;
    }

    constexpr inline SafeDecimal operator-() noexcept {        
        m_value = -m_value;
        return *this;
    }

    constexpr inline SafeDecimal &operator+=(SafeDecimal const &rhs) noexcept {  
        const ValueType result = m_value + rhs.m_value;
        Assert(!isInfinityOrNan(result));
        m_value = result;
        return *this;
    }

    constexpr inline SafeDecimal &operator-=(SafeDecimal const &rhs) noexcept {    
        const ValueType result = m_value - rhs.m_value;
        Assert(!isInfinityOrNan(result));
        m_value = result;
        return *this;
    }

    constexpr inline SafeDecimal &operator/=(SafeDecimal const &rhs) noexcept {
        const ValueType result = m_value / rhs.m_value;
        Assert(!isInfinityOrNan(result));
        m_value = result;
        return *this;
    }

    constexpr inline SafeDecimal &operator*=(SafeDecimal const &rhs) noexcept {
        const ValueType result = m_value * rhs.m_value;
        Assert(!isInfinityOrNan(result));
        m_value = result;
        return *this;
    }

    FRIEND_ARITHMETIC_OPERATORS(SafeDecimal)

private:

    static constexpr ValueType EPSILON = std::numeric_limits<ValueType>::epsilon();

    static constexpr inline Bool safeCompare(ValueType first, ValueType second, ValueType maxDiff = EPSILON, ValueType maxRelDiff = EPSILON) noexcept 
    {
        const ValueType diff = fabs(first - second);
        if (diff <= maxDiff)
            return Bool::True;

        const ValueType a = fabs(first);
        const ValueType b = fabs(second);
        const ValueType largest = (b > a) ? b : a;

        if (diff <= largest * EPSILON)
            return Bool::True;

        return Bool::False;
    }
};

template <typename ValueType>
struct FastDecimal : public DecimalBase<ValueType>
{
    using CType = ValueType;

private:
    typedef DecimalBase<ValueType> Base;
    using Base::m_value;

public:
    constexpr inline FastDecimal(ValueType value = 0) : Base(value) {}
    
    template <typename Other>
    requires (sizeof(Other) >= sizeof(ValueType))
    constexpr inline operator FastDecimal<Other>() { return m_value; }

    template <typename Other>
    requires (sizeof(Other) >= sizeof(ValueType))
    constexpr inline operator SafeDecimal<Other>() { return m_value; }

    template <typename IntType>
    requires(sizeof(IntType) * 8 <= std::numeric_limits<ValueType>::digits)
    constexpr FastDecimal(FastInt<IntType> value) : Base(+value) {}

    template <typename IntType>
    requires(sizeof(IntType) * 8 >= std::numeric_limits<ValueType>::digits)
    constexpr operator FastInt<IntType>() { return m_value; }

    constexpr inline Bool operator==(FastDecimal rhs) const noexcept { return m_value == rhs.m_value; }
    constexpr inline Bool operator!=(FastDecimal rhs) const noexcept { return m_value != rhs.m_value; }
    constexpr inline Bool operator<(FastDecimal rhs) const noexcept { return m_value < rhs.m_value; }
    constexpr inline Bool operator>(FastDecimal rhs) const noexcept { return m_value > rhs.m_value; }
    constexpr inline Bool operator<=(FastDecimal rhs) const noexcept { return m_value <= rhs.m_value; }
    constexpr inline Bool operator>=(FastDecimal rhs) const noexcept { return m_value >= rhs.m_value; }

    constexpr inline FastDecimal & operator%=(FastDecimal rhs) noexcept {
        m_value = m_value % rhs.m_value;
        return *this;
    }

    constexpr inline FastDecimal & operator+=(FastDecimal rhs) noexcept {
        m_value = m_value + rhs.m_value;
        return *this;
    };

    constexpr inline FastDecimal & operator-=(FastDecimal rhs) noexcept {
        m_value = m_value - rhs.m_value;
        return *this;
    };

    constexpr inline FastDecimal & operator/=(FastDecimal rhs) noexcept {
        m_value = m_value / rhs.m_value;
        return *this;
    };

    constexpr inline FastDecimal & operator*=(FastDecimal rhs) noexcept {
        m_value = m_value * rhs.m_value;
        return *this;
    };

    FRIEND_ARITHMETIC_OPERATORS(FastDecimal)
};

}

export namespace nh 
{
    using FastFloat = FastDecimal<float>;
    using FastDouble = FastDecimal<double>;
    using SafeFloat = SafeDecimal<float>;
    using SafeDouble = SafeDecimal<double>;
}

export namespace std {
    template <typename T>
    struct hash<nh::FastDecimal<T>> {
        size_t operator()(const nh::FastDecimal<T>& value) const noexcept {
            return +value;
        }
    };

    template <typename T>
    struct hash<nh::SafeDecimal<T>> {
        size_t operator()(const nh::SafeDecimal<T>& value) const noexcept {
            return +value;
        }
    };
}
