#pragma once

#include <libserialport.h>
#include <cstdint>
#include <span>
#include "i_connection.hpp"

namespace internal {

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
  virtual void _transmit(std::span<uint8_t const> payload,
                         uint32_t timeout) override;
  virtual void _receive(uint8_t* buffer,
                        uint32_t length,
                        int* received,
                        uint32_t timeout) override;

  sp_port* _port{nullptr};
};

} // namespace internal
