#pragma once

#include <cstdint>
#include <ranges>
#include <span>

namespace internal {
struct IConnection {
  virtual int init() = 0;

  virtual int open(uint16_t pid, uint16_t vid) = 0;
  virtual int openFd(int Fd) = 0;

  virtual int config() = 0;
  virtual int claim() = 0;

  virtual int release() = 0;
  virtual void close() = 0;

  virtual void flush() = 0;

  /**
   * Transmit range
   *
   * \tparam R      Input range type
   * \param r       Range
   * \param timeout Timeout
   *
   * \throw libusb_error
   */
  template<std::ranges::input_range R>
  requires std::constructible_from<std::span<uint8_t const>, R>
  void transmit(R const& r, uint32_t timeout) {
    _transmit({r}, timeout);
  }

  /**
   * Receive to range
   *
   * \note Used range MUST support `resize`, `size` and `data` ops
   *
   * \tparam R      Output range type
   * \param r       Range
   * \param timeout Timeout
   *
   * \throw libusb_error
   */
  template<std::ranges::output_range<uint8_t> R>
  requires requires(R r, uint32_t s) {
    { r.resize(s) };
    { r.size() } -> std::convertible_to<size_t>;
    { r.data() } -> std::same_as<uint8_t*>;
  }
  constexpr void receive(R&& r, uint32_t timeout) {
    int received{};
    _receive(r.data(), r.size(), &received, timeout);
    r.resize(received);
  }

private:
  virtual void _transmit(std::span<uint8_t const> payload,
                         uint32_t timeout) = 0;
  virtual void _receive(uint8_t* buffer,
                        uint32_t length,
                        int* received,
                        uint32_t timeout) = 0;
};

} // namespace internal
