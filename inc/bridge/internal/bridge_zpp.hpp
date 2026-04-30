#pragma once

#include <zpp/zpp.hpp>

namespace bridge {

struct ZPP {
  zpp::File* read(std::filesystem::path path);
  void release(zpp::File* file);

  unsigned int blocks(zpp::File* file);
  unsigned int cvs(zpp::File* file);
  
  std::span<uint8_t> block(zpp::File* file, unsigned int block); 

  std::string_view author(zpp::File* file);
  std::string_view email(zpp::File* file);

private: 
  unsigned long const _blockSize{256uz};
};

}  // namespace bridge