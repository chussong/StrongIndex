// example.cpp: some quick example code showing how to use
// StrongIndex, including some comments that won't compile.
//
// Copyright 2020 Charles Hussong
// Apache 2.0

#include "strong-index.hpp"

#include <cstdlib> // EXIT_SUCCESS
#include <vector>
#include <iostream>
#include <array>
#include <random>

// The simplest way to use a StrongIndex is with a "using" declaration
// like this (or the corresponding macro).
using UserId = StrongIndex::Basic<struct UserIdTag>;
//STRONG_INDEX_BASIC(UserId);
class UserDb {
  public:
    UserDb(std::size_t size, std::mt19937& rng):
            friend_counts_(size) {
        std::uniform_int_distribution<> dist(0, 100);
        for (std::size_t i = 0; i < friend_counts_.size(); ++i) {
            friend_counts_[i] = dist(rng);
        }
    }

    int friend_count(UserId id) const noexcept {
        return friend_counts_[static_cast<std::size_t>(id)];
    }

  private:
    std::vector<int> friend_counts_;
};

// You could also declare the StrongIndex type inside the class that
// will use it.
class StudentDb {
  public:
    using Id = StrongIndex::Basic<struct CustomerIdTag>;
    //STRONG_INDEX_BASIC(Id);

    explicit StudentDb(std::size_t size, std::mt19937& rng):
            gpas_(size) {
        std::uniform_real_distribution<> dist(0.0, 4.0);
        for (std::size_t i = 0; i < gpas_.size(); ++i) {
            gpas_[i] = dist(rng);
        }
    }

    double gpa(Id id) const noexcept {
        return gpas_[static_cast<std::size_t>(id)];
    }

  private:
    std::vector<double> gpas_;
};

// Now we will see that two distinct types have been created which
// can be converted to the underlying type but not to each other.

static constexpr std::size_t dbSize = 100;

int main(int argc, char** argv) {
    // To start with, we'll request two numbers in [0, dbSize) from the user
    // and store them in an array called rawIds.
    if (argc != 3) {
        std::cerr << "Exactly two arguments are required:\n"
                  << "* A userId\n"
                  << "* A customerId\n"
                  << "Both must be positive integers < " 
                  << std::to_string(dbSize) << ".\n";
        return EXIT_FAILURE;
    }

    std::array<std::size_t, 2> rawIds;
    for (std::size_t i = 0; i < rawIds.size(); ++i) {
        try {
            rawIds[i] = std::stoull(argv[i+1]);
        } catch(std::invalid_argument&) {
            std::cerr << "ERROR: Could not convert argument "
                      << std::to_string(i+1)
                      << " to an unsigned integer.\n";
            return EXIT_FAILURE;
        } catch(std::out_of_range&) {
            std::cerr << "ERROR: Argument " << std::to_string(i+1)
                      << " was outside the range of"
                      << " unsigned long long.\n";
            return EXIT_FAILURE;
        }

        if (rawIds[i] >= dbSize) {
            std::cerr << "ERROR: Argument " << std::to_string(i+1)
                      << " was larger than or equal to the DB size ("
                      << std::to_string(dbSize) << ").\n";
            return EXIT_FAILURE;
        }
    }

    // You can construct a StrongIndex from the underlying type by
    // directly invoking the constructor, using assignment operator=,
    // or with a static_cast.
    UserId userId(rawIds[0]);               // Directly invoked constructor
    // UserId doesNotCompile = rawIds[0];   // No implicit construction
    auto thisIsOk = UserId(rawIds[0]);      // This is explicit so it's OK
    thisIsOk = rawIds[0];                   // Assignment of existing index OK
    thisIsOk = userId;                      // Also works from same-type index
    auto studentId = static_cast<StudentDb::Id>(rawIds[1]);
    // StudentDb::Id notAllowed(userId);    // Can't construct from other types
    // notAllowed = userId;                 // Can't assign from other types

    // We'll just make up some random data to access using the indices.
    std::mt19937 rng(std::random_device{}());
    UserDb userDb(dbSize, rng);
    StudentDb studentDb(dbSize, rng);

    // You can stream a StrongIndex out just like its underlying type.
    std::cout << "User with ID " << userId << " has "
              << userDb.friend_count(userId) << " friend";
    if (userDb.friend_count(userId) != 1) std::cout << 's';
    std::cout << ".\n";

    std::cout << "Student with ID " << studentId << " has a "
              << studentDb.gpa(studentId) << " GPA.\n";

    // You can't look something up in the wrong container because the types 
    // don't match.
    /*
    std::cout << "Can't look up a user with a regular number: "
              << userDb.friend_count(rawIds[0]) << '\n';
    std::cout << "Can't look up a user with a student ID: "
              << userDb.friend_count(studentId) << '\n';
    // */

    // You can check if two same-type indices are equal or unequal.
    if (userId != userId) {
        std::cerr << "This shouldn't be happening!\n";
        return EXIT_FAILURE;
    }
    // But comparing two dissimilar indices doesn't compile, nor does
    // comparing to the underlying type without a cast.
    /*
    if (userId == studentId) {
        std::cout << "You really like that number, eh!\n";
    }
    if (userId != rawIds[0]) {
        std::cout << "User ID was converted strangely...\n";
    }
    // */

    return EXIT_SUCCESS;
}