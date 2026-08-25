/**
 * Internal SUSIV2 bridge
 *
 * \file    src/libklug/internal/bridge/bridge_susiv2.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/internal/bridge/bridge_susiv2.hpp"
#include <ulf/susiv2.hpp>
#include <utility>
#include <zusi/zusi.hpp>
#include "libklug/internal/exception/e_generic.hpp"
#include "libklug/internal/transmission/susiv2/base.hpp"
#include "libklug/internal/transmission/susiv2/cv_read.hpp"

namespace bridge {

/**
 * CTor
 *
 * \param ctx     Context
 */
SUSIV2::SUSIV2(Context& ctx, ZPP& zpp) : _ctx{ctx}, _zpp{zpp} {}

/**
 * Cv Read
 *
 * \note
 * Will throw an exception on error
 *
 * \param cv  Cv address
 *
 * \return uint8_t Response
 */
uint8_t SUSIV2::cvRead(uint16_t cv) {
  transmission::susiv2::CvRead t{_ctx.connection, 2000uz, cv};
  t.execute();
  return t.evaluateByte();
}

/**
 * Cv Write
 *
 * \note
 * Will throw an exception on error
 *
 * \param cv    Cv address
 * \param value Cv value
 *
 * \return bool Response
 *
 * \todo Implement
 */
bool SUSIV2::cvWrite(uint16_t cv, uint8_t value) {
  using std::operator""sv;
  throw except::generic_error{err::Error::unknown, "Missing Implementation"sv};
  std::unreachable();
}

/**
 * ZPP erase
 *
 * \note
 * Will throw an exception on error
 *
 * \return bool Response
 */
bool SUSIV2::zppErase() {
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
 * ZPP write
 *
 * \note
 * Will throw an exception on error
 *
 * \param address Block address
 * \param block   Block
 *
 * \return bool Response
 */
bool SUSIV2::zppWrite(uint32_t address, std::span<uint8_t const> block) {
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
 * ZPP write from file
 *
 * \note
 * Will throw an exception on error
 *
 * \param file  ZPP file
 * \param index Block index
 *
 * \return bool Response
 */
bool SUSIV2::zppWrite(zpp::File* file, uint32_t index) {
  auto const block{_zpp.block(file, index)};
  return zppWrite(block.first, block.second);
}

/**
 * Feature request
 *
 * \note
 * Will throw an exception on error
 *
 * \return bool Response
 */
bool SUSIV2::features() {
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
 * Exit
 *
 * \note
 * Will throw an exception on error
 *
 * \param reboot    Decoder reboot
 * \param cv8_reset Decoder Cv8 reset
 *
 * \return bool Response
 */
bool SUSIV2::exit(bool reboot, bool cv8_reset) {
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
 * LC DC query
 *
 * \note
 * Will throw an exception on error
 *
 * \param dev_code Developer code
 *
 * \return bool Response
 */
bool SUSIV2::zppLcDcQuery(uint32_t dev_code) {
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
 * LC DC query
 *
 * \note
 * Will throw an exception on error
 *
 * \param file  ZPP file
 *
 * \return bool Response
 */
bool SUSIV2::zppLcDcQuery(zpp::File* file) {
  return zppLcDcQuery(zusi::data2uint32(file->developer_code.data()));
}

} // namespace bridge
