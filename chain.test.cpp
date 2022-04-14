#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "Chain.hpp"
#include "doctest.h"

TEST_CASE("Testing Hooke class") {
  SUBCASE("Hooke with attractive force") {
    Hooke hooke{2., 10.};
    PPState p1{1., 3., 0.};
    PPState p2{1., 15., 0.};
    CHECK(hooke(p1, p2) == 4.);
  }

  SUBCASE("Hooke with null force") {
    Hooke hooke{2., 10.};
    PPState p1{1., 0., 0.};
    PPState p2{1., 10., 0.};
    CHECK(hooke(p1, p2) == 0.);
  }

  SUBCASE("Hooke with repulsive force") {
    Hooke hooke{2., 10.};
    PPState p1{1., 3., 0.};
    PPState p2{1., 10., 0.};
    CHECK(hooke(p1, p2) == -6.);
  }
}

TEST_CASE("Testing Chain class") {
  SUBCASE("Testing push_back and size") {
    Chain c{Hooke{0.1, 2}};
    c.push_back({1., 0., 0.});
    c.push_back({1., 4., 0.});
    CHECK(c.size() == 2);
  }

  SUBCASE("2 Points chain") {
    Chain c{Hooke{0.1, 2.}};
    c.push_back({1., 0., 0.});
    c.push_back({1., 4., 0.});
    c.print();
    const auto state_i = c.state();
    c.evolve(0.1);
    c.print();
    const auto state_f = c.state();
    std::vector<PPState> vi{{1., 0., 0.}, {1., 4., 0.}};
    std::vector<PPState> vf{{1., 0.001, 0.02}, {1., 3.999, -0.02}};
    CHECK((state_i == vi) == true);
    CHECK((state_f == vf) == true);
  }
}