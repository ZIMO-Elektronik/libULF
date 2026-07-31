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

constexpr uint16_t cv_address{8u};
constexpr uint8_t cv_value{145u};

} // namespace

TEST_F(TestMDU_EIN, cv_write_payload) {
  {
    InSequence i;
    EXPECT_CALL(conn,
                _write(RM(helper::mdu::packet2frame(
                         mdu::make_cv_write_packet(cv_address, cv_value))),
                       _))
      .Times(1);
    EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(1);
  }

  libklug_mdu_ein_cv_write(libHandle, cv_address, cv_value);
  libklug_job_await(libHandle);
}

TEST_F(TestMDU_EIN, cv_write_result_success) {
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::mdu::receive_ack);

  libklug_mdu_ein_cv_write(libHandle, cv_address, cv_value);
  auto const result{libklug_job_await(libHandle)};

  ASSERT_EQ(result.type, result_type::status);
  ASSERT_TRUE(result.data.success);
}

TEST_F(TestMDU_EIN, cv_write_write_error) {
  assertTransmitErrorCalls();

  libklug_mdu_ein_cv_write(libHandle, cv_address, cv_value);
  libklug_job_await(libHandle);
}

TEST_F(TestMDU_EIN, cv_write_write_error_result) {
  throwTransmitException();

  libklug_mdu_ein_cv_write(libHandle, cv_address, cv_value);
  auto const result{libklug_job_await(libHandle)};

  assertTransmitReceiveErrorResult(result);
}

TEST_F(TestMDU_EIN, cv_write_receive_error) {
  assertReceiveErrorCalls();

  libklug_mdu_ein_cv_write(libHandle, cv_address, cv_value);
  libklug_job_await(libHandle);
}

TEST_F(TestMDU_EIN, cv_write_receive_error_result) {
  throwReceiveException();

  libklug_mdu_ein_cv_write(libHandle, cv_address, cv_value);
  auto const result{libklug_job_await(libHandle)};

  assertTransmitReceiveErrorResult(result);
}
