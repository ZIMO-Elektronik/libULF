#include "susiv2.hpp"
#include <ulf/susiv2.hpp>
#include <zusi/zusi.hpp>

#include "internal/susiv2_transmission.hpp"

/**
 * SUSIV2 Features
 *
 * \param [out] result Request result
 * \return int
 * \retval LIBUSB_ERROR   Error
 * \retval LIBUSB_SUCCESS Success
 * \retval 1              Other Error
 */
int susiv2_features(bool* result) {
  transmission::SUSIV2Transmission transmission{
    conn,
    ulf::susiv2::packet2frame<std::vector<uint8_t>>(
      zusi::make_features_packet()),
    2000uz};
  auto rc{transmission.execute()};
  if (rc != 0) return rc;

  /// \todo maybe check crc8 before sending
  auto const re{transmission.result()};
  *result = re[0];
  return 0u;
}

/**
 * SUSIV2 Cv Read
 *
 * \param [in]  cv    Cv Address
 * \param [out] value Cv Value
 * \return int
 * \retval LIBUSB_ERROR   Error
 * \retval LIBUSB_SUCCESS Success
 * \retval 1              Other Error
 */
int susiv2_cv_read(uint32_t cv, uint8_t* value) {
  transmission::SUSIV2Transmission transmission{
    conn,
    ulf::susiv2::packet2frame<std::vector<uint8_t>>(
      zusi::make_cv_read_packet(0, cv - 1)),
    2000uz};
  auto rc{transmission.execute()};
  if (rc != 0) return rc;

  /// \todo maybe check crc8 before sending
  auto const result{transmission.result()};
  if (result.size() < 2) return 1;  // We dont have a value

  *value = result[1];
  return 0;
}