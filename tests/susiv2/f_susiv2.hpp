#pragma once

#include "../f_base.hpp"
#include "paths.hpp"

struct TestSUSIV2 : public TestBase {
  zpp::File zpp{zpp::read(paths::zpp_path)};
  zpp_handle zppHandle{reinterpret_cast<zpp_handle>(&zpp)};
};
