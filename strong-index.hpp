// strong-index.hpp: templates for strongly-typed indices into containers.
//
// Copyright 2020 Charles Hussong
// Apache 2.0

#ifndef STRONG_INDEX_TEMPLATE
#define STRONG_INDEX_TEMPLATE

#include <cstddef>      // size_t
#include <type_traits>  // is_nothrow_copy_constructible
#include <iostream>     // operator<<

namespace StrongIndex {

// Every instantiation of all index types must have a tag, typically an empty 
// struct like StrongIndex<struct UserIdTag>. The second parameter, T, is the 
// underlying type of the index which will actually be used inside the 
// container.

// Instead of writing the tags by hand, you can use these macros.
// If you use them in a header file, please #undef them after using.

/* 
#define STRONG_INDEX_BASIC(name) \
    using name = StrongIndex::Basic<struct name ## Tag>
#define STRONG_INDEX_INCREMENTABLE(name) \
    using name = StrongIndex::Incrementable<struct name ## Tag>
#define STRONG_INDEX_FULL_ARITHMETIC(name) \
    using name = StrongIndex::FullArithmetic<struct name ## Tag>
// */


// A Basic StrongIndex does not allow any direct modification of the value --
// it has a constructor and assignment from the underlying type, a static cast 
// into the underlying type, and equality and stream operators.
template<class Tag, typename T = std::size_t>
class Basic {
  private:
    static constexpr bool noThrowIndex 
            = std::is_nothrow_copy_constructible_v<T>;

  public:
    explicit Basic(T underlyingIndex) noexcept(noThrowIndex):
            index_(underlyingIndex) {
    }

    Basic& operator=(const T& underlyingIndex) noexcept(noThrowIndex) {
        index_ = underlyingIndex;
        return *this;
    }

    explicit operator T() const noexcept(noThrowIndex) {
        return index_;
    }

    friend bool operator==(const Basic& a, const Basic& b) 
            noexcept(noexcept(a.index_ == b.index_)) {
        return a.index_ == b.index_;
    }

    friend bool operator!=(const Basic& a, const Basic& b)
            noexcept(noexcept(a == b)) {
        return !(a == b);
    }

    friend std::ostream& operator<<(std::ostream& os, const Basic& idx) 
            noexcept(noexcept(os << idx.index_)) {
        return os << idx.index_;
    }

  private:
    T index_;
};

// Incrementable allows ++ and -- operators (pre- and post- are both OK), as
// well as adding and subtracting the underlying type. Adding or subtracting
// other instances of the index type is not allowed.
template<class Tag, typename T = std::size_t>
class Incrementable {
    // Start with the same capabilities as Basic.

  private:
    static constexpr bool noThrowIndex 
            = std::is_nothrow_copy_constructible_v<T>;

  public:
    explicit Incrementable(T underlyingIndex) noexcept(noThrowIndex):
            index_(underlyingIndex) {
    }

    Incrementable& operator=(const T& underlyingIndex) noexcept(noThrowIndex) {
        index_ = underlyingIndex;
        return *this;
    }

    explicit operator T() const noexcept(noThrowIndex) {
        return index_;
    }

    friend bool operator==(const Incrementable& a, const Incrementable& b) 
            noexcept(noexcept(a.index_ == b.index_)) {
        return a.index_ == b.index_;
    }

    friend bool operator!=(const Incrementable& a, const Incrementable& b)
            noexcept(noexcept(a == b)) {
        return !(a == b);
    }

    friend std::ostream& operator<<(std::ostream& os, const Incrementable& idx) 
            noexcept(noexcept(os << idx.index_)) {
        return os << idx.index_;
    }

  private:
    T index_;

    // New stuff for Incrementable.

  public:
    Incrementable& operator++() noexcept(noexcept(++this->index_)) {
        ++this->index_;
        return *this;
    }

    Incrementable operator++(int) noexcept(noexcept(this->index_++)) {
        Incrementable oldValue(*this);
        operator++();
        return oldValue;
    }

    Incrementable& operator--() noexcept(noexcept(--this->index_)) {
        --this->index_;
        return *this;
    }

    Incrementable operator--(int) noexcept(noexcept(this->index_--)) {
        Incrementable oldValue(*this);
        operator--();
        return oldValue;
    }

    Incrementable& operator+=(const T& idxShift)
            noexcept(noexcept(this->index_ += idxShift)) {
        this->index_ += idxShift;
        return *this;
    }

    Incrementable& operator-=(const T& idxShift)
            noexcept(noexcept(this->index_ -= idxShift)) {
        this->index_ -= idxShift;
        return *this;
    }

    friend Incrementable operator+(
            Incrementable a, const T& idxShift)
            noexcept(noexcept(a += idxShift)) {
        a += idxShift;
        return a;
    }

