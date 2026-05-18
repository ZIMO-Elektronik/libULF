#pragma once

#include <ulf/mdu_ein.hpp>
#include "../f_base.hpp"
#include "libklug/internal/managed_iterator.hpp"
#include "paths.hpp"

using testing::_;
using testing::Ge;

struct TestMDU_EIN : public TestBase {
  TestMDU_EIN() {
    auto const r{ulf::mdu_ein::response2mdu_ein(true, true)};

    ON_CALL(conn, _receive(_, Ge(r.size()), _, _))
      .WillByDefault(
        [=](uint8_t* buf, uint32_t len, int* rx_ed, uint32_t timeout) {
          assert(len >= r.size());
          std::ranges::copy(r, buf);
          *rx_ed = r.size();
          return 0;
        });
  }

  zpp::File zpp{zpp::read(paths::zpp_path)};
  zpp_handle zppHandle{reinterpret_cast<zpp_handle>(&zpp)};

  zsu::File zsu{zsu::read(paths::zsu_path)};
  zsu_handle zsuHandle{reinterpret_cast<zsu_handle>(&zsu)};

  internal::ManagedIterator<decltype(zsu.firmwares)> fwIt{zsu.firmwares};
  firmware_iterator_handle fwItHandle{
    reinterpret_cast<firmware_iterator_handle>(&fwIt)};
};
