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
      conn,
      _transmit(RM(helper::mdu::packet2frame(mdu::make_busy_packet())), _))
      .Times(1);
    EXPECT_CALL(conn, _receive(_, _, _, _)).Times(1);
  }

  libklug_mdu_ein_busy(libHandle);
  libklug_result(libHandle);
}

TEST_F(TestMDU_EIN, busy_result_not_busy) {
  ON_CALL(conn, _receive(_, _, _, _)).WillByDefault(helper::mdu::receive_ack);

  libklug_mdu_ein_busy(libHandle);
  auto const result{libklug_result(libHandle)};

  ASSERT_EQ(result.type, result_type::status);
  ASSERT_EQ(result.data.success, 1);
}

TEST_F(TestMDU_EIN, busy_result_busy) {
  ON_CALL(conn, _receive(_, _, _, _)).WillByDefault(helper::mdu::receive_nak);

  libklug_mdu_ein_busy(libHandle);
  auto const result{libklug_result(libHandle)};

  ASSERT_EQ(result.type, result_type::status);
  ASSERT_EQ(result.data.success, 0);
}

TEST_F(TestMDU_EIN, busy_transmit_error) {
  assertTransmitErrorCalls();

  libklug_mdu_ein_busy(libHandle);
  libklug_result(libHandle);
}

TEST_F(TestMDU_EIN, busy_transmit_error_result) {
  throwTransmitException();

  libklug_mdu_ein_busy(libHandle);
  auto const result{libklug_result(libHandle)};

  assertTransmitReceiveErrorResult(result);
}

TEST_F(TestMDU_EIN, busy_receive_error) {
  assertReceiveErrorCalls();

  libklug_mdu_ein_busy(libHandle);
  libklug_result(libHandle);
}

TEST_F(TestMDU_EIN, busy_receive_error_result) {
  throwReceiveException();

  libklug_mdu_ein_busy(libHandle);
  auto const result{libklug_result(libHandle)};

  assertTransmitReceiveErrorResult(result);
}
