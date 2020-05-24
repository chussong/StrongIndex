// strong-index.hpp: a template for strongly-typed indices
// into containers.
//
// Copyright 2020 Charles Hussong
// Apache 2.0

#ifndef STRONG_INDEX_TEMPLATE
#define STRONG_INDEX_TEMPLATE

#include <cstddef>      // size_t
#include <type_traits>  // is_nothrow_copy_constructible
#include <iostream>     // operator<<

namespace StrongIndex {

// Every instantiation must have a tag, typically an
// empty struct like StrongIndex<struct UserIdTag>.
// T is the underlying type of the index which will 
// actually be used inside the container.
template<class Tag, typename T = std::size_t>
class Basic {
  private:
    static constexpr bool noThrowIndex 
            = std::is_nothrow_copy_constructible_v<T>;

  public:
    explicit Basic(T underlyingIndex) noexcept(noThrowIndex):
            index_(underlyingIndex) {
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

  protected:
    T index_;
};

template<class Tag, typename T = std::size_t>
class Incrementable : public Basic<Tag, T> {
  public:
    explicit Incrementable(T underlyingIndex) 
            noexcept(noexcept(Basic<Tag, T>(underlyingIndex))):
            Basic<Tag, T>(underlyingIndex) {
    }

    Incrementable& operator++() noexcept(noexcept(++this->index_)) {
        ++this->index_;
        return *this;
    }

    Incrementable& operator++(int) noexcept(noexcept(this->index_++)) {
        Incrementable oldValue(*this);
        operator++();
        return oldValue;
    }

    Incrementable& operator--() noexcept(noexcept(--this->index_)) {
        --this->index_;
        return *this;
    }

    Incrementable& operator--(int) noexcept(noexcept(this->index_--)) {
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
};

template<class Tag, typename T = std::size_t>
class FullArithmetic : public Incrementable<Tag, T> {
  public:
    explicit FullArithmetic(T underlyingIndex) 
            noexcept(noexcept(Incrementable<Tag, T>(underlyingIndex))):
            Incrementable<Tag, T>(underlyingIndex) {
    }

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

    friend FullArithmetic operator+(
            const T& idxShift, FullArithmetic b)
            noexcept(noexcept(b + idxShift)) {
        return b + idxShift;
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

/* 
// Instead of writing the tags by hand, you can use these macros.
// If you use them in a header file, please #undef them after using.
#define STRONG_INDEX_BASIC(name) \
    using name = StrongIndex::Basic<struct name ## Tag>
#define STRONG_INDEX_INCREMENTABLE(name) \
    using name = StrongIndex::Incrementable<struct name ## Tag>
#define STRONG_INDEX_FULL_ARITHMETIC(name) \
    using name = StrongIndex::FullArithmetic<struct name ## Tag>
// */

} // namespace StrongIndex

#endif // STRONG_INDEX_TEMPLATE