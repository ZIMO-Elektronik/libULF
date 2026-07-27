#include "../helper.hpp"
#include "../range_matcher.hpp"
#include "f_com.hpp"
#include "helper.hpp"

using testing::_;
using testing::Ge;
using testing::InSequence;
using testing::Return;

TEST_F(TestCOM, reset_payload) {
  using std::operator""sv;

  auto const p{"RESET\r"sv};
  std::span<uint8_t const> const expected_payload{
    std::bit_cast<uint8_t*>(p.data()), p.size()};

  {
    testing::InSequence i;
    EXPECT_CALL(conn, _write(RM(expected_payload), _)).Times(1);
    EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(1);
  }

  libklug_com_reset(libHandle);
  libklug_job_await(libHandle);
}

TEST_F(TestCOM, reset_result_ok) {
  using std::operator""sv;

  auto const r{helper::string_view2span("OK\r")};
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::com::receive_ok);

  libklug_com_reset(libHandle);
  auto const result{libklug_job_await(libHandle)};

  ASSERT_EQ(result.type, result_type::status);
  ASSERT_TRUE(result.data.success);
}

TEST_F(TestCOM, reset_result_not_ok) {
  using std::operator""sv;

  auto const r{helper::string_view2span("NOT_OK\r")};
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(helper::com::receive_not_ok);

  libklug_com_reset(libHandle);
  auto const result{libklug_job_await(libHandle)};

  ASSERT_EQ(result.type, result_type::status);
  ASSERT_FALSE(result.data.success);
}

TEST_F(TestCOM, reset_transmit_error) {
  assertTransmitErrorCalls();

  libklug_com_reset(libHandle);
  libklug_job_await(libHandle);
}

TEST_F(TestCOM, reset_transmit_error_result) {
  throwTransmitException();

  libklug_com_reset(libHandle);
  auto const result{libklug_job_await(libHandle)};

  assertTransmitReceiveErrorResult(result);
}

TEST_F(TestCOM, reset_receive_error) {
  assertReceiveErrorCalls();

  libklug_com_reset(libHandle);
  libklug_job_await(libHandle);
}

TEST_F(TestCOM, reset_receive_error_result) {
  throwReceiveException();

  libklug_com_reset(libHandle);
  auto const result{libklug_job_await(libHandle)};

  assertTransmitReceiveErrorResult(result);
}
