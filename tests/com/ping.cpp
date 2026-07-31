#include "../helper.hpp"
#include "../range_matcher.hpp"
#include "f_com.hpp"

using testing::_;
using testing::Ge;
using testing::InSequence;
using testing::Return;

TEST_F(TestCOM, ping_payload) {
  using std::operator""sv;

  auto const p{"PING\r"sv};
  std::span<uint8_t const> const expected_payload{
    std::bit_cast<uint8_t*>(p.data()), p.size()};

  {
    InSequence i;
    EXPECT_CALL(conn, _write(RM(expected_payload), _)).Times(1);
    EXPECT_CALL(conn, _read_until(_, _, _, _, _)).Times(1);
  }

  libklug_com_ping(libHandle);
  libklug_job_await(libHandle);
}

TEST_F(TestCOM, ping_result) {
  using std::operator""sv;

  auto expected{"Super duper real device v2.0.255\r"sv};

  auto const r{helper::string_view2span(expected)};
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(
      [&](uint8_t* buf, uint32_t len, int* rx_ed, uint8_t, uint32_t timeout) {
        assert(len >= r.size());
        std::ranges::copy(r, buf);
        *rx_ed = r.size();
        return 0;
      });

  libklug_com_ping(libHandle);
  auto const result{libklug_job_await(libHandle)};

  ASSERT_EQ(result.type, result_type::string);
  ASSERT_EQ(std::string_view{result.data.string}, expected);
}

TEST_F(TestCOM, ping_transmit_error) {
  assertTransmitErrorCalls();

  libklug_com_ping(libHandle);
  libklug_job_await(libHandle);
}

TEST_F(TestCOM, ping_transmit_error_result) {
  throwTransmitException();

  libklug_com_ping(libHandle);
  auto const result{libklug_job_await(libHandle)};

  assertTransmitReceiveErrorResult(result);
}

TEST_F(TestCOM, ping_receive_error) {
  assertReceiveErrorCalls();

  libklug_com_ping(libHandle);
  libklug_job_await(libHandle);
}

TEST_F(TestCOM, ping_receive_error_result) {
  throwReceiveException();

  libklug_com_ping(libHandle);
  auto const result{libklug_job_await(libHandle)};

  assertTransmitReceiveErrorResult(result);
}
