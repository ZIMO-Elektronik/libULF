/**
 * Copyright (C) 2026 [ZIMO Elektronik]
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

#pragma once

#include <mdu/mdu.hpp>
#include "base.hpp"
#include "transmission/i_transmission.hpp"

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

  virtual std::string evaluateString() final;
  virtual bool evaluateBool() final;
  virtual int evaluateValue() final;

private:
  bool packet();
  bool special(bool fallback);

  std::shared_ptr<internal::IConnection> _conn; ///< Connection
  mdu::TransferRate _speed;                     ///< Speed
  bool _result;                                 ///< Result
};

} // namespace transmission::mdu_ein
