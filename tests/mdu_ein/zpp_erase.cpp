#include "../range_matcher.hpp"
#include "f_mdu_ein.hpp"
#include "helper.hpp"

TEST_F(TestMDU_EIN, zpp_erase_payload) {
  {
    InSequence i;
    EXPECT_CALL(
      conn,
      _write(RM(ulf::mdu_ein::bytes2mdu_ein(
               mdu::make_zpp_erase_packet(0uz, zpp.flash.size() - 1uz))),
             _));
    EXPECT_CALL(conn, _read_until(_, _, _, _, _));
  }

  libklug_mdu_ein_zpp_erase(libHandle, zppHandle);
  libklug_job_await(libHandle);
}

TEST_F(TestMDU_EIN, zpp_erase_result_success) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_ack);

  libklug_mdu_ein_zpp_erase(libHandle, zppHandle);
  auto const result{libklug_job_await(libHandle)};

  ASSERT_EQ(result.type, result_type::status);
  ASSERT_EQ(result.data.success, LIBKLUG_TRUE);
}

TEST_F(TestMDU_EIN, zpp_erase_result_no_success) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_nak);

  libklug_mdu_ein_zpp_erase(libHandle, zppHandle);
  auto const result{libklug_job_await(libHandle)};

  ASSERT_EQ(result.type, result_type::status);
  ASSERT_EQ(result.data.success, LIBKLUG_FALSE);
}

TEST_F(TestMDU_EIN, zpp_erase_write_error) {
  assertTransmitErrorCalls();

  libklug_mdu_ein_zpp_erase(libHandle, zppHandle);
  libklug_job_await(libHandle);
}

TEST_F(TestMDU_EIN, zpp_erase_write_error_result) {
  throwTransmitException();

  libklug_mdu_ein_zpp_erase(libHandle, zppHandle);
  auto const result{libklug_job_await(libHandle)};

  assertTransmitReceiveErrorResult(result);
}

TEST_F(TestMDU_EIN, zpp_erase_receive_error) {
  assertReceiveErrorCalls();

  libklug_mdu_ein_zpp_erase(libHandle, zppHandle);
  libklug_job_await(libHandle);
}

TEST_F(TestMDU_EIN, zpp_erase_receive_error_result) {
  throwReceiveException();

  libklug_mdu_ein_zpp_erase(libHandle, zppHandle);
  auto const result{libklug_job_await(libHandle)};

  assertTransmitReceiveErrorResult(result);
}
