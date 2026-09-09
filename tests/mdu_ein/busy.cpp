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

  bool r{};
  libklug_mdu_ein_busy(libHandle, &r);
}

TEST_F(TestMDU_EIN, busy_result_not_busy) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_ack);

  bool r{};
  libklug_mdu_ein_busy(libHandle, &r);
}

TEST_F(TestMDU_EIN, busy_result_busy) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_nak);

  bool r{};
  ASSERT_EQ(libklug_mdu_ein_busy(libHandle, &r), LIBKLUG_OK);
  ASSERT_FALSE(r);
}

TEST_F(TestMDU_EIN, busy_write_error) {
  assertTransmitErrorCalls();

  bool r{};
  libklug_mdu_ein_busy(libHandle, &r);
}

TEST_F(TestMDU_EIN, busy_write_error_result) {
  throwTransmitException();

  bool r{};
  ASSERT_NE(libklug_mdu_ein_busy(libHandle, &r), LIBKLUG_OK);
}

TEST_F(TestMDU_EIN, busy_receive_error) {
  assertReceiveErrorCalls();

  bool r{};
  libklug_mdu_ein_busy(libHandle, &r);
}

TEST_F(TestMDU_EIN, busy_receive_error_result) {
  throwReceiveException();

  bool r{};
  ASSERT_NE(libklug_mdu_ein_busy(libHandle, &r), LIBKLUG_OK);
}
