#include <ranges>
#include "../helper.hpp"
#include "../range_matcher.hpp"
#include "f_mdu_ein.hpp"
#include "helper.hpp"

TEST_F(TestMDU_EIN, zsu_update_payload) {
  auto const blocks{libulf_zsu_get_firmware_block_count(zsuHandle, fwIndex)};

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

    bool r{};
    libulf_mdu_ein_zsu_update(libHandle, zsuHandle, fwIndex, idx, &r);
  }
}

TEST_F(TestMDU_EIN, zsu_update_result_success) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_ack);

  bool r{};
  ASSERT_EQ(libulf_mdu_ein_zsu_update(libHandle, zsuHandle, fwIndex, 0uz, &r),
            LIBULF_OK);
  ASSERT_TRUE(r);
}

TEST_F(TestMDU_EIN, zsu_update_result_no_success) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_nak);

  bool r{};
  ASSERT_EQ(libulf_mdu_ein_zsu_update(libHandle, zsuHandle, fwIndex, 0uz, &r),
            LIBULF_OK);
  ASSERT_FALSE(r);
}

TEST_F(TestMDU_EIN, zsu_update_write_error) {
  assertTransmitErrorCalls();

  bool r{};
  libulf_mdu_ein_zsu_update(libHandle, zsuHandle, fwIndex, 0uz, &r);
}

TEST_F(TestMDU_EIN, zsu_update_write_error_result) {
  throwTransmitException();

  bool r{};
  ASSERT_NE(libulf_mdu_ein_zsu_update(libHandle, zsuHandle, fwIndex, 0uz, &r),
            LIBULF_OK);
}

TEST_F(TestMDU_EIN, zsu_update_receive_error) {
  assertReceiveErrorCalls();

  bool r{};
  libulf_mdu_ein_zsu_update(libHandle, zsuHandle, fwIndex, 0uz, &r);
}

TEST_F(TestMDU_EIN, zsu_update_receive_error_result) {
  throwReceiveException();

  bool r{};
  ASSERT_NE(libulf_mdu_ein_zsu_update(libHandle, zsuHandle, fwIndex, 0uz, &r),
            LIBULF_OK);
}
