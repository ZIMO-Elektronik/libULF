#pragma once

#include <ranges>
#include <zsu/zsu.hpp>

namespace internal {

template<typename T>
concept FirmwareRange =
  std::ranges::bidirectional_range<T> && std::ranges::input_range<T> &&
  std::same_as<zsu::Firmware, typename T::value_type>;

template<FirmwareRange R>
struct ManagedIterator {
  ManagedIterator(R const& r) : _r{r}, _current{r.begin()} {}
  ManagedIterator(ManagedIterator const& lhs) = default;

  bool next() {
    if (_current == _r.cend()) return false;
    _current++;
    return true;
  }

  bool previous() {
    if (_current == _r.cbegin()) return false;
    _current--;
    return true;
  }

  R::value_type const& get() { return *_current; }

private:
  R const& _r;

  R::const_iterator _current{};
};

} // namespace internal
