#include <catch2/catch.hpp>

#include "../src/static_vector.h"

TEST_CASE("Construct", "[static_vector]")
{
    using sv = static_vector<int, 10>;

    SECTION("constexpr static_vector() noexcept = default")
    {
        sv v;
        REQUIRE(v.size() == 0);
        REQUIRE(v.empty());
    }
    SECTION("constexpr static_vector(size_type count, const T& value)")
    {
        sv v(5ull, 20);
        REQUIRE(v.size() == 5);
        for (int i = 0; i < 5; i++)
        {
            REQUIRE(v.back() == 20);
            v.pop_back();
        }
        CHECK(v.empty());
    }
    SECTION("constexpr explicit static_vector(size_type count)")
    {
        sv v(5);
        REQUIRE(v.size() == 5);
    }
    SECTION("constexpr static_vector(InputIt first, InputIt last)")
    {
        std::vector<int> range{0, 1, 2, 3, 4, 5};
        sv v(range.begin(), range.end());

        REQUIRE(v.size() == range.size());
        for (int i = 0; i < 6; i++)
        {
            REQUIRE(range[i] == v[i]);
        }

        std::vector<int> longer_range{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        REQUIRE_THROWS(sv(longer_range.begin(), longer_range.end()));
    }
    SECTION("constexpr static_vector(std::initializer_list<T> init)")
    {
        sv v{0, 1, 2, 3, 4, 5};
        REQUIRE(v.size() == 6);
        for (int i = 0; i < 6; i++)
            REQUIRE(v[i] == i);

        REQUIRE_THROWS(sv{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    }
}

TEST_CASE("Accesses", "[static_vector]")
{
    static_vector<int, 10> v{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    CHECK(v.size() == 10);

    for (int i = 0; i < 10; i++)
        REQUIRE(v[i] == i);
    for (int i = 0; i < 10; i++)
        REQUIRE(v.at(i) == i);
}

TEST_CASE("Push/Pop", "[static_vector]")
{
    static_vector<int, 10> v{0};

    CHECK(v.size() == 1);

    for (int i = 1; i < 10; i++)
        v.push_back(i);

    CHECK(v.size() == 10);
    REQUIRE_THROWS(v.push_back(10));
    CHECK(v.size() == 10);

    for (int i = 9; i >= 0; i--)
    {
        REQUIRE(v.back() == i);
        v.pop_back();
    }
    REQUIRE(v.empty());
}
