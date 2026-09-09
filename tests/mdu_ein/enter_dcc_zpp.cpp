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

} // namespace

TEST_F(TestMDU_EIN, dcc_zpp_payload) {
  {
    InSequence i;
    EXPECT_CALL(conn,
                _write(RM(helper::mdu::special2frame(
                         helper::mdu::make_dcc_zpp_entry_command(sn, false))),
                       _))
      .Times(1);
    EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(1);
  }

  bool r{};
  libklug_mdu_ein_enter_dcc_zpp(libHandle, sn, true, &r);
}

TEST_F(TestMDU_EIN, dcc_zpp_esult_success) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_ack);

  bool r{};
  ASSERT_EQ(libklug_mdu_ein_enter_dcc_zpp(libHandle, sn, true, &r), LIBKLUG_OK);
  ASSERT_TRUE(r);
}

TEST_F(TestMDU_EIN, dcc_zpp_esult_no_success) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_nak);

  bool r{};
  ASSERT_EQ(libklug_mdu_ein_enter_dcc_zpp(libHandle, sn, true, &r), LIBKLUG_OK);
  ASSERT_FALSE(r);
}

TEST_F(TestMDU_EIN, dcc_zpp_write_error) {
  assertTransmitErrorCalls();

  bool r{};
  libklug_mdu_ein_enter_dcc_zpp(libHandle, sn, true, &r);
}

TEST_F(TestMDU_EIN, dcc_zpp_write_error_result) {
  throwTransmitException();

  bool r{};
  ASSERT_NE(libklug_mdu_ein_enter_dcc_zpp(libHandle, sn, true, &r), LIBKLUG_OK);
}

TEST_F(TestMDU_EIN, dcc_zpp_receive_error) {
  assertReceiveErrorCalls();

  bool r{};
  libklug_mdu_ein_enter_dcc_zpp(libHandle, sn, true, &r);
}

TEST_F(TestMDU_EIN, dcc_zpp_eceive_error_result) {
  throwReceiveException();

  bool r{};
  ASSERT_NE(libklug_mdu_ein_enter_dcc_zpp(libHandle, sn, true, &r), LIBKLUG_OK);
}
