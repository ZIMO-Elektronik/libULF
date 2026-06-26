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
    auto r{packet()};
    if (!std::holds_alternative<res::Status>(r) || !std::get<res::Status>(r))
      throw except::generic_error{err::Error::nak,
                                  "Unable to set speed for decoder"sv};
    r = special(false);
    if (!std::holds_alternative<res::Status>(r) || !std::get<res::Status>(r))
      throw except::generic_error{err::Error::nak,
                                  "Unable to set speed for device"sv};
    _result = true;
  } catch (std::exception const& e) {
    LOGE("{}", e.what());
    LOGD("Attempting to set fallback timing");
    auto r{special(true)};
    if (!std::holds_alternative<res::Status>(r) || !std::get<res::Status>(r))
      throw except::generic_error{err::Error::nak,
                                  "Unable to set fallback speed for device"sv};
  }
}

res::Result ConfigTransferRate::evaluate() { return res::Status{_result}; }

res::Result ConfigTransferRate::packet() {
  Base t{
    _conn,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_config_transfer_rate_packet(_speed)),
    100u};
  t.execute();
  return t.evaluate();
}

res::Result ConfigTransferRate::special(bool fallback) {
  Base t{_conn,
         ulf::mdu_ein::special2mdu_ein(
           ulf::mdu_ein::Command::Speed,
           std::to_underlying(fallback ? mdu::TransferRate::Fallback : _speed),
           std::array<uint8_t, 16>{}),
         100u};
  t.execute();
  return t.evaluate();
}

} // namespace transmission::mdu_ein
