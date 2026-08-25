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

  std::string r{};
  r.reserve(128uz);
  size_t size{r.capacity()};
  libklug_com_ping(libHandle, r.data(), &size);
}

TEST_F(TestCOM, ping_result) {
  using std::operator""sv;

  auto expected{"Super duper real device v2.0.255\r"sv};

  auto const span{helper::string_view2span(expected)};
  ON_CALL(conn, _read_until(_, _, _, _, _))
    .WillByDefault(
      [&](uint8_t* buf, uint32_t len, int* rx_ed, uint8_t, uint32_t timeout) {
        assert(len >= span.size());
        std::ranges::copy(span, buf);
        *rx_ed = span.size();
        return 0;
      });

  std::string r{};
  r.resize(128uz);
  size_t size{r.size()};
  ASSERT_EQ(libklug_com_ping(libHandle, r.data(), &size), libklug_error::ok);
  r.resize(size);
  ASSERT_EQ(r, expected);
}

TEST_F(TestCOM, ping_transmit_error) {
  assertTransmitErrorCalls();

  std::string r{};
  r.reserve(128uz);
  size_t size{r.capacity()};
  libklug_com_ping(libHandle, r.data(), &size);
}

TEST_F(TestCOM, ping_transmit_error_result) {
  throwTransmitException();

  std::string r{};
  r.reserve(128uz);
  size_t size{r.capacity()};
  ASSERT_NE(libklug_com_ping(libHandle, r.data(), &size), libklug_error::ok);
}

TEST_F(TestCOM, ping_receive_error) {
  assertReceiveErrorCalls();

  std::string r{};
  r.reserve(128uz);
  size_t size{r.capacity()};
  libklug_com_ping(libHandle, r.data(), &size);
}

TEST_F(TestCOM, ping_receive_error_result) {
  throwReceiveException();

  std::string r{};
  r.reserve(128uz);
  size_t size{r.capacity()};
  ASSERT_NE(libklug_com_ping(libHandle, r.data(), &size), libklug_error::ok);
}
