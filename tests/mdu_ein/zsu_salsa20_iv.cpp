#include "../helper.hpp"
#include "../range_matcher.hpp"
#include "f_mdu_ein.hpp"
#include "helper.hpp"

#include <cassert>

TEST_F(TestMDU_EIN, zsu_salsa20_iv_payload) {
  {
    InSequence i;
    EXPECT_CALL(
      conn,
      _write(RM(helper::mdu::packet2frame(
               mdu::make_zsu_salsa20_iv_packet(*zsu.firmwares.front().iv))),
             _))
      .Times(1);
    EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(1);
  }

  libklug_mdu_ein_zsu_salsa20_iv(libHandle, zsuHandle, fwIndex);
  libklug_job_await(libHandle);
}

TEST_F(TestMDU_EIN, zsu_salsa20_iv_result_success) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_ack);

  libklug_mdu_ein_zsu_salsa20_iv(libHandle, zsuHandle, fwIndex);
  auto const result{libklug_job_await(libHandle)};

  ASSERT_EQ(result.type, result_type::status);
  ASSERT_EQ(result.data.success, LIBKLUG_TRUE);
}

TEST_F(TestMDU_EIN, zsu_salsa20_iv_result_no_success) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_nak);

  libklug_mdu_ein_zsu_salsa20_iv(libHandle, zsuHandle, fwIndex);
  auto const result{libklug_job_await(libHandle)};

  ASSERT_EQ(result.type, result_type::status);
  ASSERT_EQ(result.data.success, LIBKLUG_FALSE);
}

TEST_F(TestMDU_EIN, zsu_salsa20_iv_write_error) {
  assertTransmitErrorCalls();

  libklug_mdu_ein_zsu_salsa20_iv(libHandle, zsuHandle, fwIndex);
  libklug_job_await(libHandle);
}

TEST_F(TestMDU_EIN, zsu_salsa20_iv_write_error_result) {
  throwTransmitException();

  libklug_mdu_ein_zsu_salsa20_iv(libHandle, zsuHandle, fwIndex);
  auto const result{libklug_job_await(libHandle)};

  assertTransmitReceiveErrorResult(result);
}

TEST_F(TestMDU_EIN, zsu_salsa20_iv_receive_error) {
  assertReceiveErrorCalls();

  libklug_mdu_ein_zsu_salsa20_iv(libHandle, zsuHandle, fwIndex);
  libklug_job_await(libHandle);
}

TEST_F(TestMDU_EIN, zsu_salsa20_iv_receive_error_result) {
  throwReceiveException();
  libklug_mdu_ein_zsu_salsa20_iv(libHandle, zsuHandle, fwIndex);
  auto const result{libklug_job_await(libHandle)};

  assertTransmitReceiveErrorResult(result);
}
