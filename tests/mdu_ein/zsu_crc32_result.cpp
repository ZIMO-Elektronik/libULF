#include <mdu/mdu.hpp>
#include "../helper.hpp"
#include "../range_matcher.hpp"
#include "f_mdu_ein.hpp"
#include "helper.hpp"

TEST_F(TestMDU_EIN, zsu_crc32_result_payload) {
  {
    InSequence i;
    EXPECT_CALL(
      conn,
      _transmit(
        RM(helper::mdu::packet2frame(mdu::make_zsu_crc32_result_packet())), _))
      .Times(1);
    EXPECT_CALL(conn, _receive(_, _, _, _)).Times(1);
  }

  libklug_mdu_ein_zsu_crc32_result(libHandle);
  libklug_result(libHandle);
}

TEST_F(TestMDU_EIN, zsu_crc32_result_result_success) {
  ON_CALL(conn, _receive(_, _, _, _)).WillByDefault(helper::mdu::receive_ack);

  libklug_mdu_ein_zsu_crc32_result(libHandle);
  auto const result{libklug_result(libHandle)};

  ASSERT_EQ(result.type, result_type::status);
  ASSERT_EQ(result.data.success, LIBKLUG_TRUE);
}

TEST_F(TestMDU_EIN, zsu_crc32_result_result_no_success) {
  ON_CALL(conn, _receive(_, _, _, _)).WillByDefault(helper::mdu::receive_nak);

  libklug_mdu_ein_zsu_crc32_result(libHandle);
  auto const result{libklug_result(libHandle)};

  ASSERT_EQ(result.type, result_type::status);
  ASSERT_EQ(result.data.success, LIBKLUG_FALSE);
}

TEST_F(TestMDU_EIN, zsu_crc32_result_transmit_error) {
  assertTransmitErrorCalls(LIBUSB_ERROR_IO);

  libklug_mdu_ein_zsu_crc32_result(libHandle);
  libklug_result(libHandle);
}

TEST_F(TestMDU_EIN, zsu_crc32_result_transmit_error_result) {
  throwTransmitException();

  libklug_mdu_ein_zsu_crc32_result(libHandle);
  auto const result{libklug_result(libHandle)};

  assertTransmitReceiveErrorResult(result);
}

TEST_F(TestMDU_EIN, zsu_crc32_result_receive_error) {
  assertReceiveErrorCalls(LIBUSB_ERROR_IO);

  libklug_mdu_ein_zsu_crc32_result(libHandle);
  libklug_result(libHandle);
}

TEST_F(TestMDU_EIN, zsu_crc32_result_receive_error_result) {
  throwReceiveException();

  libklug_mdu_ein_zsu_crc32_result(libHandle);
  auto const result{libklug_result(libHandle)};

  assertTransmitReceiveErrorResult(result);
}
