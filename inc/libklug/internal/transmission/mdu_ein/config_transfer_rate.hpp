/**
 * MDU_EIN config transfer rate transmission
 *
 * \file    inc/libklug/internal/transmission/mdu_ein/config_transfer_rate.hpp
 * \author  Jonas Gahlert
 * \date    18.05.2026
 */

#pragma once

#include <mdu/mdu.hpp>
#include "base.hpp"
#include "libklug/internal/transmission/i_transmission.hpp"

namespace transmission::mdu_ein {

/**
 * MDU_EIN config transfer rate transmission
 *
 * \note Since configuring the transfer rate requires two successive commands,
 * both will be sent
 *
 * \internal Overrides \ref ConfigTransferRate::execute to perform all
 * transmissions necessary and \ref ConfigTransferRate::evaluate to provide the
 * result
 *
 */
struct ConfigTransferRate : public ITransmission {
  ConfigTransferRate(std::shared_ptr<internal::IConnection> conn,
                     mdu::TransferRate speed);
  virtual ~ConfigTransferRate() = default;

  virtual void execute() final;

  virtual res::Result evaluate() final;
  virtual std::expected<std::string, err::Error> evaluateString() final;
  virtual std::expected<bool, err::Error> evaluateBool() final;
  virtual std::expected<uint8_t, err::Error> evaluateByte() final;

private:
  res::Result packet();
  res::Result special(bool fallback);

  std::shared_ptr<internal::IConnection> _conn; ///< Connection
  mdu::TransferRate _speed;                     ///< Speed
  bool _result;                                 ///< Result
};

} // namespace transmission::mdu_ein
