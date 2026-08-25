/**
 * FakeConnection
 *
 * \file    inc/libklug/internal/connection/fake_connection.hpp
 * \author  Jonas Gahlert
 * \date    29.05.2026
 */

#pragma once

#include <cstdint>
#include <span>
#include "i_connection.hpp"

namespace internal {

/**
 * FakeConnection
 *
 * \details
 * A connection class using no backend at all. The main purpose is to avoid
 * having to build a heavy backend-library when only doing CI-Tests.
 *
 */
struct FakeConnection : public IConnection {
  virtual void init() override {}

  virtual void open(uint16_t pid, uint16_t vid) override {}
  virtual void openFd(int Fd) override {}

  virtual void close() override {}

  virtual void flush() override {}

private:
  virtual void _write(std::span<uint8_t const> payload,
                      uint32_t timeout) override {}
  virtual void _read_until(uint8_t* buffer,
                           uint32_t length,
                           int* received,
                           uint8_t terminator,
                           uint32_t timeout) override {}
  virtual void _read_all(uint8_t* buffer,
                         uint32_t length,
                         int* received,
                         uint32_t timeout) override {}
};

} // namespace internal
