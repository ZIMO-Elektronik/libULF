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

TEST_F(TestMDU_EIN, ping_payload) {
  {
    InSequence i;
    EXPECT_CALL(
      conn,
      _write(RM(helper::mdu::packet2frame(mdu::make_ping_packet(sn, id))), _))
      .Times(1);
    EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(1);
  }

  libklug_mdu_ein_ping(libHandle, sn, id);
  libklug_job_await(libHandle);
}

TEST_F(TestMDU_EIN, ping_result_success) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_nak);

  libklug_mdu_ein_ping(libHandle, sn, id);
  auto const result{libklug_job_await(libHandle)};

  ASSERT_EQ(result.type, result_type::status);
  ASSERT_TRUE(result.data.success);
}

TEST_F(TestMDU_EIN, ping_result_no_success) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_ack);

  libklug_mdu_ein_ping(libHandle, sn, id);
  auto const result{libklug_job_await(libHandle)};

  ASSERT_EQ(result.type, result_type::status);
  ASSERT_FALSE(result.data.success);
}

TEST_F(TestMDU_EIN, ping_write_error) {
  assertTransmitErrorCalls();

  libklug_mdu_ein_ping(libHandle, sn, id);
  libklug_job_await(libHandle);
}

TEST_F(TestMDU_EIN, ping_write_error_result) {
  throwTransmitException();

  libklug_mdu_ein_ping(libHandle, sn, id);
  auto const result{libklug_job_await(libHandle)};

  assertTransmitReceiveErrorResult(result);
}

TEST_F(TestMDU_EIN, ping_receive_error) {
  assertReceiveErrorCalls();

  libklug_mdu_ein_ping(libHandle, sn, id);
  libklug_job_await(libHandle);
}

TEST_F(TestMDU_EIN, ping_receive_error_result) {
  throwTransmitException();

  libklug_mdu_ein_ping(libHandle, sn, id);
  auto const result{libklug_job_await(libHandle)};

  assertTransmitReceiveErrorResult(result);
}
