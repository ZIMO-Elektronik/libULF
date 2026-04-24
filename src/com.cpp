#include "com.hpp"
#include <algorithm>
#include "internal/com_transmission.hpp"

/**
 * Ping Device
 *
 * \param [out] result Ping Result
 * \return int
 * \retval LIBUSB_ERROR   Error
 * \retval LIBUSB_SUCCESS Success
 */
int com_ping(char* buffer, std::size_t length) {
  transmission::COMTransmission transmission{"PING\r", 2000u};
  auto rc{transmission.execute()};
  if (rc != 0) return rc;

  if (!transmission.evaluate()) return 1;  // Garbage

  auto const result{transmission.result()};
  if (result.size() > length) return 1; // We dont have enough space in buffer

  std::ranges::copy(result, buffer);
  return 0;
}

/**
 * Reset Device
 * 
 * \param success Reset success
 * \return int 
 * \retval LIBUSB_ERROR   Error
 * \retval LIBUSB_SUCCESS Success
 */
int com_reset(bool* success) {
  transmission::COMTransmission transmission{"RESET\r", 2000u};
  auto rc{transmission.execute()};
  if (rc != 0) return rc;

  if (!transmission.evaluate()) return 1;  // Garbage

  std::string const result{
    std::bit_cast<std::span<char>>(transmission.result()).data()};
  *success = result == "OK\r";
  return 0;
}

/**
 * Enter SUSIV2 Mode
 *
 * \param [out] success Mode change success
 * \return int
 * \retval LIBUSB_ERROR   Error
 * \retval LIBUSB_SUCCESS Success
 */
int com_susiv2(bool* success) {
  transmission::COMTransmission transmission{"SUSIV2\r", 5000u};
  auto rc{transmission.execute()};
  if (rc != 0) return rc;

  if (!transmission.evaluate()) return 1;  // Garbage

  std::string const result{
    std::bit_cast<std::span<char>>(transmission.result()).data()};
  *success = result == "OK\r";
  return 0;
}
