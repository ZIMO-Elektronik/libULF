/**
 * Managed Iterator
 *
 * \file    inc/libklug/internal/managed_iterator.hpp
 * \author  Jonas Gahlert
 * \date    19.05.2026
 */

#pragma once

#include <ranges>
#include <zsu/zsu.hpp>

namespace internal {

template<typename T>
concept FirmwareRange =
  std::ranges::bidirectional_range<T> && std::ranges::input_range<T> &&
  std::same_as<zsu::Firmware, typename T::value_type>;

/**
 * ManagedIterator
 *
 * \tparam R Range to iterate
 *
 * \todo  Well.. manage to manage stuff?
 */
template<FirmwareRange R>
struct ManagedIterator {
  ManagedIterator(R const& r) : _r{r}, _current{r.begin()} {}
  ManagedIterator(R const& r, bool) : _r{r}, _current{r.end()} {}
  ManagedIterator(ManagedIterator const& lhs) = default;

  bool next() {
    _current++;
    if (_current == _r.cend()) return false;
    return true;
  }

  bool previous() {
    if (_current == _r.cbegin()) return false;
    _current--;
    return true;
  }

  bool equals(ManagedIterator const& rhs) const {
    return _current == rhs._current;
  }

  R::value_type const& get() { return *_current; }

private:
  R const& _r;

  R::const_iterator _current{};
};

} // namespace internal
