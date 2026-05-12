#include "../range_matcher.hpp"
#include "f_mdu_ein.hpp"
#include "helper.hpp"

TEST_F(TestMDU_EIN, zpp_update_payload) {
  {
    InSequence i;
    EXPECT_CALL(
      conn,
      _transmit(
        RM(ulf::mdu_ein::bytes2mdu_ein(mdu::make_zpp_update_packet(
          0uz, std::span<uint8_t const, 256uz>{zpp.flash.data(), 256uz}))),
        _));
    EXPECT_CALL(conn, _receive(_, _, _, _));
  }

  libklug_mdu_ein_zpp_update(libHandle, zppHandle, 0uz);
  libklug_result(libHandle);
}

TEST_F(TestMDU_EIN, zpp_update_result_success) {
  ON_CALL(conn, _receive(_, _, _, _)).WillByDefault(helper::mdu::receive_ack);

  libklug_mdu_ein_zpp_update(libHandle, zppHandle, 0uz);
  auto const result{libklug_result(libHandle)};

  ASSERT_EQ(result.type, result_type::status);
  ASSERT_EQ(result.data.success, LIBKLUG_TRUE);
}

TEST_F(TestMDU_EIN, zpp_update_result_no_success) {
  ON_CALL(conn, _receive(_, _, _, _)).WillByDefault(helper::mdu::receive_nak);

  libklug_mdu_ein_zpp_update(libHandle, zppHandle, 0uz);
  auto const result{libklug_result(libHandle)};

  ASSERT_EQ(result.type, result_type::status);
  ASSERT_EQ(result.data.success, LIBKLUG_FALSE);
}

TEST_F(TestMDU_EIN, zpp_update_transmit_error) {
  assertTransmitErrorCalls();

  libklug_mdu_ein_zpp_update(libHandle, zppHandle, 0uz);
  libklug_result(libHandle);
}

TEST_F(TestMDU_EIN, zpp_update_transmit_error_result) {
  int error = LIBUSB_ERROR_IO;
  ON_CALL(conn, _transmit(_, _)).WillByDefault(Return(error));

  libklug_mdu_ein_zpp_update(libHandle, zppHandle, 0uz);
  auto const result{libklug_result(libHandle)};

  assertTransmitReceiveErrorResult(result, error);
}

TEST_F(TestMDU_EIN, zpp_update_receive_error) {
  assertReceiveErrorCalls();

  libklug_mdu_ein_zpp_update(libHandle, zppHandle, 0uz);
  libklug_result(libHandle);
}

TEST_F(TestMDU_EIN, zpp_update_receive_error_result) {
  int error = LIBUSB_ERROR_IO;
  ON_CALL(conn, _receive(_, _, _, _)).WillByDefault(Return(error));

  libklug_mdu_ein_zpp_update(libHandle, zppHandle, 0uz);
  auto const result{libklug_result(libHandle)};

  assertTransmitReceiveErrorResult(result, error);
}
