#include "bridge/internal/bridge_zpp.hpp"
#include <cassert>

namespace bridge {

zpp::File* read(std::filesystem::path path) {
  return new zpp::File(zpp::read(path));
}

void release(zpp::File* file) {
  assert(file);
  return delete file;
}

unsigned int ZPP::blocks(zpp::File* file) {
  return (file->flash.size() + _blockSize - 1uz) / _blockSize;
}

unsigned int ZPP::cvs(zpp::File* file) { return file->cvs.size(); }

std::span<uint8_t> ZPP::block(zpp::File* file, unsigned int block) {
  assert(file);
  assert(block <= blocks(file));

  unsigned long const remaining{file->flash.size() - (block * _blockSize)};

  return std::span<uint8_t>{file->flash}.subspan(
    block * _blockSize, std::min(_blockSize, remaining));
}

std::string_view ZPP::author(zpp::File* file) {
  assert(file);
  return file->author;
}

std::string_view ZPP::email(zpp::File* file) {
  assert(file);
  return file->email;
}

}  // namespace bridge