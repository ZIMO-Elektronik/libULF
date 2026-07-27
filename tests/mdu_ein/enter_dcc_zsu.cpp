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

  libklug_mdu_ein_enter_dcc_zsu(libHandle, id, sn, LIBKLUG_TRUE);
  libklug_job_await(libHandle);
}

TEST_F(TestMDU_EIN, dcc_zsu_esult_success) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_ack);

  libklug_mdu_ein_enter_dcc_zsu(libHandle, id, sn, LIBKLUG_TRUE);
  auto const result{libklug_job_await(libHandle)};

  ASSERT_EQ(result.type, result_type::status);
  ASSERT_TRUE(result.data.success);
}

TEST_F(TestMDU_EIN, dcc_zsu_esult_no_success) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_nak);

  libklug_mdu_ein_enter_dcc_zsu(libHandle, id, sn, LIBKLUG_TRUE);
  auto const result{libklug_job_await(libHandle)};

  ASSERT_EQ(result.type, result_type::status);
  ASSERT_FALSE(result.data.success);
}

TEST_F(TestMDU_EIN, dcc_zsu_write_error) {
  assertTransmitErrorCalls();

  libklug_mdu_ein_enter_dcc_zsu(libHandle, id, sn, LIBKLUG_TRUE);
  libklug_job_await(libHandle);
}

TEST_F(TestMDU_EIN, dcc_zsu_write_error_result) {
  throwTransmitException();

  libklug_mdu_ein_enter_dcc_zsu(libHandle, id, sn, LIBKLUG_TRUE);
  auto const result{libklug_job_await(libHandle)};

  assertTransmitReceiveErrorResult(result);
}

TEST_F(TestMDU_EIN, dcc_zsu_eceive_error) {
  assertReceiveErrorCalls();

  libklug_mdu_ein_enter_dcc_zsu(libHandle, id, sn, LIBKLUG_TRUE);
  libklug_job_await(libHandle);
}

TEST_F(TestMDU_EIN, dcc_zsu_eceive_error_result) {
  throwReceiveException();

  libklug_mdu_ein_enter_dcc_zsu(libHandle, id, sn, LIBKLUG_TRUE);
  auto const result{libklug_job_await(libHandle)};

  assertTransmitReceiveErrorResult(result);
}
