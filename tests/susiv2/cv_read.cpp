#include <ulf/susiv2.hpp>
#include <zusi/zusi.hpp>
#include "../helper.hpp"
#include "../range_matcher.hpp"
#include "f_susiv2.hpp"

using testing::_;
using testing::Ge;
using testing::InSequence;
using testing::Return;

TEST_F(TestSUSIV2, cv_read_payload) {
  uint16_t const cv{7u};

  auto const payload{ulf::susiv2::packet2frame<
    ztl::inplace_vector<uint8_t, ZUSI_MAX_PACKET_SIZE + 5uz>>(
    zusi::make_cv_read_packet(0, cv))};
  auto const expected{helper::range2span(payload)};

  {
    InSequence i;
    EXPECT_CALL(conn, _transmit(RM(expected), _)).Times(1);
    EXPECT_CALL(conn, _receive(_, _, _, _)).Times(1);
  }

  lib.susiv2().cvRead(cv);
  lib.result();
}

TEST_F(TestSUSIV2, cv_read_result_success) {
  uint16_t const cv{7u};
  uint8_t const val{145u};

  std::vector<uint8_t> r{ulf::susiv2::ack, val};
  r.push_back(zusi::crc8(r));

  ON_CALL(conn, _receive(_, Ge(r.size()), _, _))
    .WillByDefault(
      [&](uint8_t* buf, uint32_t len, int* rx_ed, uint32_t timeout) {
        assert(len >= r.size());
        std::ranges::copy(r, buf);
        *rx_ed = r.size();
        return 0;
      });

  lib.susiv2().cvRead(cv);
  auto const result{lib.result()};

  ASSERT_TRUE(std::holds_alternative<res::Cv>(result));
  ASSERT_EQ(std::get<res::Cv>(result), val);
}

TEST_F(TestSUSIV2, cv_read_transmit_error) {
  assertTransmitErrorCalls();

  lib.susiv2().cvRead(cv);
  lib.result();
}

TEST_F(TestSUSIV2, cv_read_transmit_error_result) {
  throwTransmitException();

  lib.susiv2().cvRead(cv);
  auto const result{lib.result()};

  assertTransmitReceiveErrorResult(result);
}

TEST_F(TestSUSIV2, cv_read_receive_error) {
  assertReceiveErrorCalls();

  lib.susiv2().cvRead(cv);
  lib.result();
}

TEST_F(TestSUSIV2, cv_read_receive_error_result) {
  throwReceiveException();

  lib.susiv2().cvRead(cv);
  auto const result{lib.result()};

  assertTransmitReceiveErrorResult(result);
}
