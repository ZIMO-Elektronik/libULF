#include <ranges>
#include "../helper.hpp"
#include "../range_matcher.hpp"
#include "f_mdu_ein.hpp"
#include "helper.hpp"

TEST_F(TestMDU_EIN, zsu_update_payload) {
  auto const blocks{libklug_zsu_get_firmware_block_count(zsuHandle, fwIndex)};

  for (unsigned int idx{0uz}; idx < blocks; idx++) {
    {
      InSequence i;
      EXPECT_CALL(
        conn,
        _write(RM(helper::mdu::packet2frame(mdu::make_zsu_update_packet(
                 idx * 64uz,
                 std::span<uint8_t const, 64uz>{
                   std::span<uint8_t const>(zsu.firmwares.at(fwIndex).bin)
                     .subspan(idx * 64uz, 64uz)}))),
               _))
        .Times(1);
      EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(1);
    }

    libklug_bool r{};
    libklug_mdu_ein_zsu_update(libHandle, zsuHandle, fwIndex, idx, &r);
  }
}

TEST_F(TestMDU_EIN, zsu_update_result_success) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_ack);

  libklug_bool r{};
  ASSERT_EQ(libklug_mdu_ein_zsu_update(libHandle, zsuHandle, fwIndex, 0uz, &r),
            LIBKLUG_OK);
  ASSERT_TRUE(r);
}

TEST_F(TestMDU_EIN, zsu_update_result_no_success) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_nak);

  libklug_bool r{};
  ASSERT_EQ(libklug_mdu_ein_zsu_update(libHandle, zsuHandle, fwIndex, 0uz, &r),
            LIBKLUG_OK);
  ASSERT_FALSE(r);
}

TEST_F(TestMDU_EIN, zsu_update_write_error) {
  assertTransmitErrorCalls();

  libklug_bool r{};
  libklug_mdu_ein_zsu_update(libHandle, zsuHandle, fwIndex, 0uz, &r);
}

TEST_F(TestMDU_EIN, zsu_update_write_error_result) {
  throwTransmitException();

  libklug_bool r{};
  ASSERT_NE(libklug_mdu_ein_zsu_update(libHandle, zsuHandle, fwIndex, 0uz, &r),
            LIBKLUG_OK);
}

TEST_F(TestMDU_EIN, zsu_update_receive_error) {
  assertReceiveErrorCalls();

  libklug_bool r{};
  libklug_mdu_ein_zsu_update(libHandle, zsuHandle, fwIndex, 0uz, &r);
}

TEST_F(TestMDU_EIN, zsu_update_receive_error_result) {
  throwReceiveException();

  libklug_bool r{};
  ASSERT_NE(libklug_mdu_ein_zsu_update(libHandle, zsuHandle, fwIndex, 0uz, &r),
            LIBKLUG_OK);
}
