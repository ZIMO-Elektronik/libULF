/**
 * MDU_EIN config transfer rate transmission
 *
 * \file    inc/libklug/internal/transmission/mdu_ein/config_transfer_rate.hpp
 * \author  Jonas Gahlert
 * \date    18.05.2026
 */

#include "libklug/internal/transmission/mdu_ein/config_transfer_rate.hpp"
#include <ulf/mdu_ein.hpp>
#include "libklug/internal/logging.hpp"

namespace transmission::mdu_ein {

ConfigTransferRate::ConfigTransferRate(std::shared_ptr<Connection> conn,
                                       mdu::TransferRate speed)
  : _conn{conn}, _speed{speed} {}

res::Result ConfigTransferRate::execute() {
  auto r{packet()};
  if (!std::holds_alternative<res::Status>(r) || !std::get<res::Status>(r)) {
    LOGE("Unable to configure new speed, attempting to set fallback");
    special(true);
    return r;
  }

  r = special(false);
  if (std::holds_alternative<res::Status>(r) && std::get<res::Status>(r))
    _result = true;

  return r;
}

res::Result ConfigTransferRate::evaluate() { return res::Status{_result}; }

res::Result ConfigTransferRate::packet() {
  Base t{
    _conn,
    ulf::mdu_ein::bytes2mdu_ein(mdu::make_config_transfer_rate_packet(_speed)),
    100u};
  auto r{t.execute()};
  // Check if execution resulted in an error
  if (!std::holds_alternative<res::Status>(r)) return r;
  return t.evaluate();
}

res::Result ConfigTransferRate::special(bool fallback) {
  Base t{_conn,
         ulf::mdu_ein::special2mdu_ein(
           ulf::mdu_ein::Command::Speed,
           std::to_underlying(fallback ? mdu::TransferRate::Fallback : _speed),
           std::array<uint8_t, 16>{}),
         100u};
  auto r{t.execute()};
  // Check if execution resulted in an error
  if (!std::holds_alternative<res::Status>(r)) return r;
  return t.evaluate();
}

} // namespace transmission::mdu_ein