    friend Incrementable operator-(
            Incrementable a, const T& idxShift)
            noexcept(noexcept(a -= idxShift)) {
        a -= idxShift;
        return a;
    }
};

// FullArithmetic is the most permissive with arithmetic operations.
// Indices are mostly treated as numbers, and can be added and subtracted
// from each other, as well as multiplied and divided by the underlying type.
template<class Tag, typename T = std::size_t>
class FullArithmetic {
    // Start with the same capabilities as Basic.

  private:
    static constexpr bool noThrowIndex 
            = std::is_nothrow_copy_constructible_v<T>;

  public:
    explicit FullArithmetic(T underlyingIndex) noexcept(noThrowIndex):
            index_(underlyingIndex) {
    }

    FullArithmetic& operator=(const T& underlyingIndex) noexcept(noThrowIndex) {
        index_ = underlyingIndex;
        return *this;
    }

    explicit operator T() const noexcept(noThrowIndex) {
        return index_;
    }

    friend bool operator==(const FullArithmetic& a, const FullArithmetic& b) 
            noexcept(noexcept(a.index_ == b.index_)) {
        return a.index_ == b.index_;
    }

    friend bool operator!=(const FullArithmetic& a, const FullArithmetic& b)
            noexcept(noexcept(a == b)) {
        return !(a == b);
    }

    friend std::ostream& operator<<(std::ostream& os, const FullArithmetic& idx) 
            noexcept(noexcept(os << idx.index_)) {
        return os << idx.index_;
    }

  private:
    T index_;

    // Now the full interface from Incrementable.

  public:
    FullArithmetic& operator++() noexcept(noexcept(++this->index_)) {
        ++this->index_;
        return *this;
    }

    FullArithmetic operator++(int) noexcept(noexcept(this->index_++)) {
        FullArithmetic oldValue(*this);
        operator++();
        return oldValue;
    }

    FullArithmetic& operator--() noexcept(noexcept(--this->index_)) {
        --this->index_;
        return *this;
    }

    FullArithmetic operator--(int) noexcept(noexcept(this->index_--)) {
        FullArithmetic oldValue(*this);
        operator--();
        return oldValue;
    }

    FullArithmetic& operator+=(const T& idxShift)
            noexcept(noexcept(this->index_ += idxShift)) {
        this->index_ += idxShift;
        return *this;
    }

    FullArithmetic& operator-=(const T& idxShift)
            noexcept(noexcept(this->index_ -= idxShift)) {
        this->index_ -= idxShift;
        return *this;
    }

    friend FullArithmetic operator+(
            FullArithmetic a, const T& idxShift)
            noexcept(noexcept(a += idxShift)) {
        a += idxShift;
        return a;
    }

    friend FullArithmetic operator-(
            FullArithmetic a, const T& idxShift)
            noexcept(noexcept(a -= idxShift)) {
        a -= idxShift;
        return a;
    }

    // Finally the new stuff for FullArithmetic.

    FullArithmetic& operator+=(const FullArithmetic& other)
            noexcept(noexcept(this->index_ += other.index_)) {
        this->index_ += other.index_;
        return *this;
    }

    FullArithmetic& operator-=(const FullArithmetic& other)
            noexcept(noexcept(this->index_ -= other.index_)) {
        this->index_ -= other.index_;
        return *this;
    }

    friend FullArithmetic operator+(
            FullArithmetic a, const FullArithmetic& b)
            noexcept(noexcept(a += b)) {
        a += b;
        return a;
    }

    friend FullArithmetic operator-(
            FullArithmetic a, const FullArithmetic& b)
            noexcept(noexcept(a -= b)) {
        a -= b;
        return a;
    }

    FullArithmetic& operator*=(const T& idxScale)
            noexcept(noexcept(this->index_ *= idxScale)) {
        this->index_ *= idxScale;
        return *this;
    }

    friend FullArithmetic operator*(
            FullArithmetic a, const T& idxScale)
            noexcept(noexcept(a *= idxScale)) {
        a *= idxScale;
        return a;
    }

    friend FullArithmetic operator*(
            const T& idxScale, FullArithmetic b)
            noexcept(noexcept(b * idxScale)) {
        return b * idxScale;
    }

    FullArithmetic& operator/=(const T& idxDiv)
            noexcept(noexcept(this->index_ /= idxDiv)) {
        this->index_ /= idxDiv;
        return *this;
    }

    friend FullArithmetic operator/(
            FullArithmetic a, const T& idxDiv)
            noexcept(noexcept(a /= idxDiv)) {
        a /= idxDiv;
        return a;
    }

    FullArithmetic& operator%=(const T& idxDiv)
            noexcept(noexcept(this->index_ %= idxDiv)) {
        this->index_ %= idxDiv;
        return *this;
    }

    friend FullArithmetic operator%(
            FullArithmetic a, const T& idxDiv)
            noexcept(noexcept(a %= idxDiv)) {
        a %= idxDiv;
        return a;
    }
};

} // namespace StrongIndex

#endif // STRONG_INDEX_TEMPLATE
