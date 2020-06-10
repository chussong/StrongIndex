// tests.cpp: some doctest unit tests for StrongIndex.
//
// Copyright 2020 Charles Hussong
// Apache 2.0

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "strong-index.hpp"

#include <sstream>

using Underlying = std::size_t;

using Basic = StrongIndex::Basic<struct BasicTag, Underlying>;
using Incrementable = StrongIndex::Incrementable<struct IncTag, Underlying>;
using FullArithmetic = StrongIndex::FullArithmetic<struct FullArTag, Underlying>;

// For testing, we will want to be able to assert that
// indices have particular underlying values.
bool operator==(const Basic& index, const Underlying& value) {
    return static_cast<Underlying>(index) == value;
}
bool operator==(const Incrementable& index, const Underlying& value) {
    return static_cast<Underlying>(index) == value;
}
bool operator==(const FullArithmetic& index, const Underlying& value) {
    return static_cast<Underlying>(index) == value;
}

template<class Index>
void test_basic(const Index& index, Underlying expectedValue) {
    REQUIRE(index == expectedValue);

    Index sameIndex(expectedValue);
    Index differentIndex(expectedValue+1);
    CHECK(index == sameIndex);
    CHECK(index != differentIndex);
    differentIndex = sameIndex;
    CHECK(index == differentIndex);

    std::stringstream ss;
    ss << index;
    CHECK(ss.str() == std::to_string(expectedValue));
}

template<class Index>
void test_incrementable(Index index, Underlying expectedValue) {
    REQUIRE(index == expectedValue);

    Index preInc(++index);
    CHECK(preInc  == expectedValue+1);
    CHECK(index   == expectedValue+1);
    Index postInc(index++);
    CHECK(postInc == expectedValue+1);
    CHECK(index   == expectedValue+2);
    Index preDec(--index);
    CHECK(preDec  == expectedValue+1);
    CHECK(index   == expectedValue+1);
    Index postDec(index--);
    CHECK(postDec == expectedValue+1);
    CHECK(index   == expectedValue);

    index += 2;
    CHECK(index == expectedValue+2);
    index -= 3;
    CHECK(index == expectedValue-1);

    CHECK(index+3 == expectedValue+2);
    CHECK(index-4 == expectedValue-5);
}

template<class Index>
void test_full_arithmetic(Index index, Underlying expectedValue) {
    REQUIRE(index == expectedValue);
    REQUIRE(expectedValue >= 1);

    Index twoLess(expectedValue-2);
    Index almostDouble = index + twoLess;
    CHECK(almostDouble == 2*expectedValue - 2);
    index += twoLess;
    CHECK(index == almostDouble);
    Index originalIndex = almostDouble - twoLess;
    CHECK(originalIndex == expectedValue);
    index -= twoLess;
    CHECK(index == originalIndex);

    Index quadruple = 2*almostDouble + 4;
    CHECK(quadruple == 4*expectedValue);
    index *= 4;
    CHECK(index == quadruple);

    index -= 1;
    Index three = index%4;
    CHECK(three == static_cast<Underlying>(3));
    CHECK(index/4 == expectedValue-1);
    index /= 4;
    CHECK(index == expectedValue-1);
}

TEST_CASE("Basic operations work") {
    REQUIRE(sizeof(Basic) == sizeof(Underlying));
    static constexpr Underlying value = 12;
    Basic index(value);
    test_basic(index, value);
}

TEST_CASE("Increment operations work") {
    REQUIRE(sizeof(Incrementable) == sizeof(Underlying));
    static constexpr Underlying value = 61;
    Incrementable index(value);
    test_basic(index, value);
    test_incrementable(index, value);
}

TEST_CASE("Full arithmetic operations work") {
    REQUIRE(sizeof(FullArithmetic) == sizeof(Underlying));
    static constexpr Underlying value = 107792;
    FullArithmetic index(value);
    test_basic(index, value);
    test_incrementable(index, value);
    test_full_arithmetic(index, value);
}
