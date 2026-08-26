#include <ulf/susiv2.hpp>
#include <zusi/zusi.hpp>
#include "../helper.hpp"
#include "../range_matcher.hpp"
#include "f_susiv2.hpp"
#include "helper.hpp"

using testing::_;
using testing::Ge;
using testing::InSequence;
using testing::Return;

TEST_F(TestSUSIV2, zpp_update_payload) {
  {
    InSequence i;
    EXPECT_CALL(
      conn,
      _write(RM(ulf::susiv2::packet2frame<std::vector<uint8_t>>(
               zusi::make_zpp_write_packet(
                 255uz,
                 0uz,
                 std::span<uint8_t const, 256uz>{zpp.flash.data(), 256uz}))),
             _));
    EXPECT_CALL(conn, _read_all(_, _, _, _));
  }

  libklug_bool r{};
  libklug_susiv2_zpp_write(libHandle, zppHandle, 0uz, &r);
}

TEST_F(TestSUSIV2, zpp_write_result_success) {
  std::vector<uint8_t> expected{ulf::susiv2::ack};
  expected.push_back(zusi::crc8(expected));

  ON_CALL(conn, _read_all(_, Ge(expected.size()), _, _))
    .WillByDefault(helper::susiv2::receive_ack);

  libklug_bool r{};
  ASSERT_EQ(libklug_susiv2_zpp_write(libHandle, zppHandle, 0uz, &r),
            LIBKLUG_OK);
  ASSERT_TRUE(r);
}

TEST_F(TestSUSIV2, zpp_write_write_error) {
  assertTransmitErrorCalls<true>();

  libklug_bool r{};
  libklug_susiv2_zpp_write(libHandle, zppHandle, 0uz, &r);
}

TEST_F(TestSUSIV2, zpp_write_write_error_result) {
  throwTransmitException();

  libklug_bool r{};
  ASSERT_NE(libklug_susiv2_zpp_write(libHandle, zppHandle, 0uz, &r),
            LIBKLUG_OK);
}

TEST_F(TestSUSIV2, zpp_write_receive_error) {
  assertReceiveErrorCalls<true>();

  libklug_bool r{};
  libklug_susiv2_zpp_write(libHandle, zppHandle, 0uz, &r);
}

TEST_F(TestSUSIV2, zpp_write_receive_error_result) {
  throwReceiveException();

  libklug_bool r{};
  ASSERT_NE(libklug_susiv2_zpp_write(libHandle, zppHandle, 0uz, &r),
            LIBKLUG_OK);
}
