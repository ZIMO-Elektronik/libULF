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

  auto const payload{ulf::susiv2::packet2frame(zusi::make_exit_packet(option))};
  auto const expected{helper::range2span(payload)};

  {
    InSequence i;
    EXPECT_CALL(conn, _write(RM(expected), _)).Times(1);
    EXPECT_CALL(conn, _read_all(_, _, _, _)).Times(1);
  }

  bool r{};
  libulf_susiv2_exit(libHandle, true, true, &r);
}

TEST_F(TestSUSIV2, exit_result_success) {
  std::vector<uint8_t> expected{ulf::susiv2::ack};
  expected.push_back(zusi::crc8(expected));

  ON_CALL(conn, _read_all(_, Ge(expected.size()), _, _))
    .WillByDefault(helper::susiv2::receive_ack);

  bool r{};
  ASSERT_EQ(libulf_susiv2_exit(libHandle, true, true, &r), LIBULF_OK);
  ASSERT_TRUE(r);
}

TEST_F(TestSUSIV2, exit_write_error) {
  assertTransmitErrorCalls<true>();

  bool r{};
  libulf_susiv2_exit(libHandle, true, true, &r);
}

TEST_F(TestSUSIV2, exit_write_error_result) {
  throwTransmitException();

  bool r{};
  ASSERT_NE(libulf_susiv2_exit(libHandle, true, true, &r), LIBULF_OK);
}

TEST_F(TestSUSIV2, exit_receive_error) {
  assertReceiveErrorCalls<true>();

  bool r{};
  libulf_susiv2_exit(libHandle, true, true, &r);
}

TEST_F(TestSUSIV2, exit_receive_error_result) {
  throwReceiveException();

  bool r{};
  ASSERT_NE(libulf_susiv2_exit(libHandle, true, true, &r), LIBULF_OK);
}
