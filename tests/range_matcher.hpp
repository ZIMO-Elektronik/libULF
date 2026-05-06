#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <concepts>
#include <format>
#include <span>
#include <type_traits>

using ::testing::Matcher;

template<std::ranges::input_range E_R>
class RangeMatcher {
public:
  using is_gtest_matcher = void;

  RangeMatcher(E_R e_range) : m_e_range{e_range} {}

  template<std::ranges::input_range R>
  bool MatchAndExplain(R const& range, std::ostream* os) const {
    if (os) *os << std::endl;

    {  // Size check
      auto const size{std::ranges::size(range)};
      auto const e_size{std::ranges::size(m_e_range)};
      if (size != e_size) {
        if (os)
          *os << "Size mismatch. Expected " << static_cast<int>(e_size)
              << " Actual " << static_cast<int>(size) << std::endl;
        return false;
      }
    }
    if (std::ranges::equal(range, m_e_range)) return true;
    {  // Not equal, check first cause
      auto it{std::ranges::begin(range)};
      auto const end{std::ranges::end(range)};
      auto e_it{std::ranges::begin(m_e_range)};
      auto const e_end{std::ranges::end(m_e_range)};
      for (size_t i{0uz}; it != end && e_it != e_end; it++, e_it++, i++) {
        if (*it != *e_it) {
          if (os)
            *os << "Data mismatch at position " << i << ". Expected "
                << std::format("{:#04x}", *e_it) << " Actual "
                << std::format("{:#04x}", *it) << std::endl;
        }
      }
    }
    return false;
  }

  void DescribeTo(std::ostream* os) const {
    if (os) *os << "Ranges equal" << std::endl;
  }

  void DescribeNegationTo(std::ostream* os) const {
    if (os) *os << "Ranges not equal" << std::endl;
  }

private:
  E_R m_e_range;
};

// template<std::ranges::input_range R>
// auto RM(R&& r) {
//   using T = std::ranges::range_value_t<R>;
//   return RangeMatcher<std::vector<T>>(
//     std::vector<T>(std::ranges::begin(r), std::ranges::end(r)));
// }

template<std::ranges::input_range R>
auto RM(R&& r) {
  return RangeMatcher(r);
}