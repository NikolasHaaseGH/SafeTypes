module;

#include "core/macros.hpp"

#include <functional>
#include <limits>
#include <cmath>

export module helium.types:decimals;

import :base;
import :integers;
import :boolean;

namespace HE_NAMESPACE {

#define FRIEND_ARITHMETIC_OPERATORS(Type) \
    friend Type operator+(Type lhs, Type rhs) { lhs += rhs; return lhs; } \
    friend Type operator-(Type lhs, Type rhs) { lhs -= rhs; return lhs; } \
    friend Type operator*(Type lhs, Type rhs) { lhs *= rhs; return lhs; } \
    friend Type operator/(Type lhs, Type rhs) { lhs /= rhs; return lhs; } 


template <typename FloatingPointType>
HE_CONSTEXPR HE_INLINE bool isInfinityOrNan(FloatingPointType value) HE_NOEXCEPT
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
    HE_CONSTEXPR HE_INLINE SafeDecimal(ValueType value = 0) HE_NOEXCEPT : Base(value) {
        detail::Assert(!isInfinityOrNan(value));
    };

    template <typename Other>
    requires (sizeof(Other) >= sizeof(ValueType))
    HE_INLINE HE_CONSTEXPR operator SafeDecimal<Other>() { return m_value; }

    template <typename IntType>
    requires(sizeof(IntType) * 8 <= std::numeric_limits<ValueType>::digits)
    constexpr SafeDecimal(SafeInt<IntType> value) : Base(+value) {}

    template <typename IntType>
    requires(sizeof(IntType) * 8 >= std::numeric_limits<ValueType>::digits)
    constexpr operator SafeInt<IntType>() { return m_value; }

    // Assignemnt Operators
    HE_CONSTEXPR HE_INLINE Bool operator==(SafeDecimal const &rhs) const HE_NOEXCEPT {
        return safeCompare(m_value, rhs.m_value);
    }

    HE_CONSTEXPR HE_INLINE Bool operator!=(SafeDecimal const &rhs) const HE_NOEXCEPT {
        return !safeCompare(m_value, rhs.m_value);
    }

    HE_CONSTEXPR HE_INLINE Bool operator<(SafeDecimal const &rhs) const HE_NOEXCEPT {
        return (m_value < rhs.m_value) && !safeCompare(m_value, rhs.m_value);
    }

    HE_CONSTEXPR HE_INLINE Bool operator>(SafeDecimal const &rhs) const HE_NOEXCEPT {
        return (m_value > rhs.m_value) && !safeCompare(m_value, rhs.m_value);
    }

    HE_CONSTEXPR HE_INLINE Bool operator<=(SafeDecimal const &rhs) const HE_NOEXCEPT {
        return (m_value <= rhs.m_value || safeCompare(m_value, rhs.m_value));
    }

    HE_CONSTEXPR HE_INLINE Bool operator>=(SafeDecimal const &rhs) const HE_NOEXCEPT {
        return (m_value >= rhs.m_value || safeCompare(m_value, rhs.m_value));
    }

    // Assignemnt Operators
    HE_CONSTEXPR HE_INLINE SafeDecimal &operator%=(SafeDecimal const &rhs) HE_NOEXCEPT {
        m_value = m_value % rhs.m_value;
        return *this;
    }

    HE_CONSTEXPR HE_INLINE SafeDecimal operator-() HE_NOEXCEPT {        
        m_value = -m_value;
        return *this;
    }

    HE_CONSTEXPR HE_INLINE SafeDecimal &operator+=(SafeDecimal const &rhs) HE_NOEXCEPT {  
        const ValueType result = m_value + rhs.m_value;
        detail::Assert(!isInfinityOrNan(result));
        m_value = result;
        return *this;
    }

    HE_CONSTEXPR HE_INLINE SafeDecimal &operator-=(SafeDecimal const &rhs) HE_NOEXCEPT {    
        const ValueType result = m_value - rhs.m_value;
        detail::Assert(!isInfinityOrNan(result));
        m_value = result;
        return *this;
    }

    HE_CONSTEXPR HE_INLINE SafeDecimal &operator/=(SafeDecimal const &rhs) HE_NOEXCEPT {
        const ValueType result = m_value / rhs.m_value;
        detail::Assert(!isInfinityOrNan(result));
        m_value = result;
        return *this;
    }

