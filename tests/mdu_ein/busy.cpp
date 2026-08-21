#include "../range_matcher.hpp"
#include "f_mdu_ein.hpp"
#include "helper.hpp"

using testing::_;
using testing::InSequence;
using testing::Return;

TEST_F(TestMDU_EIN, busy_payload) {
  {
    InSequence i;
    EXPECT_CALL(
      conn, _write(RM(helper::mdu::packet2frame(mdu::make_busy_packet())), _))
      .Times(1);
    EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(1);
  }

  int r{};
  libklug_mdu_ein_busy(libHandle, &r);
}

TEST_F(TestMDU_EIN, busy_result_not_busy) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_ack);

  int r{};
  libklug_mdu_ein_busy(libHandle, &r);
}

TEST_F(TestMDU_EIN, busy_result_busy) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_nak);

  int r{};
  ASSERT_EQ(libklug_mdu_ein_busy(libHandle, &r), libklug_error::ok);
  ASSERT_FALSE(r);
}

TEST_F(TestMDU_EIN, busy_write_error) {
  assertTransmitErrorCalls();

  int r{};
  libklug_mdu_ein_busy(libHandle, &r);
}

TEST_F(TestMDU_EIN, busy_write_error_result) {
  throwTransmitException();

  int r{};
  ASSERT_NE(libklug_mdu_ein_busy(libHandle, &r), libklug_error::ok);
}

TEST_F(TestMDU_EIN, busy_receive_error) {
  assertReceiveErrorCalls();

  int r{};
  libklug_mdu_ein_busy(libHandle, &r);
}

TEST_F(TestMDU_EIN, busy_receive_error_result) {
  throwReceiveException();

  int r{};
  ASSERT_NE(libklug_mdu_ein_busy(libHandle, &r), libklug_error::ok);
}
