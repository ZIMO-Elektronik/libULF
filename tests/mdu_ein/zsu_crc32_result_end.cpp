#include <mdu/mdu.hpp>
#include "../helper.hpp"
#include "../range_matcher.hpp"
#include "f_mdu_ein.hpp"
#include "helper.hpp"

TEST_F(TestMDU_EIN, zsu_crc32_result_exit_payload) {
  {
    InSequence i;
    EXPECT_CALL(conn,
                _write(RM(helper::mdu::packet2frame(
                         mdu::make_zsu_crc32_result_exit_packet())),
                       _))
      .Times(1);
    EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(1);
  }

  bool r{};
  libklug_mdu_ein_zsu_crc32_result_exit(libHandle, &r);
}

TEST_F(TestMDU_EIN, zsu_crc32_result_exit_result_success) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_ack);

  bool r{};
  ASSERT_EQ(libklug_mdu_ein_zsu_crc32_result_exit(libHandle, &r), LIBKLUG_OK);
  ASSERT_TRUE(r);
}

TEST_F(TestMDU_EIN, zsu_crc32_result_exit_result_no_success) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_nak);

  bool r{};
  ASSERT_EQ(libklug_mdu_ein_zsu_crc32_result_exit(libHandle, &r), LIBKLUG_OK);
  ASSERT_FALSE(r);
}

TEST_F(TestMDU_EIN, zsu_crc32_result_exit_write_error) {
  assertTransmitErrorCalls();

  bool r{};
  libklug_mdu_ein_zsu_crc32_result_exit(libHandle, &r);
}

TEST_F(TestMDU_EIN, zsu_crc32_result_exit_write_error_result) {
  throwTransmitException();

  bool r{};
  ASSERT_NE(libklug_mdu_ein_zsu_crc32_result_exit(libHandle, &r), LIBKLUG_OK);
}

TEST_F(TestMDU_EIN, zsu_crc32_result_exit_receive_error) {
  assertReceiveErrorCalls();

  bool r{};
  libklug_mdu_ein_zsu_crc32_result_exit(libHandle, &r);
}

TEST_F(TestMDU_EIN, zsu_crc32_result_exit_receive_error_result) {
  throwReceiveException();

  bool r{};
  ASSERT_NE(libklug_mdu_ein_zsu_crc32_result_exit(libHandle, &r), LIBKLUG_OK);
}
