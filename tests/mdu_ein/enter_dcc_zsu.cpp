#include <mdu/mdu.hpp>
#include <ulf/mdu_ein.hpp>
#include "../helper.hpp"
#include "../range_matcher.hpp"
#include "f_mdu_ein.hpp"
#include "helper.hpp"

using testing::_;
using testing::Ge;
using testing::InSequence;
using testing::Return;

namespace {

constexpr uint32_t sn{0x00FF00FFuz};
constexpr uint32_t id{0xFF00FF00uz};

} // namespace

TEST_F(TestMDU_EIN, dcc_zsu_payload) {
  {
    InSequence i;
    EXPECT_CALL(
      conn,
      _write(RM(helper::mdu::special2frame(
               helper::mdu::make_dcc_zsu_entry_command(id, sn, false))),
             _))
      .Times(1);
    EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(1);
  }

  int r{};
  libklug_mdu_ein_enter_dcc_zsu(libHandle, id, sn, LIBKLUG_TRUE, &r);
}

TEST_F(TestMDU_EIN, dcc_zsu_esult_success) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_ack);

  int r{};
  ASSERT_EQ(libklug_mdu_ein_enter_dcc_zsu(libHandle, id, sn, LIBKLUG_TRUE, &r),
            libklug_error::ok);
  ASSERT_TRUE(r);
}

TEST_F(TestMDU_EIN, dcc_zsu_esult_no_success) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_nak);

  int r{};
  ASSERT_EQ(libklug_mdu_ein_enter_dcc_zsu(libHandle, id, sn, LIBKLUG_TRUE, &r),
            libklug_error::ok);
  ASSERT_FALSE(r);
}

TEST_F(TestMDU_EIN, dcc_zsu_write_error) {
  assertTransmitErrorCalls();

  int r{};
  libklug_mdu_ein_enter_dcc_zsu(libHandle, id, sn, LIBKLUG_TRUE, &r);
}

TEST_F(TestMDU_EIN, dcc_zsu_write_error_result) {
  throwTransmitException();

  int r{};
  ASSERT_NE(libklug_mdu_ein_enter_dcc_zsu(libHandle, id, sn, LIBKLUG_TRUE, &r),
            libklug_error::ok);
}

TEST_F(TestMDU_EIN, dcc_zsu_eceive_error) {
  assertReceiveErrorCalls();

  int r{};
  libklug_mdu_ein_enter_dcc_zsu(libHandle, id, sn, LIBKLUG_TRUE, &r);
}

TEST_F(TestMDU_EIN, dcc_zsu_eceive_error_result) {
  throwReceiveException();

  int r{};
  ASSERT_NE(libklug_mdu_ein_enter_dcc_zsu(libHandle, id, sn, LIBKLUG_TRUE, &r),
            libklug_error::ok);
}
