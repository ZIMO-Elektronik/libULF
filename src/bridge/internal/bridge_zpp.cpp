/**
 * Internal ZPP bridge
 *
 * \file    src/bridge/internal/bridge_zpp.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "bridge/internal/bridge_zpp.hpp"
#include <cassert>

namespace bridge {

/**
 * Read file
 *
 * \param path  Path
 * \return zpp::File* ZPP file
 */
zpp::File* ZPP::read(std::filesystem::path path) {
  return new zpp::File(zpp::read(path));
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
 * \return std::span<uint8_t> flash block (unpadded)
 */
std::span<uint8_t> ZPP::block(zpp::File* file, unsigned int block) {
  assert(file);
  assert(block <= blocks(file));

  unsigned long const remaining{file->flash.size() - (block * _blockSize)};

  return std::span<uint8_t>{file->flash}.subspan(
    block * _blockSize, std::min(_blockSize, remaining));
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

}  // namespace bridge