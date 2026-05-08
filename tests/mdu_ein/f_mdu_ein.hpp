#pragma once

#include "../f_base.hpp"

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
};
