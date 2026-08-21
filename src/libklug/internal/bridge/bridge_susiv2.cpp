/**
 * Internal SUSIV2 bridge
 *
 * \file    src/libklug/internal/bridge/bridge_susiv2.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/internal/bridge/bridge_susiv2.hpp"
#include <ulf/susiv2.hpp>
#include <zusi/zusi.hpp>
#include "libklug/internal/transmission/susiv2/base.hpp"
#include "libklug/internal/transmission/susiv2/cv_read.hpp"

namespace bridge {

/**
 * CTor
 *
 * \param ctx     Context
 * \param worker  Worker
 */
SUSIV2::SUSIV2(Context& ctx, Worker& worker, ZPP& zpp)
  : _ctx{ctx}, _worker{worker}, _zpp{zpp} {}

/**
 * Cv Read
 *
 * \param cv  Cv address
 * \return true   Success
 * \return false  Busy
 */
std::expected<uint8_t, err::Error> SUSIV2::cvRead(uint16_t cv) {
  transmission::susiv2::CvRead t{_ctx.connection, 2000uz, cv};
  t.execute();
  return t.evaluateByte();
}

/**
 * Cv Write
 *
 * \param cv    Cv address
 * \param value Cv value
 * \return true   Success
 * \return false  Busy
 *
 * \todo Implement
 */
std::expected<bool, err::Error> SUSIV2::cvWrite(uint16_t cv, uint8_t value) {
  assert(false);
  return std::unexpected(err::Error::unknown);
}

/**
 * ZPP erase (async)
 *
 * \return true   Success
 * \return false  Busy
 */
std::expected<bool, err::Error> SUSIV2::zppErase() {
  transmission::susiv2::Base t{
    _ctx.connection,
    ulf::susiv2::packet2frame<
      ztl::inplace_vector<uint8_t, ZUSI_MAX_PACKET_SIZE + 5uz>>(
      zusi::make_zpp_erase_packet()),
    200000u};
  t.execute();
  return t.evaluateBool();
}

/**
 * ZPP write (async)
 *
 * \param address Block address
 * \param block   Block
 * \return true   Success
 * \return false  Busy
 */
std::expected<bool, err::Error>
SUSIV2::zppWrite(uint32_t address, std::span<uint8_t const> block) {
  transmission::susiv2::Base t{
    _ctx.connection,
    ulf::susiv2::packet2frame<
      ztl::inplace_vector<uint8_t, ZUSI_MAX_PACKET_SIZE + 5uz>>(
      zusi::make_zpp_write_packet(block.size() - 1u, address, block)),
    2000u};
  t.execute();
  return t.evaluateBool();
}

/**
 * ZPP write (async) from file
 *
 * \param file  ZPP file
 * \param index Block index
 * \return true   Success
 * \return false  Busy
 */
std::expected<bool, err::Error> SUSIV2::zppWrite(zpp::File* file,
                                                 uint32_t index) {
  auto const block{_zpp.block(file, index)};
  return zppWrite(block.first, block.second);
}

/**
 * Feature request (async)
 *
 * \return true   Success
 * \return false  Busy
 */
std::expected<bool, err::Error> SUSIV2::features() {
  transmission::susiv2::Base t{
    _ctx.connection,
    ulf::susiv2::packet2frame<
      ztl::inplace_vector<uint8_t, ZUSI_MAX_PACKET_SIZE + 5uz>>(
      zusi::make_features_packet()),
    2000uz};
  t.execute();
  return t.evaluateBool();
}

/**
 * Exit (async)
 *
 * \param reboot    Decoder reboot
 * \param cv8_reset Decoder Cv8 reset
 * \return true   Success
 * \return false  Busy
 */
std::expected<bool, err::Error> SUSIV2::exit(bool reboot, bool cv8_reset) {
  transmission::susiv2::Base t{
    _ctx.connection,
    ulf::susiv2::packet2frame<
      ztl::inplace_vector<uint8_t, ZUSI_MAX_PACKET_SIZE + 5uz>>(
      zusi::make_exit_packet(0xFC | (reboot << 0u) | (cv8_reset << 1u))),
    2000uz};
  t.execute();
  return t.evaluateBool();
}

/**
 * LC DC query (async)
 *
 * \param dev_code Developer code
 * \return true   Success
 * \return false  Busy
 */
std::expected<bool, err::Error> SUSIV2::zppLcDcQuery(uint32_t dev_code) {
  transmission::susiv2::Base t{
    _ctx.connection,
    ulf::susiv2::packet2frame<
      ztl::inplace_vector<uint8_t, ZUSI_MAX_PACKET_SIZE + 5uz>>(
      zusi::make_zpp_lc_dc_query_packet(dev_code)),
    2000uz};
  t.execute();
  return t.evaluateBool();
}

/**
 * LC DC query (async)
 *
 * \param file  ZPP file
 * \return true   Success
 * \return false  Busy
 */
std::expected<bool, err::Error> SUSIV2::zppLcDcQuery(zpp::File* file) {
  return zppLcDcQuery(zusi::data2uint32(file->developer_code.data()));
}

} // namespace bridge
