#pragma once

#include <libklug/internal/managed_iterator.hpp>
#include "../f_base.hpp"
#include "paths.hpp"

struct TestZsu : public TestBase {
  TestZsu() {}
  virtual ~TestZsu() override {}

  zsu::File* file{new zsu::File(zsu::read(paths::zsu_path))};
  zsu_handle fileHandle{reinterpret_cast<zsu_handle>(file)};

  internal::ManagedIterator<decltype(file->firmwares)> fwIt{file->firmwares};
  firmware_iterator_handle fwItHandle{
    reinterpret_cast<firmware_iterator_handle>(&fwIt)};
};
