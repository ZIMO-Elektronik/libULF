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

namespace bridge {

/**
 * Read file
 *
 * \param path  Path
 * \return zpp::File* ZPP file
 * \return nullptr Error
 */
zpp::File* ZPP::read(std::filesystem::path path) {
  try {
    auto file{zpp::File(zpp::read(path))};
    std::fill_n(std::back_inserter(file.flash), file.flash.size() % 256uz, 0uz);
    return new zpp::File(file);
  } catch (std::exception) { return nullptr; }
}

/**
 * Release file
 *
 * \param file ZPP file
 */
void ZPP::release(zpp::File* file) {
  assert(file);
  return delete file;
}

/**
 * Get flash block count
 *
 * \param file ZPP file
 * \return unsigned int flash block count
 */
unsigned int ZPP::blocks(zpp::File* file) {
  return (file->flash.size() + _blockSize - 1uz) / _blockSize;
}

/**
 * Get cv count
 *
 * \param file ZPP file
 * \return unsigned int cv cound
 */
unsigned int ZPP::cvs(zpp::File* file) { return file->cvs.size(); }

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
  assert(file);
  assert(block < blocks(file));

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
  assert(file);
  return file->author;
}

/**
 * Get email
 *
 * \param file ZPP file
 * \return std::string_view Email
 */
std::string_view ZPP::email(zpp::File* file) {
  assert(file);
  return file->email;
}

} // namespace bridge
