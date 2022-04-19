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
    Chain<Hooke> c{Hooke{0.1, 2}};
    c.push_back({1., 0., 0.});
    c.push_back({1., 4., 0.});
    CHECK(c.size() == 2);
  }

  SUBCASE("2 Points chain") {
    Chain<Hooke> c{Hooke{0.1, 2.}};
    c.push_back({1., 0., 0.});
    c.push_back({1., 4., 0.});
    const auto state_i = c.state();
    c.evolve(0.1);
    const auto state_f = c.state();
    std::vector<PPState> vi{{1., 0., 0.}, {1., 4., 0.}};
    std::vector<PPState> vf{{1., 0.001, 0.02}, {1., 3.999, -0.02}};
    CHECK(state_i[0].m_ == doctest::Approx(1.));
    CHECK(state_i[0].x_ == doctest::Approx(0.));
    CHECK(state_i[0].v_ == doctest::Approx(0.));
    CHECK(state_i[1].m_ == doctest::Approx(1.));
    CHECK(state_i[1].x_ == doctest::Approx(4.));
    CHECK(state_i[1].v_ == doctest::Approx(0.));
    CHECK(state_f[0].m_ == doctest::Approx(1.));
    CHECK(state_f[0].x_ == doctest::Approx(0.001));
    CHECK(state_f[0].v_ == doctest::Approx(0.02));
    CHECK(state_f[1].m_ == doctest::Approx(1.));
    CHECK(state_f[1].x_ == doctest::Approx(3.999));
    CHECK(state_f[1].v_ == doctest::Approx(-0.02));
  }

  SUBCASE("3 Points chain") {
    Chain<Hooke> c{Hooke{0.1, 2.}};
    c.push_back({1., 0., 0.5});
    c.push_back({1., 2., 0.});
    c.push_back({1., 4., 0.});
    const auto state_i = c.state();
    c.evolve(1.0);
    c.evolve(1.0);
    const auto state_f = c.state();
    CHECK(state_i[0].m_ == doctest::Approx(1.));
    CHECK(state_i[0].x_ == doctest::Approx(0.));
    CHECK(state_i[0].v_ == doctest::Approx(0.5));
    CHECK(state_i[1].m_ == doctest::Approx(1.));
    CHECK(state_i[1].x_ == doctest::Approx(2.));
    CHECK(state_i[1].v_ == doctest::Approx(0.));
    CHECK(state_i[2].m_ == doctest::Approx(1.));
    CHECK(state_i[2].x_ == doctest::Approx(4.));
    CHECK(state_i[2].v_ == doctest::Approx(0.));
    CHECK(state_f[0].m_ == doctest::Approx(1.));
    CHECK(state_f[0].x_ == doctest::Approx(0.975));
    CHECK(state_f[0].v_ == doctest::Approx(0.45));
    CHECK(state_f[1].m_ == doctest::Approx(1.));
    CHECK(state_f[1].x_ == doctest::Approx(2.025));
    CHECK(state_f[1].v_ == doctest::Approx(0.05));
    CHECK(state_f[2].m_ == doctest::Approx(1.));
    CHECK(state_f[2].x_ == doctest::Approx(4.));
    CHECK(state_f[2].v_ == doctest::Approx(0.));
  }
}