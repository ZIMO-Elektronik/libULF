/**
 * MDU_EIN cv read transmission
 *
 * \file    inc/libklug/internal/transmission/mdu_ein/cv_read.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include "base.hpp"
#include "libklug/internal/transmission/i_transmission.hpp"

namespace transmission::mdu_ein {

/**
 * MDU_EIN cv read transmission
 *
 * \note Since only single bits can be read using MDU, this encapsulates 8
 * consecutive transmissions
 *
 * \internal Overrides \ref CvRead::execute to perform all transmissions
 * necessary and \ref CvRead::evaluate to provide the resulting Cv
 *
 */
struct CvRead : public ITransmission {
  CvRead(std::shared_ptr<internal::IConnection> conn, uint16_t cv);
  virtual ~CvRead() = default;

  virtual void execute() final;

  virtual res::Result evaluate() final;

private:
  std::shared_ptr<internal::IConnection> _conn; ///< Connection
  uint16_t _cv;                                 ///< Cv to read
  uint8_t _value;                               ///< Result
};

} // namespace transmission::mdu_ein
