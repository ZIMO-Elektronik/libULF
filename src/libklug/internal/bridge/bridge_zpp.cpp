/**
 * Internal ZPP bridge
 *
 * \file    src/libklug/internal/bridge/bridge_zpp.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "libklug/internal/bridge/bridge_zpp.hpp"
#include <algorithm>
#include <cassert>
#include "libklug/internal/log/asserter.hpp"
#include "libklug/internal/log/logger.hpp"

namespace bridge {

/**
 * Read file
 *
 * \param path  Path
 * \return zpp::File* ZPP file
 * \return nullptr Error
 */
zpp::File* ZPP::read(std::filesystem::path path) {
  LOG_INFO("Reading ZPP file at {}", path.string());
  try {
    auto file{zpp::File(zpp::read(path))};
    std::fill_n(std::back_inserter(file.flash), file.flash.size() % 256uz, 0uz);
    return new zpp::File(file);
  } catch (std::exception) {
    LOG_ERROR("Unable to read file at {}", path.string());
    return nullptr;
  }
  return nullptr;
}

/**
 * Release file
 *
 * \param file ZPP file
 */
void ZPP::release(zpp::File* file) {
  LOG_INFO("Releasing ZPP file");
  LIBKLUG_ASSERT(file) << "Attempted to release NULL";
  return delete file;
}

/**
 * Get flash block count
 *
 * \param file ZPP file
 * \return unsigned int flash block count
 */
unsigned int ZPP::blocks(zpp::File* file) {
  LIBKLUG_ASSERT(file) << "Attempted to get blocks from NULL";
  return (file->flash.size() + _blockSize - 1uz) / _blockSize;
}

/**
 * Get cv count
 *
 * \param file ZPP file
 * \return unsigned int cv cound
 */
unsigned int ZPP::cvs(zpp::File* file) {
  LIBKLUG_ASSERT(file) << "Attempted to get CVs from NULL";
  return file->cvs.size();
}

/**
 * Get flash block
 *
 * \param file  ZPP file
 * \param block Block index
 * \return AddressedBlock Flash block
 *
 * \note Will pad last block with zeros
 */
ZPP::AddressedBlock ZPP::block(zpp::File* file, unsigned int block) {
  LOG_INFO("Getting block nr. ", block + 1);

  LIBKLUG_ASSERT(file) << "Attempted to get block from NULL";
  LIBKLUG_ASSERT(block < blocks(file)) << "Block out of bounds";

  return {block * _blockSize,
          std::span<uint8_t const, 256uz>{
            file->flash.data() + block * _blockSize, _blockSize}};
}

/**
 * Get author
 *
 * \param file ZPP file
 * \return std::string_view Author
 */
std::string_view ZPP::author(zpp::File* file) {
  LIBKLUG_ASSERT(file) << "Attempted to get author from NULL";
  return file->author;
}

/**
 * Get email
 *
 * \param file ZPP file
 * \return std::string_view Email
 */
std::string_view ZPP::email(zpp::File* file) {
  LIBKLUG_ASSERT(file) << "Attempted to get email from NULL";
  return file->email;
}

} // namespace bridge
