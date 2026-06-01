/**
 * LibserialportConnection
 *
 * \file    inc/libklug/internal/connection/libserialport_connection.hpp
 * \author  Jonas Gahlert
 * \date    29.05.2026
 */

#pragma once

#include <libserialport.h>
#include <cstdint>
#include <span>
#include "i_connection.hpp"

namespace internal {

/**
 * LibserialportConnection
 *
 * \details
 * A connection class using libserialport as its backend
 *
 * \note
 * Probably, using this is better on Windows as we dont have to change the
 * driver..
 *
 * \todo
 * Change error handling to exception model, since we throw anyway
 *
 */
struct LibserialportConnection : public IConnection {
  virtual int init() override;

  virtual int open(uint16_t pid, uint16_t vid) override;
  virtual int openFd(int Fd) override;

  virtual int config() override;
  virtual int claim() override;

  virtual int release() override;
  virtual void close() override;

  virtual void flush() override;

private:
  virtual void _write(std::span<uint8_t const> payload,
                      uint32_t timeout) override;
  virtual void _read_until(uint8_t* buffer,
                           uint32_t length,
                           int* received,
                           uint8_t terminator,
                           uint32_t timeout) override;
  virtual void _read_all(uint8_t* buffer,
                         uint32_t length,
                         int* received,
                         uint32_t timeout) override;

  sp_port* _port{nullptr}; ///< Port
};

} // namespace internal
