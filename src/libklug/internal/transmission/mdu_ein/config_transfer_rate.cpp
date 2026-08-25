/**
 * MDU_EIN config transfer rate transmission
 *
 * \file    inc/libklug/internal/transmission/mdu_ein/config_transfer_rate.hpp
 * \author  Jonas Gahlert
 * \date    18.05.2026
 */

#include "libklug/internal/transmission/mdu_ein/config_transfer_rate.hpp"
#include <ulf/mdu_ein.hpp>
#include "libklug/internal/exception/e_generic.hpp"
#include "libklug/internal/logging.hpp"

namespace transmission::mdu_ein {

ConfigTransferRate::ConfigTransferRate(
  std::shared_ptr<internal::IConnection> conn, mdu::TransferRate speed)
  : _conn{conn}, _speed{speed} {}

/**
 * Execute
 *
 * \throw generic_error
 *
 * \todo Maybe refactor to avoid exception abuse
 *
 */
void ConfigTransferRate::execute() {
  using std::operator""sv;
  try {
    if (!packet())
      throw except::generic_error{err::Error::nak,
                                  "Unable to set speed for decoder"sv};
    if (!special(false))
      throw except::generic_error{err::Error::nak,
                                  "Unable to set speed for device"sv};
    _result = true;
  } catch (std::exception const& e) {
    LOGE("{}", e.what());
    LOGD("Attempting to set fallback timing");
    if (!special(true))
      throw except::generic_error{err::Error::nak,
                                  "Unable to set fallback speed for device"sv};
  }
}

/// Stub
std::string ConfigTransferRate::evaluateString() {
  using std::operator""sv;
  throw except::generic_error{err::Error::unknown, "Missing Implementation"sv};
  std::unreachable();
}

bool ConfigTransferRate::evaluateBool() { return _result; }

/// Stub
uint8_t ConfigTransferRate::evaluateByte() {
  using std::operator""sv;
  throw except::generic_error{err::Error::unknown, "Missing Implementation"sv};
  std::unreachable();
}

bool ConfigTransferRate::packet() {
  Base t{
    _conn,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_config_transfer_rate_packet(_speed)),
    100u};
  t.execute();
  return t.evaluateBool();
}

bool ConfigTransferRate::special(bool fallback) {
  Base t{_conn,
         ulf::mdu_ein::special2mdu_ein(
           ulf::mdu_ein::Command::Speed,
           std::to_underlying(fallback ? mdu::TransferRate::Fallback : _speed),
           std::array<uint8_t, 16>{}),
         100u};
  t.execute();
  return t.evaluateBool();
}

} // namespace transmission::mdu_ein
