#include "../range_matcher.hpp"
#include "f_mdu_ein.hpp"
#include "helper.hpp"

TEST_F(TestMDU_EIN, zpp_lc_dc_query_payload) {
  {
    InSequence i;
    EXPECT_CALL(conn,
                _write(RM(ulf::mdu_ein::bytes2mdu_ein(
                         mdu::make_zpp_lc_dc_query_packet(zpp.developer_code))),
                       _));
    EXPECT_CALL(conn, _read_until(_, _, _, _, _));
  }

  bool r{};
  libklug_mdu_ein_zpp_lc_dc_query(libHandle, zppHandle, &r);
}

TEST_F(TestMDU_EIN, zpp_lc_dc_query_result_success) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_ack);

  bool r{};
  ASSERT_EQ(libklug_mdu_ein_zpp_lc_dc_query(libHandle, zppHandle, &r),
            LIBKLUG_OK);
  ASSERT_TRUE(r);
}

TEST_F(TestMDU_EIN, zpp_lc_dc_query_result_no_success) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_nak);

  bool r{};
  ASSERT_EQ(libklug_mdu_ein_zpp_lc_dc_query(libHandle, zppHandle, &r),
            LIBKLUG_OK);
  ASSERT_FALSE(r);
}

TEST_F(TestMDU_EIN, zpp_lc_dc_query_write_error) {
  assertTransmitErrorCalls();

  bool r{};
  libklug_mdu_ein_zpp_lc_dc_query(libHandle, zppHandle, &r);
}

TEST_F(TestMDU_EIN, zpp_lc_dc_query_write_error_result) {
  throwTransmitException();

  bool r{};
  ASSERT_NE(libklug_mdu_ein_zpp_lc_dc_query(libHandle, zppHandle, &r),
            LIBKLUG_OK);
}

TEST_F(TestMDU_EIN, zpp_lc_dc_query_receive_error) {
  assertReceiveErrorCalls();

  bool r{};
  libklug_mdu_ein_zpp_lc_dc_query(libHandle, zppHandle, &r);
}

TEST_F(TestMDU_EIN, zpp_lc_dc_query_receive_error_result) {
  throwReceiveException();

  bool r{};
  ASSERT_NE(libklug_mdu_ein_zpp_lc_dc_query(libHandle, zppHandle, &r),
            LIBKLUG_OK);
}
