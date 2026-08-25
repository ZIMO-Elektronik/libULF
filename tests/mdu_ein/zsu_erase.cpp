#include "../helper.hpp"
#include "../range_matcher.hpp"
#include "f_mdu_ein.hpp"
#include "helper.hpp"
#include "libklug/libklug.h"

TEST_F(TestMDU_EIN, zsu_erase_payload) {
  {
    InSequence i;
    EXPECT_CALL(conn,
                _write(RM(helper::mdu::packet2frame(mdu::make_zsu_erase_packet(
                         0, zsu.firmwares.front().bin.size() - 1u))),
                       _))
      .Times(1);
    EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(1);
  }

  int r{};
  libklug_mdu_ein_zsu_erase(libHandle, zsuHandle, fwIndex, &r);
}

TEST_F(TestMDU_EIN, zsu_erase_result_success) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_ack);

  int r{};
  ASSERT_EQ(libklug_mdu_ein_zsu_erase(libHandle, zsuHandle, fwIndex, &r),
            libklug_error::ok);
  ASSERT_TRUE(r);
}

TEST_F(TestMDU_EIN, zsu_erase_result_no_success) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_nak);

  int r{};
  ASSERT_EQ(libklug_mdu_ein_zsu_erase(libHandle, zsuHandle, fwIndex, &r),
            libklug_error::ok);
  ASSERT_FALSE(r);
}

TEST_F(TestMDU_EIN, zsu_erase_write_error) {
  assertTransmitErrorCalls();

  int r{};
  libklug_mdu_ein_zsu_erase(libHandle, zsuHandle, fwIndex, &r);
}

TEST_F(TestMDU_EIN, zsu_erase_write_error_result) {
  throwTransmitException();

  int r{};
  ASSERT_NE(libklug_mdu_ein_zsu_erase(libHandle, zsuHandle, fwIndex, &r),
            libklug_error::ok);
}

TEST_F(TestMDU_EIN, zsu_erase_receive_error) {
  assertReceiveErrorCalls();

  int r{};
  libklug_mdu_ein_zsu_erase(libHandle, zsuHandle, fwIndex, &r);
}

TEST_F(TestMDU_EIN, zsu_erase_receive_error_result) {
  throwReceiveException();

  int r{};
  ASSERT_NE(libklug_mdu_ein_zsu_erase(libHandle, zsuHandle, fwIndex, &r),
            libklug_error::ok);
}
