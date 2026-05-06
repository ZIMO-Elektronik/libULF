/**
 * SUSIV2 Cv read
 *
 * \file    inc/libklug/internal/transmission/susiv2/cv_read.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include "base.hpp"

namespace transmission::susiv2 {

struct CvRead : public Base {
  CvRead(Connection& conn, size_t timeout, uint16_t cv);

  virtual ~CvRead() = default;

  virtual res::Result evaluate() override;
};

} // namespace transmission::susiv2
