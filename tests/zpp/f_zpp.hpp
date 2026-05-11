#pragma once

#include <zpp/zpp.hpp>
#include "../f_base.hpp"
#include "paths.hpp"

struct TestZpp : public TestBase {
  TestZpp() { file = lib.zpp().read(paths::zpp_path); }
  virtual ~TestZpp() override { lib.zpp().release(file); }

  zpp::File* file{};
};
