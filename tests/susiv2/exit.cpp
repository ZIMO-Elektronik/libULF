#include <ulf/susiv2.hpp>
#include <zusi/zusi.hpp>
#include "../helper.hpp"
#include "../range_matcher.hpp"
#include "f_susiv2.hpp"
#include "helper.hpp"

using testing::_;
using testing::Ge;
using testing::InSequence;
using testing::Return;

TEST_F(TestSUSIV2, exit_payload) {
  uint8_t const option{0xFFu};

  auto const payload{ulf::susiv2::packet2frame<
    ztl::inplace_vector<uint8_t, ZUSI_MAX_PACKET_SIZE + 5uz>>(
    zusi::make_exit_packet(option))};
  auto const expected{helper::range2span(payload)};

  {
    InSequence i;
    EXPECT_CALL(conn, _write(RM(expected), _)).Times(1);
    EXPECT_CALL(conn, _read_all(_, _, _, _)).Times(1);
  }

  int r{};
  libklug_susiv2_exit(libHandle, LIBKLUG_TRUE, LIBKLUG_TRUE, &r);
}

TEST_F(TestSUSIV2, exit_result_success) {
  std::vector<uint8_t> expected{ulf::susiv2::ack};
  expected.push_back(zusi::crc8(expected));

  ON_CALL(conn, _read_all(_, Ge(expected.size()), _, _))
    .WillByDefault(helper::susiv2::receive_ack);

  int r{};
  ASSERT_EQ(libklug_susiv2_exit(libHandle, LIBKLUG_TRUE, LIBKLUG_TRUE, &r),
            libklug_error::ok);
  ASSERT_TRUE(r);
}

TEST_F(TestSUSIV2, exit_write_error) {
  assertTransmitErrorCalls<true>();

  int r{};
  libklug_susiv2_exit(libHandle, LIBKLUG_TRUE, LIBKLUG_TRUE, &r);
}

TEST_F(TestSUSIV2, exit_write_error_result) {
  throwTransmitException();

  int r{};
  ASSERT_NE(libklug_susiv2_exit(libHandle, LIBKLUG_TRUE, LIBKLUG_TRUE, &r),
            libklug_error::ok);
}

TEST_F(TestSUSIV2, exit_receive_error) {
  assertReceiveErrorCalls<true>();

  int r{};
  libklug_susiv2_exit(libHandle, LIBKLUG_TRUE, LIBKLUG_TRUE, &r);
}

TEST_F(TestSUSIV2, exit_receive_error_result) {
  throwReceiveException();

  int r{};
  ASSERT_NE(libklug_susiv2_exit(libHandle, LIBKLUG_TRUE, LIBKLUG_TRUE, &r),
            libklug_error::ok);
}
