#pragma once

#include <zsu/zsu.hpp>
#include "../f_base.hpp"

struct TestZsuUpdate : public TestBase {
  TestZsuUpdate() {}

  zsu::File* file{};
};
