

#include "internal/transmission.hpp"
#include "internal/connection.hpp"
#include "internal/logging.hpp"

#include <ranges>
#include <string>

#include <libusb.h>

namespace transmission {

Transmission::Transmission(std::string payload, std::size_t timeout)
    : _timeout{timeout} {
  _response.reserve(64u);
  for (auto const it : payload) {
    _payload.push_back(static_cast<uint8_t>(it));
  }
}

Transmission::Transmission(std::span<const uint8_t> payload, std::size_t timeout)
    : _timeout{timeout} {
  _response.reserve(64u);
  std::ranges::copy(payload, std::back_inserter(_payload));
}

int Transmission::execute() {
  this->transmit();
  this->receive();
  return 0;
}

int Transmission::transmit() {


  int transferred{0};
  flush();

  auto rc{libusb_bulk_transfer(conn.handle(), conn.tx_ep(),
                               std::bit_cast<unsigned char *>(_payload.data()),
                               _payload.size(), &transferred, _timeout)};
  if (rc != 0) {
    LOGE("Transfer Error. Error: {}", libusb_error_name(rc));
    return rc;
  }

  if (transferred != _payload.size()) {
    LOGE("Transferred to size mismatch. Expected {}, Actual {}",
         _payload.size(), transferred);
    return -1;
  }
  return rc;
}

int Transmission::receive() {
  int transferred{0};

  std::array<uint8_t, 64u> data;

  auto rc{libusb_bulk_transfer(conn.handle(), conn.rx_ep(),
                               std::bit_cast<unsigned char *>(data.data()),
                               data.size(), &transferred, _timeout)};
  if (rc != 0) {
    LOGE("Transfer Error. Error: {}", libusb_error_name(rc));
    return rc;
  }

  LOGD("Received {} Bytes", transferred);

  std::ranges::copy_n(data.begin(), transferred, std::back_inserter(_response));

  return rc;
}

std::span<uint8_t> Transmission::result() { return {_response}; }

void Transmission::flush() {
  std::array<uint8_t, 64u> data;
  while(libusb_bulk_transfer(conn.handle(), conn.rx_ep(), std::bit_cast<unsigned char *>(data.data()), data.size(), nullptr, 10) == 0);
}

} // namespace transmission