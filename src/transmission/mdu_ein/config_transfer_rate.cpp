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
 * MDU_EIN config transfer rate transmission
 *
 * \file    src/transmission/mdu_ein/config_transfer_rate.hpp
 * \author  Jonas Gahlert
 * \date    18.05.2026
 */

#include "config_transfer_rate.hpp"
#include <ulf/mdu_ein.hpp>
#include "log/logger.hpp"
#include "ulf/cpp/ulf_error.hpp"

namespace transmission::mdu_ein {

ConfigTransferRate::ConfigTransferRate(
  std::shared_ptr<internal::IConnection> conn, mdu::TransferRate speed)
  : _conn{conn}, _speed{speed} {}

/**
 * Execute
 *
 * \throw libulf_error   Device unresponsive or other Error
 *
 */
void ConfigTransferRate::execute() {
  using std::operator""sv;
  try {
    if (!packet())
      throw libulf::ulf_error{libulf::Error::nak,
                              "Unable to set speed for decoder"};
    if (!special(false))
      throw libulf::ulf_error{libulf::Error::nak,
                              "Unable to set speed for device"};
    _result = true;
  } catch (std::exception const& e) {
    LOG_ERROR("{}", e.what());
    LOG_WARN("Attempting to set fallback timing");
    if (!special(true))
      throw libulf::ulf_error{libulf::Error::nak,
                              "Unable to set fallback speed for device"};
  }
}

/// Stub
std::string ConfigTransferRate::evaluateString() {
  throw libulf::ulf_error{libulf::Error::unknown, "Missing Implementation"};
  std::unreachable();
}

/// Return result
bool ConfigTransferRate::evaluateBool() { return _result; }

/// Stub
int ConfigTransferRate::evaluateValue() {
  throw libulf::ulf_error{libulf::Error::unknown, "Missing Implementation"};
  std::unreachable();
}

/**
 * Set the transfer rate for the decoder
 *
 * \return true, if successful, false else
 *
 * \throw libulf_error   If format does not match protocol or the device is
 *                        unresponsive
 */
bool ConfigTransferRate::packet() {
  Base t{
    _conn,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_config_transfer_rate_packet(_speed)),
    internal::config::timeout::mdu_ein::config_transfer_rate};
  t.execute();
  return t.evaluateBool();
}

/**
 * Set the transfer rate in the device
 *
 * \param fallback  Should use fallback speed
 *
 * \return true, if successful, false else
 *
 * \throw libulf_error   If format does not match protocol or the device is
 *                        unresponsive
 */
bool ConfigTransferRate::special(bool fallback) {
  Base t{_conn,
         ulf::mdu_ein::special2mdu_ein(
           ulf::mdu_ein::Command::Speed,
           std::to_underlying(fallback ? mdu::TransferRate::Fallback : _speed),
           std::array<uint8_t, 16>{}),
         internal::config::timeout::mdu_ein::config_transfer_rate};
  t.execute();
  return t.evaluateBool();
}

} // namespace transmission::mdu_ein
