#pragma once

#include "base.hpp"
#include "internal/transmission/i_transmission.hpp"

namespace transmission::mdu_ein {

struct CvRead : public ITransmission {
  CvRead(Connection& conn, uint16_t cv);
  virtual ~CvRead() = default;

  virtual int execute() final;

  virtual result_t evaluate() final;

private:
  Connection& _conn;
  uint16_t _cv;
  uint8_t _value;
};

}  // namespace transmission::mdu_ein