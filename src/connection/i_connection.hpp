/**
 * Copyright (C) 2026 ZIMO Elektronik
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <https://gnu.org>.
 *
 *
 *
 *
 *
 * IConnection
 *
 * \file    src/connection/i_connection.hpp
 * \author  Jonas Gahlert
 * \date    29.05.2026
 */

#pragma once

#include <cstdint>
#include <ranges>
#include <span>

namespace internal {

/**
 * IConnection interface
 *
 */
struct IConnection {
  virtual void init() = 0;

  virtual void open(uint16_t pid, uint16_t vid) = 0;
  virtual void openFd(int Fd) = 0;

  virtual void close() = 0;

  virtual void flush() = 0;

  /**
   * Write range
   *
   * \tparam R      Input range type
   * \param r       Range
   * \param timeout Timeout
   *
   * \throws ulf_error   First error occurred
   */
  template<std::ranges::input_range R>
  requires std::constructible_from<std::span<uint8_t const>, R>
  void write(R const& r, uint32_t timeout) {
    _write(r, timeout);
  }

  /**
   * Read to range
   *
   * \note Used range MUST support `resize`, `size` and `data` ops
   *
   * \tparam R          Output range type
   * \tparam T          Terminator type
   * \param r           Range
   * \param terminator  Terminator
   * \param timeout     Timeout
   *
   * \throws ulf_error   First error occurred
   */
  template<std::ranges::output_range<uint8_t> R, typename T>
  requires requires(R r, uint32_t s) {
    { r.resize(s) };
    { r.size() } -> std::convertible_to<size_t>;
    { r.data() } -> std::same_as<uint8_t*>;
  }
  constexpr void read_until(R&& r, T&& terminator, uint32_t timeout) {
    int received{};
    _read_until(r.data(), r.size(), &received, terminator, timeout);
    r.resize(received);
  }

  /**
   * Read to range
   *
   * \note Used range MUST support `resize`, `size` and `data` ops
   *
   * \tparam R      Output range type
   * \param r       Range
   * \param timeout Timeout
   *
   * \throws ulf_error   First error occurred
   */
  template<std::ranges::output_range<uint8_t> R>
  requires requires(R r, uint32_t s) {
    { r.resize(s) };
    { r.size() } -> std::convertible_to<size_t>;
    { r.data() } -> std::same_as<uint8_t*>;
  }
  constexpr void read_all(R&& r, uint32_t timeout) {
    int received{};
    _read_all(r.data(), r.size(), &received, timeout);
    r.resize(received);
  }

private:
  virtual void _write(std::span<uint8_t const> payload, uint32_t timeout) = 0;
  virtual void _read_until(uint8_t* buffer,
                           uint32_t length,
                           int* received,
                           uint8_t terminator,
                           uint32_t timeout) = 0;
  virtual void _read_all(uint8_t* buffer,
                         uint32_t length,
                         int* received,
                         uint32_t timeout) = 0;
};

} // namespace internal
