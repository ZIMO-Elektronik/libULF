#pragma once

#include <zpp/zpp.hpp>
#include "../f_base.hpp"
#include "paths.hpp"

struct TestZppLoad : public TestBase {
  TestZppLoad() { file = lib.zpp().read(paths::zpp_path); }
  virtual ~TestZppLoad() { lib.zpp().release(file); }

  zpp::File* file{};
};
