#include <cassert>
#include <iostream>
#include <vector>

struct PPState {
  double m_{};
  double x_{};
  double v_{};
};

class Hooke {
  double m_k;
  double m_l;

 public:
  Hooke(double k, double l) : m_k{k}, m_l{l} {}
  double operator()(PPState const& p1, PPState const& p2) const {
    double delta_x = p2.x_ - p1.x_;
    if (delta_x < 0) delta_x = -delta_x;
    return -m_k * (m_l - delta_x);
  }
};

template <class F>
class Chain {
  F m_inter;
  std::vector<PPState> m_ppses;

  PPState solve(PPState const& pps, double f, double delta_t) {
    PPState ppres;
    ppres.m_ = pps.m_;
    double a = f / pps.m_;
    ppres.v_ = pps.v_ + (a * delta_t);
    ppres.x_ = pps.x_ + (pps.v_ * delta_t) + (0.5 * a * delta_t * delta_t);
    return ppres;
  }

 public:
  Chain(F const& inter) : m_inter{inter} {}

  void push_back(PPState const& pps) { m_ppses.push_back(pps); }

  int size() const { return m_ppses.size(); }

  void evolve(double delta_t) {
    std::vector<PPState> evolved;
    double f;

    // evolution of first point
    f = m_inter(m_ppses[0], m_ppses[1]);
    evolved.push_back(solve(m_ppses[0], f, delta_t));

    // evolution of all points in between
    for (int i = 1; i != (size() - 1); ++i) {
      f = m_inter(m_ppses[i + 1], m_ppses[i]) -
          m_inter(m_ppses[i], m_ppses[i - 1]);
      evolved.push_back(solve(m_ppses[i], f, delta_t));
    }

    // evolution of the last point
    f = -m_inter(m_ppses[size() - 1], m_ppses[size() - 2]);
    evolved.push_back(solve(m_ppses[size() - 1], f, delta_t));

    assert(m_ppses.size() == evolved.size());
    m_ppses = evolved;
  }

  std::vector<PPState> const& state() const { return m_ppses; }
};