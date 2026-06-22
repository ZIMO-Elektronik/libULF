#include <ranges>
#include "../helper.hpp"
#include "../range_matcher.hpp"
#include "f_mdu_ein.hpp"
#include "helper.hpp"

TEST_F(TestMDU_EIN, zsu_update_payload) {
  auto const blocks{libklug_zsu_firmware_iterator_get_blocks(fwItHandle)};

  for (unsigned int idx{0uz}; idx < blocks; idx++) {
    {
      InSequence i;
      EXPECT_CALL(
        conn,
        _write(RM(helper::mdu::packet2frame(mdu::make_zsu_update_packet(
                 idx * 64uz,
                 std::span<uint8_t const, 64uz>{
                   std::span<uint8_t const>(fwIt.get().bin)
                     .subspan(idx * 64uz, 64uz)}))),
               _))
        .Times(1);
      EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(1);
    }

    libklug_mdu_ein_zsu_update(libHandle, fwItHandle, idx);
    libklug_result(libHandle);
  }
}

TEST_F(TestMDU_EIN, zsu_update_result_success) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_ack);

  libklug_mdu_ein_zsu_update(libHandle, fwItHandle, 0uz);
  auto const result{libklug_result(libHandle)};

  ASSERT_EQ(result.type, result_type::status);
  ASSERT_EQ(result.data.success, LIBKLUG_TRUE);
}

TEST_F(TestMDU_EIN, zsu_update_result_no_success) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_nak);

  libklug_mdu_ein_zsu_update(libHandle, fwItHandle, 0uz);
  auto const result{libklug_result(libHandle)};

  ASSERT_EQ(result.type, result_type::status);
  ASSERT_EQ(result.data.success, LIBKLUG_FALSE);
}

TEST_F(TestMDU_EIN, zsu_update_write_error) {
  assertTransmitErrorCalls();

  libklug_mdu_ein_zsu_update(libHandle, fwItHandle, 0uz);
  libklug_result(libHandle);
}

TEST_F(TestMDU_EIN, zsu_update_write_error_result) {
  throwTransmitException();

  libklug_mdu_ein_zsu_update(libHandle, fwItHandle, 0uz);
  auto const result{libklug_result(libHandle)};

  assertTransmitReceiveErrorResult(result);
}

TEST_F(TestMDU_EIN, zsu_update_receive_error) {
  assertReceiveErrorCalls();

  libklug_mdu_ein_zsu_update(libHandle, fwItHandle, 0uz);
  libklug_result(libHandle);
}

TEST_F(TestMDU_EIN, zsu_update_receive_error_result) {
  throwReceiveException();

  libklug_mdu_ein_zsu_update(libHandle, fwItHandle, 0uz);
  auto const result{libklug_result(libHandle)};

  assertTransmitReceiveErrorResult(result);
}
