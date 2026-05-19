/**
 * USB connection
 *
 * \file    inc/libklug/internal/connection.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include <libusb.h>
#include <ranges>

/**
 * Connetion
 *
 * \note More of a fascade for libusb
 *
 */
struct Connection {
  virtual int open(uint16_t vid = 0x1FC9u, uint16_t pid = 0x81C1u);
  virtual int openFd(int Fd);

  virtual int config();
  virtual int claim();

  virtual int release();
  virtual void close();

  /**
   * Transmit range
   *
   * \tparam R      Input range type
   * \param r       Range
   * \param timeout Timeout
   * \return int  Forwarded form libusb
   */
  template<std::ranges::input_range R>
  requires std::constructible_from<std::span<uint8_t const>, R>
  int transmit(R const& r, uint32_t timeout) {
    return _transmit({r}, timeout);
  }

  /**
   * Receive to range
   *
   * \note Used range MUST support `resize`, `size` and `data` ops
   *
   * \tparam R      Output range type
   * \param r       Range
   * \param timeout Timeout
   * \return int  Forwarded from libusb
   */
  template<std::ranges::output_range<uint8_t> R>
  requires requires(R r, uint32_t s) {
    { r.resize(s) };
    { r.size() } -> std::convertible_to<size_t>;
    { r.data() } -> std::same_as<uint8_t*>;
  }
  constexpr int receive(R&& r, uint32_t timeout) {
    int received{};
    auto rc{_receive(r.data(), r.size(), &received, timeout)};
    r.resize(received);
    return rc;
  }

  virtual void flush();

  libusb_device_handle* handle();
  uint8_t tx_ep();
  uint8_t rx_ep();
  int interface();

private:
  virtual int _transmit(std::span<uint8_t const> payload, uint32_t timeout);
  virtual int
  _receive(uint8_t* buffer, uint32_t length, int* received, uint32_t timeout);

  libusb_device_handle* _handle{nullptr}; ///< Device
  uint8_t _tx_ep, _rx_ep;                 ///< Endpoints
  int _interface;                         ///< Interface
};

/**
 * Global connection
 *
 * \deprecated Legacy before the existance of the bridge
 *
 * \todo Remove
 *
 */
inline Connection conn;
