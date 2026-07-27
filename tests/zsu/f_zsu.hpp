#pragma once

#include "../f_base.hpp"
#include "paths.hpp"

struct TestZsu : public TestBase {
  TestZsu() {}
  virtual ~TestZsu() override {}

  zsu::File* file{new zsu::File(zsu::read(paths::zsu_path))};
  zsu_handle fileHandle{reinterpret_cast<zsu_handle>(file)};

  size_t fwIndex{};
};