    HE_CONSTEXPR HE_INLINE SafeDecimal &operator*=(SafeDecimal const &rhs) HE_NOEXCEPT {
        const ValueType result = m_value * rhs.m_value;
        detail::Assert(!isInfinityOrNan(result));
        m_value = result;
        return *this;
    }

    FRIEND_ARITHMETIC_OPERATORS(SafeDecimal)

private:

    static HE_CONSTEXPR ValueType EPSILON = std::numeric_limits<ValueType>::epsilon();

    static HE_CONSTEXPR HE_INLINE Bool safeCompare(ValueType first, ValueType second, ValueType maxDiff = EPSILON, ValueType maxRelDiff = EPSILON) HE_NOEXCEPT 
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
    HE_CONSTEXPR HE_INLINE FastDecimal(ValueType value = 0) : Base(value) {}
    
    template <typename Other>
    requires (sizeof(Other) >= sizeof(ValueType))
    HE_CONSTEXPR HE_INLINE operator FastDecimal<Other>() { return m_value; }

    template <typename Other>
    requires (sizeof(Other) >= sizeof(ValueType))
    HE_CONSTEXPR HE_INLINE operator SafeDecimal<Other>() { return m_value; }

    template <typename IntType>
    requires(sizeof(IntType) * 8 <= std::numeric_limits<ValueType>::digits)
    constexpr FastDecimal(FastInt<IntType> value) : Base(+value) {}

    template <typename IntType>
    requires(sizeof(IntType) * 8 >= std::numeric_limits<ValueType>::digits)
    constexpr operator FastInt<IntType>() { return m_value; }

    HE_CONSTEXPR HE_INLINE Bool operator==(FastDecimal rhs) const HE_NOEXCEPT { return m_value == rhs.m_value; }
    HE_CONSTEXPR HE_INLINE Bool operator!=(FastDecimal rhs) const HE_NOEXCEPT { return m_value != rhs.m_value; }
    HE_CONSTEXPR HE_INLINE Bool operator<(FastDecimal rhs) const HE_NOEXCEPT { return m_value < rhs.m_value; }
    HE_CONSTEXPR HE_INLINE Bool operator>(FastDecimal rhs) const HE_NOEXCEPT { return m_value > rhs.m_value; }
    HE_CONSTEXPR HE_INLINE Bool operator<=(FastDecimal rhs) const HE_NOEXCEPT { return m_value <= rhs.m_value; }
    HE_CONSTEXPR HE_INLINE Bool operator>=(FastDecimal rhs) const HE_NOEXCEPT { return m_value >= rhs.m_value; }

    HE_CONSTEXPR HE_INLINE FastDecimal & operator%=(FastDecimal rhs) HE_NOEXCEPT {
        m_value = m_value % rhs.m_value;
        return *this;
    }

    HE_CONSTEXPR HE_INLINE FastDecimal & operator+=(FastDecimal rhs) HE_NOEXCEPT {
        m_value = m_value + rhs.m_value;
        return *this;
    };

    HE_CONSTEXPR HE_INLINE FastDecimal & operator-=(FastDecimal rhs) HE_NOEXCEPT {
        m_value = m_value - rhs.m_value;
        return *this;
    };

    HE_CONSTEXPR HE_INLINE FastDecimal & operator/=(FastDecimal rhs) HE_NOEXCEPT {
        m_value = m_value / rhs.m_value;
        return *this;
    };

    HE_CONSTEXPR HE_INLINE FastDecimal & operator*=(FastDecimal rhs) HE_NOEXCEPT {
        m_value = m_value * rhs.m_value;
        return *this;
    };

    FRIEND_ARITHMETIC_OPERATORS(FastDecimal)
};

}

export namespace HE_NAMESPACE 
{
    using FastFloat = FastDecimal<float>;
    using FastDouble = FastDecimal<double>;
    using SafeFloat = SafeDecimal<float>;
    using SafeDouble = SafeDecimal<double>;
}

export namespace std {
    template <typename T>
    struct hash<HE_NAMESPACE::FastDecimal<T>> {
        size_t operator()(const HE_NAMESPACE::FastDecimal<T>& value) const noexcept {
            return +value;
        }
    };

    template <typename T>
    struct hash<HE_NAMESPACE::SafeDecimal<T>> {
        size_t operator()(const HE_NAMESPACE::SafeDecimal<T>& value) const noexcept {
            return +value;
        }
    };
}
