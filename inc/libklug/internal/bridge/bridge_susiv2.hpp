/**
 * Internal SUSIV2 bridge
 *
 * \file    inc/libklug/internal/bridge/bridge_susiv2.hpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#pragma once

#include <expected>
#include <zpp/zpp.hpp>
#include "bridge_context.hpp"
#include "bridge_zpp.hpp"

namespace bridge {

/**
 * Bridge for the SUSIV2 protocol
 *
 */
struct SUSIV2 {
  SUSIV2(Context& ctx, ZPP& zpp);

  std::expected<uint8_t, err::Error> cvRead(uint16_t cv);
  std::expected<bool, err::Error> cvWrite(uint16_t cv, uint8_t value);
  std::expected<bool, err::Error> zppErase();
  std::expected<bool, err::Error> zppWrite(uint32_t address,
                                           std::span<uint8_t const> block);
  std::expected<bool, err::Error> zppWrite(zpp::File* file, uint32_t index);
  std::expected<bool, err::Error> features();
  std::expected<bool, err::Error> exit(bool reboot, bool cv8_reset);
  std::expected<bool, err::Error> zppLcDcQuery(uint32_t dev_code);
  std::expected<bool, err::Error> zppLcDcQuery(zpp::File* file);

private:
  Context& _ctx; ///< Bridge context
  ZPP& _zpp;     ///< ZPP bridge
};

} // namespace bridge
