/**
 * Connection
 *
 * \file    src/libklug/internal/connection.cpp
 * \author  Jonas Gahlert
}* \date    21.04.2026
 */

#include "libklug/internal/connection/libusb_connection.hpp"
#include <cassert>
#include <cstdint>
#include <vector>
#include <ztl/ztl.hpp>
#include "libklug/internal/exception/e_libusb.hpp"
#include "libklug/internal/logging.hpp"

namespace internal {

/**
 * Initialize the Libusb context
 *
 * \return int
 * \retval LIBUSB_ERROR   Error
 * \retval LIBUSB_SUCCESS Success
 */
int LibusbConnection::init() {
#ifdef ANDROID
  // We can't search devices on Android
  libusb_set_option(NULL, LIBUSB_OPTION_WEAK_AUTHORITY);
  libusb_set_option(nullptr, LIBUSB_OPTION_NO_DEVICE_DISCOVERY);
#endif
  return libusb_init(nullptr);
}

/**
 * Open connection
 *
 * \warning DO NOT CALL THIS ON ANDROID
 *
 * \param vid VID
 * \param pid PID
 * \return int
 * \retval LIBUSB_ERROR   Error
 * \retval LIBUSB_SUCCESS Success
 */
int LibusbConnection::open(uint16_t vid, uint16_t pid) {
  if (_handle) {
    LOGE("Attempted to open device {:04x}:{:04x}, but a device exists already ",
         vid,
         pid);
    return LIBUSB_ERROR_OTHER;
  }
  _handle = libusb_open_device_with_vid_pid(nullptr, vid, pid);
  if (!_handle) {
    LOGE("No device {:04x}:{:04x} could be opened.", vid, pid);
    return LIBUSB_ERROR_OTHER;
  }

  LOGD("Opened device {:04x}:{:04x}", vid, pid);
  return LIBUSB_SUCCESS;
}

/**
 * Open connection from file descriptor
 *
 * \note This exists mainly for Android, as the devices need to be opened from
 * Java / Kotlin side
 *
 * \param Fd File descriptor
 * \return int
 * \retval LIBUSB_ERROR   Error
 * \retval LIBUSB_SUCCESS Success
 */
int LibusbConnection::openFd(int Fd) {
  if (_handle) {
    LOGE("Attempted to open device with descriptor ID {}, but a device exists "
         "already",
         Fd);
    return LIBUSB_ERROR_OTHER;
  }
  auto rc{libusb_wrap_sys_device(nullptr, (intptr_t)Fd, &_handle)};
  if (rc != LIBUSB_SUCCESS) {
    LOGE("Unable to open device with descriptor ID {}, [{}]",
         Fd,
         libusb_error_name(rc));
  }

  LOGD("Wrapped sys device");
  return rc;
}

/**
 * Config connection
 *
 * \note Configures internals
 *
 * \return int
 * \retval LIBUSB_ERROR   Error
 * \retval LIBUSB_SUCCESS Success
 */
int LibusbConnection::config() {
  if (!_handle) {
    LOGE("Attempted to configure connection without an open device!");
    return LIBUSB_ERROR_OTHER;
  }

  // Populate endpoints
  libusb_config_descriptor* config{};
  auto rc{
    libusb_get_active_config_descriptor(libusb_get_device(_handle), &config)};
  if (rc != LIBUSB_SUCCESS) {
    LOGE("Unable to retrieve config descriptor. Error: {}",
         libusb_error_name(rc));
    return rc;
  }

  std::vector<uint8_t> tx_eps, rx_eps;

  /// \todo Redo this section
  for (int i = 0; i < config->bNumInterfaces; i++) {
    const struct libusb_interface_descriptor* inter_desc =
      &config->interface[i].altsetting[0];

    for (int k = 0; k < inter_desc->bNumEndpoints; k++) {
      const struct libusb_endpoint_descriptor* ep = &inter_desc->endpoint[k];

      // Nur Bulk-Endpunkte beachten
      if ((ep->bmAttributes & 0x03) == LIBUSB_TRANSFER_TYPE_BULK) {
        if ((ep->bEndpointAddress & 0x80) == LIBUSB_ENDPOINT_OUT) {
          tx_eps.push_back(ep->bEndpointAddress);
          _interface = i; // Interface merken für claim_interface
        } else {
          rx_eps.push_back(ep->bEndpointAddress);
        }
      }
    }
  }
  libusb_free_config_descriptor(config);

  if (tx_eps.size() != 1) {
    LOGE("Found {} TX Endpoints, but need exactly ONE", tx_eps.size());
    return LIBUSB_ERROR_OTHER;
  }
  if (rx_eps.size() != 1) {
    LOGE("Found {} RX Endpoints, but need exactly ONE", rx_eps.size());
    return LIBUSB_ERROR_OTHER;
  }

  _tx_ep = tx_eps.front();
  _rx_ep = rx_eps.front();

  LOGD("TX-EP address: x{:02x}", _tx_ep);
  LOGD("RX-EP address: x{:02x}", _rx_ep);

  return rc;
}

/**
 * Claim Interface
 *
 * \return int
 * \retval LIBUSB_ERROR   Error
 * \retval LIBUSB_SUCCESS Success
 */
int LibusbConnection::claim() {
  if (!_handle) {
    LOGE("Attempted to claim an interface without an open device!");
    return LIBUSB_ERROR_OTHER;
  }

  LOGD("TX_EP - {} RX_EP - {} Interface {}", _tx_ep, _rx_ep, _interface);

  auto rc{libusb_kernel_driver_active(_handle, _interface)};
  if (rc == 1) {
    LOGD("Kernel driver attached, attempting to detach");

    rc = libusb_detach_kernel_driver(_handle, _interface);
    if (rc != LIBUSB_SUCCESS) {
      LOGE("Unable to detach kernel driver. Error: {}", libusb_error_name(rc));
      return rc;
    }
    LOGD("Detached kernel driver from interface {}", _interface);
  } else {
    LOGD("libusb returned {} on driver check", rc);
  }

  libusb_detach_kernel_driver(_handle, _interface);

  rc = libusb_claim_interface(_handle, _interface);
  if (rc != LIBUSB_SUCCESS) {
    LOGE("Unable to claim interface {}. Error: {}",
         _interface,
         libusb_error_name(rc));
    return rc;
  }

  LOGD("Successfully claimed interface {}", _interface);
  return rc;
}

/**
 * Release Interface
 *
 * \return int
 * \retval LIBUSB_ERROR   Error
 * \retval LIBUSB_SUCCESS Success
 */
int LibusbConnection::release() {
  if (!_handle) {
    LOGE("Attempted to release an interface without an open device!");
    return LIBUSB_ERROR_OTHER;
  }

  auto rc{libusb_release_interface(_handle, _interface)};
  if (rc != LIBUSB_SUCCESS) {
    LOGE("Unable to release interface. Error: {}", libusb_error_name(rc));
    return rc;
  }

  return -1;
}

/**
 * Close device
 *
 * \warning On Android, the device should be opened and closed from Java /
 * Kotlin
 *
 */
void LibusbConnection::close() {
  if (!_handle) {
    LOGE("Attemted to close a device without an open device!");
    return;
  }

  libusb_close(_handle);
  _handle = nullptr;
  return;
}

/**
 * Flush RX Buffer
 *
 * \todo Refactor, as waiting for an exception is probably not the most elegant
 * thing here
 */
void LibusbConnection::flush() {
  ztl::inplace_vector<uint8_t, 64u> buffer;
  try {
    while (true) receive(buffer, 1);

  } catch (except::libusb_error e) {}
}

/**
 * Transmit payload
 *
 * \param payload Payload
 * \param timeout Timeout
 *
 * \throw libusb_error
 */
void LibusbConnection::_transmit(std::span<uint8_t const> payload,
                                 uint32_t timeout) {
  using std::operator""sv;
  if (auto rc{
        libusb_bulk_transfer(_handle,
                             _tx_ep,
                             std::bit_cast<unsigned char*>(payload.data()),
                             payload.size(),
                             nullptr,
                             timeout)};
      rc != LIBUSB_SUCCESS)
    throw except::libusb_error{rc, "Unable to transmit"sv};
}

/**
 * Receive to buffer
 *
 * \param buffer    Buffer
 * \param length    Buffer length
 * \param received  Actual received
 * \param timeout   Timeout
 *
 * \throw libusb_error
 */
void LibusbConnection::_receive(uint8_t* buffer,
                                uint32_t length,
                                int* received,
                                uint32_t timeout) {
  using std::operator""sv;
  if (auto rc{libusb_bulk_transfer(
        _handle, _rx_ep, buffer, length, received, timeout)};
      rc != LIBUSB_SUCCESS)
    throw except::libusb_error{rc, "Unable to receive"sv};
}

} // namespace internal
