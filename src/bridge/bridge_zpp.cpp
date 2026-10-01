/**
 * Copyright (C) 2026 ZIMO Elektronik
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
 * Internal ZPP bridge
 *
 * \file    src/bridge/bridge_zpp.cpp
 * \author  Jonas Gahlert
 * \date    04.05.2026
 */

#include "bridge_zpp.hpp"
#include <algorithm>
#include <cassert>
#include "log/asserter.hpp"
#include "log/logger.hpp"

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
  LIBULF_ASSERT(file) << "Attempted to release NULL";
  return delete file;
}

/**
 * Get flash block count
 *
 * \param file ZPP file
 * \return unsigned int flash block count
 */
unsigned int ZPP::blocks(zpp::File* file) {
  LIBULF_ASSERT(file) << "Attempted to get blocks from NULL";
  return (file->flash.size() + _blockSize - 1uz) / _blockSize;
}

/**
 * Get cv count
 *
 * \param file ZPP file
 * \return unsigned int cv cound
 */
unsigned int ZPP::cvs(zpp::File* file) {
  LIBULF_ASSERT(file) << "Attempted to get CVs from NULL";
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

  LIBULF_ASSERT(file) << "Attempted to get block from NULL";
  LIBULF_ASSERT(block < blocks(file)) << "Block out of bounds";

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
  LIBULF_ASSERT(file) << "Attempted to get author from NULL";
  return file->author;
}

/**
 * Get email
 *
 * \param file ZPP file
 * \return std::string_view Email
 */
std::string_view ZPP::email(zpp::File* file) {
  LIBULF_ASSERT(file) << "Attempted to get email from NULL";
  return file->email;
}

} // namespace bridge
